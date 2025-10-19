#!/bin/bash

echo "Компиляция LLVM Pass"

clang -c sim.c -o sim.o `sdl2-config --cflags`
clang -c start.c -o start.o
clang++ -c instr_logger.cpp -o instr_logger.o
clang++ -shared -fPIC instr_tracer.cpp -o libInstrTracer.so `llvm-config --cxxflags --ldflags --libs core support`

echo "Создание инструментированного приложения"

clang -S -emit-llvm -O1 app.c -o app.ll
opt -load-pass-plugin=./libInstrTracer.so -passes="instr-tracer" app.ll -o app_instrumented.ll
clang app_instrumented.ll start.o sim.o instr_logger.o -o app_instrumented `sdl2-config --libs` -lstdc++
