target triple = "cgp1"

define void @kernel() "cgp1_kernel" {
entry:
  %a = alloca i32
  %b = alloca i32
  store volatile i32 1, ptr %a
  store volatile i32 2, ptr %b
  %v1 = load volatile i32, ptr %a
  %v2 = load volatile i32, ptr %b
  ret void
}
