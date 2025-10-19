import subprocess
import time
from collections import Counter

def capture_instruction_trace() -> list[str]:
    print("Запуск инструментированного приложения app")
    
    # запускаем инструементированное приложение на 5 секунд
    process = subprocess.Popen(
        ["./app_instrumented"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1,
        universal_newlines=True
    )
    
    instructions = []
    start_time = time.time()
    duration = 5.0
    
    try:
        while time.time() - start_time < duration:
            if process.poll() is not None:
                break
                
            # Читаем строку вывода и фильтруем
            line = process.stdout.readline()
            if line:
                instruction = line.strip()
                if instruction and len(instruction) > 1:
                    instructions.append(instruction)
            
            time.sleep(0.001)
    
    except Exception as e:
        print(f"Ошибка при захвате трассы: {e}")
    
    finally:
        # завершаем процесс
        if process.poll() is None:
            process.terminate()
            try:
                process.wait(timeout=2)
            except subprocess.TimeoutExpired:
                process.kill()
    
    # читаем оставшиеся данные и фильтруем
    try:
        remaining_output, errors = process.communicate(timeout=1)
        if remaining_output:
            remaining_instructions = [
                line.strip() for line in remaining_output.splitlines() 
                if line.strip() and len(line.strip()) > 1
            ]
            instructions.extend(remaining_instructions)
    except:
        pass
    
    print(f"Прочитано {len(instructions)} инструкций")
    
    return instructions

def analyze_patterns(instructions: list[str], max_pattern_length: int) -> dict[int, dict[str, int]]:
    instruction_count = len(instructions)
    pattern_statistics = {}

    # анализируем паттерны длиной от 1 до 5
    for pattern_length in range(1, max_pattern_length + 1):
        length_stats = {}
        
        for start_index in range(instruction_count - pattern_length + 1):
            pattern = ' -> '.join(instructions[start_index:start_index + pattern_length])
            length_stats[pattern] = length_stats.get(pattern, 0) + 1
            
        pattern_statistics[pattern_length] = length_stats

    return pattern_statistics

def save_statistics(pattern_statistics: dict[int, dict[str, int]]):
    output_file = "stat_pattern.txt"
    
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("Статистика паттернов инструкций app\n")
        f.write("-" * 50 + "\n")
        
        total_instructions = sum(len(stats) for stats in pattern_statistics.values())
        f.write(f"Всего уникальных паттернов: {total_instructions}\n\n")
        
        # записываем статистику для каждой длины паттерна
        for pattern_length in range(1, 6):
            stats = pattern_statistics.get(pattern_length, {})
            f.write(f"Паттерны длины {pattern_length} (Топ-20):\n")
            f.write("-" * 50 + "\n")
            
            if not stats:
                f.write("Паттерны не найдены\n\n")
                continue
            
            sorted_patterns = sorted(stats.items(), key=lambda x: x[1], reverse=True)
            
            for i, (pattern, count) in enumerate(sorted_patterns[:20], 1):
                f.write(f"{i:2d}. {pattern} - {count} повторений\n")
            
            f.write(f"\nВсего уникальных паттернов длины {pattern_length}: {len(stats)}\n\n")

def main() -> None:
    try:
        # захватываем трассу инструкций
        instruction_trace = capture_instruction_trace()
        
        if not instruction_trace:
            print("Инструкции не захвачены!")
            return
        
        # анализируем паттерны длиной 1-5
        stat_pattern = analyze_patterns(instruction_trace, max_pattern_length=5)
        
        # сохраняем статистику в файл
        save_statistics(stat_pattern)

        print("Статистика сохранена в pattern_stats.txt")
        
    except Exception as error:
        print(f"Анализ завершился ошибкой: {error}")
        raise

if __name__ == '__main__':
    main()