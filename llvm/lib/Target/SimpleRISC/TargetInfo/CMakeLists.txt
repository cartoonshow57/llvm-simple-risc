add_llvm_library(SimpleRISCInfo
  # This single file's job is to register our target with LLVM's
  # TargetRegistry so it becomes visible to tools like 'llc'.
  TargetInfoSimpleRISC.cpp

  # List the LLVM components this library depends on.
  LINK_COMPONENTS
  MC
  Support

  # Add this library to our target's main component group.
  ADD_TO_COMPONENT
  SimpleRISC
  )

# Explicitly state that this library cannot be built until after the
# TableGen files for our target have been generated. This prevents
# potential errors in parallel builds.
add_dependencies(SimpleRISCInfo SimpleRISCCommonTableGen)