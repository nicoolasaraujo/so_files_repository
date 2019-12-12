#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<ctype.h>

#define FILES_DIRECTORY "./storageFiles"

int existFileDirectory(char *fileName, char **outFileNameExt){
    int result = 0;
    struct stat st = {0};
    if (stat(FILES_DIRECTORY, &st) == -1) {
        mkdir(FILES_DIRECTORY, 0700);
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(FILES_DIRECTORY);

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(strstr(dir->d_name, fileName) != NULL){
                *outFileNameExt = dir->d_name;
                result = 1;
                break;
            }

            printf("%s\n", dir->d_name);
        }   
        closedir(d);
        return result;
    }
}

int main(){
FILE* f;
char *completeFileName;
char *completeFilePath;
char *separator = "/";
if(existFileDirectory("TST", &completeFileName)){
    printf("arquivo existe: %s\n", completeFileName);
    completeFilePath = malloc(strlen(FILES_DIRECTORY) + strlen(completeFileName)+1 );
    strcpy(completeFilePath, FILES_DIRECTORY);
    // completeFilePath = FILES_DIRECTORY;
    strcat(completeFilePath,separator);
    strcat(completeFilePath, completeFileName);
    // strcat(completeFilePath, separator);
    printf("%s\n", completeFilePath);

    FILE *f = fopen(completeFilePath, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    char *buffer = malloc(fsize + 1);
    fread(buffer, 1, fsize, f);
    fclose(f);

    buffer[fsize] = 0;

    char *newFilePath;
    newFilePath = malloc(strlen(completeFilePath)+ 10);
    strcpy(newFilePath, completeFilePath);
    strcat(newFilePath, "copy");
    printf("%s", newFilePath);
    f = fopen(newFilePath, "wb");
    fwrite(buffer, fsize, 1, f);
    fclose(f);
}
else {
    printf("arquivo nem existe");
}

// FILE* pFile;
// char* yourFilePath  = "test.txt";
// char* yourBuffer    = "HelloWorld!";
// int   yorBufferSize = strlen(yourBuffer) + 1;

// /* Reserve memory for your readed buffer */
// char* readedBuffer = malloc(yorBufferSize);

// /* Write your buffer to disk. */
// pFile = fopen(yourFilePath,"wb");

// if (pFile){
//     fwrite(yourBuffer, yorBufferSize, 1, pFile);
//     puts("Wrote to file!");
// }
// else{
//     puts("Something wrong writing to File.");
// }

// fclose(pFile);

// /* Now, we read the file and get its information. */
// pFile = fopen(yourFilePath,"rb");

// if (pFile){
//     fread(readedBuffer, yorBufferSize, 1, pFile);
//     puts("Readed from file!");
// }
// else{
//     puts("Something wrong reading from File.\n");
// }

// /* Compare buffers. */
// if (!memcmp(readedBuffer, yourBuffer, yorBufferSize)){
//     puts("readedBuffer = yourBuffer");
// }
// else{
//     puts("Buffers are different!");
// }

// /* Free the reserved memory. */
// free(readedBuffer);

// fclose(pFile);
// return 0;
}