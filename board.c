#include "config.h"

void system_init(void)
{
	/* Watchdog init */
	writel(0xA5A5A500, RWTCSRA);
	writel(0xA5A5A500, SWTCSRA);
}

void clock_init(void)
{
	// CPG register configuration block
	u32 val;

	/* TMU0 */
	val = readl(MSTPSR1);
	val &= ~TMU0_MSTP125;
	writel(val, SMSTPCR1);

	val = readl(MSTPSR7); // CPG
	val &= ~SCIF0_MSTP721;
	writel(val, SMSTPCR7);

	/* ETHER */
	val = readl(MSTPSR8);
	val &= ~ETHER_MSTP813;
	writel(val, SMSTPCR8);

	/* SD */
	val = readl(MSTPSR3);
	val &= ~(SDHI0_MSTP314 | SDHI1_MSTP312 | SDHI2_MSTP311);
	writel(val, SMSTPCR3);
}

void pio_pin_mux(void)
{
#if 1
	writel(~0x04D40008 , PFC_PMMR);
	writel( 0x04D40008 , PFC_MOD_SEL);

	writel(~0x40140040 , PFC_PMMR);
	writel( 0x40140040 , PFC_MOD_SEL2);

	writel(~0x00440200 , PFC_PMMR);
	writel( 0x00440200 , PFC_MOD_SEL3);

	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_MOD_SEL4);

	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR0);

	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR1);
	
	writel(~0x108244C8 , PFC_PMMR);
	writel( 0x108244C8 , PFC_IPSR2);
	
	writel(~0x00000082 , PFC_PMMR);
	writel( 0x00000082 , PFC_IPSR3);
	
	writel(~0x00002400 , PFC_PMMR);
	writel( 0x00002400 , PFC_IPSR4);
	
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR5);
	
	writel(~0x00701100 , PFC_PMMR);	
	writel( 0x00701100 , PFC_IPSR6);

	writel(~0x04800120 , PFC_PMMR);
	writel( 0x04800120 , PFC_IPSR7);
	
	writel(~0x00120120 , PFC_PMMR);
	writel( 0x00120120 , PFC_IPSR8);
	
	writel(~0x00046300 , PFC_PMMR);
	writel( 0x00046300 , PFC_IPSR9);

	writel(~0x7ED80000 , PFC_PMMR);
	writel( 0x7ED80000 , PFC_IPSR10);

	writel(~0x00003723 , PFC_PMMR);
	writel( 0x00003723 , PFC_IPSR11);

	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR12);
	
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR13);
	
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR14);
	
	writel(~0x00002400 , PFC_PMMR);
	writel( 0x00002400 , PFC_IPSR15);
	
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR16);

	writel(~0xFFFEFFFF , PFC_PMMR);
	writel( 0xFFFEFFFF , PFC_GPSR0);
	
	writel(~0x03FDBFFF , PFC_PMMR);
	writel( 0x03FDBFFF , PFC_GPSR1);
	
	writel(~0xBFFFFFE7 , PFC_PMMR);
	writel( 0xBFFFFFE7 , PFC_GPSR2);
	
	writel(~0xBF03F3FF , PFC_PMMR);	
	writel( 0xBF03F3FF , PFC_GPSR3);
	
	writel(~0x1FFFFFFF , PFC_PMMR);
	writel( 0x1FFFFFFF , PFC_GPSR4);
	
	writel(~0x01FFE003 , PFC_PMMR);
	writel( 0x01FFE003 , PFC_GPSR5);
	
	writel(~0xDDFFFFFF , PFC_PMMR);
	writel( 0xDDFFFFFF , PFC_GPSR6);
	
	writel(~0x026F9C7B , PFC_PMMR);
	writel( 0x026F9C7B , PFC_GPSR7);

	writel( 0x00000000 , PFC_PUPR0);
	writel( 0x04000000 , PFC_PUPR1);
	writel( 0x00000000 , PFC_PUPR2);
	writel( 0xE0030303 , PFC_PUPR3);	
	writel( 0xE0000040 , PFC_PUPR4);
	writel( 0x00000001 , PFC_PUPR5);
	writel( 0x00000000 , PFC_PUPR6);
	writel( 0x00000000 , PFC_PUPR7);

	writel( 0x00000000 , 0xE6051020);
	writel( 0x00000000 , 0xE6052020);
	writel( 0x00000000 , 0xE6053020);
	writel( 0x00000000 , 0xE6054020);
	writel( 0x00000000 , 0xE6055020);
	
	writel( 0x00000000 , 0xE6055420);
	writel( 0x00000000 , 0xE6055820);
	writel( 0x00000000 , 0xE6051000);
	writel( 0x00000000 , 0xE6052000);
	writel( 0x00000000 , 0xE6053000);
	writel( 0x00000000 , 0xE6054000);
	writel( 0x20000000 , 0xE6055000);
	writel( 0x00000000 , 0xE6055400);
	writel( 0x00000000 , 0xE6055800);
	writel( 0x00000000 , 0xE6052008);
	writel( 0x00000000 , 0xE6055008);
	writel( 0x00010000 , 0xE6055808);
	writel( 0x00000100 , 0xE6051004);
	writel( 0x01000003 , 0xE6052004);
	writel( 0x00070100 , 0xE6053004);
	writel( 0x03000000 , 0xE6054004);
	writel( 0x1E00070F , 0xE6055004);
	writel( 0x02000000 , 0xE6055404);
	writel( 0x00030001 , 0xE6055804);

