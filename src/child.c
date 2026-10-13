#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Ошибка: не передано имя файла\n");
        exit(EXIT_FAILURE);
    }

    int file_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1) {
        perror("Ошибка при открытии/создании файла");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        int first_number, current_number, offset;
        char *ptr = line;

        if (sscanf(ptr, "%d%n", &first_number, &offset) == 1) {
            double result = (double)first_number;
            ptr += offset;

            while (sscanf(ptr, "%d%n", &current_number, &offset) == 1) {
                if (current_number == 0) {
                    fprintf(stderr, "Ошибка: деление на ноль\n");
                    close(file_fd);
                    exit(EXIT_FAILURE);
                }
                result = result / current_number;
                ptr += offset;
            }

            char buffer[256];
            int len = snprintf(buffer, sizeof(buffer), "Ввод: %sРезультат: %f\n\n", line, result);

            if (write(file_fd, buffer, len) == -1) {
                perror("Ошибка записи в файл");
                close(file_fd);
                exit(EXIT_FAILURE);
            }
        }
    }
    
    close(file_fd);
    return 0;
}