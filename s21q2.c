#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

// Define a structure to hold file name and file size
struct file_info {
    char *filename;
    off_t size;
};

// Comparator function to compare files based on their sizes
int compare_files(const void *a, const void *b) {
    struct file_info *file1 = (struct file_info *)a;
    struct file_info *file2 = (struct file_info *)b;
    
    // Compare based on the size of the files
    if (file1->size < file2->size)
        return -1;
    else if (file1->size > file2->size)
        return 1;
    else
        return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }

    // Allocate memory for storing file information
    struct file_info *files = malloc((argc - 1) * sizeof(struct file_info));
    if (files == NULL) {
        perror("malloc");
        return 1;
    }

    // Retrieve the file sizes using stat() and store them
    for (int i = 1; i < argc; i++) {
        struct stat file_stat;
        if (stat(argv[i], &file_stat) == -1) {
            perror(argv[i]);
            free(files);  // Free memory before exit
            return 1;
        }

        // Allocate memory for filename and store the file info
        files[i - 1].filename = argv[i];
        files[i - 1].size = file_stat.st_size;
    }

    // Sort the files based on their size using qsort
    qsort(files, argc - 1, sizeof(struct file_info), compare_files);

    // Display the sorted filenames and their sizes
    printf("Files sorted by size (ascending order):\n");
    for (int i = 0; i < argc - 1; i++) {
        printf("%s - %ld bytes\n", files[i].filename, files[i].size);
    }

    // Free the allocated memory
    free(files);
    
    return 0;
}
