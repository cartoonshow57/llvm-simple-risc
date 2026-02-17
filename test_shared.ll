target triple = "cgp1"

define void @kernel(ptr %out) "cgp1_kernel" {
entry:
  %val = call i32 @llvm.cgp1.shared.load(i32 0)
  %new_val = add i32 %val, 1
  call void @llvm.cgp1.shared.store(i32 4, i32 %new_val)
  ret void
}

declare i32 @llvm.cgp1.shared.load(i32)
declare void @llvm.cgp1.shared.store(i32, i32)
