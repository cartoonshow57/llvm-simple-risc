; ModuleID = 'gpu_kernel.c'
source_filename = "gpu_kernel.c"
target datalayout = "E-m:m-p:32:32-p1:32:32-p3:32:32-p4:32:32-i8:8:32-i16:16:32-i64:64-n32-S64"
target triple = "cgp1"

; Function Attrs: noinline nounwind optnone
define dso_local i32 @get_thread_id() #0 {
  %1 = call i32 @llvm.cgp1.thread.id.x()
  ret i32 %1
}

; Function Attrs: nounwind memory(argmem: read, inaccessiblemem: read)
declare i32 @llvm.cgp1.thread.id.x() #1

; Function Attrs: noinline nounwind optnone
define dso_local i32 @get_block_id() #0 {
  %1 = call i32 @llvm.cgp1.block.id.x()
  ret i32 %1
}

; Function Attrs: nounwind memory(argmem: read, inaccessiblemem: read)
declare i32 @llvm.cgp1.block.id.x() #1

; Function Attrs: noinline nounwind optnone
define dso_local i32 @get_combined_id() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = call i32 @llvm.cgp1.thread.id.x()
  store i32 %3, ptr %1, align 4
  %4 = call i32 @llvm.cgp1.block.id.x()
  store i32 %4, ptr %2, align 4
  %5 = load i32, ptr %1, align 4
  %6 = load i32, ptr %2, align 4
  %7 = add nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone
define dso_local void @sync_threads() #0 {
  call void @llvm.cgp1.barrier()
  ret void
}

; Function Attrs: convergent nounwind
declare void @llvm.cgp1.barrier() #2

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #1 = { nounwind memory(argmem: read, inaccessiblemem: read) }
attributes #2 = { convergent nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 21.0.0git"}
