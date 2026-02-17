	.file	"test_global_id.c"
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
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
