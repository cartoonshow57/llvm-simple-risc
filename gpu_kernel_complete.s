	.file	"gpu_kernel_complete.c"
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
	.globl	compare_values                  # -- Begin function compare_values
	.type	compare_values,@function
compare_values:                         # @compare_values
# %bb.0:
	store	[$r13 + 28], $r4
	store	[$r13 + 24], $r1
	store	[$r13 + 20], $r2
	load	$r1, [$r13 + 24]
	load	$r2, [$r13 + 20]
	sub	$r1, $r1, $r2
	loadi	$r3, 0
	sub	$r2, $r3, $r1
	or	$r1, $r1, $r2
	loadi	$r2, -1
	xor	$r1, $r1, $r2
	loadi	$r2, 31
	shr	$r1, $r1, $r2
	store	[$r13 + 16], $r1
	load	$r1, [$r13 + 24]
	load	$r4, [$r13 + 20]
	sub	$r1, $r1, $r4
	sub	$r3, $r3, $r1
	or	$r1, $r1, $r3
	shr	$r1, $r1, $r2
	store	[$r13 + 12], $r1
	load	$r1, [$r13 + 24]
	load	$r3, [$r13 + 20]
	sub	$r1, $r1, $r3
	shr	$r1, $r1, $r2
	store	[$r13 + 8], $r1
	load	$r3, [$r13 + 24]
	load	$r1, [$r13 + 20]
	sub	$r1, $r1, $r3
	shr	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 16]
	load	$r2, [$r13 + 12]
	add	$r1, $r1, $r2
	load	$r2, [$r13 + 8]
	add	$r1, $r1, $r2
	load	$r2, [$r13 + 4]
	add	$r1, $r1, $r2
	load	$r4, [$r13 + 28]
	ret
.Lfunc_end1:
	.size	compare_values, .Lfunc_end1-compare_values
                                        # -- End function
	.globl	sum_to_n                        # -- Begin function sum_to_n
	.type	sum_to_n,@function
sum_to_n:                               # @sum_to_n
# %bb.0:
	store	[$r13 + 12], $r1
	loadi	$r1, 0
	store	[$r13 + 8], $r1
	store	[$r13 + 4], $r1
	jump	.LBB2_1
.LBB2_1:                                # =>This Inner Loop Header: Depth=1
	load	$r1, [$r13 + 4]
	load	$r2, [$r13 + 12]
	sub	$r1, $r1, $r2
	brgez	$r1, .LBB2_3
	jump	.LBB2_2
.LBB2_2:                                #   in Loop: Header=BB2_1 Depth=1
	load	$r1, [$r13 + 8]
	load	$r2, [$r13 + 4]
	add	$r1, $r1, $r2
	store	[$r13 + 8], $r1
	load	$r1, [$r13 + 4]
	loadi	$r2, 1
	add	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	jump	.LBB2_1
.LBB2_3:
	load	$r1, [$r13 + 8]
	ret
.Lfunc_end2:
	.size	sum_to_n, .Lfunc_end2-sum_to_n
                                        # -- End function
	.globl	compute_lighting                # -- Begin function compute_lighting
	.type	compute_lighting,@function
compute_lighting:                       # @compute_lighting
# %bb.0:
	store	[$r13 + 12], $r1
	store	[$r13 + 8], $r2
	load	$r1, [$r13 + 12]
	load	$r2, [$r13 + 8]
	fmul	$r2, $r2, $r2
	fmul	$r1, $r1, $r1
	fadd	$r1, $r1, $r2
	frsqrt	$r1, $r1
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 12]
	fsin	$r1, $r1
	load	$r2, [$r13 + 8]
	fcos	$r2, $r2
	fmul	$r1, $r1, $r2
	store	[$r13], $r1
	load	$r1, [$r13 + 4]
	load	$r2, [$r13]
	fmul	$r1, $r1, $r2
	ret
.Lfunc_end3:
	.size	compute_lighting, .Lfunc_end3-compute_lighting
                                        # -- End function
	.globl	warp_reduce                     # -- Begin function warp_reduce
	.type	warp_reduce,@function
warp_reduce:                            # @warp_reduce
# %bb.0:
	store	[$r13 + 12], $r1
	warp_size	$r1
	store	[$r13 + 8], $r1
	lane_id	$r1
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 12]
	load	$r2, [$r13 + 4]
	warp_shuffle	$r1, $r1, $r2
	store	[$r13], $r1
	load	$r1, [$r13]
	ret
.Lfunc_end4:
	.size	warp_reduce, .Lfunc_end4-warp_reduce
                                        # -- End function
	.globl	shared_mem_test                 # -- Begin function shared_mem_test
	.type	shared_mem_test,@function
