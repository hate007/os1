#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

// Structure to hold file name and its size
struct FileInfo {
    char *filename;
    off_t size;
};

// Comparison function for qsort (sorting files by size)
int compareFiles(const void *a, const void *b) {
    struct FileInfo *fileA = (struct FileInfo *)a;
    struct FileInfo *fileB = (struct FileInfo *)b;

    // Compare file sizes for ascending order
    if (fileA->size < fileB->size) return -1;
    else if (fileA->size > fileB->size) return 1;
    else return 0;
}

int main(int argc, char *argv[]) {
    // Check if at least one filename is provided
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    // Allocate memory for file information
    struct FileInfo *files = malloc((argc - 1) * sizeof(struct FileInfo));
    if (files == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // Get the size of each file
    for (int i = 1; i < argc; i++) {
        struct stat fileStat;

        // Get file information using stat()
        if (stat(argv[i], &fileStat) == -1) {
            perror("stat failed");
            free(files);
            return 1;
        }

        // Store the filename and its size
        files[i - 1].filename = argv[i];
        files[i - 1].size = fileStat.st_size;
    }

    // Sort the files array by size in ascending order
    qsort(files, argc - 1, sizeof(struct FileInfo), compareFiles);

    // Print the sorted filenames with their sizes
    printf("Files sorted by size (ascending order):\n");
    for (int i = 0; i < argc - 1; i++) {
        printf("%s (Size: %ld bytes)\n", files[i].filename, files[i].size);
    }

    // Free the allocated memory
    free(files);

    return 0;
}