#else //u-boot pin setting also ok
	writel(~0x04D40008 , PFC_PMMR);
	writel( 0x04D40008 , PFC_MOD_SEL);	// pio 0T
//	writel(~0x04D50008 , PFC_PMMR);
//	writel( 0x04D50008 , PFC_MOD_SEL);	// SCIF0 KOELSCH Setting
	writel(~0x40140040 , PFC_PMMR);
	writel( 0x40140040 , PFC_MOD_SEL2);	// pio 0T
//	writel(~0x60140040 , PFC_PMMR);
//	writel( 0x60140040 , PFC_MOD_SEL2);	// SCIF0 KOELSCH Setting
	
	// PFC_MOD_SEL3を以下の値で設定してみたらおかしな場所に飛んだ
	// 多分IRQの設定をしている為
//	writel(~0x00440200 , PFC_PMMR);
//	writel( 0x00440200 , PFC_MOD_SEL3);	// NG
	writel( ~(readl(PFC_MOD_SEL3)) , PFC_PMMR);
	writel( readl(PFC_MOD_SEL3) , PFC_MOD_SEL3);	// IRQ対応
	

	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_MOD_SEL4);

	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR0);
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR1);
	writel(~0x108244C8 , PFC_PMMR);
	writel( 0x108244C8 , PFC_IPSR2);
	writel(~0x00000082 , PFC_PMMR);
	writel( 0x00000082 , PFC_IPSR3);
	writel(~0x00002400 , PFC_PMMR);
	writel( 0x00002400 , PFC_IPSR4);
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR5);	// pio 0T
//	writel(~0x00120000 , PFC_PMMR);
//	writel( 0x00120000 , PFC_IPSR5);	// SCIF0 KOELSCH Setting
	writel(~0x00701100 , PFC_PMMR);	
	writel( 0x00701100 , PFC_IPSR6);	// pio 0T
//	writel(~0x00701000 , PFC_PMMR);	
//	writel( 0x00701000 , PFC_IPSR6);	// SCIFB1_RXD_D -> IRQ0
//	writel(~0x00701103 , PFC_PMMR);
//	writel( 0x00701103 , PFC_IPSR6);	// SCIF0 KOELSCH Setting
	writel(~0x04800120 , PFC_PMMR);
	writel( 0x04800120 , PFC_IPSR7);
	writel(~0x00120120 , PFC_PMMR);
	writel( 0x00120120 , PFC_IPSR8);
//	writel(~0x00046300 , PFC_PMMR);
//	writel( 0x00046300 , PFC_IPSR9);	// pio 0T
	writel(~0x00040300 , PFC_PMMR);
	writel( 0x00040300 , PFC_IPSR9);	// pio 0T立ち上げ用
	writel(~0x7ED80000 , PFC_PMMR);
	writel( 0x7ED80000 , PFC_IPSR10);	// pio 0T
//	writel(~0x1ED80000 , PFC_PMMR);
//	writel( 0x1ED80000 , PFC_IPSR10);	// SCIF0 KOELSCH Setting
	writel(~0x00003723 , PFC_PMMR);
	writel( 0x00003723 , PFC_IPSR11);	// pio 0T
