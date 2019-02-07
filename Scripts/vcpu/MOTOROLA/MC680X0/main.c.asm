#NO_APP
	.file	"main.c"
	.section	.rodata
.LC0:
	.string	"test"
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	link.w %fp,#0
	pea .LC0
	jsr puts
	addq.l #4,%sp
	clr.l %d0
	unlk %fp
	rts
	.size	main, .-main
	.ident	"GCC: (Debian 6.3.0-18) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
