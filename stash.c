#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAGIC_NUMBER_SIZE 8  // Размер магического числа
#define OFFSET 0              // Смещение магического числа в файле
#define START_XOR_KEY 0xFF         // Стартовый ключ для XOR

void distort(const char *filename) {
    FILE *file = fopen(filename, "rb+"); // Открываем файлв режиме чтение+запись и проверяем успешность
    if (!file) {
        printf("Не удалось открыть файл");
        exit;
    }
    
    unsigned char magic[MAGIC_NUMBER_SIZE];
    
    fseek(file, OFFSET, SEEK_SET); // Читаем текущее magicnum с 0 позиции на указанное число байт
    fread(magic, 1, MAGIC_NUMBER_SIZE, file); // Записываем в массив
    
    int key = START_XOR_KEY;

    for (int i = 0; i < MAGIC_NUMBER_SIZE; i++) {
        int key += 0x0A
        magic[i] ^= key;
    }
    
    fseek(file, OFFSET, SEEK_SET);
    fwrite(magic, 1, MAGIC_NUMBER_SIZE, file);
    
    fclose(file);
    printf("Файл искажен");
}

void restore(const char *filename) {
    FILE *file = fopen(filename, "rb+");
    if (!file) {
        printf("Не удалось открыть файл");
        exit;
    }
    
    unsigned char magic[MAGIC_NUMBER_SIZE];
  
    fseek(file, OFFSET, SEEK_SET);
    fread(magic, 1, MAGIC_NUMBER_SIZE, file);
    
    for (int i = 0; i < MAGIC_NUMBER_SIZE; i++) {
        magic[i] ^= XOR_KEY;
    }
    
    fseek(file, OFFSET, SEEK_SET);
    fwrite(magic, 1, MAGIC_NUMBER_SIZE, file);
    
    fclose(file);
    printf("Файл восстановлен");
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Ошибка во входных данных");
        exit;
    }

    if (strcmp(argv[1], "distort") == 0) {
        distort(argv[2]);
    } else if (strcmp(argv[1], "restore") == 0) {
        restore(argv[2]);
    } else {
        printf("Неверная команда: %s. Используйте 'distort' или 'restore'.\n");
        exit;
    }

    return 0;
}


int key = START_XOR_KEY;

    for (int i = 0; i < MAGIC_NUMBER_SIZE; i++) {
        int key += 0x0A
        magic[i] ^= key;
    }





