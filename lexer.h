#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

// ===== TOKEN TYPES =====
typedef enum {
    TOKEN_KEYWORD,              // if, while, int, etc.
    TOKEN_IDENTIFIER,           // variable names, function names
    TOKEN_CONSTANT,             // integers, floats
    TOKEN_CHARACTER_CONSTANT,   // 'a', '\n', etc.
    TOKEN_STRING_LITERAL,       // "hello", "world", etc.
    TOKEN_OPERATOR,             // +, -, *, /, %, ==, !=, etc.
    TOKEN_SPECIAL_SYMBOL,       // ;, {, }, (, ), [, ], , etc.
    TOKEN_EOF                   // End of file
} TokenType;

// ===== TOKEN STRUCTURE =====
typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

// ===== KEYWORD LIST =====
// Array of C keywords for comparison
extern const char* keywordList[];
extern const int keywordCount;

// ===== LEXICAL ANALYSIS FUNCTIONS =====

// Check if a word is a C keyword
// Returns 1 if keyword, 0 otherwise
int isKeyword(const char* word);

// Perform lexical analysis on the input file
// Reads characters one by one and generates tokens
// Returns an array of tokens
Token* lexicalAnalysis(FILE* file, int* tokenCount);

// Display all tokens and their types in formatted output
void displayTokens(Token* tokens, int tokenCount);

// Free allocated token memory
void freeTokens(Token* tokens, int tokenCount);

#endif
