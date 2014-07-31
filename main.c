#include "config.h"
#include "mmc.h"
#include "bockw_mmc_sd.h"

#define SDHI0_BASE		0xEE100000
#define SDHI1_BASE		0xEE140000
#define SDHI2_BASE		0xEE160000
struct bockw_esdhc_cfg esdhc_cfg[3] = {
	{SDHI0_BASE, 1, 1},
	{SDHI1_BASE, 1, 1},
	{SDHI2_BASE, 1, 1},
};
extern struct mmc *pmmc;

void* memcpy(void* pvTo, const void* pvFrom, size_t size)
{
	unsigned char* pbTo = (unsigned char*)pvTo;
	unsigned char* pbFrom = (unsigned char*)pvFrom;
	while(size-- > 0)
	{
		*pbTo++ = *pbFrom++;
	}
	return pvTo;
}


void boot_main(void)
{
#ifdef BOOT_FROM_SPI_NOR
	char buf[CONFIG_SPIREAD_BUF_LENGTH];
	u32 addr = CONFIG_UBOOT_SPI_OFFSET;
	int i, ret;
#endif
	char *p = (char *)CONFIG_UBOOT_RUN_ADDR;
	
	board_init();

	// device init
	uart_init();
	

	print("\nMINIBOOT VER--0.2\n");

#ifdef BOOT_FROM_SPI_NOR
	spi_init();

	for(i = 0; i < (196*1024/CONFIG_SPIREAD_BUF_LENGTH); i++) {
		ret = spi_flash_cmd_read_quad(addr, CONFIG_SPIREAD_BUF_LENGTH, buf);		
		
		if(ret){
			print("READ UBOOT FAILED\n");
			return;
		}

		memcpy(p, buf, CONFIG_SPIREAD_BUF_LENGTH);
		p += CONFIG_SPIREAD_BUF_LENGTH;
		addr += CONFIG_SPIREAD_BUF_LENGTH;
	}
	print("BOOT FROM SPI NOR FLASH(512K-708K)\n");
#else
	bockw_mmc_init( &esdhc_cfg[2] );
	mmc_init(pmmc);
	pmmc->block_dev.block_read(0, 2, 392, p); //download sd 1K-196K
	print("BOOT FROM SD(1K-196K)\n");
#endif

	print("READ UBOOT OK WILL START ...\n");
	(*((void(*)(void))CONFIG_UBOOT_RUN_ADDR))();
	
	return;
}
