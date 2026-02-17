	.file	"simple_program.c"
	.text
	.globl	main                            # -- Begin function main
	.type	main,@function
main:                                   # @main
# %bb.0:
	loadi	$r1, 0
	store	[$r13 + 12], $r1
	loadi	$r2, 10
	store	[$r13 + 8], $r2
	loadi	$r2, 20
	store	[$r13 + 4], $r2
	load	$r2, [$r13 + 8]
	load	$r3, [$r13 + 4]
	add	$r2, $r2, $r3
	store	[$r13], $r2
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
