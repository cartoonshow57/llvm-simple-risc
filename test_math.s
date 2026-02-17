	.file	"test_math.c"
	.text
	.globl	rcp_test                        # -- Begin function rcp_test
	.type	rcp_test,@function
rcp_test:                               # @rcp_test
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	frcp	$r1, $r1
	ret
.Lfunc_end0:
	.size	rcp_test, .Lfunc_end0-rcp_test
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
