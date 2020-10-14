cmake_minimum_required(VERSION 2.8)

# Set these to point to the prefix under which the *installed* LLVM library can
# be found and the path to the LLVM source tree.
set(LLVM_PREFIX ~/llvm/build)
set(LLVM_SRC_DIR ~/llvm)
set(CMAKE_BUILD_TYPE Debug)

set(LLVM_DIR ${LLVM_PREFIX}/share/llvm/cmake)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIRS})
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

set(LLVM_LINK_COMPONENTS aarch64 aarch64asmparser aarch64asmprinter aarch64codegen aarch64desc aarch64disassembler aarch64info aarch64utils all amdgpu amdgpuasmparser amdgpuasmprinter amdgpucodegen amdgpudesc amdgpuinfo amdgpuutils analysis arm armasmparser armasmprinter armcodegen armdesc armdisassembler arminfo asmparser asmprinter bitreader bitwriter bpf bpfasmprinter bpfcodegen bpfdesc bpfinfo codegen core cppbackend cppbackendcodegen cppbackendinfo debuginfocodeview debuginfodwarf debuginfopdb executionengine hexagon hexagonasmparser hexagoncodegen hexagondesc hexagondisassembler hexagoninfo instcombine instrumentation interpreter ipo irreader libdriver lineeditor linker lto mc mcdisassembler mcjit mcparser mips mipsasmparser mipsasmprinter mipscodegen mipsdesc mipsdisassembler mipsinfo mirparser msp430 msp430asmprinter msp430codegen msp430desc msp430info native nativecodegen nvptx nvptxasmprinter nvptxcodegen nvptxdesc nvptxinfo objcarcopts object option orcjit passes powerpc powerpcasmparser powerpcasmprinter powerpccodegen powerpcdesc powerpcdisassembler powerpcinfo profiledata runtimedyld scalaropts selectiondag sparc sparcasmparser sparcasmprinter sparccodegen sparcdesc sparcdisassembler sparcinfo support symbolize systemz systemzasmparser systemzasmprinter systemzcodegen systemzdesc systemzdisassembler systemzinfo tablegen target transformutils vectorize x86 x86asmparser x86asmprinter x86codegen x86desc x86disassembler x86info x86utils xcore xcoreasmprinter xcorecodegen xcoredesc xcoredisassembler xcoreinfo)
set(LLVM_BUILD_TOOLS true)
add_llvm_executable(trace
  trace.cpp
  ExtensibleInterpreter.cpp
)
set_target_properties(trace PROPERTIES 
    COMPILE_FLAGS "-fno-rtti -fvisibility-inlines-hidden"
)
target_link_libraries(trace ffi)

# This allows our interpreter to include:
# ExecutionEngine/Interpreter/Interpreter.h
# which is not part of the installed headers.
include_directories(${LLVM_SRC_DIR}/lib)
