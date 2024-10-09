#include "game.h"

// Function to display an error message and then exit
void fatal(const char *message) {
    char error_message[100];

    // Using snprintf to safely concatenate strings
    snprintf(error_message, sizeof(error_message), "[!!] Fatal Error: %s", message);

    // Print the error message with perror to display the system error
    perror(error_message);

    // Exit with EXIT_FAILURE to indicate an error
    exit(EXIT_FAILURE);
}

// An error-checked malloc() wrapper function
void *ec_malloc(unsigned int size) {
    void *ptr;

    // Check if the size is valid (greater than 0)
    if (size == 0) {
        fatal("in ec_malloc() on zero size memory allocation");
    }

    // Try to allocate memory
    ptr = malloc(size);

    // If malloc fails, call fatal() to display an error and exit
    if (ptr == NULL) {
        fatal("in ec_malloc() on memory allocation");
    }

    return ptr;
}