//	writel(~0x00003720 , PFC_PMMR);
//	writel( 0x00003720 , PFC_IPSR11);	// SCIF0 KOELSCH Setting
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR12);
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR13);
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR14);
	writel(~0x00002400 , PFC_PMMR);
	writel( 0x00002400 , PFC_IPSR15);
	writel(~0x00000000 , PFC_PMMR);
	writel( 0x00000000 , PFC_IPSR16);

	writel(~0xFFFEFFFF , PFC_PMMR);
	writel( 0xFFFEFFFF , PFC_GPSR0);
	writel(~0x03FDBFFF , PFC_PMMR);
	writel( 0x03FDBFFF , PFC_GPSR1);
	writel(~0xBFFFFFE7 , PFC_PMMR);
	writel( 0xBFFFFFE7 , PFC_GPSR2);
//	writel(~0xBF03F3FF , PFC_PMMR);	
//	writel( 0xBF03F3FF , PFC_GPSR3);	// pio 0T
	writel(~0xBB03F3FF , PFC_PMMR);	
	writel( 0xBB03F3FF , PFC_GPSR3);	// pio 0T立ち上げ
	writel(~0x1FFFFFFF , PFC_PMMR);
	writel( 0x1FFFFFFF , PFC_GPSR4);
//	writel(~0x01FFE003 , PFC_PMMR);
//	writel( 0x01FFE003 , PFC_GPSR5);	// pio 0T
	writel(~0x01BFE003 , PFC_PMMR);
	writel( 0x01BFE003 , PFC_GPSR5);	// ETHER KOELSCH Setting pio 0Tでも必須
	writel(~0xDDFFFFFF , PFC_PMMR);
	writel( 0xDDFFFFFF , PFC_GPSR6);
	writel(~0x026F9C7B , PFC_PMMR);
	writel( 0x026F9C7B , PFC_GPSR7);

	writel( 0x00000000 , PFC_PUPR0);
	writel( 0x04000000 , PFC_PUPR1);
	writel( 0x00000000 , PFC_PUPR2);
	writel( 0xE0030303 , PFC_PUPR3);	
//	writel( 0xE0000040 , PFC_PUPR4);	// pio 0T
	writel( 0xE0000044 , PFC_PUPR4);	// pio 0T立ち上げ用
//	writel( 0x00000001 , PFC_PUPR5);	// pio 0T
	writel( 0x08000001 , PFC_PUPR5);	// pio 0T立ち上げ用
	writel( 0x00000000 , PFC_PUPR6);
	writel( 0x00000000 , PFC_PUPR7);

	writel( 0x00000000 , GPIO_POSNEG(0));
	writel( 0x00000000 , GPIO_IOINTSEL(0));
	writel( 0x00000000 , GPIO_OUTDT(0));
	writel( 0x00000000 , GPIO_INOUTSEL(0));

	writel( 0x00000000 , GPIO_POSNEG(1));
	writel( 0x00000000 , GPIO_IOINTSEL(1));
	writel( 0x00000000 , GPIO_OUTDT(1));
	writel( 0x00004000 , GPIO_INOUTSEL(1));

	writel( 0x00000000 , GPIO_POSNEG(2));
	writel( 0x00000000 , GPIO_IOINTSEL(2));
	writel( 0x00000000 , GPIO_OUTDT(2));
	writel( 0x40000018 , GPIO_INOUTSEL(2));

	writel( 0x00000000 , GPIO_POSNEG(3));
	writel( 0x00000000 , GPIO_IOINTSEL(3));
	writel( 0x00000000 , GPIO_OUTDT(3));
	writel( 0x001C0400 , GPIO_INOUTSEL(3));

	writel( 0x00000000 , GPIO_POSNEG(4));
	writel( 0x00000000 , GPIO_IOINTSEL(4));
	writel( 0x00000000 , GPIO_OUTDT(4));
	writel( 0x60000000 , GPIO_INOUTSEL(4));

	writel( 0x00000000 , GPIO_POSNEG(5));
	writel( 0x40000000 , GPIO_IOINTSEL(5));
	writel( 0x00000000 , GPIO_OUTDT(5));
	writel( 0x3C00073C , GPIO_INOUTSEL(5));

	writel( 0x00000000 , GPIO_POSNEG(6));
	writel( 0x00000000 , GPIO_IOINTSEL(6));
	writel( 0x00000000 , GPIO_OUTDT(6));
	writel( 0x20000000 , GPIO_INOUTSEL(6));

	writel( 0x00000000 , GPIO_POSNEG(7));
	writel( 0x00000000 , GPIO_IOINTSEL(7));
	writel( 0x00100000 , GPIO_OUTDT(7));
	writel( 0x00900004 , GPIO_INOUTSEL(7));

	writel( 0x8000AAAA , PFC_IOCTRL0);
	writel( 0xAAAAAAAA , PFC_IOCTRL1);
	writel( 0x00001405 , PFC_IOCTRL4);
	writel( 0x55554401 , PFC_IOCTRL5);
	writel( 0xFFFFFF00 , PFC_IOCTRL6);
	writel( 0x00000000 , PFC_IOCTRL7);

