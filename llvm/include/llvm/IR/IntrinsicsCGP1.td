//===- IntrinsicsCGP1.td - CGP1 target-specific intrinsics ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines all CGP1-specific intrinsics for GPU programming.
//
//===----------------------------------------------------------------------===//

let TargetPrefix = "cgp1" in {

//===----------------------------------------------------------------------===//
// Thread Indexing Intrinsics
//===----------------------------------------------------------------------===//

// Thread ID within a block (0 to block_dim - 1)
// Use IntrInaccessibleMemOrArgMemOnly + IntrReadMem to read from GPU registers
def int_cgp1_thread_id_x : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;
def int_cgp1_thread_id_y : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;
def int_cgp1_thread_id_z : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;

// Block ID within the grid (0 to grid_dim - 1)
def int_cgp1_block_id_x : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;
def int_cgp1_block_id_y : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;
def int_cgp1_block_id_z : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;

// Block dimensions (number of threads per block)
def int_cgp1_block_dim_x : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;
def int_cgp1_block_dim_y : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;
def int_cgp1_block_dim_z : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;

// Grid dimensions (number of blocks in the grid)
def int_cgp1_grid_dim_x : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;
def int_cgp1_grid_dim_y : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;
def int_cgp1_grid_dim_z : Intrinsic<[llvm_i32_ty], [], [IntrInaccessibleMemOrArgMemOnly, IntrReadMem]>;

//===----------------------------------------------------------------------===//
// Synchronization Intrinsics
//===----------------------------------------------------------------------===//

// Barrier - synchronize all threads in a block
def int_cgp1_barrier : Intrinsic<[], [], [IntrConvergent, IntrHasSideEffects]>;

//===----------------------------------------------------------------------===//
// SIMT Execution Mask (EXEC) Intrinsics
//
// Minimal plumbing for SIMT divergence management. These intrinsics expose
// an "EXEC"-like active-lane mask as a 32-bit integer.
//
// Note: The backend currently models EXEC as a dedicated physical register.
// These intrinsics will later be used by divergence/structurization passes.
//===----------------------------------------------------------------------===//

// Read current EXEC mask
def int_cgp1_exec_get : Intrinsic<[llvm_i32_ty], [], [IntrNoMem, IntrSpeculatable]>;

// Write EXEC mask
def int_cgp1_exec_set : Intrinsic<[], [llvm_i32_ty], [IntrHasSideEffects]>;

// Push current EXEC mask onto a mask stack
def int_cgp1_exec_push : Intrinsic<[], [], [IntrHasSideEffects]>;

// Pop EXEC mask from mask stack into EXEC
def int_cgp1_exec_pop : Intrinsic<[], [], [IntrHasSideEffects]>;

// EXEC = EXEC & mask
def int_cgp1_exec_and : Intrinsic<[], [llvm_i32_ty], [IntrHasSideEffects]>;

//===----------------------------------------------------------------------===//
// Graphics Intrinsics
//===----------------------------------------------------------------------===//

// Write a pixel to the framebuffer
// Arguments: x (i32), y (i32), color (i32 - RGB format 0x00RRGGBB)
def int_cgp1_write_pixel : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
                                     [IntrWriteMem, IntrWillReturn]>;

// Draw a line between two points
// Arguments: x0, y0, x1, y1, color
def int_cgp1_draw_line : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty, 
                                        llvm_i32_ty, llvm_i32_ty],
                                   [IntrWriteMem, IntrWillReturn]>;

// Draw a filled triangle
// Arguments: x0, y0, x1, y1, x2, y2, color
def int_cgp1_draw_triangle : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty,
                                            llvm_i32_ty, llvm_i32_ty, llvm_i32_ty,
                                            llvm_i32_ty],
                                       [IntrWriteMem, IntrWillReturn]>;

// Clear the entire framebuffer with a color
// Arguments: color
def int_cgp1_clear_screen : Intrinsic<[], [llvm_i32_ty],
                                      [IntrWriteMem, IntrWillReturn]>;

//===----------------------------------------------------------------------===//
// Shared Memory Intrinsics
//===----------------------------------------------------------------------===//

