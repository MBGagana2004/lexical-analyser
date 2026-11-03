#include "lexer.h"
#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 10000
#define MAX_TOKEN_LENGTH 256

// ===== KEYWORD LIST =====
const char* keywordList[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary"
};

const int keywordCount = sizeof(keywordList) / sizeof(keywordList[0]);

// ===== HELPER FUNCTIONS =====

// Check if a word is a C keyword
int isKeyword(const char* word) {
    for (int i = 0; i < keywordCount; i++) {
        if (strcmp(word, keywordList[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Get token type name for display
const char* getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER:
            return "IDENTIFIER";
        case TOKEN_KEYWORD:
            return "KEYWORD";
        case TOKEN_CONSTANT:
            return "CONSTANT";
        case TOKEN_CHARACTER_CONSTANT:
            return "CHARACTER_CONSTANT";
        case TOKEN_STRING_LITERAL:
            return "STRING_LITERAL";
        case TOKEN_OPERATOR:
            return "OPERATOR";
        case TOKEN_SPECIAL_SYMBOL:
            return "SPECIAL_SYMBOL";
        case TOKEN_EOF:
            return "EOF";
        default:
            return "UNKNOWN";
    }
}

// Check if character is an operator
int isOperatorChar(char c) {
    return strchr("+-*/%=<>!&|^~", c) != NULL;
}

// Check if character is a special symbol (punctuation)
int isSpecialSymbol(char c) {
    return strchr("(){};:,.[?", c) != NULL;
}

// ===== LEXICAL ANALYSIS ALGORITHM =====

Token* lexicalAnalysis(FILE* file, int* tokenCount) {
    Token* tokens = (Token*)malloc(MAX_TOKENS * sizeof(Token));
    *tokenCount = 0;
    
    if (file == NULL) {
        return tokens;
    }
    
    int line = 1, column = 1;
    int c;
    
    // Start lexical analysis - read one character at a time
    while ((c = readChar(file)) != EOF) {
        
        // ===== STEP 1: Skip whitespace (space, tab) and ignore them =====
        if (c == ' ' || c == '\t') {
            column++;
            continue;
        }
        
        // Handle newlines (track line numbers but don't create tokens for them)
        if (c == '\n') {
            line++;
            column = 1;
            continue;
        }
        
        // ===== STEP 2: Check for comments =====
        // Check for // single-line comment
        if (c == '/') {
            int nextChar = peekChar(file);
            if (nextChar == '/') {
                // Skip // comment until newline
                readChar(file); // consume the second /
                column += 2;
                while ((c = readChar(file)) != EOF && c != '\n') {
                    column++;
                }
                if (c == '\n') {
                    line++;
                    column = 1;
                }
                continue;
            }
            // Check for /* multi-line comment */
            else if (nextChar == '*') {
                // Skip /* comment until */
                readChar(file); // consume the *
                column += 2;
                int prevChar = 0;
                while ((c = readChar(file)) != EOF) {
                    if (prevChar == '*' && c == '/') {
                        column++;
                        break;
                    }
                    if (c == '\n') {
                        line++;
                        column = 1;
                    } else {
                        column++;
                    }
                    prevChar = c;
                }
                continue;
            }
            // Otherwise it's a division operator, handle below
        }
        
        // ===== STEP 3: Check for alphabets or underscore (identifiers/keywords) =====
        if (isalpha(c) || c == '_') {
            char lexeme[MAX_TOKEN_LENGTH];
            int pos = 0;
            int startColumn = column;
            
            // Read alphabets, digits, and underscores continuously
            while (isalnum(c) || c == '_') {
                if (pos < MAX_TOKEN_LENGTH - 1) {
                    lexeme[pos++] = c;
                }
                c = readChar(file);
                column++;
            }
            
            // Put back the last character that was not part of the lexeme
            if (c != EOF) {
                ungetc(c, file);
                column--;
            }
            
            lexeme[pos] = '\0';
            
            // Compare lexeme with keyword list
            tokens[*tokenCount].type = isKeyword(lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            tokens[*tokenCount].value = (char*)malloc(strlen(lexeme) + 1);
            strcpy(tokens[*tokenCount].value, lexeme);
            tokens[*tokenCount].line = line;
            tokens[*tokenCount].column = startColumn;
            (*tokenCount)++;
            continue;
        }
        
        // ===== STEP 4: Check for digit (constants) =====
        if (isdigit(c)) {
            char lexeme[MAX_TOKEN_LENGTH];
            int pos = 0;
            int startColumn = column;
            
            // Read the complete number (digits and decimal point)
            while (isdigit(c) || c == '.') {
                if (pos < MAX_TOKEN_LENGTH - 1) {
                    lexeme[pos++] = c;
                }
                c = readChar(file);
                column++;
            }
            
            // Put back the last character that was not part of the number
            if (c != EOF) {
                ungetc(c, file);
                column--;
            }
            
            lexeme[pos] = '\0';
            
            tokens[*tokenCount].type = TOKEN_CONSTANT;
            tokens[*tokenCount].value = (char*)malloc(strlen(lexeme) + 1);
            strcpy(tokens[*tokenCount].value, lexeme);
            tokens[*tokenCount].line = line;
            tokens[*tokenCount].column = startColumn;
            (*tokenCount)++;
            continue;
        }
        
        // ===== STEP 5: Check for string and character constants =====
        if (c == '\'' || c == '"') {
            char quote = c;
            char lexeme[MAX_TOKEN_LENGTH];
            int pos = 0;
            int startColumn = column;
            lexeme[pos++] = c;
            column++;
            
            // Read characters until the corresponding closing quote
            while ((c = readChar(file)) != EOF && c != quote) {
                if (pos < MAX_TOKEN_LENGTH - 1) {
                    lexeme[pos++] = c;
                }
                if (c == '\\') {
                    // Handle escape sequence
                    c = readChar(file);
                    if (c != EOF && pos < MAX_TOKEN_LENGTH - 1) {
                        lexeme[pos++] = c;
                    }
                    column++;
                } else if (c == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
            }
            
            // Add closing quote
            if (c == quote && pos < MAX_TOKEN_LENGTH - 1) {
                lexeme[pos++] = c;
                column++;
            }
            lexeme[pos] = '\0';
            
            // Classify as character constant or string literal
            if (quote == '\'') {
                tokens[*tokenCount].type = TOKEN_CHARACTER_CONSTANT;
            } else {
                tokens[*tokenCount].type = TOKEN_STRING_LITERAL;
            }
            
            tokens[*tokenCount].value = (char*)malloc(strlen(lexeme) + 1);
            strcpy(tokens[*tokenCount].value, lexeme);
            tokens[*tokenCount].line = line;
            tokens[*tokenCount].column = startColumn;
            (*tokenCount)++;
            continue;
        }
        
        // ===== STEP 6: Check for operators =====
        if (isOperatorChar(c)) {
            char lexeme[MAX_TOKEN_LENGTH];
            int pos = 0;
            int startColumn = column;
            lexeme[pos++] = c;
            column++;
            
            int nextChar = peekChar(file);
            
            // Check for multi-character operators
            if ((c == '+' && nextChar == '+') ||
                (c == '+' && nextChar == '=') ||
                (c == '-' && nextChar == '-') ||
                (c == '-' && nextChar == '=') ||
                (c == '*' && nextChar == '=') ||
                (c == '/' && nextChar == '=') ||
                (c == '%' && nextChar == '=') ||
                (c == '=' && nextChar == '=') ||
                (c == '!' && nextChar == '=') ||
                (c == '<' && nextChar == '<') ||
                (c == '<' && nextChar == '=') ||
                (c == '>' && nextChar == '>') ||
                (c == '>' && nextChar == '=') ||
                (c == '&' && nextChar == '&') ||
                (c == '&' && nextChar == '=') ||
                (c == '|' && nextChar == '|') ||
                (c == '|' && nextChar == '=') ||
                (c == '^' && nextChar == '=')) {
                readChar(file);
                lexeme[pos++] = nextChar;
                column++;
            }
            
            lexeme[pos] = '\0';
            
            tokens[*tokenCount].type = TOKEN_OPERATOR;
            tokens[*tokenCount].value = (char*)malloc(strlen(lexeme) + 1);
            strcpy(tokens[*tokenCount].value, lexeme);
            tokens[*tokenCount].line = line;
            tokens[*tokenCount].column = startColumn;
            (*tokenCount)++;
            continue;
        }
        
        // ===== STEP 7: Check for special symbols =====
        if (isSpecialSymbol(c)) {
            char lexeme[3];
            lexeme[0] = c;
            lexeme[1] = '\0';
            
            tokens[*tokenCount].type = TOKEN_SPECIAL_SYMBOL;
            tokens[*tokenCount].value = (char*)malloc(2);
            strcpy(tokens[*tokenCount].value, lexeme);
            tokens[*tokenCount].line = line;
            tokens[*tokenCount].column = column;
            (*tokenCount)++;
            column++;
            continue;
        }
        
        // Skip unknown characters
        column++;
    }
    
    return tokens;
}

// ===== DISPLAY AND CLEANUP FUNCTIONS =====

void displayTokens(Token* tokens, int tokenCount) {
    printf("\nLexical Analysis Started...\n");
    printf("\n");
    printf("TOKEN                TYPE\n");
    printf("--------------------------------\n");
    
    for (int i = 0; i < tokenCount; i++) {
        const char* typeName = getTokenTypeName(tokens[i].type);
        printf("%-20s %s\n", tokens[i].value, typeName);
    }
    
    printf("\nLexical Analysis Completed\n");
}

void freeTokens(Token* tokens, int tokenCount) {
    for (int i = 0; i < tokenCount; i++) {
        free(tokens[i].value);
    }
    free(tokens);
}
