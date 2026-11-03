#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

// ===== FILE OPENING AND CLOSING =====

// Open a file for reading in read mode ("r")
// Returns FILE pointer on success, NULL on failure
FILE* openInputFile(const char* filename);

// Close the input file
// Safely closes if file pointer is not NULL
void closeInputFile(FILE* file);


// ===== CHARACTER READING FUNCTIONS =====

// Read a character from the file
// Returns the character read, or EOF if end of file is reached
int readChar(FILE* file);

// Peek at the next character without consuming it
// Returns the next character or EOF without advancing file position
int peekChar(FILE* file);

// Check whether we are at the end of the file
// Returns 1 (true) if at EOF, 0 (false) otherwise
int isEndOfFile(FILE* file);


// ===== FILE NAVIGATION FUNCTIONS =====

// Get the current position in the file
// Returns the current file position (byte offset from start)
long getCurrentPosition(FILE* file);

// Move to a specific position in the file
// Returns 0 on success, non-zero on failure
int seekFile(FILE* file, long position);

// Move to the beginning of the file
// Returns 0 on success, non-zero on failure
int seekToBeginning(FILE* file);

#endif
