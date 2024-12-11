#include <unistd.h>
#include <regex.h>
#include <string.h>
#include "lab2.h"

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

int changeText(char** ptrs, int n, char* str)
{
    if (str == NULL || ptrs == NULL)
    {
        return 1; // NULL pointer
    }
    for (int i = 0; i < n; i++)
    {
        if (ptrs[i] == NULL)
        {
            return 1; // NULL pointer
        }
    }
    regex_t regex;
    // Replace
    int regnok = regcomp(&regex, "^s\/[^\/][^\/]*\/[^\/]*\/$", 0);
    if (regnok)
    {
        regfree(&regex);
        return 2; // Reg compile error
    }
    regnok = regexec(&regex, str, 0, NULL, 0);
    if (!regnok)
    {
        regfree(&regex);
        return 0;
    }
    // Delete
    regnok = regcomp(&regex, "^\/[^\/][^\/]*\/d$", 0);
    if (regnok)
    {
        regfree(&regex);
        return 2; // Reg compile error
    }
    regnok = regexec(&regex, str, 0, NULL, 0);
    if (!regnok)
    {
        regfree(&regex);
        return 0;
    }
    // Insert front
    regnok = regcomp(&regex, "^s\/\^\/[^\/][^\/]*\/$", 0);
    if (regnok)
    {
        regfree(&regex);
        return 2; // Reg compile error
    }
    regnok = regexec(&regex, str, 0, NULL, 0);
    if (!regnok)
    {
        regfree(&regex);
        return 0;
    }
    // Insert back
    regnok = regcomp(&regex, "^s\/\$\/[^\/][^\/]*\/$", 0);
    if (regnok)
    {
        regfree(&regex);
        return 2; // Reg compile error
    }
    regnok = regexec(&regex, str, 0, NULL, 0);
    if (!regnok)
    {
        regfree(&regex);
        return 0;
    }
    regfree(&regex);
    return 3; // No match
}
