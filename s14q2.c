#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

// Function to print file permissions
void print_permissions(mode_t mode) {
    char permissions[10] = "---------";  // Default to no permissions

    // File type
    if (S_ISDIR(mode))      permissions[0] = 'd';
    else if (S_ISLNK(mode)) permissions[0] = 'l';
    else if (S_ISREG(mode)) permissions[0] = '-';

    // User permissions
    if (mode & S_IRUSR) permissions[1] = 'r';
    if (mode & S_IWUSR) permissions[2] = 'w';
    if (mode & S_IXUSR) permissions[3] = 'x';

    // Group permissions
    if (mode & S_IRGRP) permissions[4] = 'r';
    if (mode & S_IWGRP) permissions[5] = 'w';
    if (mode & S_IXGRP) permissions[6] = 'x';

    // Other permissions
    if (mode & S_IROTH) permissions[7] = 'r';
    if (mode & S_IWOTH) permissions[8] = 'w';
    if (mode & S_IXOTH) permissions[9] = 'x';

    printf("Permissions: %s\n", permissions);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // Check if the user provided the file name
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    struct stat fileStat;

    // Get file status
    if (stat(filename, &fileStat) == -1) {
        perror("stat failed");
        return 1;
    }

    // Display the file properties
    printf("File: %s\n", filename);
    printf("Inode number: %ld\n", (long)fileStat.st_ino);
    printf("Number of hard links: %ld\n", (long)fileStat.st_nlink);
    print_permissions(fileStat.st_mode);
    printf("File size: %ld bytes\n", (long)fileStat.st_size);

    // Convert time to a readable format
    char timebuf[100];

    // Access time (last time file was read)
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_atime));
    printf("Last access time: %s\n", timebuf);

    // Modification time (last time file content was modified)
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_mtime));
    printf("Last modification time: %s\n", timebuf);

    // Status change time (last time file metadata changed)
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_ctime));
    printf("Last status change time: %s\n", timebuf);

    // User ID of the file owner
    printf("User ID of owner: %d\n", fileStat.st_uid);
    // Group ID of the file owner
    printf("Group ID of owner: %d\n", fileStat.st_gid);

    return 0;
}
