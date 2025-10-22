#include "sim.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

int main() 
{
  LLVMContext context;
  Module *module = new Module("app.c", context);
  IRBuilder<> builder(context);

  // объявляем внешних функций 
  // #include "sim.h"
  Type *voidType = Type::getVoidTy(context);
  Type *int32Type = Type::getInt32Ty(context);
  
  // объявляем функцю для рисования пикселя
  // void @simPutPixel(i32, i32, i32)
  ArrayRef<Type *> putPixelParams = {int32Type, int32Type, int32Type};
  FunctionType *simPutPixelType = FunctionType::get(voidType, putPixelParams, false);
  FunctionCallee simPutPixelFunc = module->getOrInsertFunction("simPutPixel", simPutPixelType);

  // объявляем функцию для обновления экрана
  // void @simFlush() 
  FunctionType *simFlushType = FunctionType::get(voidType, false);
  FunctionCallee simFlushFunc = module->getOrInsertFunction("simFlush", simFlushType);

  // объявление ыункции app()
  // void @app()
  FunctionType *appFuncType = FunctionType::get(voidType, false);
  Function *appFunc = Function::Create(appFuncType, Function::ExternalLinkage, "app", module);


  // создание основных базовых блоков
  BasicBlock *entryBB = BasicBlock::Create(context, "entry", appFunc);
  BasicBlock *outerLoopHeader = BasicBlock::Create(context, "outer_loop_header", appFunc);
  BasicBlock *outerLoopBody = BasicBlock::Create(context, "outer_loop_body", appFunc); 
  
  // создание базовых блоков для очистки экрана (вложенные циклы)
  BasicBlock *clearYHeader = BasicBlock::Create(context, "clear_y_header", appFunc);
  BasicBlock *clearYBody = BasicBlock::Create(context, "clear_y_body", appFunc);
  BasicBlock *clearXHeader = BasicBlock::Create(context, "clear_x_header", appFunc);
  BasicBlock *clearXBody = BasicBlock::Create(context, "clear_x_body", appFunc);
  BasicBlock *clearXEnd = BasicBlock::Create(context, "clear_x_end", appFunc);
  BasicBlock *clearYEnd = BasicBlock::Create(context, "clear_y_end", appFunc);
  
  // создание базовых блоков для проверки столкновений и обновления позиции
  BasicBlock *bounceCheck = BasicBlock::Create(context, "bounce_check", appFunc);
  BasicBlock *updateLogo = BasicBlock::Create(context, "update_logo", appFunc);
  
  // создание базовых блоков для отрисовки логотипа (вложенные циклы)
  BasicBlock *drawYHeader = BasicBlock::Create(context, "draw_y_header", appFunc);
  BasicBlock *drawYBody = BasicBlock::Create(context, "draw_y_body", appFunc);
  BasicBlock *drawXHeader = BasicBlock::Create(context, "draw_x_header", appFunc);
  BasicBlock *drawXBody = BasicBlock::Create(context, "draw_x_body", appFunc);
  BasicBlock *drawPixelBB = BasicBlock::Create(context, "draw_pixel", appFunc);
  BasicBlock *skipPixelBB = BasicBlock::Create(context, "skip_pixel", appFunc);
  BasicBlock *drawXEnd = BasicBlock::Create(context, "draw_x_end", appFunc);
  BasicBlock *drawYEnd = BasicBlock::Create(context, "draw_y_end", appFunc);
  
  // создание базовых блоков для завершения кадра и основного цикла
  BasicBlock *flushBB = BasicBlock::Create(context, "flush", appFunc);
  BasicBlock *outerLoopLatch = BasicBlock::Create(context, "outer_loop_latch", appFunc);
  BasicBlock *exitBB = BasicBlock::Create(context, "exit", appFunc);
  
  builder.SetInsertPoint(entryBB);
  
  // инициализация переменных и констант
  // #define SIM_X_SIZE 512
  // #define SIM_Y_SIZE 256
  // int logo_width = 60;
  // int logo_height = 40;
  Constant *logoWidth = builder.getInt32(60);
  Constant *logoHeight = builder.getInt32(40);
  Constant *simXSize = builder.getInt32(SIM_X_SIZE);
  Constant *simYSize = builder.getInt32(SIM_Y_SIZE);
  Constant *halfX = builder.getInt32(SIM_X_SIZE / 2);
  Constant *halfY = builder.getInt32(SIM_Y_SIZE / 2);
  Constant *halfLogoWidth = builder.getInt32(30);
  Constant *halfLogoHeight = builder.getInt32(20);
  
  // logo_x = SIM_X_SIZE / 2 - logo_width / 2
  // logo_y = SIM_Y_SIZE / 2 - logo_height / 2
  Value *logoX = builder.CreateSub(halfX, halfLogoWidth);
  Value *logoY = builder.CreateSub(halfY, halfLogoHeight);

  // int speed_x = 4;
  // int speed_y = 5;
  // int bg_color = 0xFF000000;
  // int logo_color = 0xCD5C5C;
  Constant *speedX = builder.getInt32(4);
  Constant *speedY = builder.getInt32(5);
  Constant *bgColor = builder.getInt32(0xFF000000);
  Constant *initialLogoColor = builder.getInt32(0xCD5C5C);
  Constant *altLogoColor = builder.getInt32(0x90EE90);
  
  // переходим к основному циклу анимации
  // for (int step = 0; step < 200; ++step)
  builder.CreateBr(outerLoopHeader);
  builder.SetInsertPoint(outerLoopHeader);
  
  PHINode *stepPhi = builder.CreatePHI(int32Type, 2);
  PHINode *logoXPhi = builder.CreatePHI(int32Type, 2);
  PHINode *logoYPhi = builder.CreatePHI(int32Type, 2);
  PHINode *currentSpeedX = builder.CreatePHI(int32Type, 2);
  PHINode *currentSpeedY = builder.CreatePHI(int32Type, 2);
  PHINode *currentLogoColor = builder.CreatePHI(int32Type, 2);
  
  // инициализируем PHI-узлы начальными значениями
  // step = 0
  // logo_x = 226
  // logo_y = 108
  // speed_x = 4
  // speed_y = 5
  // logo_color = 0xCD5C5C
  stepPhi->addIncoming(builder.getInt32(0), entryBB);
  logoXPhi->addIncoming(logoX, entryBB);                         
  logoYPhi->addIncoming(logoY, entryBB);                         
  currentSpeedX->addIncoming(speedX, entryBB);                   
  currentSpeedY->addIncoming(speedY, entryBB);                   
  currentLogoColor->addIncoming(initialLogoColor, entryBB);      
  
  // переходим к телу главного цикла
  // for (int y = 0; y < SIM_Y_SIZE; ++y) 
  // {
  //   for (int x = 0; x < SIM_X_SIZE; ++x) 
  //   {
  //     simPutPixel(x, y, bg_color);
  //   }
  // }
  builder.CreateBr(outerLoopBody);
  builder.SetInsertPoint(outerLoopBody);
  builder.CreateBr(clearYHeader);
  
  // внешний цикл очистки по Y координате
  builder.SetInsertPoint(clearYHeader);
  PHINode *clearY = builder.CreatePHI(int32Type, 2); 
  clearY->addIncoming(builder.getInt32(0), outerLoopBody);
  builder.CreateBr(clearXHeader);
  
  // внутренний цикл очистки  по X координате
  builder.SetInsertPoint(clearXHeader);
  PHINode *clearX = builder.CreatePHI(int32Type, 2);
  clearX->addIncoming(builder.getInt32(0), clearYHeader);
  builder.CreateBr(clearXBody);
  
  // тело цикла очистки
  builder.SetInsertPoint(clearXBody);
  // simPutPixel(x, y, bg_color)
  Value *clearArgs[] = {clearX, clearY, bgColor};
  builder.CreateCall(simPutPixelFunc, clearArgs);
  
  // x = x + 1
  Value *nextX = builder.CreateAdd(clearX, builder.getInt32(1));
  clearX->addIncoming(nextX, clearXBody);
  
  // x < SIM_X_SIZE
  Value *xCleared = builder.CreateICmpEQ(nextX, simXSize);
  builder.CreateCondBr(xCleared, clearXEnd, clearXHeader);
  
  // y = y + 1
  builder.SetInsertPoint(clearXEnd);
  Value *nextY = builder.CreateAdd(clearY, builder.getInt32(1));
  clearY->addIncoming(nextY, clearXEnd);
  
  // y < SIM_Y_SIZE
  Value *yCleared = builder.CreateICmpEQ(nextY, simYSize);
  builder.CreateCondBr(yCleared, clearYEnd, clearYHeader); 
  
  // конец внешнего цикла по Y
  builder.SetInsertPoint(clearYEnd);
  builder.CreateBr(bounceCheck);

  // проверка столкновений со стенками
  // if (logo_x <= 0 || logo_x + logo_width >= SIM_X_SIZE - 1) 
  // {
  //   speed_x = -speed_x;
  //   logo_color = (logo_color == 0xCD5C5C) ? 0x90EE90 : 0xCD5C5C;
  // }
  // if (logo_y <= 0 || logo_y + logo_height >= SIM_Y_SIZE - 1) 
  // {
  //   speed_y = -speed_y;
  //   logo_color = (logo_color == 0xCD5C5C) ? 0x90EE90 : 0xCD5C5C;
  // }
  builder.SetInsertPoint(bounceCheck);

  // logo_x <= 0 || logo_x + logo_width >= SIM_X_SIZE - 1
  Value *leftCollision = builder.CreateICmpSLE(logoXPhi, builder.getInt32(0)); 
  Value *logoRight = builder.CreateAdd(logoXPhi, logoWidth);                            
  Value *rightCollision = builder.CreateICmpSGE(logoRight, builder.getInt32(SIM_X_SIZE - 1)); 
  Value *xCollision = builder.CreateOr(leftCollision, rightCollision); 

  // peed_x = -speed_x;
  Value *newSpeedX = builder.CreateSelect(xCollision, builder.CreateNeg(currentSpeedX), currentSpeedX); 

  // logo_color = (logo_color == 0xCD5C5C) ? 0x90EE90 : 0xCD5C5C;
  Value *isCurrentRed = builder.CreateICmpEQ(currentLogoColor, initialLogoColor);
  Value *newColorAfterX = builder.CreateSelect(isCurrentRed, altLogoColor, initialLogoColor);
  Value *finalColorAfterX = builder.CreateSelect(xCollision, newColorAfterX, currentLogoColor);

  // logo_y <= 0 || logo_y + logo_height >= SIM_Y_SIZE - 1;
  Value *topCollision = builder.CreateICmpSLE(logoYPhi, builder.getInt32(0)); 
  Value *logoBottom = builder.CreateAdd(logoYPhi, logoHeight);
  Value *bottomCollision = builder.CreateICmpSGE(logoBottom, builder.getInt32(SIM_Y_SIZE - 1));
  Value *yCollision = builder.CreateOr(topCollision, bottomCollision);

  // speed_y = -speed_y;
  Value *newSpeedY = builder.CreateSelect(yCollision, builder.CreateNeg(currentSpeedY), currentSpeedY);

  // logo_color = (logo_color == 0xCD5C5C) ? 0x90EE90 : 0xCD5C5C;
  Value *isCurrentColor = builder.CreateICmpEQ(finalColorAfterX, initialLogoColor); 
  Value *newColorAfterY = builder.CreateSelect(isCurrentColor, altLogoColor, initialLogoColor);
  Value *finalColor = builder.CreateSelect(yCollision, newColorAfterY, finalColorAfterX);

  builder.CreateBr(updateLogo);

  // обновление позиции логотипа
  // logo_x += speed_x;
  // logo_y += speed_y;
  builder.SetInsertPoint(updateLogo);
  Value *updatedLogoX = builder.CreateAdd(logoXPhi, newSpeedX);
  Value *updatedLogoY = builder.CreateAdd(logoYPhi, newSpeedY);
  
  builder.CreateBr(drawYHeader);

  // отрисовка логотипа
  // for (int y = logo_y; y < logo_y + logo_height; ++y) 
  // {
  //   for (int x = logo_x; x < logo_x + logo_width; ++x) 
  //   {
  //     if (x >= 0 && x < SIM_X_SIZE && y >= 0 && y < SIM_Y_SIZE) 
  //     {
  //       simPutPixel(x, y, logo_color);
  //     }
  //   }
  // }
  
  // внешний цикл отрисовки по Y координате логотипа
  builder.SetInsertPoint(drawYHeader);
  PHINode *drawY = builder.CreatePHI(int32Type, 2);
  drawY->addIncoming(updatedLogoY, updateLogo);
  
  // y < logo_y + logo_height
  Value *drawYEndCond = builder.CreateICmpSGE(drawY, builder.CreateAdd(updatedLogoY, logoHeight));
  builder.CreateCondBr(drawYEndCond, drawYEnd, drawYBody);

  builder.SetInsertPoint(drawYBody);
  builder.CreateBr(drawXHeader);

  // внутренний цикл отрисовки  по X координате логотипа
  builder.SetInsertPoint(drawXHeader);
  PHINode *drawX = builder.CreatePHI(int32Type, 2);
  drawX->addIncoming(updatedLogoX, drawYBody);
  
  // x < logo_x + logo_width
  Value *drawXEndCond = builder.CreateICmpSGE(drawX, builder.CreateAdd(updatedLogoX, logoWidth));
  builder.CreateCondBr(drawXEndCond, drawXEnd, drawXBody);

  // тело цикла отрисовки
  builder.SetInsertPoint(drawXBody);
  
  // if (x >= 0 && x < SIM_X_SIZE && y >= 0 && y < SIM_Y_SIZE)
  Value *xInBounds1 = builder.CreateICmpSGE(drawX, builder.getInt32(0));
  Value *xInBounds2 = builder.CreateICmpSLT(drawX, simXSize);
  Value *xInBounds = builder.CreateAnd(xInBounds1, xInBounds2);
  
  Value *yInBounds1 = builder.CreateICmpSGE(drawY, builder.getInt32(0));
  Value *yInBounds2 = builder.CreateICmpSLT(drawY, simYSize);
  Value *yInBounds = builder.CreateAnd(yInBounds1, yInBounds2);
  
  Value *inBounds = builder.CreateAnd(xInBounds, yInBounds);
  
  builder.CreateCondBr(inBounds, drawPixelBB, skipPixelBB);
  
  // simPutPixel(x, y, logo_color)
  builder.SetInsertPoint(drawPixelBB);
  Value *drawArgs[] = {drawX, drawY, finalColor};
  builder.CreateCall(simPutPixelFunc, drawArgs);
  builder.CreateBr(skipPixelBB);
  
  // x = x + 1
  builder.SetInsertPoint(skipPixelBB);
  Value *nextDrawX = builder.CreateAdd(drawX, builder.getInt32(1));
  drawX->addIncoming(nextDrawX, skipPixelBB);
  builder.CreateBr(drawXHeader);

  // конец внутреннего цикла по X
  builder.SetInsertPoint(drawXEnd);
  // y = y + 1
  Value *nextDrawY = builder.CreateAdd(drawY, builder.getInt32(1)); 
  drawY->addIncoming(nextDrawY, drawXEnd);
  builder.CreateBr(drawYHeader); 

  // конец внешнего цикла по Y
  builder.SetInsertPoint(drawYEnd);
  builder.CreateBr(flushBB);

  // обновление экрана
  // simFlush();
  builder.SetInsertPoint(flushBB);
  builder.CreateCall(simFlushFunc);
  builder.CreateBr(outerLoopLatch);

  // обновление счетчика кадров
  // ++step
  builder.SetInsertPoint(outerLoopLatch);
  Value *nextStep = builder.CreateAdd(stepPhi, builder.getInt32(1));
  
  // обновляем PHI-узлы для следующей итерации главного цикла
  stepPhi->addIncoming(nextStep, outerLoopLatch);
  logoXPhi->addIncoming(updatedLogoX, outerLoopLatch);
  logoYPhi->addIncoming(updatedLogoY, outerLoopLatch);
  currentSpeedX->addIncoming(newSpeedX, outerLoopLatch);
  currentSpeedY->addIncoming(newSpeedY, outerLoopLatch);
  currentLogoColor->addIncoming(finalColor, outerLoopLatch);
  
  // step < 200;
  Value *loopEnd = builder.CreateICmpEQ(nextStep, builder.getInt32(200));
  builder.CreateCondBr(loopEnd, exitBB, outerLoopHeader);

  // return из функции app()
  builder.SetInsertPoint(exitBB);
  builder.CreateRetVoid();  

  // дамп LLVM IR 
  module->print(outs(), nullptr);
  outs() << '\n';
  
  // проверяем валидность сгенерированного IR кода
  bool verif = verifyModule(*module, &outs());
  outs() << "[VERIFICATION] " << (verif ? "FAIL\n\n" : "OK\n\n");

  // выполняем сгенерированный IR код
  outs() << "[EE] Run\n";
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
  ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
    if (fnName == "simPutPixel") {
      return reinterpret_cast<void *>(simPutPixel);
    }
    if (fnName == "simFlush") {
      return reinterpret_cast<void *>(simFlush);
    }
    return nullptr;
  });
  ee->finalizeObject();

  simInit();

  ArrayRef<GenericValue> noargs;
  GenericValue v = ee->runFunction(appFunc, noargs);
  outs() << "[EE] Result: " << v.IntVal << '\n';

  simExit();
  return EXIT_SUCCESS;
}