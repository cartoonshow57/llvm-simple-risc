target triple = "cgp1-unknown-unknown"

declare i32 @llvm.cgp1.thread.id.x()

define void @kernel(ptr %out) "cgp1_kernel" {
entry:
  %tid = call i32 @llvm.cgp1.thread.id.x()
  store i32 %tid, ptr %out
  ret void
}