// Load from shared memory (fast on-chip memory shared within a block)
// Arguments: address (i32) -> value (i32)
def int_cgp1_shared_load : Intrinsic<[llvm_i32_ty], [llvm_i32_ty],
                                     [IntrReadMem, IntrWillReturn]>;

// Store to shared memory
// Arguments: address (i32), value (i32)
def int_cgp1_shared_store : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty],
                                      [IntrWriteMem, IntrWillReturn]>;

//===----------------------------------------------------------------------===//
// Atomic Operations
//===----------------------------------------------------------------------===//

// Atomic add: atomically adds value to memory location, returns old value
// Arguments: address (ptr), value (i32) -> old_value (i32)
def int_cgp1_atomic_add : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty],
                                    [IntrArgMemOnly, IntrWillReturn]>;

// Atomic compare-and-swap: if *addr == cmp, store val; returns old value
// Arguments: address (ptr), compare (i32), value (i32) -> old_value (i32)
def int_cgp1_atomic_cas : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty, llvm_i32_ty],
                                    [IntrArgMemOnly, IntrWillReturn]>;

// Atomic max: atomically stores max(*addr, val), returns old value
def int_cgp1_atomic_max : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty],
                                    [IntrArgMemOnly, IntrWillReturn]>;

// Atomic min: atomically stores min(*addr, val), returns old value
def int_cgp1_atomic_min : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty],
                                    [IntrArgMemOnly, IntrWillReturn]>;

// Atomic exchange: atomically stores val, returns old value
def int_cgp1_atomic_exch : Intrinsic<[llvm_i32_ty], [llvm_ptr_ty, llvm_i32_ty],
                                     [IntrArgMemOnly, IntrWillReturn]>;

//===----------------------------------------------------------------------===//
// Warp-Level Primitives
//===----------------------------------------------------------------------===//

// Get warp size (typically 32 threads per warp)
def int_cgp1_warp_size : Intrinsic<[llvm_i32_ty], [], [IntrNoMem, IntrSpeculatable]>;

// Get lane ID within warp (0 to warp_size-1)
def int_cgp1_lane_id : Intrinsic<[llvm_i32_ty], [], [IntrNoMem]>;

// Warp vote: returns 1 if all active threads have predicate true
def int_cgp1_warp_all : Intrinsic<[llvm_i32_ty], [llvm_i32_ty],
                                  [IntrConvergent, IntrNoMem]>;

// Warp vote: returns 1 if any active thread has predicate true
def int_cgp1_warp_any : Intrinsic<[llvm_i32_ty], [llvm_i32_ty],
                                  [IntrConvergent, IntrNoMem]>;

// Warp ballot: returns bitmask of active threads with predicate true
def int_cgp1_warp_ballot : Intrinsic<[llvm_i32_ty], [llvm_i32_ty],
                                     [IntrConvergent, IntrNoMem]>;

// Warp shuffle: exchange data between threads in a warp
// Arguments: value, src_lane -> value from src_lane
def int_cgp1_warp_shuffle : Intrinsic<[llvm_i32_ty], [llvm_i32_ty, llvm_i32_ty],
                                      [IntrConvergent, IntrNoMem]>;

//===----------------------------------------------------------------------===//
// Math Intrinsics (fast GPU implementations)
//===----------------------------------------------------------------------===//

// Fast reciprocal (1/x)
def int_cgp1_rcp : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;

// Fast reciprocal square root (1/sqrt(x))
def int_cgp1_rsqrt : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                               [IntrNoMem, IntrSpeculatable]>;

// Fast sine
def int_cgp1_sin : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;

// Fast cosine
def int_cgp1_cos : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;

// Fast exponential (e^x)
def int_cgp1_exp : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;

// Fast log (ln(x))
def int_cgp1_log : Intrinsic<[llvm_float_ty], [llvm_float_ty],
                             [IntrNoMem, IntrSpeculatable]>;

//===----------------------------------------------------------------------===//
// OpenGL-like Graphics Intrinsics
//===----------------------------------------------------------------------===//

// Viewport setup: set_viewport(x, y, width, height)
def int_cgp1_set_viewport : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
                                       [IntrWriteMem, IntrWillReturn]>;

