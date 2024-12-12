#include <stdlib.h>
#include <unistd.h>
#include "lab2f.h"

int checkArgv(int argc, char** argv) // 0 - correct; other numbers - errors
{
    if (argc != 3)
    {
        return 1; // wrong aruments number
    }
    if (access(argv[1], F_OK) != 0)
    {
        return 2; // cannot find selected file
    }
    return 0;
}
