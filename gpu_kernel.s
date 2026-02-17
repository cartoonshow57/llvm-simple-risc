	.file	"gpu_kernel.c"
	.text
	.globl	get_thread_id                   # -- Begin function get_thread_id
	.type	get_thread_id,@function
get_thread_id:                          # @get_thread_id
# %bb.0:
	thread_id_x	$r1
	ret
.Lfunc_end0:
	.size	get_thread_id, .Lfunc_end0-get_thread_id
                                        # -- End function
	.globl	get_block_id                    # -- Begin function get_block_id
	.type	get_block_id,@function
get_block_id:                           # @get_block_id
# %bb.0:
	block_id_x	$r1
	ret
.Lfunc_end1:
	.size	get_block_id, .Lfunc_end1-get_block_id
                                        # -- End function
	.globl	get_combined_id                 # -- Begin function get_combined_id
	.type	get_combined_id,@function
get_combined_id:                        # @get_combined_id
# %bb.0:
	thread_id_x	$r1
	store	[$r13 + 4], $r1
	block_id_x	$r1
	store	[$r13], $r1
	load	$r1, [$r13 + 4]
	load	$r2, [$r13]
	add	$r1, $r1, $r2
	ret
.Lfunc_end2:
	.size	get_combined_id, .Lfunc_end2-get_combined_id
                                        # -- End function
	.globl	sync_threads                    # -- Begin function sync_threads
	.type	sync_threads,@function
sync_threads:                           # @sync_threads
# %bb.0:
	barrier
	ret
.Lfunc_end3:
	.size	sync_threads, .Lfunc_end3-sync_threads
                                        # -- End function
	.globl	conditional_kernel              # -- Begin function conditional_kernel
	.type	conditional_kernel,@function
conditional_kernel:                     # @conditional_kernel
# %bb.0:
	store	[$r13], $r1
	load	$r1, [$r13]
	loadi	$r2, -1
	add	$r1, $r1, $r2
	brltz	$r1, .LBB4_2
	jump	.LBB4_1
.LBB4_1:
	load	$r1, [$r13]
	loadi	$r2, 1
	add	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	jump	.LBB4_3
.LBB4_2:
	load	$r1, [$r13]
	loadi	$r2, -1
	add	$r1, $r1, $r2
	store	[$r13 + 4], $r1
	jump	.LBB4_3
.LBB4_3:
	load	$r1, [$r13 + 4]
	ret
.Lfunc_end4:
	.size	conditional_kernel, .Lfunc_end4-conditional_kernel
                                        # -- End function
	.ident	"clang version 21.0.0git"
	.section	".note.GNU-stack","",@progbits
	.addrsig
