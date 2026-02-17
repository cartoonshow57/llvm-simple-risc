target triple = "cgp1"

define void @func() {
entry:
  %a = alloca i32
  store volatile i32 42, ptr %a
  ret void
}

define void @caller() {
entry:
  call void @func()
  ret void
}