#endif
};

void lbsc_init(void)
{
	//u32 cnt = 0x186a0;
	
	writel( 0x00000020 , 0xFEC00200);
	writel( 0x00000020 , 0xFEC00204);
	writel( 0x00002020 , 0xFEC00208);
	writel( 0x00002020 , 0xFEC0020C);
	writel( 0xFF70FF70 , 0xFEC00230);
	writel( 0xFF70FF70 , 0xFEC00234);
	writel( 0xFF70FF70 , 0xFEC00238);
	writel( 0xFF70FF70 , 0xFEC0023C);
	writel( 0x00000000 , 0xFEC00280);
	writel( 0x00000000 , 0xFEC00284);
	writel( 0x00000000 , 0xFEC00288);
	writel( 0x00000000 , 0xFEC0028C);
	writel( 0x00000000 , 0xFEC002A0);
	writel( 0x00000012 , 0xFEC002C0);
	writel( 0x00000012 , 0xFEC002C4);
	writel( 0x00000000 , 0xFEC002C8);
	writel( 0x00000000 , 0xFEC00380);

	//while(cnt--);
}

void ddr_init(void)
{
	writel( 0x21000000 , DBSC3_0_DBCMD);
	writel( 0x21000000 , DBSC3_1_DBCMD);

	writel( 0x11000000 , DBSC3_0_DBCMD);
	writel( 0x11000000 , DBSC3_1_DBCMD);

	writel( 0x10000000 , DBSC3_0_DBCMD);
	writel( 0x10000000 , DBSC3_1_DBCMD);

	writel( 0x0000A55A , DBSC3_0_DBPDLCK);
	writel( 0x0000A55A , DBSC3_1_DBPDLCK);

	writel( 0x00000010 , DBSC3_0_DBPDRGA);
	writel( 0x00000010 , DBSC3_1_DBPDRGA);

	writel( 0xF004649B , DBSC3_0_DBPDRGD);
	writel( 0xF004649B , DBSC3_1_DBPDRGD);

	writel( 0x00000007 , DBSC3_0_DBKIND);
	writel( 0x00000007 , DBSC3_1_DBKIND);

	writel( 0x0F030A02 , DBSC3_0_DBCONF0);
	writel( 0x0F030A02 , DBSC3_1_DBCONF0);

	writel( 0x00000001 , DBSC3_0_PHYT);
	writel( 0x00000001 , DBSC3_1_PHYT);

	writel( 0x00000000 , DBSC3_0_DBBL);
	writel( 0x00000000 , DBSC3_1_DBBL);

	writel( 0x0000000B , DBSC3_0_DBTR0);
	writel( 0x0000000B , DBSC3_1_DBTR0);
	writel( 0x00000008 , DBSC3_0_DBTR1);
	writel( 0x00000008 , DBSC3_1_DBTR1);
	writel( 0x00000000 , DBSC3_0_DBTR2);
	writel( 0x00000000 , DBSC3_1_DBTR2);
	writel( 0x0000000B , DBSC3_0_DBTR3);
	writel( 0x0000000B , DBSC3_1_DBTR3);
	writel( 0x000C000B , DBSC3_0_DBTR4);
	writel( 0x000C000B , DBSC3_1_DBTR4);
	writel( 0x00000027 , DBSC3_0_DBTR5);
	writel( 0x00000027 , DBSC3_1_DBTR5);
	writel( 0x0000001C , DBSC3_0_DBTR6);
	writel( 0x0000001C , DBSC3_1_DBTR6);
	writel( 0x00000006 , DBSC3_0_DBTR7);
	writel( 0x00000006 , DBSC3_1_DBTR7);
	writel( 0x00000020 , DBSC3_0_DBTR8);
	writel( 0x00000020 , DBSC3_1_DBTR8);
	writel( 0x00000008 , DBSC3_0_DBTR9);
	writel( 0x00000008 , DBSC3_1_DBTR9);
	writel( 0x0000000C , DBSC3_0_DBTR10);
	writel( 0x0000000C , DBSC3_1_DBTR10);
	writel( 0x00000009 , DBSC3_0_DBTR11);
	writel( 0x00000009 , DBSC3_1_DBTR11);
	writel( 0x00000012 , DBSC3_0_DBTR12);
	writel( 0x00000012 , DBSC3_1_DBTR12);
	writel( 0x000000D0 , DBSC3_0_DBTR13);
	writel( 0x000000D0 , DBSC3_1_DBTR13);
	writel( 0x00140005 , DBSC3_0_DBTR14);
	writel( 0x00140005 , DBSC3_1_DBTR14);
	writel( 0x00050004 , DBSC3_0_DBTR15);
	writel( 0x00050004 , DBSC3_1_DBTR15);
	writel( 0x70233005 , DBSC3_0_DBTR16);
	writel( 0x70233005 , DBSC3_1_DBTR16);
	writel( 0x000C0000 , DBSC3_0_DBTR17);
	writel( 0x000C0000 , DBSC3_1_DBTR17);
	writel( 0x00000300 , DBSC3_0_DBTR18);
	writel( 0x00000300 , DBSC3_1_DBTR18);
	writel( 0x00000040 , DBSC3_0_DBTR19);
	writel( 0x00000040 , DBSC3_1_DBTR19);

	writel( 0x00000001 , DBSC3_0_DBRNK0);
	writel( 0x00000001 , DBSC3_0_DBRNK0);

	writel( 0x00020001 , DBSC3_0_DBADJ0);
	writel( 0x00020001 , DBSC3_0_DBADJ0);
	
	writel( 0x20082008 , DBSC3_0_DBADJ2);
	writel( 0x20082008 , DBSC3_0_DBADJ2);

	writel( 0x00020002 , DBSC3_0_DBWT0CNF0);
	writel( 0x00020002 , DBSC3_1_DBWT0CNF0);
	writel( 0x0000001F , DBSC3_0_DBWT0CNF4);
	writel( 0x0000001F , DBSC3_1_DBWT0CNF4);

	while( 1 != (readl(DBSC3_0_DBDFISTAT) & 0x1) );
	while( 1 != (readl(DBSC3_1_DBDFISTAT) & 0x1) );

	writel( 0x00000011 , DBSC3_0_DBDFICNT);
	writel( 0x00000011 , DBSC3_1_DBDFICNT);

	writel( 0x00000006 , DBSC3_0_DBPDRGA);
	writel( 0x00000006 , DBSC3_1_DBPDRGA);
	writel( 0x0005C000 , DBSC3_0_DBPDRGD);
	writel( 0x0005C000 , DBSC3_1_DBPDRGD);

	writel( 0x00000003 , DBSC3_0_DBPDRGA);
	writel( 0x00000003 , DBSC3_1_DBPDRGA);
	writel( 0x0300C481 , DBSC3_0_DBPDRGD);
	writel( 0x0300C481 , DBSC3_1_DBPDRGD);

	writel( 0x00000023 , DBSC3_0_DBPDRGA);
	writel( 0x00000023 , DBSC3_1_DBPDRGA);
	writel( 0x00FDB6C0 , DBSC3_0_DBPDRGD);
	writel( 0x00FDB6C0 , DBSC3_1_DBPDRGD);

	writel( 0x00000011 , DBSC3_0_DBPDRGA);
	writel( 0x00000011 , DBSC3_1_DBPDRGA);
	writel( 0x1000040B , DBSC3_0_DBPDRGD);
	writel( 0x1000040B , DBSC3_1_DBPDRGD);

	writel( 0x00000012 , DBSC3_0_DBPDRGA);
	writel( 0x00000012 , DBSC3_1_DBPDRGA);
	writel( 0x9D9CBB66 , DBSC3_0_DBPDRGD);
	writel( 0x9D9CBB66 , DBSC3_1_DBPDRGD);

	writel( 0x00000013 , DBSC3_0_DBPDRGA);
	writel( 0x00000013 , DBSC3_1_DBPDRGA);
	writel( 0x1A868400 , DBSC3_0_DBPDRGD);
	writel( 0x1A868400 , DBSC3_1_DBPDRGD);

	writel( 0x00000014 , DBSC3_0_DBPDRGA);
	writel( 0x00000014 , DBSC3_1_DBPDRGA);
	writel( 0x300214D8 , DBSC3_0_DBPDRGD);
	writel( 0x300214D8 , DBSC3_1_DBPDRGD);

	writel( 0x00000015 , DBSC3_0_DBPDRGA);
	writel( 0x00000015 , DBSC3_1_DBPDRGA);
	writel( 0x00000D70 , DBSC3_0_DBPDRGD);
	writel( 0x00000D70 , DBSC3_1_DBPDRGD);

	writel( 0x00000016 , DBSC3_0_DBPDRGA);
	writel( 0x00000016 , DBSC3_1_DBPDRGA);
	writel( 0x00000006 , DBSC3_0_DBPDRGD);
	writel( 0x00000006 , DBSC3_1_DBPDRGD);

	writel( 0x00000017 , DBSC3_0_DBPDRGA);
	writel( 0x00000017 , DBSC3_1_DBPDRGA);
	writel( 0x00000018 , DBSC3_0_DBPDRGD);
	writel( 0x00000018 , DBSC3_1_DBPDRGD);

	writel( 0x0000001A , DBSC3_0_DBPDRGA);
	writel( 0x0000001A , DBSC3_1_DBPDRGA);
	writel( 0x910035C7 , DBSC3_0_DBPDRGD);
	writel( 0x910035C7 , DBSC3_1_DBPDRGD);

	writel( 0x00000004 , DBSC3_0_DBPDRGA);
	writel( 0x00000004 , DBSC3_1_DBPDRGA);

	while( 1 != (readl(DBSC3_0_DBPDRGD)&0x1) );
	while( 1 != (readl(DBSC3_1_DBPDRGD)&0x1) );

	writel( 0x00000001 , DBSC3_0_DBPDRGA);
	writel( 0x00000001 , DBSC3_1_DBPDRGA);
	writel( 0x00000181 , DBSC3_0_DBPDRGD);
	writel( 0x00000181 , DBSC3_1_DBPDRGD);

	
	writel( 0x11000000 , DBSC3_0_DBCMD);
	writel( 0x11000000 , DBSC3_1_DBCMD);

	writel( 0x00000004 , DBSC3_0_DBPDRGA);
	writel( 0x00000004 , DBSC3_1_DBPDRGA);

	while( 1 != (readl(DBSC3_0_DBPDRGD)&0x1) );
	while( 1 != (readl(DBSC3_1_DBPDRGD)&0x1) );

	writel( 0x00000001 , DBSC3_0_DBPDRGA);
	writel( 0x00000001 , DBSC3_1_DBPDRGA);
	writel( 0x0000FE01 , DBSC3_0_DBPDRGD);
	writel( 0x0000FE01 , DBSC3_1_DBPDRGD);

	writel( 0x00000004 , DBSC3_0_DBPDRGA);
	writel( 0x00000004 , DBSC3_1_DBPDRGA);

	while( 1 != (readl(DBSC3_0_DBPDRGD)&0x1) );
	while( 1 != (readl(DBSC3_1_DBPDRGD)&0x1) );

	writel( 0x00000000 , DBSC3_0_DBBS0CNT1);
	writel( 0x00000000 , DBSC3_1_DBBS0CNT1);

	writel( 0x01004C20 , DBSC3_0_DBCALCNF);
	writel( 0x01004C20 , DBSC3_1_DBCALCNF);

	writel( 0x014000AA , DBSC3_0_DBCALTR);
	writel( 0x014000AA , DBSC3_1_DBCALTR);

	writel( 0x00000140 , DBSC3_0_DBRFCNF0);
	writel( 0x00000140 , DBSC3_1_DBRFCNF0);
	writel( 0x00081860 , DBSC3_0_DBRFCNF1);
	writel( 0x00081860 , DBSC3_1_DBRFCNF1);
	writel( 0x00010000 , DBSC3_0_DBRFCNF2);
	writel( 0x00010000 , DBSC3_1_DBRFCNF2);

	writel( 0x00000001 , DBSC3_0_DBRFEN);
	writel( 0x00000001 , DBSC3_1_DBRFEN);

	writel( 0x00000001 , DBSC3_0_DBACEN);
	writel( 0x00000001 , DBSC3_1_DBACEN);

	writel( 0x00000000 , DBSC3_0_DBPDLCK);
	writel( 0x00000000 , DBSC3_1_DBPDLCK);

	writel( 0x0000000C , 0xE678400C);
}

void board_init(void)
{
	system_init();
	
	clock_init();
	
	pio_pin_mux();

	lbsc_init();

	ddr_init();
}
