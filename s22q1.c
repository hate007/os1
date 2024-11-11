#include <stdio.h>

int main() {
    // Redirect stdout to a file named "output.txt"
    FILE *file = freopen("output.txt", "w", stdout);
    
    if (file == NULL) {
        perror("Failed to redirect stdout to output.txt");
        return 1;
    }

    // Now printf will write to output.txt instead of the terminal
    printf("This will be written to the file 'output.txt'\n");
    printf("This is another line in the output file.\n");

    // Close the file
    fclose(file);

    return 0;
}
