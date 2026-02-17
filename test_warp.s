	.file	"test_warp.c"
	.text
	.globl	warp_test                       # -- Begin function warp_test
	.type	warp_test,@function
warp_test:                              # @warp_test
# %bb.0:
	warp_size	$r1
	ret
.Lfunc_end0:
	.size	warp_test, .Lfunc_end0-warp_test
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
