# compilers_7_sem_03_task
## Задание: 
  1. Реализовать IR генератор графического приложения.
  2. Подключить интерпретацию с пробросом графических функций в ExecutionEngine.

## Инструкция по сборке и запуску:
  - генерация make файла
```
	mkdir build
    cd build
    cmake ..
    make
```
  - запуск IR генератора
```
	./app_ir_gen > app_ir_gen.txt
```

## Результат работы программы из app_ir_gen.txt:
```
; ModuleID = 'app.c'
source_filename = "app.c"

declare void @simPutPixel(i32, i32, i32)

declare void @simFlush()

define void @app() {
entry:
  br label %outer_loop_header

outer_loop_header:                                ; preds = %outer_loop_latch, %entry
  %0 = phi i32 [ 0, %entry ], [ %47, %outer_loop_latch ]
  %1 = phi i32 [ 226, %entry ], [ %30, %outer_loop_latch ]
  %2 = phi i32 [ 108, %entry ], [ %31, %outer_loop_latch ]
  %3 = phi i32 [ 4, %entry ], [ %17, %outer_loop_latch ]
  %4 = phi i32 [ 5, %entry ], [ %26, %outer_loop_latch ]
  %5 = phi i32 [ 13458524, %entry ], [ %29, %outer_loop_latch ]
  br label %outer_loop_body

outer_loop_body:                                  ; preds = %outer_loop_header
  br label %clear_y_header

clear_y_header:                                   ; preds = %clear_x_end, %outer_loop_body
  %6 = phi i32 [ 0, %outer_loop_body ], [ %10, %clear_x_end ]
  br label %clear_x_header
 ...
```  



