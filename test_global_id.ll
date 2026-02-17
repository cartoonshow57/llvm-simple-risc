target triple = "cgp1"

define void @kernel(ptr %out) "cgp1_kernel" {
entry:
  %tid = call i32 @llvm.cgp1.thread.id.x()
  %bid = call i32 @llvm.cgp1.block.id.x()
  %bdim = call i32 @llvm.cgp1.block.dim.x()
  %gdim = call i32 @llvm.cgp1.grid.dim.x()
  %mul = mul i32 %bid, %bdim
  %gid = add i32 %mul, %tid
  %res = add i32 %gid, %gdim
  store i32 %res, ptr %out
  ret void
}

declare i32 @llvm.cgp1.thread.id.x()
declare i32 @llvm.cgp1.block.id.x()
declare i32 @llvm.cgp1.block.dim.x()
declare i32 @llvm.cgp1.grid.dim.x()
