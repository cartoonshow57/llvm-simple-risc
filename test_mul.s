	.file	"test_mul.c"
	.text
	.globl	multiply                        # -- Begin function multiply
	.type	multiply,@function
multiply:                               # @multiply
# %bb.0:
	store	[$r13 + 4], $r1
	store	[$r13], $r2
	load	$r1, [$r13 + 4]
	load	$r2, [$r13]
	mul	$r1, $r1, $r2
	ret
.Lfunc_end0:
	.size	multiply, .Lfunc_end0-multiply
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
