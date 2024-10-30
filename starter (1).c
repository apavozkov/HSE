#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Некорректные данные на входе"); // Проверяем, что на вход пришла команда
        exit;
    }

    pid_t pid = fork(); // Через fork создаём дочерний процесс-копию и проверяем его состояние
    if (pid == -1) {
        printf("Ошибка в fork");
        exit;
    }

    if (pid == 0) { // Проверка, находимся ли мы в дочернем процессе
        int out_fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); // Создаём файл для вывода с флагами и проверяем возможность открытия
        if (out_fd == -1) {
            printf("Не удалось открыть out.txt");
            exit;
        }

        int err_fd = open("err.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); //Создаём файл для ошибок с флагами и проверяем возможность открытия
        if (err_fd == -1) {
            printf("Не удалось открыть err.txt");
            close(out_fd);
            exit;
        }

        if (dup2(out_fd, STDOUT_FILENO) == -1) { // Перенаправляем стандартный вывод на файловый дескриптор и проверяем
            printf("Ошибка перенаправления вывода");
            close(out_fd);
            close(err_fd);
            exit;
        }

        if (dup2(err_fd, STDERR_FILENO) == -1) { // Перенаправляем вывод ошибок на файловый дескриптор и проверяем
            printf("Ошибка перенаправления ошибок");
            close(out_fd);
            close(err_fd);
            exit;
        }

        close(out_fd); // Закрываем исходные дескрипторы
        close(err_fd);

        execvp(argv[1], &argv[1]); // Запускаем программу, которую передали в аргументе
        printf("Ошибка при запуске программы из аргумента");
        exit;
    } else {

        int status;
        waitpid(pid, &status, 0);  // Ожидаем завершения дочернего процесса и смотрим результат
        if (WIFEXITED(status)) {
            printf("Процесс завершился с кодом %d\n", WEXITSTATUS(status));
        } else {
            printf("Процесс завершился плохо\n");
        }
    }

    return 0;
}