// Set current drawing color (RGBA packed as i32: 0xAARRGGBB)
def int_cgp1_set_color : Intrinsic<[], [llvm_i32_ty],
                                   [IntrWriteMem, IntrWillReturn]>;

// Set depth value for subsequent vertices
def int_cgp1_set_depth : Intrinsic<[], [llvm_float_ty],
                                   [IntrWriteMem, IntrWillReturn]>;

// Begin primitive: mode (0=points, 1=lines, 2=triangles, 3=quads)
def int_cgp1_begin : Intrinsic<[], [llvm_i32_ty],
                               [IntrHasSideEffects]>;

// End primitive batch
def int_cgp1_end : Intrinsic<[], [],
                             [IntrHasSideEffects]>;

// Emit vertex with 2D coordinates
def int_cgp1_vertex2i : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty],
                                  [IntrWriteMem, IntrWillReturn]>;

// Emit vertex with 2D float coordinates
def int_cgp1_vertex2f : Intrinsic<[], [llvm_float_ty, llvm_float_ty],
                                  [IntrWriteMem, IntrWillReturn]>;

// Emit vertex with 3D float coordinates
def int_cgp1_vertex3f : Intrinsic<[], [llvm_float_ty, llvm_float_ty, llvm_float_ty],
                                  [IntrWriteMem, IntrWillReturn]>;

// Draw rectangle: draw_rect(x, y, width, height)
def int_cgp1_draw_rect : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
                                   [IntrWriteMem, IntrWillReturn]>;

// Draw filled rectangle
def int_cgp1_fill_rect : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
                                   [IntrWriteMem, IntrWillReturn]>;

// Draw circle: draw_circle(cx, cy, radius)
def int_cgp1_draw_circle : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
                                     [IntrWriteMem, IntrWillReturn]>;

// Fill circle
def int_cgp1_fill_circle : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
                                     [IntrWriteMem, IntrWillReturn]>;

// Read pixel color from framebuffer
def int_cgp1_read_pixel : Intrinsic<[llvm_i32_ty], [llvm_i32_ty, llvm_i32_ty],
                                    [IntrReadMem, IntrWillReturn]>;

// Swap buffers (for double buffering)
def int_cgp1_swap_buffers : Intrinsic<[], [],
                                      [IntrHasSideEffects]>;

// Set blend mode: 0=none, 1=alpha, 2=additive
def int_cgp1_set_blend_mode : Intrinsic<[], [llvm_i32_ty],
                                        [IntrWriteMem, IntrWillReturn]>;

//===----------------------------------------------------------------------===//
// Texture Operations
//===----------------------------------------------------------------------===//

// Bind texture: bind_texture(texture_id)
def int_cgp1_bind_texture : Intrinsic<[], [llvm_i32_ty],
                                      [IntrWriteMem, IntrWillReturn]>;

// Sample texture at UV coordinates: tex2d(u, v) -> color
def int_cgp1_tex2d : Intrinsic<[llvm_i32_ty], [llvm_float_ty, llvm_float_ty],
                               [IntrReadMem, IntrWillReturn]>;

// Set texture coordinates for next vertex
def int_cgp1_texcoord2f : Intrinsic<[], [llvm_float_ty, llvm_float_ty],
                                    [IntrWriteMem, IntrWillReturn]>;

//===----------------------------------------------------------------------===//
// Matrix Operations (for transforms)
//===----------------------------------------------------------------------===//

// Load identity matrix
def int_cgp1_load_identity : Intrinsic<[], [],
                                       [IntrWriteMem, IntrWillReturn]>;

// Translate: translate(x, y, z)
def int_cgp1_translate : Intrinsic<[], [llvm_float_ty, llvm_float_ty, llvm_float_ty],
                                   [IntrWriteMem, IntrWillReturn]>;

// Scale: scale(sx, sy, sz)
def int_cgp1_scale : Intrinsic<[], [llvm_float_ty, llvm_float_ty, llvm_float_ty],
                               [IntrWriteMem, IntrWillReturn]>;

// Rotate around Z axis: rotate_z(angle_radians)
def int_cgp1_rotate_z : Intrinsic<[], [llvm_float_ty],
                                  [IntrWriteMem, IntrWillReturn]>;

} // End TargetPrefix = "cgp1"
