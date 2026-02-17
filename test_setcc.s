	.file	"test_setcc.c"
	.text
	.globl	compare_gt                      # -- Begin function compare_gt
	.type	compare_gt,@function
compare_gt:                             # @compare_gt
# %bb.0:
	store	[$r13 + 4], $r1
	store	[$r13], $r2
	load	$r2, [$r13 + 4]
	load	$r1, [$r13]
	sub	$r1, $r1, $r2
	loadi	$r2, 31
	shr	$r1, $r1, $r2
	ret
.Lfunc_end0:
	.size	compare_gt, .Lfunc_end0-compare_gt
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
