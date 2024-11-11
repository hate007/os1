#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void check_file_type(const char *filename) {
    struct stat file_stat;

    // Use stat() to get the file status
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return;
    }

    // Check the file type using the st_mode field
    if (S_ISDIR(file_stat.st_mode)) {
        printf("The file '%s' is a Directory.\n", filename);
    } else if (S_ISCHR(file_stat.st_mode)) {
        printf("The file '%s' is a Character Device.\n", filename);
    } else if (S_ISBLK(file_stat.st_mode)) {
        printf("The file '%s' is a Block Device.\n", filename);
    } else if (S_ISREG(file_stat.st_mode)) {
        printf("The file '%s' is a Regular File.\n", filename);
    } else if (S_ISFIFO(file_stat.st_mode)) {
        printf("The file '%s' is a FIFO (Named Pipe).\n", filename);
    } else if (S_ISLNK(file_stat.st_mode)) {
        printf("The file '%s' is a Symbolic Link.\n", filename);
    } else if (S_ISSOCK(file_stat.st_mode)) {
        printf("The file '%s' is a Socket.\n", filename);
    } else {
        printf("The file '%s' is of an unknown type.\n", filename);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    // Check the file type
    check_file_type(filename);

    return 0;
}
