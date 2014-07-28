#include "global_cfg.h"

typedef unsigned int 	u32;
typedef unsigned short 	u16;
typedef unsigned char 	u8;
typedef unsigned int 	size_t;

#define NULL ((void *)0)

#define writel(v,a)		(*(volatile u32 *)(a) = (v))
#define writew(v,a)		(*(volatile u16 *)(a) = (v))
#define writeb(v,a)		(*(volatile u8  *)(a) = (v))

#define readl(a)		(*(volatile u32 *)(a))
#define readw(a)		(*(volatile u16 *)(a))
#define readb(a)		(*(volatile u8 *)(a))

//Function declare=======================================================================
void board_init(void);

void uart_init(void);
void print(const char *s);

void spi_init(void);
int spi_flash_cmd_read_quad(u32 offset,size_t len, void *data);

//REG ADDR===============================================================================
#define	RWDT_BASE				0xE6020000
#define RWTCSRA					(RWDT_BASE + 0x4)

#define	SWDT_BASE				0xE6030000
#define SWTCSRA					(SWDT_BASE + 0x4)

#define PFC_BASE                0xE6060000
#define PFC_PMMR                PFC_BASE + 0
#define PFC_GPSR0               PFC_BASE + 0x4
#define PFC_GPSR1               PFC_BASE + 0x8
#define PFC_GPSR2               PFC_BASE + 0xC
#define PFC_GPSR3               PFC_BASE + 0x10
#define PFC_GPSR4               PFC_BASE + 0x14
#define PFC_GPSR5               PFC_BASE + 0x18
#define PFC_GPSR6               PFC_BASE + 0x1C
#define PFC_GPSR7               PFC_BASE + 0x74
#define PFC_IPSR0               PFC_BASE + 0x20
#define PFC_IPSR1               PFC_BASE + 0x24
#define PFC_IPSR2               PFC_BASE + 0x28
#define PFC_IPSR3               PFC_BASE + 0x2C
#define PFC_IPSR4               PFC_BASE + 0x30
#define PFC_IPSR5               PFC_BASE + 0x34
#define PFC_IPSR6               PFC_BASE + 0x38
#define PFC_IPSR7               PFC_BASE + 0x3C
#define PFC_IPSR8               PFC_BASE + 0x40
#define PFC_IPSR9               PFC_BASE + 0x44
#define PFC_IPSR10              PFC_BASE + 0x48
#define PFC_IPSR11              PFC_BASE + 0x4C
#define PFC_IPSR12              PFC_BASE + 0x50
#define PFC_IPSR13              PFC_BASE + 0x54
#define PFC_IPSR14              PFC_BASE + 0x58
#define PFC_IPSR15              PFC_BASE + 0x5C
#define PFC_IPSR16              PFC_BASE + 0x160
#define PFC_MOD_SEL             PFC_BASE + 0x90
#define PFC_MOD_SEL2            PFC_BASE + 0x94
#define PFC_MOD_SEL3            PFC_BASE + 0x98
#define PFC_MOD_SEL4            PFC_BASE + 0x9C
#define PFC_PUPR0               PFC_BASE + 0x100
#define PFC_PUPR1               PFC_BASE + 0x104
#define PFC_PUPR2               PFC_BASE + 0x108
#define PFC_PUPR3               PFC_BASE + 0x10C
#define PFC_PUPR4               PFC_BASE + 0x110
#define PFC_PUPR5               PFC_BASE + 0x114
#define PFC_PUPR6               PFC_BASE + 0x118
#define PFC_PUPR7               PFC_BASE + 0x11C
#define PFC_IOCTRL0             PFC_BASE + 0x60
#define PFC_IOCTRL1             PFC_BASE + 0x64
#define PFC_IOCTRL4             PFC_BASE + 0x84
#define PFC_IOCTRL5             PFC_BASE + 0x88
#define PFC_IOCTRL6             PFC_BASE + 0x8C
#define PFC_IOCTRL7             PFC_BASE + 0x70

