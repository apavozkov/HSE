#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

void detach() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Не удалось создать новый процесс");
        exit;
    }

    if (pid == 0) {
        if (setsid() < 0) {
            printf("Не удалось создать новый сеанс");
            exit;
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        int fd = open("/dev/null", O_RDWR);
        if (fd < 0) {
            print("Не удалось открыть /dev/null");
            exit;
        }

        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        char *args[] = {"path_program", NULL}; 
        execvp(args[0], args);
        
        perror("execvp failed");
        exit;
    }
}

int main(int argc, char *argv[]) {
    // Проверка количества аргументов
    if (argc < 2) {
        printf("Ошибка во входных данных");
        exit;
    }

    detach();

    // Завершаем родительский процесс
    return EXIT_SUCCESS;
}
