#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void distort(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Не удалось открыть файл для искажения");
        exit;
    }

    FILE *temp_file = fopen("temp.dat", "wb");
    if (!temp_file) {
        printf("Не удалось создать временный файл");
        close(file);
        return;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        fputc(ch ^ 0xFF, temp_file);
    }

    fclose(file);
    fclose(temp_file);

    remove(filename);
    rename("temp.dat", filename);

    printf("Файл %s искажен.\n", filename);
}

void recover_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Не удалось открыть файл для восстановления");
        exit;
    }

    FILE *temp_file = fopen("temp.dat", "wb");
    if (!temp_file) {
        printf("Не удалось создать временный файл");
        close(file);
        exit;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        fputc(ch ^ 0xFF, temp_file);
    }

    close(file);
    close(temp_file);

    remove(filename);
    rename("temp.dat", filename);

    printf("Файл %s восстановлен.\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <режим> <имя файла>\n");
        exit;
    }

    if (strcmp(argv[1], "distort") == 0) {
        distort(argv[2]);
    } else if (strcmp(argv[1], "recover") == 0) {
        recover(argv[2]);
    } else {
        printf(stderr, "Неизвестный режим: %s\n", argv[1]);
        exit;
    }

    return 0;
}
