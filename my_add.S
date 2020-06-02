.globl my_add
.p2align 2
.type my_add,%function

/*
 * int my_add(int a, int b, int c);
 */

my_add:
	.fnstart
	add	r0, r0, r1
	add	r0, r0, r2
	bx       lr
	.fnend