#define GPIO_BASE               0xE6050000
#define GPIO_0_BASE             GPIO_BASE + 0x0
#define GPIO_1_BASE             GPIO_BASE + 0x1000
#define GPIO_2_BASE             GPIO_BASE + 0x2000
#define GPIO_3_BASE             GPIO_BASE + 0x3000
#define GPIO_4_BASE             GPIO_BASE + 0x4000
#define GPIO_5_BASE             GPIO_BASE + 0x5000
#define GPIO_6_BASE             GPIO_BASE + 0x5400
#define GPIO_7_BASE             GPIO_BASE + 0x5800
#define GPIO_IOINTSEL(pfx)      GPIO_##pfx##_BASE + 0x0
#define GPIO_INOUTSEL(pfx)      GPIO_##pfx##_BASE + 0x4
#define GPIO_OUTDT(pfx)         GPIO_##pfx##_BASE + 0x8
#define GPIO_POSNEG(pfx)        GPIO_##pfx##_BASE + 0x20

#define DBSC3_0_BASE			0xE6790000
#define DBSC3_1_BASE			0xE67A0000
#define DBSC3_0_DBACEN			(DBSC3_0_BASE + 0x10)
#define DBSC3_1_DBACEN			(DBSC3_1_BASE + 0x10)
#define DBSC3_0_DBRFEN			(DBSC3_0_BASE + 0x14)
#define DBSC3_1_DBRFEN			(DBSC3_1_BASE + 0x14)
#define DBSC3_0_DBCMD			(DBSC3_0_BASE + 0x18)
#define DBSC3_1_DBCMD			(DBSC3_1_BASE + 0x18)
#define DBSC3_0_DBKIND			(DBSC3_0_BASE + 0x20)
#define DBSC3_1_DBKIND			(DBSC3_1_BASE + 0x20)
#define DBSC3_0_DBCONF0			(DBSC3_0_BASE + 0x24)
#define DBSC3_1_DBCONF0			(DBSC3_1_BASE + 0x24)
#define DBSC3_0_PHYT			(DBSC3_0_BASE + 0x30)
#define DBSC3_1_PHYT			(DBSC3_1_BASE + 0x30)
#define DBSC3_0_DBTR0			(DBSC3_0_BASE + 0x40)
#define DBSC3_1_DBTR0			(DBSC3_1_BASE + 0x40)
#define DBSC3_0_DBTR1			(DBSC3_0_BASE + 0x44)
#define DBSC3_1_DBTR1			(DBSC3_1_BASE + 0x44)
#define DBSC3_0_DBTR2			(DBSC3_0_BASE + 0x48)
#define DBSC3_1_DBTR2			(DBSC3_1_BASE + 0x48)
#define DBSC3_0_DBTR3			(DBSC3_0_BASE + 0x50)
#define DBSC3_1_DBTR3			(DBSC3_1_BASE + 0x50)
#define DBSC3_0_DBTR4			(DBSC3_0_BASE + 0x54)
#define DBSC3_1_DBTR4			(DBSC3_1_BASE + 0x54)
#define DBSC3_0_DBTR5			(DBSC3_0_BASE + 0x58)
#define DBSC3_1_DBTR5			(DBSC3_1_BASE + 0x58)
#define DBSC3_0_DBTR6			(DBSC3_0_BASE + 0x5C)
#define DBSC3_1_DBTR6			(DBSC3_1_BASE + 0x5C)
#define DBSC3_0_DBTR7			(DBSC3_0_BASE + 0x60)
#define DBSC3_1_DBTR7			(DBSC3_1_BASE + 0x60)
#define DBSC3_0_DBTR8			(DBSC3_0_BASE + 0x64)
#define DBSC3_1_DBTR8			(DBSC3_1_BASE + 0x64)
#define DBSC3_0_DBTR9			(DBSC3_0_BASE + 0x68)
#define DBSC3_1_DBTR9			(DBSC3_1_BASE + 0x68)
#define DBSC3_0_DBTR10			(DBSC3_0_BASE + 0x6C)
#define DBSC3_1_DBTR10			(DBSC3_1_BASE + 0x6C)
#define DBSC3_0_DBTR11			(DBSC3_0_BASE + 0x70)
#define DBSC3_1_DBTR11			(DBSC3_1_BASE + 0x70)
#define DBSC3_0_DBTR12			(DBSC3_0_BASE + 0x74)
#define DBSC3_1_DBTR12			(DBSC3_1_BASE + 0x74)
#define DBSC3_0_DBTR13			(DBSC3_0_BASE + 0x78)
#define DBSC3_1_DBTR13			(DBSC3_1_BASE + 0x78)
#define DBSC3_0_DBTR14			(DBSC3_0_BASE + 0x7C)
#define DBSC3_1_DBTR14			(DBSC3_1_BASE + 0x7C)
#define DBSC3_0_DBTR15			(DBSC3_0_BASE + 0x80)
#define DBSC3_1_DBTR15			(DBSC3_1_BASE + 0x80)
#define DBSC3_0_DBTR16			(DBSC3_0_BASE + 0x84)
#define DBSC3_1_DBTR16			(DBSC3_1_BASE + 0x84)
#define DBSC3_0_DBTR17			(DBSC3_0_BASE + 0x88)
#define DBSC3_1_DBTR17			(DBSC3_1_BASE + 0x88)
#define DBSC3_0_DBTR18			(DBSC3_0_BASE + 0x8C)
#define DBSC3_1_DBTR18			(DBSC3_1_BASE + 0x8C)
#define DBSC3_0_DBTR19			(DBSC3_0_BASE + 0x90)
#define DBSC3_1_DBTR19			(DBSC3_1_BASE + 0x90)
#define DBSC3_0_DBBL			(DBSC3_0_BASE + 0xB0)
#define DBSC3_1_DBBL			(DBSC3_1_BASE + 0xB0)
#define DBSC3_0_DBADJ0			(DBSC3_0_BASE + 0xC0)
#define DBSC3_1_DBADJ0			(DBSC3_1_BASE + 0xC0)
#define DBSC3_0_DBADJ2			(DBSC3_0_BASE + 0xC8)
#define DBSC3_1_DBADJ2			(DBSC3_1_BASE + 0xC8)
#define DBSC3_0_DBRFCNF0		(DBSC3_0_BASE + 0xE0)
#define DBSC3_1_DBRFCNF0		(DBSC3_1_BASE + 0xE0)
#define DBSC3_0_DBRFCNF1		(DBSC3_0_BASE + 0xE4)
#define DBSC3_1_DBRFCNF1		(DBSC3_1_BASE + 0xE4)
#define DBSC3_0_DBRFCNF2		(DBSC3_0_BASE + 0xE8)
#define DBSC3_1_DBRFCNF2		(DBSC3_1_BASE + 0xE8)
#define DBSC3_0_DBCALCNF		(DBSC3_0_BASE + 0xF4)
#define DBSC3_1_DBCALCNF		(DBSC3_1_BASE + 0xF4)
#define DBSC3_0_DBCALTR			(DBSC3_0_BASE + 0xF8)
#define DBSC3_1_DBCALTR			(DBSC3_1_BASE + 0xF8)
#define DBSC3_0_DBRNK0			(DBSC3_0_BASE + 0x100)
#define DBSC3_1_DBRNK0			(DBSC3_1_BASE + 0x100)
#define DBSC3_0_DBDFISTAT		(DBSC3_0_BASE + 0x240)
#define DBSC3_1_DBDFISTAT		(DBSC3_1_BASE + 0x240)
#define DBSC3_0_DBDFICNT		(DBSC3_0_BASE + 0x244)
#define DBSC3_1_DBDFICNT		(DBSC3_1_BASE + 0x244)
#define DBSC3_0_DBPDLCK			(DBSC3_0_BASE + 0x280)
#define DBSC3_1_DBPDLCK			(DBSC3_1_BASE + 0x280)
#define DBSC3_0_DBPDRGA			(DBSC3_0_BASE + 0x290)
#define DBSC3_1_DBPDRGA			(DBSC3_1_BASE + 0x290)
#define DBSC3_0_DBPDRGD			(DBSC3_0_BASE + 0x2A0)
#define DBSC3_1_DBPDRGD			(DBSC3_1_BASE + 0x2A0)
#define DBSC3_0_DBBS0CNT1		(DBSC3_0_BASE + 0x304)
#define DBSC3_1_DBBS0CNT1		(DBSC3_1_BASE + 0x304)
#define DBSC3_0_DBWT0CNF0		(DBSC3_0_BASE + 0x380)
#define DBSC3_1_DBWT0CNF0		(DBSC3_1_BASE + 0x380)
#define DBSC3_0_DBWT0CNF4		(DBSC3_0_BASE + 0x390)
#define DBSC3_1_DBWT0CNF4		(DBSC3_1_BASE + 0x390)


