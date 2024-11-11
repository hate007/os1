#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void print_file_info(const char *filename) {
    struct stat file_stat;

    // Get file information using stat()
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return;
    }

    // Print the inode number
    printf("File: %s\n", filename);
    printf("Inode Number: %ld\n", (long)file_stat.st_ino);

    // Determine the file type
    if (S_ISREG(file_stat.st_mode)) {
        printf("File Type: Regular file\n");
    } else if (S_ISDIR(file_stat.st_mode)) {
        printf("File Type: Directory\n");
    } else if (S_ISLNK(file_stat.st_mode)) {
        printf("File Type: Symbolic link\n");
    } else if (S_ISCHR(file_stat.st_mode)) {
        printf("File Type: Character device\n");
    } else if (S_ISBLK(file_stat.st_mode)) {
        printf("File Type: Block device\n");
    } else if (S_ISFIFO(file_stat.st_mode)) {
        printf("File Type: FIFO (named pipe)\n");
    } else if (S_ISSOCK(file_stat.st_mode)) {
        printf("File Type: Socket\n");
    } else {
        printf("File Type: Unknown\n");
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    // Check if filenames are provided as command line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }

    // Loop through all the command line arguments (files) and print their information
    for (int i = 1; i < argc; i++) {
        print_file_info(argv[i]);
    }

    return 0;
}
