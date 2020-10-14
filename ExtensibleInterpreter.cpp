#include "ExtensibleInterpreter.h"
#include <cerrno>
#include "llvm/Support/DynamicLibrary.h"

using namespace llvm;

ExtensibleInterpreter::ExtensibleInterpreter(Module *M) :
      ExecutionEngine(std::unique_ptr<Module>{M}),
      module(M)
{
  interp = new Interpreter(std::unique_ptr<Module>{M});
  pubInterp = (PublicInterpreter *)interp;  // GIANT HACK
  module = M;
  M->setDataLayout(interp->getDataLayout());
  // This is hacky: remove M from the ExecutionEngine's module list to avoid a
  // double-delete when both it and the Interpreter are destructed.
  //Modules.clear();

}

ExtensibleInterpreter::~ExtensibleInterpreter() {
  delete interp;
}


// The main interpreter loop.

void ExtensibleInterpreter::run() {
  while (!pubInterp->ECStack.empty()) {
    ExecutionContext &SF = pubInterp->ECStack.back();
    Instruction &I = *SF.CurInst++;

    execute(I);
  }
}

void ExtensibleInterpreter::execute(Instruction &I) {
  interp->visit(I);
}


// Convenient entry point.

int ExtensibleInterpreter::runMain(std::vector<std::string> args,
                                   char * const *envp) {
  // Get the main function from the module.
  Function *EntryFn = module->getFunction("main");

  if (!EntryFn) {
    errs() << '\'' << "main" << "\' function not found in module.\n";
    return -1;
  }

  // Reset errno to zero on entry to main.
  errno = 0;

  std::string ErrorMsg;
  if (sys::DynamicLibrary::LoadLibraryPermanently(0, &ErrorMsg)) {
    errs() << "???\n";
    return -1;
  }

  // Run main.
  return runFunctionAsMain(EntryFn, args, envp);
}


// The ExecutionEngine interface.

GenericValue ExtensibleInterpreter::runFunction(
    Function *F,
    llvm::ArrayRef<llvm::GenericValue> ArgValues
) {
  // Copied & pasted from Interpreter, essentially.
  std::vector<GenericValue> ActualArgs;
  const unsigned ArgCount = F->getFunctionType()->getNumParams();
  for (unsigned i = 0; i < ArgCount; ++i)
    ActualArgs.push_back(ArgValues[i]);
  interp->callFunction(F, ActualArgs);
  run();
  return pubInterp->ExitValue;
}
void *ExtensibleInterpreter::getPointerToNamedFunction(
    llvm::StringRef,
    bool AbortOnFailure
) {
  return 0;
}
void *ExtensibleInterpreter::recompileAndRelinkFunction(Function *F) {
  return getPointerToFunction(F);
}
void ExtensibleInterpreter::freeMachineCodeForFunction(Function *F) {
}
void *ExtensibleInterpreter::getPointerToFunction(Function *F) {
  return (void*)F;
}
void *ExtensibleInterpreter::getPointerToBasicBlock(BasicBlock *BB) {
  return (void*)BB;
}
