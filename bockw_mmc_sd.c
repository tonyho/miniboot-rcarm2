/*
 * (C) Copyright 2008
 * Texas Instruments, <www.ti.com>
 * Sukumar Ghorai <s-ghorai@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation's version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include "config.h"
#include "mmc.h"
#include "part.h"
#include "errno.h"
#include "bockw_mmc_sd.h"

static void udelay(u32 us)
{
	u32 i = 0;

	for(i=0; i<(us*100); i++);
}


static inline u16 sd_ctrl_read16( u32 base, int addr)
{
	return readw(base + addr);
}

static inline u32 sd_ctrl_read32( u32 base, int addr)
{
	return readw(base + addr) |
	       readw(base + addr + 2) << 16;
}

static int sh_mobile_sdhi_wait_idle( u32 base )
{
	int timeout = 1000;
	int delay = 0;

	while (--timeout && !(sd_ctrl_read16(base, CTL_STATUS2) & (1 << 13)))
		for(delay=0; delay<100000; delay++);

	if (!timeout) {
		print("timeout waiting for SD bus idle\n");
		return -EBUSY;
	}

	return 0;
}

static int sh_mobile_sdhi_write16_hook( u32 base, int addr)
{
	switch (addr)
	{
	case CTL_SD_CMD:
	case CTL_STOP_INTERNAL_ACTION:
	case CTL_XFER_BLK_COUNT:
	case CTL_SD_CARD_CLK_CTL:
	case CTL_SD_XFER_LEN:
	case CTL_SD_MEM_CARD_OPT:
	case CTL_TRANSACTION_CTL:
	case CTL_DMA_ENABLE:
		return sh_mobile_sdhi_wait_idle(base);
	}

	return 0;
}


static inline void sd_ctrl_write16( u32 base, int addr, u16 val)
{
	if( 0 != sh_mobile_sdhi_write16_hook(base, addr) ) 
		return;
	writew(val, base + addr);
}

static inline void sd_ctrl_write32( u32 base, int addr, u32 val)
{
	writew(val, base + addr);
	writew(val >> 16, base + addr + 2);
}

static void bockw_mmc_enable_mmc_irqs( u32 base, u32 i)
{
	sd_ctrl_write32( base, CTL_IRQ_MASK, sd_ctrl_read32(base, CTL_IRQ_MASK) & ~(i & TMIO_MASK_IRQ) );
}

static void bockw_mmc_disable_mmc_irqs( u32 base, u32 i)
{
	sd_ctrl_write32( base, CTL_IRQ_MASK, sd_ctrl_read32(base, CTL_IRQ_MASK) | (i & TMIO_MASK_IRQ) );	
}

static void bockw_mmc_ack_mmc_irqs( u32 base, u32 i)
{
	sd_ctrl_write32(base, CTL_STATUS, ~i);
}


#define ALL_ERROR (TMIO_STAT_CMD_IDX_ERR  |	 TMIO_STAT_CRCFAIL |\
		   TMIO_STAT_STOPBIT_ERR | TMIO_STAT_DATATIMEOUT |	\
		   TMIO_STAT_RXOVERFLOW | TMIO_STAT_TXUNDERRUN |	\
		   TMIO_STAT_CMDTIMEOUT | TMIO_STAT_ILL_ACCESS )

static int sdhi_intr( u32 base)
{
	unsigned long state = sd_ctrl_read32(base, CTL_STATUS);


	if (state & ALL_ERROR) {
		// 1.clean status about ALL_ERROR
		bockw_mmc_ack_mmc_irqs(base, ALL_ERROR);

		// 2. mask the irq ALL_ERROR
		bockw_mmc_disable_mmc_irqs(base, ALL_ERROR);
		
		// 3. do the ERROR operation			
		//printf("EINVAL state=0x%08x , ALL_ERROR=0x%08x\n", state, ALL_ERROR);		
		return WAIT_RESPONSE_ERROR;
	}
	if (state & TMIO_STAT_CMDRESPEND) {
		// 1.clean status about TMIO_STAT_CMDRESPEND
		bockw_mmc_ack_mmc_irqs(base, TMIO_STAT_CMDRESPEND);

		// 2. mask the irq TMIO_STAT_CMDRESPEND
		bockw_mmc_disable_mmc_irqs(base, TMIO_STAT_CMDRESPEND);
		
		// 3. do the cmd operation
		//printf("return WAIT_RESPONSE_CMD\n");
		return WAIT_RESPONSE_CMD;
	}
	if (state & TMIO_STAT_RXRDY) {
		// 1.clean status about TMIO_STAT_RXRDY
		bockw_mmc_ack_mmc_irqs(base, TMIO_STAT_RXRDY);

		// 2. mask the irq TMIO_STAT_RXRDY
		bockw_mmc_disable_mmc_irqs(base, TMIO_STAT_RXRDY);
		
		// 3. do the read operation
		//printf("return WAIT_RESPONSE_DATA_RX\n");
		return WAIT_RESPONSE_DATA_RX;
	}
	if (state & TMIO_STAT_TXRQ) {
		// 1.clean status about TMIO_STAT_TXRQ
		bockw_mmc_ack_mmc_irqs(base, TMIO_STAT_TXRQ);

		// 2. mask the irq TMIO_STAT_TXRQ
		bockw_mmc_disable_mmc_irqs(base, TMIO_STAT_TXRQ);
		
		// 3. do the write operation
		//printf("return WAIT_RESPONSE_DATA_TX\n");
		return WAIT_RESPONSE_DATA_TX;
	}
	if (state & TMIO_STAT_DATAEND) {
		// 1.clean status about TMIO_STAT_DATAEND
		bockw_mmc_ack_mmc_irqs(base, TMIO_STAT_DATAEND);

		// 2. mask the irq TMIO_STAT_DATAEND
		bockw_mmc_disable_mmc_irqs(base, TMIO_STAT_DATAEND);
		
		// 3. do the data end operation
		//printf("return WAIT_RESPONSE_DATA_END\n");
		return WAIT_RESPONSE_DATA_END;
	}

	return -EAGAIN;
}


static void sdhi_mmc_set_clock( struct mmc *mmc, int new_clock)
{
	u32 clk = 0, clock;
	struct bockw_esdhc_cfg *cfg = (struct bockw_esdhc_cfg *)mmc->priv;

	//kernel mmc->f_min = 0x17d78, but the SDCLK did not set 00(can not write)
	mmc->f_min = 0x17d78;
	if (new_clock) {
		for (clock = mmc->f_min, clk = 0x80000080;
			new_clock >= (clock<<1); clk >>= 1)
			clock <<= 1;
		clk |= 0x100;
	}
	sd_ctrl_write16(cfg->esdhc_base, CTL_SD_CARD_CLK_CTL, clk & 0x1ff);
}

static int sdhi_boot_wait_resp_end( u32 base )
{
	int err = -EAGAIN, timeout = 10000000;

	while (timeout--) {
		err = sdhi_intr(base);
		if (err != -EAGAIN)
			break;
		udelay(1);
	}
	
	return err;
}

/* SDHI_CLK_CTRL */
#define CLK_MMC_ENABLE                 (1 << 8)
#define CLK_MMC_INIT                   (1 << 6)        /* clk / 256 */
static void sdhi_boot_mmc_clk_stop( u32 base)
{
	sd_ctrl_write16(base, CTL_CLK_AND_WAIT_CTL, 0x0000);
	udelay(10000);
	sd_ctrl_write16(base, CTL_SD_CARD_CLK_CTL, ~CLK_MMC_ENABLE &
		sd_ctrl_read16(base, CTL_SD_CARD_CLK_CTL));
	udelay(10000);
}

