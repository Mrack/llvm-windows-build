#pragma comment(linker, "/export:llvmGetPassPluginInfo")

#include <iostream>
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

StringRef md5(StringRef str) {
    using namespace support;
    MD5 Hash;
    Hash.update(str);
    MD5::MD5Result Result{};
    Hash.final(Result);
    return Result.digest().str();
}

namespace llvm {
    struct NewPassHelloWorld : public PassInfoMixin<NewPassHelloWorld> {
        static PreservedAnalyses run(Module &module, ModuleAnalysisManager &AM) {
            for (auto &F: module) {
                if (F.hasComdat() || F.isDeclaration() ||
                    F.getName().compare("main") == 0)
                    continue;

                std::cout << "Function Encryption: " << F.getName().str() << std::endl;
                F.setName(md5(F.getName()));
            }
            return PreservedAnalyses::all();
        }
    };
}


bool registerNewPassHelloWorld(ModulePassManager &FPM) {
    FPM.addPass(NewPassHelloWorld());
    return true;
}

extern "C" ::llvm::PassPluginLibraryInfo
LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
    return {
            LLVM_PLUGIN_API_VERSION, "NewPassHelloWorld", "v0.1",
            [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                        [](StringRef PassName, ModulePassManager &FPM, ...) {
                            if (PassName == "NewPassHelloWorld") {
                                registerNewPassHelloWorld(FPM);
                                return true;
                            }
                            return false;
                        }
                );
            }
    };
}