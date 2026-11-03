#include "input.h"
#include <stdlib.h>

// ===== FILE OPENING AND CLOSING =====

// Open a file for reading in read mode ("r")
FILE* openInputFile(const char* filename) {
    if (filename == NULL) {
        return NULL;
    }
    FILE* file = fopen(filename, "r");
    return file;
}

// Close the input file
void closeInputFile(FILE* file) {
    if (file != NULL) {
        fclose(file);
    }
}


// ===== CHARACTER READING FUNCTIONS =====

// Read a character from the file
int readChar(FILE* file) {
    if (file == NULL) {
        return EOF;
    }
    return fgetc(file);
}

// Peek at the next character without consuming it
int peekChar(FILE* file) {
    if (file == NULL) {
        return EOF;
    }
    int c = fgetc(file);
    if (c != EOF) {
        ungetc(c, file);
    }
    return c;
}

// Check whether we are at the end of the file
int isEndOfFile(FILE* file) {
    if (file == NULL) {
        return 1;
    }
    return feof(file);
}


// ===== FILE NAVIGATION FUNCTIONS =====

// Get the current position in the file
long getCurrentPosition(FILE* file) {
    if (file == NULL) {
        return -1L;
    }
    return ftell(file);
}

// Move to a specific position in the file
int seekFile(FILE* file, long position) {
    if (file == NULL) {
        return -1;
    }
    return fseek(file, position, SEEK_SET);
}

// Move to the beginning of the file
int seekToBeginning(FILE* file) {
    if (file == NULL) {
        return -1;
    }
    return fseek(file, 0, SEEK_SET);
}

