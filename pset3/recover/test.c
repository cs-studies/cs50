#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    char *name = malloc(sizeof(char));
    for (int i = 0; i < 1000; i++)
    {
        sprintf(name, "%03d.tmp", i);
        printf("%s\n", name);
    }
    free(name);
    return 0;
}