shared_mem_test:                        # @shared_mem_test
# %bb.0:
	store	[$r13 + 12], $r1
	store	[$r13 + 8], $r2
	load	$r1, [$r13 + 12]
	load	$r2, [$r13 + 8]
	shared_store	$r1, $r2
	barrier
	load	$r1, [$r13 + 12]
	shared_load	$r1, $r1
	store	[$r13 + 4], $r1
	ret
.Lfunc_end5:
	.size	shared_mem_test, .Lfunc_end5-shared_mem_test
                                        # -- End function
	.globl	draw_colored_triangle           # -- Begin function draw_colored_triangle
	.type	draw_colored_triangle,@function
draw_colored_triangle:                  # @draw_colored_triangle
# %bb.0:
	store	[$r13 + 36], $r5
	store	[$r13 + 32], $r6
	store	[$r13 + 28], $r7
	copy	$r5, $r3
	copy	$r6, $r2
	copy	$r7, $r1
	copy	$r1, $r13
	load	$r3, [$r1]
	copy	$r1, $r13
	load	$r2, [$r1 + 4]
	copy	$r1, $r13
	load	$r1, [$r1 + 8]
	store	[$r13 + 24], $r7
	store	[$r13 + 20], $r6
	store	[$r13 + 16], $r5
	store	[$r13 + 12], $r4
	store	[$r13 + 8], $r3
	store	[$r13 + 4], $r2
	store	[$r13], $r1
	load	$r1, [$r13]
	set_color	$r1
	loadi	$r1, 2
	gfx_begin	$r1
	load	$r1, [$r13 + 24]
	load	$r2, [$r13 + 20]
	vertex2i	$r1, $r2
	load	$r1, [$r13 + 16]
	load	$r2, [$r13 + 12]
	vertex2i	$r1, $r2
	load	$r1, [$r13 + 8]
	load	$r2, [$r13 + 4]
	vertex2i	$r1, $r2
	gfx_end
	load	$r7, [$r13 + 28]
	load	$r6, [$r13 + 32]
	load	$r5, [$r13 + 36]
	ret
.Lfunc_end6:
	.size	draw_colored_triangle, .Lfunc_end6-draw_colored_triangle
                                        # -- End function
	.globl	draw_transformed_rect           # -- Begin function draw_transformed_rect
	.type	draw_transformed_rect,@function
draw_transformed_rect:                  # @draw_transformed_rect
# %bb.0:
	store	[$r13 + 52], $r4
	store	[$r13 + 48], $r5
	store	[$r13 + 44], $r6
	store	[$r13 + 40], $r7
	copy	$r5, $r3
	store	[$r13], $r5
	copy	$r5, $r2
	store	[$r13 + 4], $r5
	copy	$r5, $r1
	store	[$r13 + 8], $r5
	copy	$r5, $r3
	load	$r3, [$r13]
	copy	$r6, $r2
	load	$r2, [$r13 + 4]
	copy	$r7, $r1
	load	$r1, [$r13 + 8]
	store	[$r13 + 36], $r7
	store	[$r13 + 32], $r6
	store	[$r13 + 28], $r5
	store	[$r13 + 24], $r1
	store	[$r13 + 20], $r2
	store	[$r13 + 16], $r3
	store	[$r13 + 12], $r4
	load_identity
	load	$r1, [$r13 + 36]
	load	$r2, [$r13 + 32]
	translate	$r1, $r2, $r1
	load	$r1, [$r13 + 28]
	scale	$r1, $r1, $r1
	loadi	$r1, 65280
	set_color	$r1
	load	$r1, [$r13 + 24]
	load	$r2, [$r13 + 20]
	load	$r3, [$r13 + 16]
	load	$r4, [$r13 + 12]
	fill_rect	$r1, $r2, $r3, $r4
	load	$r7, [$r13 + 40]
	load	$r6, [$r13 + 44]
	load	$r5, [$r13 + 48]
	load	$r4, [$r13 + 52]
	ret
.Lfunc_end7:
	.size	draw_transformed_rect, .Lfunc_end7-draw_transformed_rect
                                        # -- End function
	.globl	sample_and_blend                # -- Begin function sample_and_blend
	.type	sample_and_blend,@function
sample_and_blend:                       # @sample_and_blend
# %bb.0:
	copy	$r3, $r1
	store	[$r13 + 4], $r3
	copy	$r3, $r1
	load	$r1, [$r13 + 4]
	store	[$r13 + 20], $r3
	store	[$r13 + 16], $r2
	store	[$r13 + 12], $r1
	loadi	$r1, 0
	bind_texture	$r1
	load	$r1, [$r13 + 20]
	load	$r2, [$r13 + 16]
	tex2d	$r1, $r1, $r2
	store	[$r13 + 8], $r1
	loadi	$r1, 1
	set_blend_mode	$r1
	load	$r1, [$r13 + 8]
	ret
