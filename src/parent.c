#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    char filename[256];
    printf("Введите имя файла: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Ошибка при создании канала");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка при создании дочернего процесса");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("Ошибка dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);

        execl("./child", "./child", filename, NULL);

        perror("Ошибка execl");
        exit(EXIT_FAILURE);
    } else {
        close(pipefd[0]);
        printf("Введите числа (через пробел). Для завершения нажмите Ctrl+D:\n");
        char buffer[1024];

        while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (write(pipefd[1], buffer, strlen(buffer)) == -1) {
                perror("Ошибка записи");
                break;
            }
        }

        close(pipefd[1]);

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("Ошибка waitpid");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("Дочерний процесс завершился с ошибкой. Завершаю родительский процесс.\n");
            exit(EXIT_FAILURE);
        }

        printf("Родительский процесс успешно завершен.\n");
    }
    return 0;
}