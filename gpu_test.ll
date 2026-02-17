; CGP1 GPU Intrinsics Test
target datalayout = "E-m:m-p:32:32-p1:32:32-p3:32:32-p4:32:32-i8:8:32-i16:16:32-i64:64-n32-S64"
target triple = "cgp1"

; Declare GPU intrinsics with nounwind readnone to prevent optimization
declare i32 @llvm.cgp1.thread.id.x() nounwind readnone
declare i32 @llvm.cgp1.thread.id.y() nounwind readnone
declare i32 @llvm.cgp1.block.id.x() nounwind readnone
declare i32 @llvm.cgp1.block.dim.x() nounwind readnone
declare void @llvm.cgp1.barrier() nounwind convergent

define i32 @get_thread_id() nounwind {
entry:
  %tid = call i32 @llvm.cgp1.thread.id.x()
  ret i32 %tid
}

define i32 @get_global_id() nounwind {
entry:
  %tid = call i32 @llvm.cgp1.thread.id.x()
  %bid = call i32 @llvm.cgp1.block.id.x()
  %bdim = call i32 @llvm.cgp1.block.dim.x()
  %tmp = add i32 %tid, %bid
  %result = add i32 %tmp, %bdim
  ret i32 %result
}