#define	MSTPSR1					0xE6150038
#define	SMSTPCR1				0xE6150134
#define TMU0_MSTP125			(1 << 25)

#define MSTPSR3					0xE6150048
#define SMSTPCR3				0xE615011C
#define MMC0_MSTP315			(1 << 15)
#define SDHI0_MSTP314			(1 << 14)
#define SDHI1_MSTP312			(1 << 12)
#define SDHI2_MSTP311			(1 << 11)

#define	MSTPSR7					0xE61501C4
#define	SMSTPCR7				0xE615014C
#define SCIF0_MSTP721			(1 << 21)

#define	MSTPSR8					0xE61509A0
#define	SMSTPCR8				0xE6150990
#define ETHER_MSTP813			(1 << 13)

#define	PMMR					0xE6060000
#define	GPSR4					0xE6060014
#define	IPSR14					0xE6060058
//UART INC=============================================================================== 
/*
 * Copy and modify from linux/drivers/serial/sh-sci.h
 */

struct uart_port {
	unsigned long	iobase;		/* in/out[bwl] */
	unsigned char	*membase;	/* read/write[bwl] */
	unsigned long	mapbase;	/* for ioremap */
	unsigned int	type;		/* port type */
};

#define PORT_SCI	52
#define PORT_SCIF	53
#define PORT_SCIFA	83
#define PORT_SCIFB	93

