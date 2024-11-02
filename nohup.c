#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

void detach() {
    pid_t pid = fork(); // Создаём новый процесс

    if (pid < 0) { // Проверяем, не возникло ли ошибки при создании процесса, остальные значения нас устраивают
        printf("Не удалось создать новый процесс");
        exit;
    }

    if (pid == 0) { // Мы в дочернем процессе
        if (setsid() < 0) { // Создаём новый сеанс, проверяем успешность этого действия
            printf("Не удалось создать новый сеанс");
            exit;
        }

        close(STDIN_FILENO); // Обрубаем зависимость от тьерминала, закрывая дескрипторы ввода, вывода и ошибок
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        int fd = open("/dev/null", O_RDWR); // Открываем /dev/null и проверяем успешность
        if (fd < 0) { 
            printf("Не удалось открыть /dev/null");
            exit;
        }

        dup2(fd, STDIN_FILENO); // НАправляем дескрипторы в /dev/null
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        char *args[] = {"path_program", NULL}; // Запускаем целевую программу
        execvp(args[0], args);
        printf("Не удалось запустить целевую программу");
        exit;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) { // Проверяем входные данные
        printf("Ошибка во входных данных");
        exit;
    }

    detach(); // Запускаем основную рабочую функцию

    return 0;
}
