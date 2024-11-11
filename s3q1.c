#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

void print_file_info(const char *file_path) {
    struct stat file_stat;

    // Get the file status using stat()
    if (stat(file_path, &file_stat) == -1) {
        perror("Error getting file info");
        return;
    }

    // Print inode number
    printf("Inode: %ld\n", (long)file_stat.st_ino);

    // Print file type
    printf("File Type: ");
    switch (file_stat.st_mode & S_IFMT) {
        case S_IFREG:  printf("Regular File\n"); break;
        case S_IFDIR:  printf("Directory\n"); break;
        case S_IFLNK:  printf("Symbolic Link\n"); break;
        case S_IFCHR:  printf("Character Device\n"); break;
        case S_IFBLK:  printf("Block Device\n"); break;
        case S_IFIFO:  printf("FIFO (Named Pipe)\n"); break;
        case S_IFSOCK: printf("Socket\n"); break;
        default:       printf("Unknown Type\n"); break;
    }
}

int main(int argc, char *argv[]) {
    // Check if a file name is provided as an argument
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    // Call the function to print file information
    print_file_info(argv[1]);

    return 0;
}
