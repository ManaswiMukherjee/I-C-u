#include <stdio.h>

#include "my_library.h"

int a = 10;
void display()
{
    printf("This function from my_library.c displays the variable present in the file %d\n", a);
    printf("Bye\n");
}