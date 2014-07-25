#define CR_V	(1 << 13)	/* Vectors relocated to 0xffff0000	*/
#define CONFIG_SYS_INIT_SP_ADDR		0xE6330000

.global _start
_start: 
	B	_reset_handler
	B	_undefined_handler
	B	_swi_handler
	B	_prefetch_handler
	B	_abort_handler		
	B	_reserved_vector
	B	_irq_handler		
	B	_fiq_handler
	
_undefined_handler:
	B	_undefined_handler
	
_swi_handler:
	B	_swi_handler
	
_prefetch_handler:
	B	_prefetch_handler
	
_abort_handler:
	B	_abort_handler
	
_reserved_vector:
	B	_reserved_vector
	
_irq_handler:
	B	_irq_handler
	
_fiq_handler:
	B	_fiq_handler
	

_reset_handler:
	/*set the cpu to SVC32 mode*/
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr,r0
	
	/* Set V=0 in CP15 SCTRL register - for VBAR to point to vector */
	mrc	p15, 0, r0, c1, c0, 0	@ Read CP15 SCTRL Register
	bic	r0, #CR_V		@ V = 0
	mcr	p15, 0, r0, c1, c0, 0	@ Write CP15 SCTRL Register

	/* Set vector address in CP15 VBAR register */
	ldr	r0, =_start
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR
		
	/*
	 * Invalidate L1 I/D
	 */
	mov	r0, #0			@ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
	mcr	p15, 0, r0, c7, c5, 6	@ invalidate BP array
	mcr p15, 0, r0, c7, c10, 4	@ DSB
	mcr p15, 0, r0, c7, c5, 4	@ ISB

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	@ set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800	@ set bit 11 (Z---) BTB
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-cache
	mcr	p15, 0, r0, c1, c0, 0
	
	/*
	 * Jump to board specific initialization...
	 * The Mask ROM will have already initialized
	 * basic memory. Go here to bump up clock rate and handle
	 * wake up conditions.
	 */
	@ go setup pll,mux,memory
	mrc	p15, 0, r4, c0, c0, 5	/* mpidr */
	orr	r4, r4, r4, lsr #6
	and	r4, r4, #7		/* id 0-3 = ca15.0,1,2,3 */
	
	/* surpress wfe if ca15 */
	tst	r4, #4
	mrceq	p15, 0, r0, c1, c0, 1	/* actlr */
	orreq	r0, r0, #(1<<7)
	mcreq	p15, 0, r0, c1, c0, 1
	/* and set l2 latency */
	mrceq	p15, 1, r0, c9, c0, 2	/* l2ctlr */
	orreq	r0, r0, #0x00000800
	orreq	r0, r0, #0x00000003
	mcreq	p15, 1, r0, c9, c0, 2

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
	bl	main
	
	/* We should never reach here */
    b   .

	