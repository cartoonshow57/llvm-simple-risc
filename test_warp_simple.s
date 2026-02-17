	.file	"test_warp_simple.c"
	.text
	.globl	warp_simple_test                # -- Begin function warp_simple_test
	.type	warp_simple_test,@function
warp_simple_test:                       # @warp_simple_test
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	warp_all	$r1, $r1
	ret
.Lfunc_end0:
	.size	warp_simple_test, .Lfunc_end0-warp_simple_test
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