//#elif defined(CONFIG_R8A7790) || defined(CONFIG_R8A7791)
# define SCIF_ORER	0x0001
#if 0 // 外部クロック
# define SCSCR_INIT(port)	0x32	/* TIE=0,RIE=0,TE=1,RE=1,REIE=0, */
#else
# define SCSCR_INIT(port)	0x31	/* TIE=0,RIE=0,TE=1,RE=1,REIE=0,CLK=input */
#endif
//#endif

/* SCSCR */
#define SCI_CTRL_FLAGS_TIE  0x80 /* all */
#define SCI_CTRL_FLAGS_RIE  0x40 /* all */
#define SCI_CTRL_FLAGS_TE   0x20 /* all */
#define SCI_CTRL_FLAGS_RE   0x10 /* all */
#define SCI_CTRL_FLAGS_REIE 0

/* SCxSR SCI */
#define SCI_TDRE  0x80 /* 7707 SCI, 7708 SCI, 7709 SCI, 7750 SCI */
#define SCI_RDRF  0x40 /* 7707 SCI, 7708 SCI, 7709 SCI, 7750 SCI */
#define SCI_ORER  0x20 /* 7707 SCI, 7708 SCI, 7709 SCI, 7750 SCI */
#define SCI_FER   0x10 /* 7707 SCI, 7708 SCI, 7709 SCI, 7750 SCI */
#define SCI_PER   0x08 /* 7707 SCI, 7708 SCI, 7709 SCI, 7750 SCI */
#define SCI_TEND  0x04 /* 7707 SCI, 7708 SCI, 7709 SCI, 7750 SCI */

#define SCI_ERRORS ( SCI_PER | SCI_FER | SCI_ORER)

/* SCxSR SCIF */
#define SCIF_ER    0x0080 /* 7705 SCIF, 7707 SCIF, 7709 SCIF, 7750 SCIF */
#define SCIF_TEND  0x0040 /* 7705 SCIF, 7707 SCIF, 7709 SCIF, 7750 SCIF */
#define SCIF_TDFE  0x0020 /* 7705 SCIF, 7707 SCIF, 7709 SCIF, 7750 SCIF */
#define SCIF_BRK   0x0010 /* 7705 SCIF, 7707 SCIF, 7709 SCIF, 7750 SCIF */
#define SCIF_FER   0x0008 /* 7705 SCIF, 7707 SCIF, 7709 SCIF, 7750 SCIF */
#define SCIF_PER   0x0004 /* 7705 SCIF, 7707 SCIF, 7709 SCIF, 7750 SCIF */
#define SCIF_RDF   0x0002 /* 7705 SCIF, 7707 SCIF, 7709 SCIF, 7750 SCIF */
#define SCIF_DR    0x0001 /* 7705 SCIF, 7707 SCIF, 7709 SCIF, 7750 SCIF */

//#elif defined(CONFIG_R8A7790) || defined(CONFIG_R8A7791)
# define SCIF_ERRORS (SCIF_PER | SCIF_FER | SCIF_ER | SCIF_BRK)
# define SCIF_RFDC_MASK	0x003f

//#endif

#ifndef SCIF_ORER
#define SCIF_ORER	0x0000
#endif

#define SCxSR_TEND(port)\
		(((port)->type == PORT_SCI) ? SCI_TEND	: SCIF_TEND)
