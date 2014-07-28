#include "global_cfg.h"

.global _start
_start: 
	/*set the cpu to SVC32 mode, disable fiq&irq*/
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr,r0
	
	ldr	sp, =(CONFIG_SYS_INIT_SP_ADDR)
	
	/* Clear the .bss section (zero init) */
    LDR     r1,=__bss_start
    LDR     r2,=__bss_end
    MOV     r3,#0
1:
    CMP     r1,r2
    STMLTIA r1!,{r3}
    BLT     1b
		
	/* Branch to C Library entry point */
	bl	boot_main
	
	/* We should never reach here */
    b   .
