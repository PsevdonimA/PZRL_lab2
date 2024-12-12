#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include <string.h>
#include "lab2.h"

// --- ADDITIONS ---

const int char_s = (int)sizeof(char);

enum E_adding
{
    E_FRONT = 0,
    E_BACK = 1
};

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

int addsize(char** str)
{
    int n = (int)strlen(*str);
    char* new_str = (char*)realloc(*str, char_s * (n+1));
    if (new_str == NULL)
    {
        return 1; // resize error
    }
    *str = new_str;
    return 0;
}

// --- REPLACE ---

int textReplace(char** ptrs, int n, char* str)
{
    int l = (int)strlen(str);
    // Get text
    int l1 = 0;
    char* old_text = (char*)malloc(char_s * l);
    int l2 = 0;
    char* new_text = (char*)malloc(char_s * l);
    if (old_text == NULL || new_text == NULL)
    {
        return 1; // allocation error
    }
    int status = 0;
    for (int i = 2; i < l-1; i++)
    {
        if (str[i] == '/')
        {
            status = 1;
        }
        else if (status == 0)
        {
            old_text[l1] = str[i];
            l1++;
        }
        else
        {
            new_text[l2] = str[i];
            l2++;
        }
    }
    // Replace text
    int curl = 1;
    int curpos = 0;
    int test = 0;
    char* buff;
    for (int i = 0; i < n; i++)
    {
        curpos = 0;
        curl = 1;
        buff = (char*)malloc(char_s * curl);
        if (buff == NULL)
        {
            free(old_text);
            free(new_text);
            return 1;
        }
        for (int j = 0; j < (int)strlen(ptrs[i]); j++)
        {
            if ((ptrs[i][j] == old_text[0]) && ((int)strlen(ptrs[i]) - j >= l1))
            {
                test = 1;
                for (int k = j+1; k < j+l1; k++)
                {
                    if (ptrs[i][k] != old_text[k-j])
                    {
                        test = 0;
                        break;
                    }
                }
                if (test)
                {
                    for (int k = 0; k < l2; k++)
                    {
                        if (curpos == curl)
                        {
                            if (addsize(&buff))
                            {
                                free(old_text);
                                free(new_text);
                                free(buff);
                                return 1;
                            }
                            curl++;
                        }
                        buff[curpos] = new_text[k];
                        curpos++;
                    }
                    j += l1-1;
                }
                else
                {
                    if (curpos == curl)
                    {
                        if (addsize(&buff))
                        {
                            free(old_text);
                            free(new_text);
                            free(buff);
                            return 1;
                        }
                        curl++;
                    }
                    buff[curpos] = ptrs[i][j];
                    curpos++;
                }
            }
            else
            {
                if (curpos == curl)
                {
                    if (addsize(&buff))
                    {
                        free(old_text);
                        free(new_text);
                        free(buff);
                        return 1;
                    }
                    curl++;
                }
                buff[curpos] = ptrs[i][j];
                curpos++;
            }
        }
        free(ptrs[i]);
        ptrs[i] = buff;
    }
    free(old_text);
    free(new_text);
    return 0;
}

// --- DELETE ---

