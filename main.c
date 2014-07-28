#include "config.h"

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
	char *p = NULL;
	char buf[CONFIG_SPIREAD_BUF_LENGTH];
	u32 addr = CONFIG_UBOOT_SPI_OFFSET;
	p = (char *)CONFIG_UBOOT_RUN_ADDR;
	int i, ret;
	
	board_init();

	print("\nMINIBOOT VER--0.1\n");

	for(i = 0; i < (196*1024/CONFIG_SPIREAD_BUF_LENGTH); i++) {
		ret = spi_flash_cmd_read_quad(addr, CONFIG_SPIREAD_BUF_LENGTH, buf);		
		
		if(ret){
			print("Read Uboot Failed\n");
			return;
		}

		memcpy(p, buf, CONFIG_SPIREAD_BUF_LENGTH);
		p += CONFIG_SPIREAD_BUF_LENGTH;
		addr += CONFIG_SPIREAD_BUF_LENGTH;
	}

	print("Read Uboot OK\n");
	(*((void(*)(void))CONFIG_UBOOT_RUN_ADDR))();
	
	return;
}
