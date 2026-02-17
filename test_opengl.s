	.file	"test_opengl.c"
	.text
	.globl	draw_triangle_test              # -- Begin function draw_triangle_test
	.type	draw_triangle_test,@function
draw_triangle_test:                     # @draw_triangle_test
# %bb.0:
	loadi	$r1, 16711680
	set_color	$r1
	loadi	$r1, 2
	gfx_begin	$r1
	loadi	$r2, 100
	vertex2i	$r2, $r2
	loadi	$r1, 200
	vertex2i	$r1, $r2
	loadi	$r2, 50
	loadi	$r1, 150
	vertex2i	$r1, $r2
	gfx_end
	ret
.Lfunc_end0:
	.size	draw_triangle_test, .Lfunc_end0-draw_triangle_test
                                        # -- End function
	.globl	draw_shapes                     # -- Begin function draw_shapes
	.type	draw_shapes,@function
draw_shapes:                            # @draw_shapes
# %bb.0:
	loadi	$r1, 255
	set_color	$r1
	loadi	$r2, 50
	loadi	$r1, 10
	fill_rect	$r1, $r1, $r2, $r2
	loadi	$r1, 65280
	set_color	$r1
	loadi	$r1, 30
	loadi	$r2, 100
	draw_circle	$r2, $r2, $r1
	loadi	$r1, 16776960
	set_color	$r1
	loadi	$r3, 25
	loadi	$r1, 200
	fill_circle	$r1, $r2, $r3
	ret
.Lfunc_end1:
	.size	draw_shapes, .Lfunc_end1-draw_shapes
                                        # -- End function
	.globl	setup_scene                     # -- Begin function setup_scene
	.type	setup_scene,@function
setup_scene:                            # @setup_scene
# %bb.0:
	store	[$r13 + 20], $r1
	store	[$r13 + 16], $r2
	store	[$r13 + 12], $r3
	store	[$r13 + 8], $r4
	store	[$r13 + 4], $r5
	loadi	$r3, 480
	loadi	$r2, 640
	loadi	$r1, 0
	set_viewport	$r1, $r1, $r2, $r3
	load_identity
	load	$r1, [$r13 + 20]
	load	$r2, [$r13 + 16]
	translate	$r1, $r2, $r1
	load	$r1, [$r13 + 12]
	load	$r2, [$r13 + 8]
	scale	$r1, $r2, $r1
	load	$r1, [$r13 + 4]
	rotate_z	$r1
	ret
.Lfunc_end2:
	.size	setup_scene, .Lfunc_end2-setup_scene
                                        # -- End function
	.globl	sample_texture                  # -- Begin function sample_texture
	.type	sample_texture,@function
sample_texture:                         # @sample_texture
# %bb.0:
	store	[$r13 + 4], $r1
	store	[$r13], $r2
	loadi	$r1, 0
	bind_texture	$r1
	load	$r1, [$r13 + 4]
	load	$r2, [$r13]
	tex2d	$r1, $r1, $r2
	ret
.Lfunc_end3:
	.size	sample_texture, .Lfunc_end3-sample_texture
                                        # -- End function
	.globl	get_pixel                       # -- Begin function get_pixel
	.type	get_pixel,@function
get_pixel:                              # @get_pixel
# %bb.0:
	store	[$r13 + 4], $r1
	store	[$r13], $r2
	load	$r1, [$r13 + 4]
	load	$r2, [$r13]
	read_pixel	$r1, $r1, $r2
	ret
.Lfunc_end4:
	.size	get_pixel, .Lfunc_end4-get_pixel
                                        # -- End function
	.globl	render                          # -- Begin function render
	.type	render,@function
render:                                 # @render
# %bb.0:
	store	[$r13 + 44], $r4
	store	[$r13 + 40], $r5
	store	[$r13 + 36], $r6
	store	[$r13 + 32], $r7
	store	[$r13 + 28], $r8
	store	[$r13 + 24], $r9
	store	[$r13 + 20], $r10
	store	[$r13 + 16], $r11
	store	[$r13 + 12], $r14
	store	[$r13 + 8], $r1
	store	[$r13 + 4], $r2
	loadi	$r1, 0
	clear_screen
	load	$r5, [$r13 + 8]
	load	$r4, [$r13 + 4]
	copy	$r1, $r5
	copy	$r2, $r4
	copy	$r3, $r5
	call	setup_scene
	call	draw_triangle_test
	call	draw_shapes
	swap_buffers
	load	$r14, [$r13 + 12]
	load	$r11, [$r13 + 16]
	load	$r10, [$r13 + 20]
	load	$r9, [$r13 + 24]
	load	$r8, [$r13 + 28]
	load	$r7, [$r13 + 32]
	load	$r6, [$r13 + 36]
	load	$r5, [$r13 + 40]
	load	$r4, [$r13 + 44]
	ret
.Lfunc_end5:
	.size	render, .Lfunc_end5-render
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym draw_triangle_test
	.addrsig_sym draw_shapes
	.addrsig_sym setup_scene
