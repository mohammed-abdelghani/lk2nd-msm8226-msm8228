/*
 * Copyright (c) 2007-2015 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <debug.h>
#include <arch/defines.h>
#include <lib/fs.h>
#include "ext2_priv.h"

#define LOCAL_TRACE 0

static void endian_swap_superblock(struct ext2_super_block *sb)
{
    LE32SWAP(sb->s_inodes_count);
    LE32SWAP(sb->s_blocks_count);
    LE32SWAP(sb->s_r_blocks_count);
    LE32SWAP(sb->s_free_blocks_count);
    LE32SWAP(sb->s_free_inodes_count);
    LE32SWAP(sb->s_first_data_block);
    LE32SWAP(sb->s_log_block_size);
    LE32SWAP(sb->s_log_frag_size);
    LE32SWAP(sb->s_blocks_per_group);
    LE32SWAP(sb->s_frags_per_group);
    LE32SWAP(sb->s_inodes_per_group);
    LE32SWAP(sb->s_mtime);
    LE32SWAP(sb->s_wtime);
    LE16SWAP(sb->s_mnt_count);
    LE16SWAP(sb->s_max_mnt_count);
    LE16SWAP(sb->s_magic);
    LE16SWAP(sb->s_state);
    LE16SWAP(sb->s_errors);
    LE16SWAP(sb->s_minor_rev_level);
    LE32SWAP(sb->s_lastcheck);
    LE32SWAP(sb->s_checkinterval);
    LE32SWAP(sb->s_creator_os);
    LE32SWAP(sb->s_rev_level);
    LE16SWAP(sb->s_def_resuid);
    LE16SWAP(sb->s_def_resgid);
    LE32SWAP(sb->s_first_ino);
    LE16SWAP(sb->s_inode_size);
    LE16SWAP(sb->s_block_group_nr);
    LE32SWAP(sb->s_feature_compat);
    LE32SWAP(sb->s_feature_incompat);
    LE32SWAP(sb->s_feature_ro_compat);
    LE32SWAP(sb->s_algorithm_usage_bitmap);

    /* ext3 journal stuff */
    LE32SWAP(sb->s_journal_inum);
    LE32SWAP(sb->s_journal_dev);
    LE32SWAP(sb->s_last_orphan);
    LE32SWAP(sb->s_default_mount_opts);
    LE32SWAP(sb->s_first_meta_bg);
}

static void endian_swap_inode(struct ext2_inode *inode)
{
    LE16SWAP(inode->i_mode);
    LE16SWAP(inode->i_uid_low);
    LE32SWAP(inode->i_size);
    LE32SWAP(inode->i_atime);
    LE32SWAP(inode->i_ctime);
    LE32SWAP(inode->i_mtime);
    LE32SWAP(inode->i_dtime);
    LE16SWAP(inode->i_gid_low);
    LE16SWAP(inode->i_links_count);
    LE32SWAP(inode->i_blocks);
    LE32SWAP(inode->i_flags);

    // leave block pointers/symlink data alone

    LE32SWAP(inode->i_generation);
    LE32SWAP(inode->i_file_acl);
    LE32SWAP(inode->i_dir_acl);
    LE32SWAP(inode->i_faddr);

    LE16SWAP(inode->i_uid_high);
    LE16SWAP(inode->i_gid_high);
}

static void endian_swap_group_desc(struct ext2_group_desc *gd)
{
    LE32SWAP(gd->bg_block_bitmap);
    LE32SWAP(gd->bg_inode_bitmap);
    LE32SWAP(gd->bg_inode_table);
    LE16SWAP(gd->bg_free_blocks_count);
    LE16SWAP(gd->bg_free_inodes_count);
    LE16SWAP(gd->bg_used_dirs_count);
}

