	.file	"gpu_test.ll"
	.text
	.globl	get_thread_id                   # -- Begin function get_thread_id
	.type	get_thread_id,@function
get_thread_id:                          # @get_thread_id
# %bb.0:                                # %entry
	thread_id_x	$r1
	ret
.Lfunc_end0:
	.size	get_thread_id, .Lfunc_end0-get_thread_id
                                        # -- End function
	.globl	get_global_id                   # -- Begin function get_global_id
	.type	get_global_id,@function
get_global_id:                          # @get_global_id
# %bb.0:                                # %entry
	thread_id_x	$r1
	block_id_x	$r3
	block_dim_x	$r2
	add	$r1, $r1, $r3
	add	$r1, $r1, $r2
	ret
.Lfunc_end1:
	.size	get_global_id, .Lfunc_end1-get_global_id
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
