#include "config.h"

struct sh_qspi_regs *regs = (struct sh_qspi_regs *)CONFIG_SH_QSPI_BASE;

int sh_qspi_xfer(
	unsigned char *tdata,
	unsigned char *rdata, unsigned long flags)
{
	int i=0;
	while (!(readb(&regs->spsr) & SH_QSPI_SPTEF)) {
		for(i=0; i<10000; i++);
	}

	writeb(*tdata, &regs->spdr);

	while (!(readb(&regs->spsr) & SH_QSPI_SPRFF)) {
		for(i=0; i<10000; i++);
	}

	*rdata = readb(&regs->spdr);

	return 0;
}

int sh_qspi_send(unsigned char *tdata, unsigned long flags)
{
	int i;
	while (!(readb(&regs->spsr) & SH_QSPI_SPTEF)) {
		for(i=0; i<10000; i++);
	}

	writeb(*tdata, &regs->spdr);

	return 0;
}

static int sh_qspi_recv(unsigned char *tdata, unsigned long flags)
{
	int i;
	while (!(readb(&regs->spsr) & SH_QSPI_SPRFF)) {
		for(i=0; i<10000; i++);
	}

	*tdata = readb(&regs->spdr);

	return 0;
}

int sh_qspi_xfer_quad(
	unsigned int cbyte, void *cmd,
	unsigned int dbyte, void *dout, void *din, unsigned long flags)
{
	unsigned char *tdata, ddata = 0;
	int ret = 0;
	int i;

	writeb(0x08, &regs->spcr);
	writew(0xe084, &regs->spcmd0);
	writew(0xe084, &regs->spcmd1);

	if (dout != NULL)
		writew(0xe044, &regs->spcmd2);
	else
		writew(0xe051, &regs->spcmd2);

	writeb(0xc0, &regs->spbfcr);
	writeb(0x00, &regs->spbfcr);
	writeb(0x02, &regs->spscr);
	writel(1, &regs->spbmul0);
	writel(cbyte - 1, &regs->spbmul1);
	writel(dbyte, &regs->spbmul2);
	writeb(0x48, &regs->spcr);

	/* command transfer */
	if (cmd != NULL)
		tdata = (unsigned char *)cmd;
	else
		return 1;

	while (cbyte > 0) {
		ret = sh_qspi_xfer(tdata, &ddata, flags);
		if (ret)
			break;
		tdata++;
		cbyte--;
	}

	/* data transfer */
	if (dout != NULL && din != NULL)
		uart_puts("sh_qspi_xfer_qread: " \
		       "full duplex is no supported\n");

	if (dout != NULL) {
		tdata = (unsigned char *)dout;

		while (dbyte > 0) {
			ret = sh_qspi_send(tdata, flags);
			if (ret)
				break;
			tdata++;
			dbyte--;
		}
		while ((readw(&regs->spbdcr) & 0x3f00)) {
			for(i=0; i<10000; i++);
		}
	} else if (din != NULL) {
		tdata = (unsigned char *)din;

		while (dbyte > 0) {
			ret = sh_qspi_recv(tdata, flags);
			if (ret)
				break;
			tdata++;
			dbyte--;
		}
	}

	return ret;
}


int spi_xfer_quad(
	unsigned int cmdlen, void *cmd,
	unsigned int datalen, void *dout, void *din, unsigned long flags)
{
	unsigned int cbyte, dbyte;
	int ret = 0;

	if (dout == NULL && din == NULL) {
		if (flags & SPI_XFER_END)
			writeb(0x08, &regs->spcr);
		return 0;
	}

	if (cmdlen % 8) {
		uart_puts("spi_xfer_quad: cmdlen is not 8bit alined\n");
		return 1;
	}

	if (datalen % 8) {
		uart_puts("spi_xfer_quad: datalen is not 8bit alined\n");
		return 1;
	}

	cbyte = cmdlen / 8;
	dbyte = datalen / 8;

	ret = sh_qspi_xfer_quad(cbyte, cmd, dbyte, dout, din, flags);

	return ret;
}


int sh_qspi_xfer_fast(
	unsigned int nbyte, const void *dout,
	void *din, unsigned long flags)
{
	unsigned char *tdata, *rdata, dtdata = 0, drdata;
	int ret = 0;

	if (flags & SPI_XFER_BEGIN) {
		writeb(0x08, &regs->spcr);
		writew(0xe084, &regs->spcmd0);
		writeb(0xc0, &regs->spbfcr);
		writeb(0x00, &regs->spbfcr);
		writeb(0x00, &regs->spscr);
	}

	writel(nbyte, &regs->spbmul0);
	writeb(0x48, &regs->spcr);

	if (dout != NULL)
		tdata = (unsigned char *)dout;
	else
		tdata = &dtdata;

	if (din != NULL)
		rdata = din;
	else
		rdata = &drdata;

	while (nbyte > 0) {
		ret = sh_qspi_xfer(tdata, rdata, flags);
		if (ret)
			break;

		if (dout != NULL)
			tdata++;

		if (din != NULL)
			rdata++;

		nbyte--;
	}

	if (flags & SPI_XFER_END)
		writeb(0x08, &regs->spcr);

	return ret;
}