#define SCxSR_ERRORS(port)\
		(((port)->type == PORT_SCI) ? SCI_ERRORS : SCIF_ERRORS)
#define SCxSR_RDxF(port)\
		(((port)->type == PORT_SCI) ? SCI_RDRF	: SCIF_RDF)
#define SCxSR_TDxE(port)\
		(((port)->type == PORT_SCI) ? SCI_TDRE	: SCIF_TDFE)
#define SCxSR_FER(port)\
		(((port)->type == PORT_SCI) ? SCI_FER	: SCIF_FER)
#define SCxSR_PER(port)\
		(((port)->type == PORT_SCI) ? SCI_PER	: SCIF_PER)
#define SCxSR_BRK(port)\
		((port)->type == PORT_SCI) ? 0x00		: SCIF_BRK)
#define SCxSR_ORER(port)\
		(((port)->type == PORT_SCI) ? SCI_ORER	: SCIF_ORER)


# define SCxSR_RDxF_CLEAR(port)	 (((port)->type == PORT_SCI) ? 0xbc : 0x00fc)
# define SCxSR_ERROR_CLEAR(port) (((port)->type == PORT_SCI) ? 0xc4 : 0x0073)
# define SCxSR_TDxE_CLEAR(port)  (((port)->type == PORT_SCI) ? 0x78 : 0x00df)
# define SCxSR_BREAK_CLEAR(port) (((port)->type == PORT_SCI) ? 0xc4 : 0x00e3)


/* SCFCR */
#define SCFCR_RFRST 0x0002
#define SCFCR_TFRST 0x0004
#define SCFCR_TCRST 0x4000
#define SCFCR_MCE   0x0008

#define SCI_MAJOR		204
#define SCI_MINOR_START		8

/* Generic serial flags */
#define SCI_RX_THROTTLE		0x0000001

#define SCI_MAGIC 0xbabeface

/*
 * Events are used to schedule things to happen at timer-interrupt
 * time, instead of at rs interrupt time.
 */
#define SCI_EVENT_WRITE_WAKEUP	0

#define SCI_IN(size, offset)\
	if ((size) == 8) {\
		return readb(port->membase + (offset));\
	} else {\
		return readw(port->membase + (offset));\
	}
#define SCI_OUT(size, offset, value)\
	if ((size) == 8) {\
		writeb(value, port->membase + (offset));\
	} else if ((size) == 16) {\
		writew(value, port->membase + (offset));\
	}

#define CPU_SCIx_FNS(name, sci_offset, sci_size, scif_offset, scif_size)\
	static inline unsigned int sci_##name##_in(struct uart_port *port) {\
		if (port->type == PORT_SCIF || port->type == PORT_SCIFB) {\
			SCI_IN(scif_size, scif_offset)\
		} else { /* PORT_SCI or PORT_SCIFA */\
			SCI_IN(sci_size, sci_offset);\
		}\
	}\
static inline void sci_##name##_out(struct uart_port *port,\
				unsigned int value) {\
	if (port->type == PORT_SCIF || port->type == PORT_SCIFB) {\
		SCI_OUT(scif_size, scif_offset, value)\
	} else {	/* PORT_SCI or PORT_SCIFA */\
		SCI_OUT(sci_size, sci_offset, value);\
	}\
}


#define CPU_SCIF_FNS(name, scif_offset, scif_size)			\
	static inline unsigned int sci_##name##_in(struct uart_port *port) {\
		SCI_IN(scif_size, scif_offset);\
	}\
	static inline void sci_##name##_out(struct uart_port *port,\
					unsigned int value) {\
		SCI_OUT(scif_size, scif_offset, value);\
	}

#define CPU_SCI_FNS(name, sci_offset, sci_size)\
	static inline unsigned int sci_##name##_in(struct uart_port *port) {\
		SCI_IN(sci_size, sci_offset);\
	}\
	static inline void sci_##name##_out(struct uart_port *port,\
					unsigned int value) {\
		SCI_OUT(sci_size, sci_offset, value);\
	}

#define SCIx_FNS(name, sh3_sci_offset, sh3_sci_size,\
				sh4_sci_offset, sh4_sci_size, \
				sh3_scif_offset, sh3_scif_size,\
				sh4_scif_offset, sh4_scif_size, \
				h8_sci_offset, h8_sci_size) \
	CPU_SCIx_FNS(name, sh4_sci_offset, sh4_sci_size,\
					sh4_scif_offset, sh4_scif_size)
