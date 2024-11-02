#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void distort_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Не удалось открыть файл для искажения");
        return;
    }

    FILE *temp_file = fopen("temp.dat", "wb");
    if (!temp_file) {
        perror("Ошибка создания временного файла");
        fclose(file);
        return;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        // Применение простого XOR шифрования
        fputc(ch ^ 0xFF, temp_file);
    }

    fclose(file);
    fclose(temp_file);

    // Заменяем оригинальный файл временным
    remove(filename);
    rename("temp.dat", filename);

    printf("Файл %s искажен.\n", filename);
}

void recover_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Ошибка открытия файла для восстановления");
        return;
    }

    FILE *temp_file = fopen("temp.dat", "wb");
    if (!temp_file) {
        perror("Ошибка создания временного файла");
        fclose(file);
        return;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        // Восстановление простым XOR шифрованием
        fputc(ch ^ 0xFF, temp_file);
    }

    fclose(file);
    fclose(temp_file);

    // Заменяем оригинальный файл временным
    remove(filename);
    rename("temp.dat", filename);

    printf("Файл %s восстановлен.\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <режим> <имя файла>\n", argv[0]);
        fprintf(stderr, "<режим> может быть 'distort' для искажения или 'recover' для восстановления.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "distort") == 0) {
        distort_file(argv[2]);
    } else if (strcmp(argv[1], "recover") == 0) {
        recover_file(argv[2]);
    } else {
        fprintf(stderr, "Неизвестный режим: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