static void sdhi_boot_mmc_clk_start( u32 base)
{
	sd_ctrl_write16(base, CTL_SD_CARD_CLK_CTL, CLK_MMC_ENABLE |
		sd_ctrl_read16(base, CTL_SD_CARD_CLK_CTL));
	udelay(10000);
	sd_ctrl_write16(base, CTL_CLK_AND_WAIT_CTL, CLK_MMC_ENABLE);
	udelay(10000);
}

static void sdhi_boot_reset( u32 base)
{
	sd_ctrl_write16(base, CTL_RESET_SD, 0x0000);
	udelay(10000);
	sd_ctrl_write16(base, CTL_RESET_SD, 0x0001);
	udelay(10000);
}

static int bockw_init(struct mmc *mmc)
{
	struct bockw_esdhc_cfg *cfg = (struct bockw_esdhc_cfg *)mmc->priv;

	sdhi_boot_mmc_clk_stop(cfg->esdhc_base);
	sdhi_boot_reset(cfg->esdhc_base);
	bockw_mmc_disable_mmc_irqs(cfg->esdhc_base, TMIO_MASK_ALL);
	
	return 0;
}

static int bockw_setup_data(struct mmc *mmc, struct mmc_data *data)
{
	struct bockw_esdhc_cfg *cfg = (struct bockw_esdhc_cfg *)mmc->priv;

	/* Set transfer length / blocksize */
	sd_ctrl_write16(cfg->esdhc_base, CTL_SD_XFER_LEN, data->blocksize);
	sd_ctrl_write16(cfg->esdhc_base, CTL_XFER_BLK_COUNT, data->blocks);

	return 0;
}

