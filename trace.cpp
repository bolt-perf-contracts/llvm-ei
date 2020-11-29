// A demonstration interpreter based on ExtensibleInterpreter that just prints
// out every instruction executed.

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "ExtensibleInterpreter.h"
#include "iostream"

using namespace llvm;

namespace {
  cl::opt<std::string>
  bitcodeFile(cl::desc("<input bitcode>"), cl::Positional, cl::init("-"));

  cl::list<std::string>
  commandArgs(cl::ConsumeAfter, cl::desc("<program arguments>..."));
}

class Tracer : public ExtensibleInterpreter {
public:
  Tracer(Module *M) : ExtensibleInterpreter(M) {};
  virtual void execute(llvm::Instruction &I) {
	llvm::Function* f = I.getParent()->getParent();
	std::cout << f->getName().data() << "|\n";
	I.dump();
    ExtensibleInterpreter::execute(I);
  }
};

int main(int argc, char **argv, char * const *envp) {
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);

  cl::ParseCommandLineOptions(argc, argv,
                              "EnerC execution tracer\n");

  cl::opt<std::string> startFunction("start_func", cl::desc("Specify start function"), cl::value_desc("start_func"));
  return interpret<Tracer>(bitcodeFile, commandArgs, envp);
}