int textDelete(char** ptrs, int n, char* str)
{
    int l = (int)strlen(str);
    char* reg = (char*)malloc(char_s * (l-2));
    if (reg == NULL)
    {
        return 1; // allocation error
    }
    for (int i = 1; i < l-2; i++)
    {
        reg[i-1] = str[i];
    }
    reg[l-3] = '\0';
    regex_t delreg;
    int regnok = regcomp(&delreg, reg, 0);
    if (regnok)
    {
        free(reg);
        regfree(&delreg);
        return 2; // Reg compile error
    }
    char* buff;
    for (int i = 0; i < n; i++)
    {
        buff = (char*)malloc(char_s * (int)strlen(ptrs[i]));
        if (buff == NULL)
        {
            free(reg);
            regfree(&delreg);
            return 1;
        }
        for (int j = 0; j < (int)strlen(ptrs[i]); j++)
        {
            if (ptrs[i][j] == '\n' || ptrs[i][j] == '\0')
            {
                buff[j] = '\0';
                break;
            }
            else
            {
                buff[j] = ptrs[i][j];
            }
        }
        regnok = regexec(&delreg, buff, 0, NULL, 0);
        if (!regnok)
        {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
        else if (regnok == REG_NOMATCH){}
        else
        {
            free(buff);
            free(reg);
            regfree(&delreg);
            return 3; // unexpected error
        }
        free(buff);
    }
    free(reg);
    regfree(&delreg);
    return 0;
}

// --- ADD FRONT/BACK ---

int textAdd(char** ptrs, int n, char* str, int connection)
{
    int l = (int)strlen(str) - 5;
    // Get text
    char* text = (char*)malloc(char_s * (l+1));
    if (text == NULL)
    {
        return 1; // allocation error
    }
    for (int i = 4; i < l+4; i++)
    {
        text[i-4] = str[i];
    }
    text[l] = '\0';
    // Add text
    char* buff;
    int lstr = 0;
    for (int i = 0; i < n; i++)
    {
        lstr = (int)strlen(ptrs[i]) - 1;
        buff = (char*)malloc(char_s * (lstr + l + 1));
        if (buff == NULL)
        {
            free(text);
            return 1;
        }
        if (connection == E_FRONT)
        {
            for (int j = 0; j < l; j++)
            {
                buff[j] = text[j];
            }
            for (int j = l; j < l+lstr; j++)
            {
                buff[j] = ptrs[i][j-l];
            }
        }
        else
        {
            for (int j = 0; j < lstr; j++)
            {
                buff[j] = ptrs[i][j];
            }
            for (int j = lstr; j < l+lstr; j++)
            {
                buff[j] = text[j-lstr];
            }
        }
        buff[lstr+l] = '\n';
        free(ptrs[i]);
        ptrs[i] = buff;
    }
    free(text);
    return 0;
}

// --- DESISION PART ---

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
    // Insert front
    int regnok = regcomp(&regex, "^s\\/\\^\\/[^\\/][^\\/]*\\/$", 0);
    if (regnok)
    {
        regfree(&regex);
        return 2; // Reg compile error
    }
    regnok = regexec(&regex, str, 0, NULL, 0);
    if (!regnok)
    {
        if (textAdd(ptrs, n, str, E_FRONT))
        {
            return 6; // Error in textAdd (FRONT)
        }
        regfree(&regex);
        return 0;
    }
    regfree(&regex);
    // Insert back
    regnok = regcomp(&regex, "^s\\/\\$\\/[^\\/][^\\/]*\\/$", 0);
    if (regnok)
    {
        regfree(&regex);
        return 2; // Reg compile error
    }
    regnok = regexec(&regex, str, 0, NULL, 0);
    if (!regnok)
    {
        if (textAdd(ptrs, n, str, E_BACK))
        {
            return 7; // Error in textAdd (BACK)
        }
        regfree(&regex);
        return 0;
    }
    regfree(&regex);
    // Replace
    regnok = regcomp(&regex, "^s\\/[^\\/][^\\/]*\\/[^\\/]*\\/$", 0);
    if (regnok)
    {
        regfree(&regex);
        return 2; // Reg compile error
    }
    regnok = regexec(&regex, str, 0, NULL, 0);
    if (!regnok)
    {
        if (textReplace(ptrs, n, str))
        {
            return 4; // Error in textReplace
        }
        regfree(&regex);
        return 0;
    }
    regfree(&regex);
    // Delete
    regnok = regcomp(&regex, "^\\/[^\\/][^\\/]*\\/d$", 0);
    if (regnok)
    {
        regfree(&regex);
        return 2; // Reg compile error
    }
    regnok = regexec(&regex, str, 0, NULL, 0);
    if (!regnok)
    {
        if (textDelete(ptrs, n, str))
        {
            return 5; // Error in textDelete
        }
        regfree(&regex);
        return 0;
    }
    regfree(&regex);
    return 3; // No match
}
