/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Fundation, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _UFS_H_
#define _UFS_H_

#include <sys/types.h>

struct ufs_dev
{
	uint8_t                      instance;
	uint32_t                     base;
	uint8_t                      num_lus;
	uint8_t                      current_lun;
};

int ufs_init(struct ufs_dev *dev);
int ufs_read(struct ufs_dev* dev, uint64_t start_lba, addr_t buffer, uint32_t num_blocks);
int ufs_write(struct ufs_dev* dev, uint64_t start_lba, addr_t buffer, uint32_t num_blocks);
int ufs_erase(struct ufs_dev* dev, uint64_t start_lba, uint32_t num_blocks);
uint64_t ufs_get_dev_capacity(struct ufs_dev* dev);
uint32_t ufs_get_serial_num(struct ufs_dev* dev);
uint8_t ufs_get_num_of_luns(struct ufs_dev* dev);
uint32_t ufs_get_erase_blk_size(struct ufs_dev* dev);
uint32_t ufs_get_page_size(struct ufs_dev* dev);
void ufs_dump_is_register(struct ufs_dev* dev);
void ufs_dump_hc_registers(struct ufs_dev* dev);
void ufs_rpmb_init(struct ufs_dev *dev);
#endif