.Lfunc_end8:
	.size	sample_and_blend, .Lfunc_end8-sample_and_blend
                                        # -- End function
	.globl	render_kernel                   # -- Begin function render_kernel
	.type	render_kernel,@function
render_kernel:                          # @render_kernel
# %bb.0:
	store	[$r13 + 52], $r4
	store	[$r13 + 48], $r5
	store	[$r13 + 44], $r6
	store	[$r13 + 40], $r7
	store	[$r13 + 36], $r8
	store	[$r13 + 32], $r9
	store	[$r13 + 28], $r10
	store	[$r13 + 24], $r11
	store	[$r13 + 20], $r14
	store	[$r13 + 16], $r1
	loadi	$r1, 0
	store	[$r13], $r1
	load	$r1, [$r13]
	clear_screen
	loadi	$r3, 480
	loadi	$r2, 640
	set_viewport	$r1, $r1, $r2, $r3
	call	get_global_id
	store	[$r13 + 12], $r1
	load	$r1, [$r13 + 12]
	loadi	$r2, 10
	mod	$r1, $r1, $r2
	loadi	$r3, 6
	shl	$r1, $r1, $r3
	store	[$r13 + 8], $r1
	load	$r1, [$r13 + 12]
	div	$r1, $r1, $r2
	loadi	$r2, 48
	mul	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	loadi	$r1, 16711680
	set_color	$r1
	load	$r1, [$r13 + 8]
	load	$r2, [$r13 + 4]
	loadi	$r4, 44
	loadi	$r3, 60
	fill_rect	$r1, $r2, $r3, $r4
	loadi	$r1, 255
	set_color	$r1
	load	$r1, [$r13 + 8]
	loadi	$r2, 30
	add	$r1, $r1, $r2
	load	$r2, [$r13 + 4]
	loadi	$r3, 22
	add	$r2, $r2, $r3
	loadi	$r3, 20
	draw_circle	$r1, $r2, $r3
	barrier
	swap_buffers
	load	$r14, [$r13 + 20]
	load	$r11, [$r13 + 24]
	load	$r10, [$r13 + 28]
	load	$r9, [$r13 + 32]
	load	$r8, [$r13 + 36]
	load	$r7, [$r13 + 40]
	load	$r6, [$r13 + 44]
	load	$r5, [$r13 + 48]
	load	$r4, [$r13 + 52]
	ret
.Lfunc_end9:
	.size	render_kernel, .Lfunc_end9-render_kernel
                                        # -- End function
	.globl	pixel_shader                    # -- Begin function pixel_shader
	.type	pixel_shader,@function
pixel_shader:                           # @pixel_shader
# %bb.0:
	store	[$r13 + 68], $r4
	store	[$r13 + 64], $r5
	store	[$r13 + 60], $r6
	store	[$r13 + 56], $r7
	store	[$r13 + 52], $r8
	store	[$r13 + 48], $r9
	store	[$r13 + 44], $r10
	store	[$r13 + 40], $r11
	store	[$r13 + 36], $r14
	copy	$r3, $r1
	store	[$r13 + 4], $r3
	copy	$r3, $r1
	load	$r1, [$r13 + 4]
	store	[$r13 + 32], $r3
	store	[$r13 + 28], $r2
	store	[$r13 + 24], $r1
	load	$r1, [$r13 + 32]
	call	__floatsisf
	store	[$r13 + 20], $r1
	load	$r1, [$r13 + 28]
	call	__floatsisf
	store	[$r13 + 16], $r1
	load	$r1, [$r13 + 20]
	load	$r2, [$r13 + 16]
	call	compute_lighting
	store	[$r13 + 12], $r1
	load	$r1, [$r13 + 32]
	load	$r2, [$r13 + 28]
	read_pixel	$r1, $r1, $r2
	store	[$r13 + 8], $r1
	load	$r1, [$r13 + 8]
	load	$r14, [$r13 + 36]
	load	$r11, [$r13 + 40]
	load	$r10, [$r13 + 44]
	load	$r9, [$r13 + 48]
	load	$r8, [$r13 + 52]
	load	$r7, [$r13 + 56]
	load	$r6, [$r13 + 60]
	load	$r5, [$r13 + 64]
	load	$r4, [$r13 + 68]
	ret
.Lfunc_end10:
	.size	pixel_shader, .Lfunc_end10-pixel_shader
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym get_global_id
	.addrsig_sym compute_lighting
