#include <stdio.h>
#include <sys/stat.h>

void print_file_info(const char *file_path) {
    struct stat file_stat;

    if (stat(file_path, &file_stat) == -1) {
        perror("Error");
        return;
    }

    printf("File: %s\n", file_path);
    printf("Inode: %d\n", (int)file_stat.st_ino);

    printf("Type: ");
    switch (file_stat.st_mode & S_IFMT) {
        case S_IFREG:  printf("Regular file\n"); break;
        case S_IFDIR:  printf("Directory\n"); break;
        case S_IFCHR:  printf("Character device\n"); break;
        case S_IFBLK:  printf("Block device\n"); break;
        default:       printf("Unknown type\n"); break;
    }
    printf("-------------------------\n");
}

void main(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        print_file_info(argv[i]);
    }

}

//i saw this in a solution is this right 

/* Takes multiple files as Command Line Arguments and print their inode number */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
int main(int argc, char *argv[])
{
	char d[50];
	if(argc==2)
	{
	bzero(d,sizeof(d));
	strcat(d,"ls ");
	strcat(d,"-i ");
	strcat(d,argv[1]);
	system(d);
	}
	else
	printf("\nInvalid No. of inputs");
	
}