status_t ext2_mount(bdev_t *dev, fscookie **cookie)
{
    int err;

    LTRACEF("dev %p\n", dev);

    ext2_t *ext2 = malloc(sizeof(ext2_t));
    if (!ext2)
        return -1;
    memset(ext2, 0, sizeof(*ext2));
    ext2->dev = dev;

    struct ext2_super_block *sb = memalign(CACHE_LINE,
                                           ROUNDUP(sizeof(struct ext2_super_block), CACHE_LINE));
    if (!sb) {
        free(ext2);
        return -1;
    }

    err = bio_read(dev, sb, 1024, sizeof(struct ext2_super_block));
    if (err < 0) {
        free(sb);
        goto err;
    }

    memcpy(&ext2->sb, sb, sizeof(struct ext2_super_block));
    free(sb);

    endian_swap_superblock(&ext2->sb);

    /* see if the superblock is good */
    if (ext2->sb.s_magic != EXT2_SUPER_MAGIC) {
        err = -1;
        goto err;
    }

    /* reject insane block/fragment sizes before they are used */
    if (ext2->sb.s_log_block_size > 2 || ext2->sb.s_log_frag_size > 2) {
        dprintf(INFO, "ext2: unsupported block/frag size (log2=%u/%u)\n",
                ext2->sb.s_log_block_size, ext2->sb.s_log_frag_size);
        err = -5;
        goto err;
    }

    /* these fields are divisors below, they must not be zero */
    if (ext2->sb.s_blocks_per_group == 0 ||
        ext2->sb.s_inodes_per_group == 0) {
        dprintf(INFO, "ext2: blocks/inodes per group is zero\n");
        err = -5;
        goto err;
    }

    /* calculate group count, rounded up (64-bit to avoid overflow) */
    ext2->s_group_count = ((uint64_t)ext2->sb.s_blocks_count + ext2->sb.s_blocks_per_group - 1) / ext2->sb.s_blocks_per_group;
    if (ext2->s_group_count <= 0 || ext2->s_group_count > 65536) {
        dprintf(INFO, "ext2: invalid group count %d\n", ext2->s_group_count);
        err = -5;
        goto err;
    }

    /* print some info */
    LTRACEF("rev level %d\n", ext2->sb.s_rev_level);
    LTRACEF("compat features 0x%x\n", ext2->sb.s_feature_compat);
    LTRACEF("incompat features 0x%x\n", ext2->sb.s_feature_incompat);
    LTRACEF("ro compat features 0x%x\n", ext2->sb.s_feature_ro_compat);
    LTRACEF("block size %d\n", EXT2_BLOCK_SIZE(ext2->sb));
    LTRACEF("inode size %d\n", EXT2_INODE_SIZE(ext2->sb));
    LTRACEF("block count %d\n", ext2->sb.s_blocks_count);
    LTRACEF("blocks per group %d\n", ext2->sb.s_blocks_per_group);
    LTRACEF("group count %d\n", ext2->s_group_count);
    LTRACEF("inodes per group %d\n", ext2->sb.s_inodes_per_group);

    /* we only support dynamic revs */
    if (ext2->sb.s_rev_level > EXT2_DYNAMIC_REV) {
        err = -2;
        goto err;
    }

    /* sanity check the inode size for dynamic revision superblocks */
    if (ext2->sb.s_rev_level == EXT2_DYNAMIC_REV &&
        (ext2->sb.s_inode_size < 128 ||
         ext2->sb.s_inode_size > EXT2_BLOCK_SIZE(ext2->sb) ||
         EXT2_BLOCK_SIZE(ext2->sb) % ext2->sb.s_inode_size != 0)) {
        dprintf(INFO, "ext2: invalid inode size %u\n", ext2->sb.s_inode_size);
        err = -5;
        goto err;
    }

    /* make sure it doesn't have any ro features we don't support */
    if (ext2->sb.s_feature_ro_compat & ~(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER|EXT2_FEATURE_RO_COMPAT_LARGE_FILE)) {
        err = -3;
        goto err;
    }

    /* read in all the group descriptors */
    size_t gd_size = sizeof(struct ext2_group_desc) * ext2->s_group_count;
    ext2->gd = memalign(CACHE_LINE, ROUNDUP(gd_size, CACHE_LINE));
    if (!ext2->gd) {
        err = -4;
        goto err;
    }
    err = bio_read(ext2->dev, (void *)ext2->gd,
                   (EXT2_BLOCK_SIZE(ext2->sb) == 4096) ? 4096 : 2048,
                   gd_size);
    if (err < 0) {
        err = -4;
        goto err;
    }

    int i;
    for (i=0; i < ext2->s_group_count; i++) {
        endian_swap_group_desc(&ext2->gd[i]);
        LTRACEF("group %d:\n", i);
        LTRACEF("\tblock bitmap %d\n", ext2->gd[i].bg_block_bitmap);
        LTRACEF("\tinode bitmap %d\n", ext2->gd[i].bg_inode_bitmap);
        LTRACEF("\tinode table %d\n", ext2->gd[i].bg_inode_table);
        LTRACEF("\tfree blocks %d\n", ext2->gd[i].bg_free_blocks_count);
        LTRACEF("\tfree inodes %d\n", ext2->gd[i].bg_free_inodes_count);
        LTRACEF("\tused dirs %d\n", ext2->gd[i].bg_used_dirs_count);
    }

    /* scratch buffer for partial block reads (replaces stack VLAs) */
    ext2->block_tmp = memalign(CACHE_LINE, EXT2_BLOCK_SIZE(ext2->sb));
    if (!ext2->block_tmp) {
        err = -4;
        goto err;
    }

    /* initialize the block cache */
    ext2->cache = bcache_create(ext2->dev, EXT2_BLOCK_SIZE(ext2->sb), 4);
    if (!ext2->cache) {
        err = -4;
        goto err;
    }

    /* load the first inode */
    err = ext2_load_inode(ext2, EXT2_ROOT_INO, &ext2->root_inode);
    if (err < 0)
        goto err;

//  TRACE("successfully mounted volume\n");

    *cookie = (fscookie *)ext2;

    return 0;

err:
    LTRACEF("exiting with err code %d\n", err);

    if (ext2->cache)
        bcache_destroy(ext2->cache);
    free(ext2->gd);
    free(ext2->block_tmp);
    free(ext2);
    return err;
}

