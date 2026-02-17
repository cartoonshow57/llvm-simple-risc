target triple = "cgp1"

define void @kernel(ptr %out) "cgp1_kernel" {
entry:
  %val = call i32 @llvm.cgp1.thread.id.x()
  %res = call i32 @llvm.cgp1.atomic.add(ptr %out, i32 %val)
  ret void
}

declare i32 @llvm.cgp1.thread.id.x()
declare i32 @llvm.cgp1.atomic.add(ptr, i32)
