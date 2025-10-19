#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/Passes/OptimizationLevel.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace {

// имя и тип функции логирования
StringRef TraceFunctionName{"recordInstructionOpcode"};

FunctionType* getTraceFunctionType(LLVMContext &Context) 
{
  Type* returnType = Type::getVoidTy(Context);
  PointerType* argumentType = Type::getInt8Ty(Context)->getPointerTo();
  return FunctionType::get(returnType, ArrayRef<Type*>{argumentType}, 
                          false /* isVarArg */);
}

struct InstructionPatternTracer final : public PassInfoMixin<InstructionPatternTracer> 
{
  // основной метод pass
  PreservedAnalyses run(Module &module, ModuleAnalysisManager &analysisManager) 
  {
    LLVMContext &context = module.getContext();
    // создаем функцию логирования
    FunctionType* traceFunctionType = getTraceFunctionType(context);
    FunctionCallee traceFunction = module.getOrInsertFunction(TraceFunctionName, traceFunctionType);
    IRBuilder<> builder(context);

    auto isNotTraceFunction = [](const Function &function) 
    {
      return function.getName() != TraceFunctionName;
    };

    // проходим по всем инструкциям, пропуская PHI-узлы
    for (auto &function : make_filter_range(module, isNotTraceFunction)) 
    {
      for (auto &basicBlock : function) 
      {
        for (auto &instruction : basicBlock) 
        {
          if (isa<PHINode>(&instruction)) 
          {
            continue;
          }
          
          // вставляем вызов функции логирования перед текущей инструкцией
          builder.SetInsertPoint(&instruction);
          Constant* opcodeString = builder.CreateGlobalStringPtr(instruction.getOpcodeName());
          builder.CreateCall(traceFunction, ArrayRef<Value*>{opcodeString});
        }
      }
    }
    return PreservedAnalyses::none();
  }

  static bool isRequired() 
  { 
    return true; 
  }
};

}

// регистрируем pass под именем "instr-tracer"
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() 
{
  return 
  {
    LLVM_PLUGIN_API_VERSION, 
    "InstructionPatternTracer", 
    "1.0.0",
    [](PassBuilder &PB) 
    {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
           ArrayRef<PassBuilder::PipelineElement>) 
        {
          if (Name == "instr-tracer") 
          {
            MPM.addPass(InstructionPatternTracer{});
            return true;
          }
          return false;
        });
    }
  };
}