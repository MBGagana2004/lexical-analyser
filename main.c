#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "lexer.h"

#define MAX_FILENAME_LENGTH 256

int main() {
    // Get the input C source-file name
    printf("Enter the C source file name: ");
    char filename[MAX_FILENAME_LENGTH];
    fgets(filename, MAX_FILENAME_LENGTH, stdin);
    
    // Remove newline from filename if present
    int len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    // Call the file-opening function from input.c
    FILE* inputFile = openInputFile(filename);
    
    // If the file cannot be opened, display an error and stop
    if (inputFile == NULL) {
        printf("Error: Cannot open file '%s'\n", filename);
        return 1;
    }
    
    // Call the lexical analysis function from lexer.c
    int tokenCount = 0;
    Token* tokens = lexicalAnalysis(inputFile, &tokenCount);
    
    // Display the tokens and their types
    displayTokens(tokens, tokenCount);
    
    // Close the input file
    closeInputFile(inputFile);
    
    // Free allocated memory
    freeTokens(tokens, tokenCount);
    
    return 0;
}
