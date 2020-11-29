cmake_minimum_required(VERSION 2.8)

# Set these to point to the prefix under which the *installed* LLVM library can
# be found and the path to the LLVM source tree.
set(LLVM_PREFIX ~/projects/Bolt/llvm-8/build)
set(LLVM_SRC_DIR ~/projects/Bolt/llvm-8)
set(FFI_LIBRARY_PATH /usr/local/lib)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0")

set(LLVM_DIR ${LLVM_PREFIX}/share/llvm/cmake)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIRS} ${FFI_LIBRARY_PATH})
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

set(LLVM_LINK_COMPONENTS  aarch64asmparser aarch64asmprinter aarch64codegen aarch64desc aarch64disassembler aarch64info aarch64utils all amdgpuasmparser amdgpuasmprinter amdgpucodegen amdgpudesc amdgpuinfo amdgpuutils analysis  armasmparser armasmprinter armcodegen armdesc armdisassembler arminfo asmparser asmprinter bitreader bitwriter bpfasmprinter bpfcodegen bpfdesc bpfinfo codegen core debuginfocodeview debuginfodwarf debuginfopdb executionengine hexagonasmparser hexagoncodegen hexagondesc hexagondisassembler hexagoninfo instcombine instrumentation interpreter ipo irreader libdriver lineeditor linker lto mc mcdisassembler mcjit mcparser mipsasmparser mipsasmprinter mipscodegen mipsdesc mipsdisassembler mipsinfo mirparser msp430asmprinter msp430codegen msp430desc msp430info native nativecodegen nvptxasmprinter nvptxcodegen nvptxdesc nvptxinfo objcarcopts object option orcjit passes powerpcasmparser powerpcasmprinter powerpccodegen powerpcdesc powerpcdisassembler powerpcinfo profiledata runtimedyld scalaropts selectiondag sparcasmparser sparcasmprinter sparccodegen sparcdesc sparcdisassembler sparcinfo support symbolize systemzasmparser systemzasmprinter systemzcodegen systemzdesc systemzdisassembler systemzinfo tablegen target transformutils vectorize x86asmparser x86asmprinter x86codegen x86desc x86disassembler x86info x86utils xcoreasmprinter xcorecodegen xcoredesc xcoredisassembler xcoreinfo)
set(LLVM_BUILD_TOOLS true)
add_llvm_executable(trace
  trace.cpp
  ExtensibleInterpreter.cpp
)
set_target_properties(trace PROPERTIES 
    COMPILE_FLAGS "-fno-rtti -fvisibility-inlines-hidden"
)
target_link_libraries(trace ${FFI_LIBRARY_PATH})

# This allows our interpreter to include:
# ExecutionEngine/Interpreter/Interpreter.h
# which is not part of the installed headers.
include_directories(${LLVM_SRC_DIR}/lib)
