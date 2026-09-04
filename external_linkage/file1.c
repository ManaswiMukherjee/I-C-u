#include <stdio.h>

extern int a;//calling variable from another file
void display();//function of another file

int main(void)
{
    a=3;//changing value of external variable if needed
    display();//calling external function

    return 0;
}