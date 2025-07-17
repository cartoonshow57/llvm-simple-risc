add_llvm_library(SimpleRISCDesc
  # This file registers the components of this library. It's essential.
  SimpleRISCMCTargetDesc.cpp

  # Defines properties of the assembly language (e.g., comment syntax).
  SimpleRISCMCAsmInfo.cpp

  # Converts LLVM's machine instructions into bytes for the object file.
  SimpleRISCMCCodeEmitter.cpp

  # Handles converting machine instructions back into assembly text.
  SimpleRISCInstPrinter.cpp

  # Handles the backend-specific logic for writing object files.
  SimpleRISCAsmBackend.cpp

  # The libraries our code will depend on.
  LINK_COMPONENTS
  MC
  SimpleRISCInfo
  Support

  # Add this library to our target's component group.
  ADD_TO_COMPONENT
  SimpleRISC
  )