/* These are the bitmasks the tmio chip requires to implement the MMC response
 * types. Note that R1 and R6 are the same in this scheme. */
#define RESP_NONE      0x0300
#define RESP_R1        0x0400
#define RESP_R1B       0x0500
#define RESP_R2        0x0600
#define RESP_R3        0x0700
#define DATA_PRESENT   0x0800
#define TRANSFER_READ  0x1000
#define TRANSFER_MULTI 0x2000

static int bockw_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
			struct mmc_data *data)
{	
	int i, addr, err, c = cmd->cmdidx;
	struct bockw_esdhc_cfg *cfg = (struct bockw_esdhc_cfg *)mmc->priv;
	u32 irq_mask = TMIO_MASK_CMD;

	/* CMD12 is handled by hardware */
	if (cmd->cmdidx == MMC_CMD_STOP_TRANSMISSION && !cmd->cmdarg) {
		sd_ctrl_write16(cfg->esdhc_base, CTL_STOP_INTERNAL_ACTION, 0x001);
		return 0;
	}
		
	switch ( cmd->resp_type ) {
	case MMC_RSP_NONE: c |= RESP_NONE; break;
	case MMC_RSP_R1:   c |= RESP_R1;   break;
	case MMC_RSP_R1b:  c |= RESP_R1B;  break;
	case MMC_RSP_R2:   c |= RESP_R2;   break;
	case MMC_RSP_R3:   c |= RESP_R3;   break;
	default:
		print("Unknown response type %d\n");
		return -EINVAL;
	}

	if (data) {
		// Set blocksize and blocks
		bockw_setup_data(mmc, data);
		
		c |= DATA_PRESENT;		

		// clear LastData Block Transfer
		sd_ctrl_write16(cfg->esdhc_base, CTL_STOP_INTERNAL_ACTION, 0x000);
		
		if (data->blocks > 1) {			
			sd_ctrl_write16(cfg->esdhc_base, CTL_STOP_INTERNAL_ACTION, 0x100);
			c |= TRANSFER_MULTI;
		}
		if (data->flags & MMC_DATA_READ){			
			c |= TRANSFER_READ;
		}
	}

	// enable the command irq and clear status
	bockw_mmc_enable_mmc_irqs(cfg->esdhc_base, irq_mask);
	bockw_mmc_ack_mmc_irqs(cfg->esdhc_base, TMIO_STAT_CMDRESPEND);

	/* Fire off the command */
	sd_ctrl_write32(cfg->esdhc_base, CTL_ARG_REG, cmd->cmdarg);
	sd_ctrl_write16(cfg->esdhc_base, CTL_SD_CMD, c);

	// wait the command response end
	while(1) {
		err = sdhi_boot_wait_resp_end(cfg->esdhc_base);
		if ( WAIT_RESPONSE_CMD == err ) 
			break;
		
		if ( (-EAGAIN == err) || (WAIT_RESPONSE_ERROR == err) ) {
			print("wait command response error occur err:0x%x\n");
			return err;
		}
	}
		
	// get the response data
	for (i = 3, addr = CTL_RESPONSE ; i >= 0 ; i--, addr += 4) {
		cmd->response[i] = sd_ctrl_read32(cfg->esdhc_base, addr);
	}
	if (cmd->resp_type &  MMC_RSP_136) {
		cmd->response[0] = (cmd->response[0] << 8) | (cmd->response[1] >> 24);
		cmd->response[1] = (cmd->response[1] << 8) | (cmd->response[2] >> 24);
		cmd->response[2] = (cmd->response[2] << 8) | (cmd->response[3] >> 24);
		cmd->response[3] <<= 8;
	} else if (cmd->resp_type & MMC_RSP_R3) {
		cmd->response[0] = cmd->response[3];
	}

	//debug("cmd->response[0] = 0x%x\n",cmd->response[0]);
	//debug("cmd->response[1] = 0x%x\n",cmd->response[1]);
	//debug("cmd->response[2] = 0x%x\n",cmd->response[2]);
	//debug("cmd->response[3] = 0x%x\n",cmd->response[3]);
	
