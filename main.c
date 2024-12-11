#include <stdio.h>
#include <stdlib.h>
#include "lab2.h"

void dbl_free(char** ptr, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(ptr[i]);
    }
    free(ptr);
}

int main(int argc, char** argv)
{
    // file input
    int check = checkArgv(argc, argv);
    switch(check)
    {
        case(1):
            printf("Invalid number of arguments\n");
            return 1;
        case(2):
            printf("Unable to find selected file\n");
            return 1;
    }
    FILE* file;
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Unexpected error while opening file\n");
        return 1;  
    }
    // reading text from file
    int n = 1;
    char** ptrs = (char**)malloc(sizeof(char*));
    if (ptrs == NULL)
    {
        printf("Allocation error\n");
        return 1;
    }
    char* str;
    size_t l;
    getline(&str, &l, file);
    ptrs[0] = (char*)malloc(sizeof(char) * (int)l);
    if (ptrs[0] == NULL)
    {
        free(ptrs);
        printf("Allocation error\n");
        return 1;
    }
    for (int i = 0; i < (int)l; i++)
    {
        ptrs[0][i] = str[i];
    }
    char** new_ptrs;
    while (!feof(file) && !ferror(file))
    {
        n++;
        new_ptrs = (char**)realloc(ptrs, sizeof(char*) * n);
        if (new_ptrs == NULL)
        {
            dbl_free(ptrs, n-1);
            printf("Reallocation error\n");
            return 1;
        }
        ptrs = new_ptrs;
        getline(&str, &l, file);
        ptrs[n-1] = (char*)malloc(sizeof(char) * (int)l);
        if (ptrs[n-1] == NULL)
        {
            dbl_free(ptrs, n-1);
            printf("Allocation error\n");
            return 1;
        }
        for (int i = 0; i < (int)l; i++)
        {
            ptrs[n-1][i] = str[i];
        }
    }
    fclose(file);
    // main part
    int res = changeText(ptrs, n-1, argv[2]);
    if (res)
    {
        printf("Error %d\n", res+10);
        dbl_free(ptrs, n);
        free(str);
        return res+10;
    }
    else
    {
        file = fopen(argv[1], "w");
        if (file == NULL)
        {
            printf("Unexpected error while opening file\n");
            dbl_free(ptrs, n);
            free(str);
            return 1;
        }
        for (int i = 0; i < n-1; i++)
        {
            if (ptrs[i] != NULL)
            {
                fprintf(file, "%s", ptrs[i]);
            }
        }
    }
    fclose(file);
    // ending
    printf("Success\n");
    dbl_free(ptrs, n);
    free(str);
    return 0;
}
