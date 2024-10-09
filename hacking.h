#ifndef HACKING_H
#define HACKING_H
#include <stdlib.h>
#include <stdio.h>



// Function to display an error message and then exit
void fatal(const char *message);
// An error-checked malloc() wrapper function
void *ec_malloc(unsigned int size);
#endif
