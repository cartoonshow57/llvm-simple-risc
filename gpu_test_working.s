	.file	"gpu_test_working.c"
	.text
	.globl	get_global_id                   # -- Begin function get_global_id
	.type	get_global_id,@function
get_global_id:                          # @get_global_id
# %bb.0:
	thread_id_x	$r1
	store	[$r13 + 12], $r1
	block_id_x	$r1
	store	[$r13 + 8], $r1
	block_dim_x	$r1
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 8]
	load	$r2, [$r13 + 4]
	mul	$r1, $r1, $r2
	load	$r2, [$r13 + 12]
	add	$r1, $r1, $r2
	ret
.Lfunc_end0:
	.size	get_global_id, .Lfunc_end0-get_global_id
                                        # -- End function
	.globl	arithmetic                      # -- Begin function arithmetic
	.type	arithmetic,@function
arithmetic:                             # @arithmetic
# %bb.0:
	store	[$r13 + 28], $r1
	store	[$r13 + 24], $r2
	load	$r1, [$r13 + 28]
	load	$r2, [$r13 + 24]
	add	$r1, $r1, $r2
	store	[$r13 + 20], $r1
	load	$r1, [$r13 + 28]
	load	$r2, [$r13 + 24]
	sub	$r1, $r1, $r2
	store	[$r13 + 16], $r1
	load	$r1, [$r13 + 28]
	load	$r2, [$r13 + 24]
	mul	$r1, $r1, $r2
	store	[$r13 + 12], $r1
	load	$r1, [$r13 + 28]
	load	$r2, [$r13 + 24]
	div	$r1, $r1, $r2
	store	[$r13 + 8], $r1
	load	$r1, [$r13 + 28]
	load	$r2, [$r13 + 24]
	mod	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 20]
	load	$r2, [$r13 + 16]
	add	$r1, $r1, $r2
	load	$r2, [$r13 + 12]
	add	$r1, $r1, $r2
	load	$r2, [$r13 + 8]
	add	$r1, $r1, $r2
	load	$r2, [$r13 + 4]
	add	$r1, $r1, $r2
	ret
.Lfunc_end1:
	.size	arithmetic, .Lfunc_end1-arithmetic
                                        # -- End function
	.globl	bitwise                         # -- Begin function bitwise
	.type	bitwise,@function
bitwise:                                # @bitwise
# %bb.0:
	store	[$r13 + 12], $r1
	store	[$r13 + 8], $r2
	load	$r1, [$r13 + 12]
	load	$r2, [$r13 + 8]
	and	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	load	$r2, [$r13 + 12]
	load	$r3, [$r13 + 8]
	xor	$r2, $r2, $r3
	or	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	loadi	$r2, 2
	shl	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	loadi	$r2, 1
	shra	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	ret
.Lfunc_end2:
	.size	bitwise, .Lfunc_end2-bitwise
                                        # -- End function
	.globl	conditional                     # -- Begin function conditional
	.type	conditional,@function
conditional:                            # @conditional
# %bb.0:
	store	[$r13], $r1
	load	$r1, [$r13]
	loadi	$r2, -1
	add	$r1, $r1, $r2
	brltz	$r1, .LBB3_2
	jump	.LBB3_1
.LBB3_1:
	load	$r1, [$r13]
	loadi	$r2, 1
	shl	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	jump	.LBB3_3
.LBB3_2:
	load	$r1, [$r13]
	loadi	$r2, -1
	add	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	jump	.LBB3_3
.LBB3_3:
	load	$r1, [$r13 + 4]
	ret
.Lfunc_end3:
	.size	conditional, .Lfunc_end3-conditional
                                        # -- End function
	.globl	loop_sum                        # -- Begin function loop_sum
	.type	loop_sum,@function
loop_sum:                               # @loop_sum
# %bb.0:
	store	[$r13 + 12], $r1
	loadi	$r1, 0
	store	[$r13 + 8], $r1
	store	[$r13 + 4], $r1
	jump	.LBB4_1
.LBB4_1:                                # =>This Inner Loop Header: Depth=1
	load	$r1, [$r13 + 4]
	load	$r2, [$r13 + 12]
	sub	$r1, $r1, $r2
	brgez	$r1, .LBB4_3
	jump	.LBB4_2
.LBB4_2:                                #   in Loop: Header=BB4_1 Depth=1
	load	$r1, [$r13 + 8]
	load	$r2, [$r13 + 4]
	add	$r1, $r1, $r2
	store	[$r13 + 8], $r1
	load	$r1, [$r13 + 4]
	loadi	$r2, 1
	add	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	jump	.LBB4_1
.LBB4_3:
	load	$r1, [$r13 + 8]
	ret
.Lfunc_end4:
	.size	loop_sum, .Lfunc_end4-loop_sum
                                        # -- End function
	.globl	sync_block                      # -- Begin function sync_block
	.type	sync_block,@function
sync_block:                             # @sync_block
# %bb.0:
	barrier
	ret
