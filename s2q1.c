#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

void print_file_properties(const char *file_path) {
    struct stat file_stat;

    stat(file_path, &file_stat);

    printf("File: %s\n", file_path);

    printf("Inode: %d\n", (int)file_stat.st_ino);

    printf("Hard links: %d\n", (int)file_stat.st_nlink);

    printf("Size: %d bytes\n", (int)file_stat.st_size);

    printf("Permissions: ");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    printf("Last accessed: %s", ctime(&file_stat.st_atime));
    printf("Last modified: %s", ctime(&file_stat.st_mtime));
    printf("Last status change: %s", ctime(&file_stat.st_ctime));
}

int main(int argc, char *argv[]) {

    print_file_properties(argv[1]);
}
