/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) 2017 Theobroma Systems Design und Consulting GmbH
 */

#include <config.h>
#include <asm/macro.h>
#include <linux/linkage.h>

.pushsection .text.setjmp, "ax"
ENTRY(setjmp)
	/* Preserve all callee-saved registers and the SP */
	stp  x19, x20, [x0,#0]
	stp  x21, x22, [x0,#16]
	stp  x23, x24, [x0,#32]
	stp  x25, x26, [x0,#48]
	stp  x27, x28, [x0,#64]
	stp  x29, x30, [x0,#80]
	mov  x2, sp
	str  x2, [x0, #96]
	mov  x0, #0
	ret
ENDPROC(setjmp)
.popsection

.pushsection .text.longjmp, "ax"
ENTRY(longjmp)
	ldp  x19, x20, [x0,#0]
	ldp  x21, x22, [x0,#16]
	ldp  x23, x24, [x0,#32]
	ldp  x25, x26, [x0,#48]
	ldp  x27, x28, [x0,#64]
	ldp  x29, x30, [x0,#80]
	ldr  x2, [x0,#96]
	mov  sp, x2
	/* Move the return value in place, but return 1 if passed 0. */
	adds x0, xzr, x1
	csinc x0, x0, xzr, ne
	ret
ENDPROC(longjmp)
.popsection

.pushsection .text.initjmp, "ax"
ENTRY(initjmp)
	/* x1: entry point address, x2: stack base, x3: stack size */
	add x2, x2, x3
	stp x1, x2, [x0,#88]
	mov  x0, #0
	ret
ENDPROC(initjmp)
.popsection
