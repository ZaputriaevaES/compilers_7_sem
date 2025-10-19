# compilers_7_sem_02_task
## Задание: 
  1. С помощью инструментирующего Pass собрать в рантайме трассу исполненных IR инструкций
  2. Провести анализ часто повторяемых паттернов инструкций длины 1-5

## Как работает программа:
- Сначала компилируется LLVM Pass ibInstrTracer.so, который позволит в рантайме собрать статистику исполняемых IR инструкций. Потом с его помощью создается инструментированное графическое приложение app_instrumented. И в конце запускается питоновский скрипт stat_analyser.py, который собирает статистику паттернов IR инструкций.


## Инструкция по запуску через bash скрипт:

```bash
chmod +x build.sh
./build.sh
./app_instrumented
python3 stat_analyser.py
```

## Инструкция по запуску вручную:
  - Компиляция LLVM Pass
```bash
clang -c sim.c -o sim.o `sdl2-config --cflags`
clang -c start.c -o start.o
clang++ -c instr_logger.cpp -o instr_logger.o
clang++ -shared -fPIC instr_tracer.cpp -o libInstrTracer.so `llvm-config --cxxflags --ldflags --libs core support`
```  
  - Создание инструментированного приложения
```bash
clang -S -emit-llvm -O1 app.c -o app.ll
opt -load-pass-plugin=./libInstrTracer.so -passes="instr-tracer" app.ll -o app_instrumented.ll
clang app_instrumented.ll start.o sim.o instr_logger.o -o app_instrumented `sdl2-config --libs` -lstdc++

```   
  - Запуск анализа паттернов
```bash
python3 stat_analyser.py app_instrumented
```   
