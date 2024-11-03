#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAGIC_NUMBER_SIZE 8  // Размер магического числа
#define OFFSET 0              // Смещение магического числа в файле
#define START_XOR_KEY 0xFF         // Стартовый ключ для XOR
#define STEP_KEY 0x0A        // Размер шага до ключа следующего байта

void alteration (const char *filename) {
    FILE *file = fopen(filename, "rb+"); // Открываем файлв режиме чтение+запись и проверяем успешность
    if (!file) {
        printf("Не удалось открыть файл");
        exit;
    }
    
    unsigned char magic[MAGIC_NUMBER_SIZE];
    
    fseek(file, OFFSET, SEEK_SET); // Читаем текущее magicnum с 0 позиции на указанное число байт
    fread(magic, 1, MAGIC_NUMBER_SIZE, file); 
    
    int key = START_XOR_KEY; // Ставим в переменную ключа стартовое значение

    for (int i = 0; i < MAGIC_NUMBER_SIZE; i++) { // Перебираем в цикле все байты
        int key = STEP_KEY + key; // Создаётся новый ключ для байта
        magic[i] ^= key; // Искажаем значение через XOR
    }
    
    fseek(file, OFFSET, SEEK_SET); // Возвращаем искаженное magicnum обратно в файл
    fwrite(magic, 1, MAGIC_NUMBER_SIZE, file);
    
    fclose(file);  // Закрываем файл и выводим сообщение с результатом
    printf("Файл искажен");
}

int main(int argc, char **argv) {
    if (argc != 3) { // Проверяем входные данные
        printf("Ошибка во входных данных");
        exit;
    }

    if (strcmp(argv[1], "distort") == 0) { // Вызываем функцию в зависимости от режима запуска
        alteration(argv[2]);
        printf("Файл искажен");
    } else if (strcmp(argv[1], "restore") == 0) {
        alteration(argv[2]);
        printf("Файл восстановлен");
    } else {
        printf("Неверная команда: %s. Используйте 'distort' или 'restore'.\n");
        exit;
    }

    return 0;
}
