target triple = "cgp1"

define void @kernel(ptr %out) "cgp1_kernel" {
entry:
  %val = call i32 @llvm.cgp1.atomic.add(ptr %out, i32 1)
  %old = call i32 @llvm.cgp1.atomic.cas(ptr %out, i32 %val, i32 100)
  call i32 @llvm.cgp1.atomic.max(ptr %out, i32 200)
  call i32 @llvm.cgp1.atomic.min(ptr %out, i32 10)
  call i32 @llvm.cgp1.atomic.exch(ptr %out, i32 50)
  ret void
}

declare i32 @llvm.cgp1.atomic.add(ptr, i32)
declare i32 @llvm.cgp1.atomic.cas(ptr, i32, i32)
declare i32 @llvm.cgp1.atomic.max(ptr, i32)
declare i32 @llvm.cgp1.atomic.min(ptr, i32)
declare i32 @llvm.cgp1.atomic.exch(ptr, i32)
