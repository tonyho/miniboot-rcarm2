#include "config.h"

#define SCIF0_BASE		0xe6e60000
#define SCIF_BASE		SCIF0_BASE
#define SCIF_BASE_PORT	PORT_SCIF

static struct uart_port sh_sci = {
	.membase	= (unsigned char*)SCIF_BASE,
	.mapbase	= SCIF_BASE,
	.type		= SCIF_BASE_PORT,
};

void uart_init(void)
{
	sci_out(&sh_sci, SCSCR , 0x01);

	sci_out(&sh_sci, SCFCR, SCFCR_RFRST|SCFCR_TFRST);
	sci_in(&sh_sci, SCxSR);
	sci_out(&sh_sci, SCxSR, 0);
	sci_out(&sh_sci, SCSMR, 0);
	sci_out(&sh_sci, SCSMR, 0);
	sci_out(&sh_sci, SCSCR , 0x01);
	
	sci_out(&sh_sci, SCBRR, 0x34);

	sci_out(&sh_sci, DL,    0x8D);
	sci_out(&sh_sci, CKS,   0x4000);	
	
	sci_out(&sh_sci, SCFCR, 0);
	sci_out(&sh_sci, SCSCR , SCSCR_INIT(&sh_sci));
	sci_out(&sh_sci, SCSCR , SCSCR_INIT(&sh_sci));
}


void serial_raw_putc(const char c)
{
	while (1) {
		/* Tx fifo is empty */
		if (sci_in(&sh_sci, SCxSR) & SCxSR_TEND(&sh_sci))
			break;
	}

	sci_out(&sh_sci, SCxTDR, c);
	sci_out(&sh_sci, SCxSR, sci_in(&sh_sci, SCxSR) & ~SCxSR_TEND(&sh_sci));
}

void uart_putc(const char c)
{
	if (c == '\n')
		serial_raw_putc('\r');
	serial_raw_putc(c);
}

void print(const char *s)
{
	while (*s) {
		uart_putc(*s++);
	}
}

void handle_error(void)
{
	sci_in(&sh_sci, SCxSR);
	sci_out(&sh_sci, SCxSR, SCxSR_ERROR_CLEAR(&sh_sci));
	sci_in(&sh_sci, SCLSR);
	sci_out(&sh_sci, SCLSR, 0x00);
}

int serial_getc_check(void)
{
	unsigned short status;

	status = sci_in(&sh_sci, SCxSR);

	if (status & SCIF_ERRORS)
		handle_error();
	if (sci_in(&sh_sci, SCLSR) & SCxSR_ORER(&sh_sci))
		handle_error();
	return status & (SCIF_DR | SCxSR_RDxF(&sh_sci));
}

int sh_serial_getc(void)
{
	unsigned short status;
	char ch;

	while (!serial_getc_check())
		;

	ch = sci_in(&sh_sci, SCxRDR);
	status = sci_in(&sh_sci, SCxSR);

	sci_out(&sh_sci, SCxSR, SCxSR_RDxF_CLEAR(&sh_sci));

	if (status & SCIF_ERRORS)
			handle_error();

	if (sci_in(&sh_sci, SCLSR) & SCxSR_ORER(&sh_sci))
		handle_error();
	return ch;
}