.Lfunc_end5:
	.size	sync_block, .Lfunc_end5-sync_block
                                        # -- End function
	.globl	shared_store                    # -- Begin function shared_store
	.type	shared_store,@function
shared_store:                           # @shared_store
# %bb.0:
	store	[$r13 + 4], $r1
	store	[$r13], $r2
	load	$r1, [$r13 + 4]
	loadi	$r2, 2
	shl	$r1, $r1, $r2
	load	$r2, [$r13]
	shared_store	$r1, $r2
	ret
.Lfunc_end6:
	.size	shared_store, .Lfunc_end6-shared_store
                                        # -- End function
	.globl	shared_load                     # -- Begin function shared_load
	.type	shared_load,@function
shared_load:                            # @shared_load
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	loadi	$r2, 2
	shl	$r1, $r1, $r2
	shared_load	$r1, $r1
	ret
.Lfunc_end7:
	.size	shared_load, .Lfunc_end7-shared_load
                                        # -- End function
	.globl	get_warp_info                   # -- Begin function get_warp_info
	.type	get_warp_info,@function
get_warp_info:                          # @get_warp_info
# %bb.0:
	warp_size	$r1
	store	[$r13 + 4], $r1
	lane_id	$r1
	store	[$r13], $r1
	load	$r1, [$r13 + 4]
	load	$r2, [$r13]
	add	$r1, $r1, $r2
	ret
.Lfunc_end8:
	.size	get_warp_info, .Lfunc_end8-get_warp_info
                                        # -- End function
	.globl	warp_vote                       # -- Begin function warp_vote
	.type	warp_vote,@function
warp_vote:                              # @warp_vote
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	warp_all	$r1, $r1
	ret
.Lfunc_end9:
	.size	warp_vote, .Lfunc_end9-warp_vote
                                        # -- End function
	.globl	warp_shfl                       # -- Begin function warp_shfl
	.type	warp_shfl,@function
warp_shfl:                              # @warp_shfl
# %bb.0:
	store	[$r13 + 4], $r1
	store	[$r13], $r2
	load	$r1, [$r13 + 4]
	load	$r2, [$r13]
	warp_shuffle	$r1, $r1, $r2
	ret
.Lfunc_end10:
	.size	warp_shfl, .Lfunc_end10-warp_shfl
                                        # -- End function
	.globl	draw_pixel                      # -- Begin function draw_pixel
	.type	draw_pixel,@function
draw_pixel:                             # @draw_pixel
# %bb.0:
	store	[$r13 + 12], $r4
	store	[$r13 + 8], $r1
	store	[$r13 + 4], $r2
	store	[$r13], $r3
	load	$r3, [$r13 + 8]
	load	$r2, [$r13 + 4]
	load	$r1, [$r13]
	loadi	$r4, 640
	mul	$r2, $r2, $r4
	add	$r2, $r2, $r3
	loadi	$r3, 2
	shl	$r2, $r2, $r3
	loadi	$r3, 268435456
	add	$r2, $r2, $r3
	store	[$r2], $r1
	load	$r4, [$r13 + 12]
	ret
.Lfunc_end11:
	.size	draw_pixel, .Lfunc_end11-draw_pixel
                                        # -- End function
	.globl	clear                           # -- Begin function clear
	.type	clear,@function
clear:                                  # @clear
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	clear_screen
	ret
.Lfunc_end12:
	.size	clear, .Lfunc_end12-clear
                                        # -- End function
	.globl	fast_rcp                        # -- Begin function fast_rcp
	.type	fast_rcp,@function
fast_rcp:                               # @fast_rcp
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	frcp	$r1, $r1
	ret
.Lfunc_end13:
	.size	fast_rcp, .Lfunc_end13-fast_rcp
                                        # -- End function
	.globl	fast_rsqrt                      # -- Begin function fast_rsqrt
	.type	fast_rsqrt,@function
fast_rsqrt:                             # @fast_rsqrt
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 4]
	frsqrt	$r1, $r1
	ret
.Lfunc_end14:
	.size	fast_rsqrt, .Lfunc_end14-fast_rsqrt
                                        # -- End function
	.globl	fast_sincos                     # -- Begin function fast_sincos
	.type	fast_sincos,@function
fast_sincos:                            # @fast_sincos
# %bb.0:
	store	[$r13 + 4], $r1
	load	$r2, [$r13 + 4]
	fsin	$r1, $r2
	fcos	$r2, $r2
	fadd	$r1, $r1, $r2
	ret
.Lfunc_end15:
	.size	fast_sincos, .Lfunc_end15-fast_sincos
                                        # -- End function
	.globl	fp_ops                          # -- Begin function fp_ops
	.type	fp_ops,@function
fp_ops:                                 # @fp_ops
# %bb.0:
	store	[$r13 + 4], $r1
	store	[$r13], $r2
	load	$r1, [$r13 + 4]
	load	$r3, [$r13]
	fadd	$r2, $r1, $r3
	fmul	$r1, $r1, $r3
	fadd	$r1, $r1, $r2
	ret
.Lfunc_end16:
	.size	fp_ops, .Lfunc_end16-fp_ops
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
