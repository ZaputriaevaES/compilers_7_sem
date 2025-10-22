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

## Результат работы программы из stat_pattern.txt:
```
Статистика паттернов инструкций app
--------------------------------------------------
Всего уникальных паттернов: 56

Паттерны длины 1 (Топ-20):
--------------------------------------------------
 1. br - 4949 повторений
 2. add - 4935 повторений
 3. icmp - 4935 повторений
 4. call - 4926 повторений

Всего уникальных паттернов длины 1: 4

Паттерны длины 2 (Топ-20):
--------------------------------------------------
 1. icmp -> br - 4935 повторений
 2. add -> icmp - 4934 повторений
 3. br -> call - 4926 повторений
 4. call -> add - 4925 повторений
 5. br -> br - 12 повторений
 6. br -> add - 10 повторений
 7. call -> icmp - 1 повторений
 8. add -> br - 1 повторений
 ...
```   