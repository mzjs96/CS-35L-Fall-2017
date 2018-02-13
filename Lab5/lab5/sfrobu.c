#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>


int foption = 0;

int ptrtofrob(const void *a, const void *b) 
{
    return frobcmp(*(char**)a, *(char**)b);

}

int frobcmp(const char *a, const char *b)
{
    while(1)
    {
        if (foption == 1)
        {
            char as = *a ^ 42;
            char bs = *b ^ 42;
            as = toupper((unsigned char)as);
            bs = toupper((unsigned char)bs);
            if (*a == *b) {
                if (*a == ' ')
                {
                    return 0;
                }

            }
            else if (as < bs || *a == ' ')
            {
                return -1;
            }
            else if (as > bs || *b == ' ')
            {
                return 1;
            }
        }
        if (foption == 0) {
            if (*a == *b) {
                if (*a == ' ')
                {
                    return 0;
                }

            }
            else if (((*a ^ 42) < (*b ^ 42)) || *a == ' ')
            {
                return -1;
            }
            else if (((*a ^ 42) >(*b ^ 42)) || *b == ' ')
            {
                return 1;
            }
        }
        a++;
        b++;
    }
}

int main(int argc, char *argv[])
{

    int c;
    while ((c = getopt(argc, argv, "f")) != -1)
    {
        switch (c)
        {
        case 'f':
            foption = 1;
            break;
        default:
            fprintf(stderr, "error in argument");
            exit(1);

        }

    }

    if (optind < argc)
    {
        fprintf(stderr, "option error");
        exit(1);
    }

    int(*func_ptr) (const void *, const void *) = &ptrtofrob;
    int strlen = 0;
    int totlen = 10;

    int idxstorage = 0;
    int lineNumber = 0;
    int end = feof(stdin);
    struct stat buffer1;          
    if (fstat(0, &buffer1) < 0)      
    {
        fprintf(stderr, "something is wrong with fstat");
        exit(1);
    };

    totlen = buffer1.st_size + 1;       
    char * storage = (char*)malloc(totlen * sizeof(char));   
    if (storage == NULL)
    {
        fprintf(stderr, "memory allocation error with initailizing storage");
        exit(1);
    }
    char temp[1];
    ssize_t state = read(0, temp, 1);
    while (state >= 0)     
    {
        if (state == 0)
        {
            if (lineNumber == 0 && strlen == 0)
            {
                free(storage);
                return 0;
            }
            if (storage[idxstorage - 1] != ' ')
            {
                storage[idxstorage] = ' ';
                idxstorage++;
            }
            lineNumber++;
            break;
        }

        char now = temp[0];

        if (idxstorage == totlen)
        {
            char * buffer2 = (char*)realloc(storage, sizeof(char)*totlen * 2);
            if (buffer2 == NULL)
            {
                fprintf(stderr, "memory allocation error with growing array");
                exit(1);
            }
            storage = buffer2;
            totlen *= 2;
        }

        storage[idxstorage] = now;
        idxstorage++;
        strlen++;

        if (now == ' ')
        {
            lineNumber++;
            strlen = 0;

        }
        state = read(0, temp, 1);
    }

    char** strings = (char**)malloc(sizeof(char*)*lineNumber);
    if (strings == NULL)
    {
        fprintf(stderr, "memory allocation error");
        exit(1);
    }
    char* start = storage;
    int indexStrings = 0;
    int i = 0;
    for (i = 0; i < idxstorage; i++)
    {
        if (storage[i] == ' ')
        {
            strings[indexStrings] = start;
            indexStrings++;
            start = storage + i + 1;
        }
    }

    int j = 0;
    qsort(strings, lineNumber, sizeof(char*), func_ptr);
    ssize_t status = 0;
    for (j = 0; j < lineNumber; j++)
    {
        char* t = strings[j];
        for (t = strings[j];*t != ' ';t++)
        {

            status = write(1, t, 1);
            if (status<0)
            {
                fprintf(stderr, "system writing error");
            }

        }
        char space[1];space[0] = ' ';
        status = write(1, space, 1);
    }
    free(storage);
    free(strings);
    return 0;
}


