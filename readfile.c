#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char buffer[100];
    char *bench = argv[1];

    file = fopen(bench, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
    return 0;
}
