	.file	"test_warp_pred.c"
	.text
	.globl	warp_pred_test                  # -- Begin function warp_pred_test
	.type	warp_pred_test,@function
warp_pred_test:                         # @warp_pred_test
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r2, [$r13 + 4]
	loadi	$r1, 0
	sub	$r1, $r1, $r2
	loadi	$r2, 31
	shr	$r1, $r1, $r2
	warp_all	$r1, $r1
	ret
.Lfunc_end0:
	.size	warp_pred_test, .Lfunc_end0-warp_pred_test
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
