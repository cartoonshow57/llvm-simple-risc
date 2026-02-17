	.file	"vertex_shader_only.c"
	.text
	.globl	vertex_shader                   # -- Begin function vertex_shader
	.type	vertex_shader,@function
vertex_shader:                          # @vertex_shader
# %bb.0:
	store	[$r13 + 36], $r4
	store	[$r13 + 32], $r5
	store	[$r13 + 28], $r1
	store	[$r13 + 24], $r2
	store	[$r13 + 20], $r3
	load	$r1, [$r13 + 28]
	load	$r1, [$r1]
	store	[$r13 + 16], $r1
	load	$r1, [$r13 + 28]
	load	$r1, [$r1 + 4]
	store	[$r13 + 12], $r1
	load	$r1, [$r13 + 28]
	load	$r1, [$r1 + 8]
	store	[$r13 + 8], $r1
	load	$r1, [$r13 + 28]
	load	$r1, [$r1 + 12]
	store	[$r13 + 4], $r1
	load	$r1, [$r13 + 24]
	load	$r2, [$r1]
	load	$r4, [$r13 + 16]
	load	$r3, [$r1 + 4]
	load	$r5, [$r13 + 12]
	fmul	$r3, $r3, $r5
	fmul	$r2, $r2, $r4
	fadd	$r3, $r2, $r3
	load	$r2, [$r1 + 8]
	load	$r4, [$r13 + 8]
	fmul	$r2, $r2, $r4
	fadd	$r2, $r2, $r3
	load	$r1, [$r1 + 12]
	load	$r3, [$r13 + 4]
	fmul	$r1, $r1, $r3
	fadd	$r1, $r1, $r2
	load	$r2, [$r13 + 20]
	store	[$r2], $r1
	load	$r1, [$r13 + 24]
	load	$r2, [$r1 + 16]
	load	$r4, [$r13 + 16]
	load	$r3, [$r1 + 20]
	load	$r5, [$r13 + 12]
	fmul	$r3, $r3, $r5
	fmul	$r2, $r2, $r4
	fadd	$r3, $r2, $r3
	load	$r2, [$r1 + 24]
	load	$r4, [$r13 + 8]
	fmul	$r2, $r2, $r4
	fadd	$r2, $r2, $r3
	load	$r1, [$r1 + 28]
	load	$r3, [$r13 + 4]
	fmul	$r1, $r1, $r3
	fadd	$r1, $r1, $r2
	load	$r2, [$r13 + 20]
	store	[$r2 + 4], $r1
	load	$r1, [$r13 + 24]
	load	$r2, [$r1 + 32]
	load	$r4, [$r13 + 16]
	load	$r3, [$r1 + 36]
	load	$r5, [$r13 + 12]
	fmul	$r3, $r3, $r5
	fmul	$r2, $r2, $r4
	fadd	$r3, $r2, $r3
	load	$r2, [$r1 + 40]
	load	$r4, [$r13 + 8]
	fmul	$r2, $r2, $r4
	fadd	$r2, $r2, $r3
	load	$r1, [$r1 + 44]
	load	$r3, [$r13 + 4]
	fmul	$r1, $r1, $r3
	fadd	$r1, $r1, $r2
	load	$r2, [$r13 + 20]
	store	[$r2 + 8], $r1
	load	$r1, [$r13 + 24]
	load	$r2, [$r1 + 48]
	load	$r4, [$r13 + 16]
	load	$r3, [$r1 + 52]
	load	$r5, [$r13 + 12]
	fmul	$r3, $r3, $r5
	fmul	$r2, $r2, $r4
	fadd	$r3, $r2, $r3
	load	$r2, [$r1 + 56]
	load	$r4, [$r13 + 8]
	fmul	$r2, $r2, $r4
	fadd	$r2, $r2, $r3
	load	$r1, [$r1 + 60]
	load	$r3, [$r13 + 4]
	fmul	$r1, $r1, $r3
	fadd	$r1, $r1, $r2
	load	$r2, [$r13 + 20]
	store	[$r2 + 12], $r1
	load	$r5, [$r13 + 32]
	load	$r4, [$r13 + 36]
	ret
.Lfunc_end0:
	.size	vertex_shader, .Lfunc_end0-vertex_shader
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
