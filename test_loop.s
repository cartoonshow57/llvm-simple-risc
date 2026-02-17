	.file	"test_loop.c"
	.text
	.globl	loop_sum                        # -- Begin function loop_sum
	.type	loop_sum,@function
loop_sum:                               # @loop_sum
# %bb.0:
	store	[$r13 + 12], $r1
	loadi	$r1, 0
	store	[$r13 + 8], $r1
	store	[$r13 + 4], $r1
	jump	.LBB0_1
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	load	$r1, [$r13 + 4]
	load	$r2, [$r13 + 12]
	sub	$r1, $r1, $r2
	brgez	$r1, .LBB0_3
	jump	.LBB0_2
.LBB0_2:                                #   in Loop: Header=BB0_1 Depth=1
	load	$r1, [$r13 + 8]
	load	$r2, [$r13 + 4]
	add	$r1, $r1, $r2
	store	[$r13 + 8], $r1
	load	$r1, [$r13 + 4]
	loadi	$r2, 1
	add	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	jump	.LBB0_1
.LBB0_3:
	load	$r1, [$r13 + 8]
	ret
.Lfunc_end0:
	.size	loop_sum, .Lfunc_end0-loop_sum
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