status_t ext2_unmount(fscookie *cookie)
{
    // free it up
    ext2_t *ext2 = (ext2_t *)cookie;

    bcache_destroy(ext2->cache);
    free(ext2->gd);
    free(ext2->block_tmp);
    free(ext2);

    return 0;
}

static void get_inode_addr(ext2_t *ext2, inodenum_t num, blocknum_t *block, size_t *block_offset)
{
    num--;

    uint32_t group = num / ext2->sb.s_inodes_per_group;

    // calculate the start of the inode table for the group it's in
    *block = ext2->gd[group].bg_inode_table;

    // add the offset of the inode within the group
    size_t offset = (num % EXT2_INODES_PER_GROUP(ext2->sb)) * EXT2_INODE_SIZE(ext2->sb);
    *block_offset = offset % EXT2_BLOCK_SIZE(ext2->sb);
    *block += offset / EXT2_BLOCK_SIZE(ext2->sb);
}

int ext2_load_inode(ext2_t *ext2, inodenum_t num, struct ext2_inode *inode)
{
    int err;

    LTRACEF("num %d, inode %p\n", num, inode);

    /* inode numbers and group indexes come from untrusted directory data */
    if (num == 0)
        return -1;
    if ((num - 1) / ext2->sb.s_inodes_per_group >= (uint32_t)ext2->s_group_count)
        return -1;

    blocknum_t bnum;
    size_t block_offset;
    get_inode_addr(ext2, num, &bnum, &block_offset);

    LTRACEF("bnum %u, offset %zd\n", bnum, block_offset);

    /* get a pointer to the cache block */
    void *cache_ptr;
    err = bcache_get_block(ext2->cache, &cache_ptr, bnum);
    if (err < 0)
        return err;

    /* copy the inode out */
    memcpy(inode, (uint8_t *)cache_ptr + block_offset, sizeof(struct ext2_inode));

    /* put the cache block */
    bcache_put_block(ext2->cache, bnum);

    /* endian swap it */
    endian_swap_inode(inode);

    LTRACEF("read inode: mode 0x%x, size %d\n", inode->i_mode, inode->i_size);

    return 0;
}

static const struct fs_api ext2_api = {
    .mount = ext2_mount,
    .unmount = ext2_unmount,
    .open = ext2_open_file,
    .stat = ext2_stat_file,
    .read = ext2_read_file,
    .close = ext2_close_file,
    .opendir = ext2_open_directory,
    .readdir = ext2_read_directory,
    .closedir = ext2_close_directory
};

void ext2_init(void)
{
    fs_register_type("ext2", &ext2_api);
}
