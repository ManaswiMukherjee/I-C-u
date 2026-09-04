#ifndef MY_LIBRARY_H /* This is a macro guard, across a same project no 2 header files can have
the same macro guard name. The convention is to name the guard according to the header file
name, in this case MY_LIBRARY_H for my_library.h*/

#define MY_LIBRARY_H

extern int a;
void display(void);

#endif