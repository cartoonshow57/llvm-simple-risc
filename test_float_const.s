	.file	"test_float_const.c"
	.text
	.globl	test_floats                     # -- Begin function test_floats
	.type	test_floats,@function
test_floats:                            # @test_floats
# %bb.0:
	loadi	$r3, 0
	loadi	$r2, 1128792064
	loadi	$r1, 1120403456
	translate	$r1, $r2, $r3
	loadi	$r2, 1065353216
	loadi	$r1, 1073741824
	scale	$r1, $r1, $r2
	loadi	$r1, 1056964608
	rotate_z	$r1
	ret
.Lfunc_end0:
	.size	test_floats, .Lfunc_end0-test_floats
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