int spi_xfer(
	unsigned int bitlen, const void *dout,
	void *din, unsigned long flags)
{
	unsigned int nbyte;
	int ret = 0;

	if (dout == NULL && din == NULL) {
		if (flags & SPI_XFER_END)
			writeb(0x08, &regs->spcr);
		return 0;
	}

	if (bitlen % 8) {
		uart_puts("spi_xfer: bitlen is not 8bit alined");
		return 1;
	}

	nbyte = bitlen / 8;

	ret = sh_qspi_xfer_fast(nbyte, dout, din, flags);

	return ret;
}

int spi_flash_read_write(
				u8 *cmd, size_t cmd_len,
				u8 *data_out, u8 *data_in,
				size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	switch (cmd[0]) {
	case CMD_READ_ARRAY_QUAD:
		ret = spi_xfer_quad(
			cmd_len * 8, cmd, data_len * 8, NULL, data_in,
			(SPI_XFER_BEGIN | SPI_XFER_END));
		return ret;
	case CMD_QUAD4_PAGE_PROGRAM:
		ret = spi_xfer_quad(
			cmd_len * 8, cmd, data_len * 8, data_out, NULL,
			(SPI_XFER_BEGIN | SPI_XFER_END));
		return ret;
	default:
		break;
	}

	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		uart_puts("SF: Failed to send command\n");
	} else if (data_len != 0) {
		ret = spi_xfer(data_len * 8, data_out, data_in, SPI_XFER_END);
		if (ret)
			uart_puts("SF: Failed to transfer\n");
	}

	return ret;
}

int spi_flash_cmd_read(u8 *cmd,	size_t cmd_len, void *data, size_t data_len)
{
	return spi_flash_read_write(cmd, cmd_len, NULL, data, data_len);
}

int spi_flash_cmd(u8 cmd, void *response, size_t len)
{
	return spi_flash_cmd_read(&cmd, 1, response, len);
}

int spi_flash_cmd_poll_bit(unsigned long timeout,
			   u8 cmd, u8 poll_bit)
{
	int ret;
	u8 status;

	ret = spi_xfer(8, &cmd, NULL, SPI_XFER_BEGIN);
	if (ret) {
		uart_puts("SF: Failed to send command \n");
		return ret;
	}
	
	do {
		ret = spi_xfer(8, NULL, &status, 0);
		if (ret)
			return -1;

		if ((status & poll_bit) == 0)
			break;

	} while (1);

	spi_xfer(0, NULL, NULL, SPI_XFER_END);

	if ((status & poll_bit) == 0)
		return 0;

	/* Timed out */
	uart_puts("SF: time out!\n");
	return -1;
}

int spi_flash_cmd_write_status_config(u8 *srcr)
{
	u8 cmd;
	int ret;

	ret = spi_flash_cmd(CMD_WRITE_ENABLE, NULL, 0);
	if (ret < 0) {
		uart_puts("SF: enabling write failed\n");
		return ret;
	}

	cmd = CMD_WRITE_STATUS;
	ret = spi_flash_read_write(cmd, 1, srcr, NULL, 2);
	if (ret) {
		uart_puts("SF: fail to write status and config register\n");
		return ret;
	}
	
	ret = spi_flash_cmd_poll_bit(SPI_FLASH_PROG_TIMEOUT, CMD_READ_STATUS, STATUS_WIP);
	if (ret < 0) {
		uart_puts("SF: write status register timed out\n");
		return ret;
	}

	return 0;
}

void spi_init(void)
{	
	u8 idcode[5];
	int ret;
	u8 cr, sr, srcr[2];
	
	/* QSPI initialize */
	writeb(0x08, &regs->spcr);
	writeb(0x00, &regs->sslp);
	writeb(0x06, &regs->sppcr);
	writeb(0x02, &regs->spbr);
	writeb(0x00, &regs->spdcr);
	writeb(0x00, &regs->spckd);
	writeb(0x00, &regs->sslnd);
	writeb(0x00, &regs->spnd);
	writew(0xe084, &regs->spcmd0);
	writew(0x8084, &regs->spcmd0);
	writeb(0xc0, &regs->spbfcr);
	writeb(0x30, &regs->spbfcr);
	writeb(0x00, &regs->spscr);
	writeb(0x48, &regs->spcr);
	
	/* Read the ID codes */
	ret = spi_flash_cmd(CMD_READ_ID, idcode, sizeof(idcode));
	if (ret)
		uart_puts("spi read id failed\n");
		
	/* enable quad transfer */
	spi_flash_cmd(CMD_READ_STATUS, &sr, 1);
	spi_flash_cmd(CMD_READ_CONFIG, &cr, 1);
	cr |= 0x02;
	srcr[0] = sr;
	srcr[1] = cr;
	if (spi_flash_cmd_write_status_config(srcr) < 0)
		uart_puts("spi_flash_cmd_write_status_config failed\n");
}

static void spi_flash_addr4(u32 addr, u8 *cmd)
{
	/* cmd[0] is actual command */
	cmd[1] = addr >> 24;
	cmd[2] = addr >> 16;
	cmd[3] = addr >> 8;
	cmd[4] = addr >> 0;
}
int spi_flash_read_common(const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
	int ret;

	ret = spi_flash_cmd_read(cmd, cmd_len, data, data_len);

	return ret;
}

int spi_flash_cmd_read_quad(u32 offset,
		size_t len, void *data)
{
	u8 cmd[6];

	cmd[0] = CMD_READ_ARRAY_QUAD;
	spi_flash_addr4(offset, cmd);
	cmd[5] = 0x00;

	return spi_flash_read_common(cmd, sizeof(cmd), data, len);
}

