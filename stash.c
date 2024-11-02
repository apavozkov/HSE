#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAGIC_NUMBER_SIZE 8  // Размер магического числа
#define OFFSET 0              // Смещение магического числа в файле
#define START_XOR_KEY 0xFF         // Стартовый ключ для XOR
#define STEP_KEY 0x0A        // Размер шага до ключа следующего байта

void distort(const char *filename) {
    FILE *file = fopen(filename, "rb+"); // Открываем файлв режиме чтение+запись и проверяем успешность
    if (!file) {
        printf("Не удалось открыть файл");
        exit;
    }
    
    unsigned char magic[MAGIC_NUMBER_SIZE];
    
    fseek(file, OFFSET, SEEK_SET); // Читаем текущее magicnum с 0 позиции на указанное число байт
    fread(magic, 1, MAGIC_NUMBER_SIZE, file); 
    
    int dkey = START_XOR_KEY; // Ставим в переменную ключа стартовое значение

    for (int i = 0; i < MAGIC_NUMBER_SIZE; i++) { // Перебираем в цикле все байты
        int dkey = STEP_KEY + dkey; // Создаётся новый ключ для следующего байта
        magic[i] ^= dkey; // Искажаем значение через XOR
    }
    
    fseek(file, OFFSET, SEEK_SET); // Возвращаем искаженное magicnum обратно в файл
    fwrite(magic, 1, MAGIC_NUMBER_SIZE, file);
    
    fclose(file);  // Закрываем файл и выводим сообщение с результатом
    printf("Файл искажен");
}

void restore(const char *filename) {
    FILE *file = fopen(filename, "rb+"); // Открываем файлв режиме чтение+запись и проверяем успешность
    if (!file) {
        printf("Не удалось открыть файл");
        exit;
    }
    
    unsigned char magic[MAGIC_NUMBER_SIZE];
  
    fseek(file, OFFSET, SEEK_SET); // Читаем текущее magicnum с 0 позиции на указанное число байт
    fread(magic, 1, MAGIC_NUMBER_SIZE, file); // Записываем в массив
    
    int rkey = START_XOR_KEY; // Ставим в переменную ключа стартовое значение

    for (int i = 0; i < MAGIC_NUMBER_SIZE; i++) { // Перебираем в цикле все байты
        int rkey = STEP_KEY + rkey; // Создаётся новый ключ для следующего байта
        magic[i] ^= rkey; // Искажаем значение через XOR
    }
    
    fseek(file, OFFSET, SEEK_SET);  // Возвращаем искаженное magicnum обратно в файл
    fwrite(magic, 1, MAGIC_NUMBER_SIZE, file);
    
    fclose(file); // Закрываем файл и выводим сообщение с результатом
    printf("Файл восстановлен");
}

int main(int argc, char **argv) {
    if (argc != 3) { // Проверяем входные данные
        printf("Ошибка во входных данных");
        exit;
    }

    if (strcmp(argv[1], "distort") == 0) { // Вызываем функцию в зависимости от режима запуска
        distort(argv[2]);
    } else if (strcmp(argv[1], "restore") == 0) {
        restore(argv[2]);
    } else {
        printf("Неверная команда: %s. Используйте 'distort' или 'restore'.\n");
        exit;
    }

    return 0;
}