	/* Wait until all of the blocks are transferred */
	if (data) {
		int i = 0, j = 0;
		u16 *tmp_ptr = NULL;
		uint block_size = data->blocksize;
		uint block_cnt = data->blocks;	

		//enable the irq read and write
		bockw_mmc_enable_mmc_irqs(cfg->esdhc_base, TMIO_MASK_READOP | TMIO_MASK_WRITEOP);

		if (data->flags & MMC_DATA_READ) {
			tmp_ptr = (u16 *)data->dest;

			for (i = 0; i < (block_cnt); ++i) {
				// wait TMIO_STAT_RXRDY status
				while(1){
					err = sdhi_boot_wait_resp_end(cfg->esdhc_base);
					if ( WAIT_RESPONSE_DATA_RX == err ) {
						break;
					}
					
					if ( (-EAGAIN == err) || (WAIT_RESPONSE_ERROR == err) ) {
						print("read operation error occur err:0x%x\n");
						return err;
					}										
				}

				// read data for sd data port
				for (j = 0; j < (block_size/sizeof(*tmp_ptr)); ++j, ++tmp_ptr) {
					*tmp_ptr = sd_ctrl_read16(cfg->esdhc_base, CTL_SD_DATA_PORT);
				}						
			}
		}
		else {
			tmp_ptr = (u16 *)data->src;

			for (i = 0; i < (block_cnt); ++i) {
				// wait TMIO_STAT_TXRQ status
				while(1){
					err = sdhi_boot_wait_resp_end(cfg->esdhc_base);
					if ( WAIT_RESPONSE_DATA_TX == err ) {
						break;
					}
					
					if ( (-EAGAIN == err) || (WAIT_RESPONSE_ERROR == err) ) {
						print("write operation error occur err:0x%x\n");
						return err;
					}										
				}

				// write data to sd data port
				for (j = 0; j < (block_size/sizeof(*tmp_ptr)); ++j, ++tmp_ptr) {
					sd_ctrl_write16(cfg->esdhc_base, CTL_SD_DATA_PORT, *tmp_ptr);
				}									
			}
		}


		while(1){
			// wait data end
			err = sdhi_boot_wait_resp_end(cfg->esdhc_base);
			if ( WAIT_RESPONSE_DATA_END == err ) {				
				break;
			}
			
			if ( (-EAGAIN == err) || (WAIT_RESPONSE_ERROR == err) ) {
				print("data end operation error occur err:0x%x\n");
				return err;
			}										
		}		

		//Disable irq read and write 
		bockw_mmc_disable_mmc_irqs(cfg->esdhc_base, TMIO_MASK_READOP | TMIO_MASK_WRITEOP);
	}

	return 0;

}

static void bockw_set_ios(struct mmc *mmc)
{
	struct bockw_esdhc_cfg *cfg = (struct bockw_esdhc_cfg *)mmc->priv;

	/* Set the clock speed */
	if (mmc->clock) {
		sdhi_mmc_set_clock(mmc, mmc->clock);
		sdhi_boot_mmc_clk_start(cfg->esdhc_base);
	}
	else{
		sdhi_boot_mmc_clk_stop(cfg->esdhc_base);
	}

	/* configue bus width */
	switch (mmc->bus_width) {
	case 4:
		sd_ctrl_write16(cfg->esdhc_base, CTL_SD_MEM_CARD_OPT, 0x00e0);
		break;

	case 1:
	default:
		sd_ctrl_write16(cfg->esdhc_base, CTL_SD_MEM_CARD_OPT, 0x80e0);
		break;
	}	
}

struct mmc *pmmc;
struct mmc smmc;

int bockw_mmc_init(struct bockw_esdhc_cfg *cfg)
{
	pmmc = &smmc;

	if (!cfg)
		return -1;

	pmmc->priv 		= cfg;
	pmmc->send_cmd 	= bockw_send_cmd;
	pmmc->set_ios 	= bockw_set_ios;
	pmmc->init 		= bockw_init;

	pmmc->voltages 	= MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;
	pmmc->host_caps 	= MMC_MODE_4BIT | MMC_MODE_HS_52MHz | MMC_MODE_HS;

	pmmc->f_min 		= 400000;
	pmmc->f_max 		= 52000000;

	mmc_register(pmmc);

	return 0;
}

