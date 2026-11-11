/*
 * Copyright (c) 2026, The Linux Foundation. All rights reserved.
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
 *     * Neither the name of The Linux Foundation, Inc. nor the names of its
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
 */

/*
 * Weak fallback implementations of the QUP SPI/I2C driver API used by the
 * i2c/spi tests in this directory. lk2nd currently ships no QUP driver, so
 * these stubs let ENABLE_UNITTEST_FW builds link on every target and make
 * the tests report gracefully instead of failing at runtime. If a real
 * driver is added later, its strong symbols override these stubs.
 */

#include <compiler.h>
#include <debug.h>
#include <i2c_qup.h>
#include <spi_qup.h>

__WEAK struct qup_i2c_dev *qup_blsp_i2c_init(uint8_t blsp_id, uint8_t qup_id,
					     uint32_t clk_freq, uint32_t src_clk_freq)
{
	dprintf(CRITICAL, "qup_blsp_i2c_init: QUP I2C driver not available\n");
	return NULL;
}

__WEAK int qup_i2c_xfer(struct qup_i2c_dev *dev, struct i2c_msg msgs[], int num)
{
	dprintf(CRITICAL, "qup_i2c_xfer: QUP I2C driver not available\n");
	return -ENODEV;
}

__WEAK struct qup_spi_dev *qup_blsp_spi_init(uint8_t blsp_id, uint8_t qup_id)
{
	dprintf(CRITICAL, "qup_blsp_spi_init: QUP SPI driver not available\n");
	return NULL;
}

__WEAK int spi_qup_write(struct qup_spi_dev *dev, const unsigned char *tx_buf,
			 unsigned int data_size)
{
	dprintf(CRITICAL, "spi_qup_write: QUP SPI driver not available\n");
	return -ENODEV;
}

__WEAK int spi_qup_read(struct qup_spi_dev *dev, unsigned char *rx_buf,
			unsigned int data_size)
{
	dprintf(CRITICAL, "spi_qup_read: QUP SPI driver not available\n");
	return -ENODEV;
}