#define SCIF_FNS(name, sh3_scif_offset, sh3_scif_size, \
				sh4_scif_offset, sh4_scif_size) \
	CPU_SCIF_FNS(name, sh4_scif_offset, sh4_scif_size)


/*      reg      SCI/SH3   SCI/SH4  SCIF/SH3   SCIF/SH4  SCI/H8*/
/*      name     off  sz   off  sz   off  sz   off  sz   off  sz*/
SCIx_FNS(SCSMR,  0x00,  8, 0x00,  8, 0x00,  8, 0x00, 16, 0x00,  8)
SCIx_FNS(SCBRR,  0x02,  8, 0x04,  8, 0x02,  8, 0x04,  8, 0x01,  8)
SCIx_FNS(SCSCR,  0x04,  8, 0x08,  8, 0x04,  8, 0x08, 16, 0x02,  8)
SCIx_FNS(SCxTDR, 0x06,  8, 0x0c,  8, 0x06,  8, 0x0C,  8, 0x03,  8)
SCIx_FNS(SCxSR,  0x08,  8, 0x10,  8, 0x08, 16, 0x10, 16, 0x04,  8)
SCIx_FNS(SCxRDR, 0x0a,  8, 0x14,  8, 0x0A,  8, 0x14,  8, 0x05,  8)
SCIF_FNS(SCFCR,                      0x0c,  8, 0x18, 16)
SCIF_FNS(SCFDR,                      0x0e, 16, 0x1C, 16)
SCIF_FNS(SCSPTR,                        0,  0, 0x20, 16)
//#if defined(CONFIG_R8A7790) || defined(CONFIG_R8A7791)
SCIF_FNS(DL,				0,  0, 0x30, 16)
SCIF_FNS(CKS,				0,  0, 0x34, 16)
//#endif
SCIF_FNS(SCLSR,                         0,  0, 0x24, 16)
#define sci_in(port, reg) sci_##reg##_in(port)
#define sci_out(port, reg, value) sci_##reg##_out(port, value)

#define DL_VALUE(bps, clk)	(clk / bps / 16)
#define SCBRR_VALUE(bps, clk) ((clk+16*bps)/(32*bps)-1)


//SPI FLASH INC===============================================================================
#define CONFIG_SH_QSPI_BASE			0xE6B10000
#define SPI_FLASH_PROG_TIMEOUT		(2 * 1000)

struct sh_qspi_regs {
	unsigned char spcr;
	unsigned char sslp;
	unsigned char sppcr;
	unsigned char spsr;
	unsigned long spdr;
	unsigned char spscr;
	unsigned char spssr;
	unsigned char spbr;
	unsigned char spdcr;
	unsigned char spckd;
	unsigned char sslnd;
	unsigned char spnd;
	unsigned char dummy0;
	unsigned short spcmd0;
	unsigned short spcmd1;
	unsigned short spcmd2;
	unsigned short spcmd3;
	unsigned char spbfcr;
	unsigned char dummy1;
	unsigned short spbdcr;
	unsigned int spbmul0;
	unsigned int spbmul1;
	unsigned int spbmul2;
	unsigned int spbmul3;
};
#define SH_QSPI_SPRFF	0x80
#define SH_QSPI_SPTEF	0x20

/* Common commands */
#define CMD_READ_ID			0x9f

#define CMD_READ_ARRAY_SLOW		0x03
#define CMD_READ_ARRAY_FAST		0x0b
#define CMD_READ_ARRAY_QUAD		0x6c

#define CMD_WRITE_STATUS		0x01
#define CMD_PAGE_PROGRAM		0x02
#define CMD_WRITE_DISABLE		0x04
#define CMD_READ_STATUS			0x05
#define CMD_WRITE_ENABLE		0x06
#define CMD_ERASE_4K			0x20
#define CMD_QUAD4_PAGE_PROGRAM		0x34
#define CMD_READ_CONFIG			0x35
#define CMD_ERASE_32K			0x52
#define CMD_ERASE_64K			0xd8
#define CMD_ERASE_CHIP			0xc7
#define CMD_ERASE_256K			0xdc

/* Common status */
#define STATUS_WIP			0x01

/* SPI transfer flags */
#define SPI_XFER_BEGIN	0x01			/* Assert CS before transfer */
#define SPI_XFER_END	0x02			/* Deassert CS after transfer */