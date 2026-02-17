	.file	"vertex_shader.c"
	.text
	.globl	vertex_shader                   # -- Begin function vertex_shader
	.type	vertex_shader,@function
vertex_shader:                          # @vertex_shader
# %bb.0:
	addiu	$sp, $sp, -32
	sw	$4, 28($sp)
	sw	$5, 24($sp)
	sw	$6, 20($sp)
	lw	$at, 28($sp)
	lwc1	$f0, 0($at)
	swc1	$f0, 16($sp)
	lw	$at, 28($sp)
	lwc1	$f0, 4($at)
	swc1	$f0, 12($sp)
	lw	$at, 28($sp)
	lwc1	$f0, 8($at)
	swc1	$f0, 8($sp)
	lw	$at, 28($sp)
	lwc1	$f0, 12($at)
	swc1	$f0, 4($sp)
	lw	$at, 24($sp)
	lwc1	$f0, 0($at)
	lwc1	$f2, 16($sp)
	lwc1	$f1, 4($at)
	lwc1	$f3, 12($sp)
	mul.s	$f1, $f1, $f3
	mul.s	$f0, $f0, $f2
	add.s	$f1, $f0, $f1
	lwc1	$f0, 8($at)
	lwc1	$f2, 8($sp)
	mul.s	$f0, $f0, $f2
	add.s	$f1, $f0, $f1
	lwc1	$f0, 12($at)
	lwc1	$f2, 4($sp)
	mul.s	$f0, $f0, $f2
	add.s	$f0, $f0, $f1
	lw	$at, 20($sp)
	swc1	$f0, 0($at)
	lw	$at, 24($sp)
	lwc1	$f0, 16($at)
	lwc1	$f2, 16($sp)
	lwc1	$f1, 20($at)
	lwc1	$f3, 12($sp)
	mul.s	$f1, $f1, $f3
	mul.s	$f0, $f0, $f2
	add.s	$f1, $f0, $f1
	lwc1	$f0, 24($at)
	lwc1	$f2, 8($sp)
	mul.s	$f0, $f0, $f2
	add.s	$f1, $f0, $f1
	lwc1	$f0, 28($at)
	lwc1	$f2, 4($sp)
	mul.s	$f0, $f0, $f2
	add.s	$f0, $f0, $f1
	lw	$at, 20($sp)
	swc1	$f0, 4($at)
	lw	$at, 24($sp)
	lwc1	$f0, 32($at)
	lwc1	$f2, 16($sp)
	lwc1	$f1, 36($at)
	lwc1	$f3, 12($sp)
	mul.s	$f1, $f1, $f3
	mul.s	$f0, $f0, $f2
	add.s	$f1, $f0, $f1
	lwc1	$f0, 40($at)
	lwc1	$f2, 8($sp)
	mul.s	$f0, $f0, $f2
	add.s	$f1, $f0, $f1
	lwc1	$f0, 44($at)
	lwc1	$f2, 4($sp)
	mul.s	$f0, $f0, $f2
	add.s	$f0, $f0, $f1
	lw	$at, 20($sp)
	swc1	$f0, 8($at)
	lw	$at, 24($sp)
	lwc1	$f0, 48($at)
	lwc1	$f2, 16($sp)
	lwc1	$f1, 52($at)
	lwc1	$f3, 12($sp)
	mul.s	$f1, $f1, $f3
	mul.s	$f0, $f0, $f2
	add.s	$f1, $f0, $f1
	lwc1	$f0, 56($at)
	lwc1	$f2, 8($sp)
	mul.s	$f0, $f0, $f2
	add.s	$f1, $f0, $f1
	lwc1	$f0, 60($at)
	lwc1	$f2, 4($sp)
	mul.s	$f0, $f0, $f2
	add.s	$f0, $f0, $f1
	lw	$at, 20($sp)
	swc1	$f0, 12($at)
	addiu	$sp, $sp, 32
	ret
.Lfunc_end0:
	.size	vertex_shader, .Lfunc_end0-vertex_shader
                                        # -- End function
	.globl	main                            # -- Begin function main
	.type	main,@function
main:                                   # @main
# %bb.0:
	addiu	$sp, $sp, -128
	sw	$21, 124($sp)
	sw	$22, 120($sp)
	sw	$23, 116($sp)
	sw	$fp, 112($sp)
	sw	$ra, 108($sp)
	addiu	$5, $zero, 0
	sw	$5, 4($sp)
	sw	$5, 104($sp)
	ori	$2, $zero, ($__const.main.in_v)+12
	lui	$at, ($__const.main.in_v)+12
	add	$at, $at, $2
	lw	$at, 0($at)
	sw	$at, 100($sp)
	ori	$2, $zero, ($__const.main.in_v)+8
	lui	$at, ($__const.main.in_v)+8
	add	$at, $at, $2
	lw	$at, 0($at)
	sw	$at, 96($sp)
	ori	$2, $zero, ($__const.main.in_v)+4
	lui	$at, ($__const.main.in_v)+4
	add	$at, $at, $2
	lw	$at, 0($at)
	sw	$at, 92($sp)
	ori	$2, $zero, $__const.main.in_v
	lui	$at, $__const.main.in_v
	add	$at, $at, $2
	lw	$at, 0($at)
	sw	$at, 88($sp)
	addiu	$6, $zero, 64
	jal	memset
	lui	$at, 16256
	ori	$at, $at, 0
	sw	$at, 8($sp)
	sw	$at, 24($sp)
	sw	$at, 40($sp)
	sw	$at, 56($sp)
	add	$5, $4, $zero
	add	$6, $4, $zero
	jal	vertex_shader
	lw	$2, 4($sp)
	lw	$ra, 108($sp)
	lw	$fp, 112($sp)
	lw	$23, 116($sp)
	lw	$22, 120($sp)
	lw	$21, 124($sp)
	addiu	$sp, $sp, 128
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
                                        # -- End function
	.type	$__const.main.in_v,@object      # @__const.main.in_v
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	2, 0x0
$__const.main.in_v:
	.long	0x3f800000                      # float 1
	.long	0x3f000000                      # float 0.5
	.long	0x3f000000                      # float 0.5
	.long	0x3f800000                      # float 1
	.size	$__const.main.in_v, 16

	.ident	"clang version 21.0.0git (https://github.com/optimisan/llvm-project.git 5af6f101269be852f303172c1f1307003d94d146)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym vertex_shader
