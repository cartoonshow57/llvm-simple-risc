target triple = "cgp1"

define void @kernel(ptr %out) "cgp1_kernel" {
entry:
  %lane = call i32 @llvm.cgp1.lane.id()
  %wsize = call i32 @llvm.cgp1.warp.size()
  %val = call i32 @llvm.cgp1.warp.shuffle(i32 %lane, i32 0)
  %all = call i32 @llvm.cgp1.warp.all(i32 %val)
  %any = call i32 @llvm.cgp1.warp.any(i32 %val)
  %ballot = call i32 @llvm.cgp1.warp.ballot(i32 %val)
  
  ; Combine all results to prevent optimization
  %sum1 = add i32 %lane, %wsize
  %sum2 = add i32 %sum1, %val
  %sum3 = add i32 %sum2, %all
  %sum4 = add i32 %sum3, %any
  %sum5 = add i32 %sum4, %ballot
  
  store i32 %sum5, ptr %out
  ret void
}

declare i32 @llvm.cgp1.lane.id()
declare i32 @llvm.cgp1.warp.size()
declare i32 @llvm.cgp1.warp.shuffle(i32, i32)
declare i32 @llvm.cgp1.warp.all(i32)
declare i32 @llvm.cgp1.warp.any(i32)
declare i32 @llvm.cgp1.warp.ballot(i32)
