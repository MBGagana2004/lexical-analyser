// ===== TEST PROGRAM FOR LEXICAL ANALYZER =====
// This file tests the lexical analyzer with various C code samples

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "lexer.h"

#define TEST_FILE "test_sample.c"

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║           LEXICAL ANALYZER TEST PROGRAM                    ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Create a sample C source file for testing
    printf("Creating test sample file: %s\n", TEST_FILE);
    FILE* sampleFile = fopen(TEST_FILE, "w");
    if (sampleFile == NULL) {
        printf("Error: Cannot create test sample file.\n");
        return 1;
    }
    
    // Write sample C code with various token types
    fprintf(sampleFile, "// This is a single-line comment\n");
    fprintf(sampleFile, "/* This is a\n");
    fprintf(sampleFile, "   multi-line comment */\n");
    fprintf(sampleFile, "\n");
    fprintf(sampleFile, "#include <stdio.h>\n");
    fprintf(sampleFile, "\n");
    fprintf(sampleFile, "// Function prototype\n");
    fprintf(sampleFile, "int add(int x, int y);\n");
    fprintf(sampleFile, "\n");
    fprintf(sampleFile, "int main() {\n");
    fprintf(sampleFile, "    int a = 10;\n");
    fprintf(sampleFile, "    float b = 3.14;\n");
    fprintf(sampleFile, "    char ch = 'A';\n");
    fprintf(sampleFile, "    const char* str = \"Hello, World!\";\n");
    fprintf(sampleFile, "    \n");
    fprintf(sampleFile, "    if (a > 5) {\n");
    fprintf(sampleFile, "        printf(\"a is greater than 5\\n\");\n");
    fprintf(sampleFile, "    } else {\n");
    fprintf(sampleFile, "        printf(\"a is less than or equal to 5\\n\");\n");
    fprintf(sampleFile, "    }\n");
    fprintf(sampleFile, "    \n");
    fprintf(sampleFile, "    int result = add(a, 20);\n");
    fprintf(sampleFile, "    return 0;\n");
    fprintf(sampleFile, "}\n");
    fprintf(sampleFile, "\n");
    fprintf(sampleFile, "int add(int x, int y) {\n");
    fprintf(sampleFile, "    return x + y;\n");
    fprintf(sampleFile, "}\n");
    
    fclose(sampleFile);
    printf("Test file created successfully.\n\n");
    
    // Open the test file
    printf("Opening test file: %s\n", TEST_FILE);
    FILE* testFile = openInputFile(TEST_FILE);
    
    if (testFile == NULL) {
        printf("Error: Cannot open test file '%s'\n", TEST_FILE);
        return 1;
    }
    printf("File opened successfully.\n\n");
    
    // Run the lexer
    printf("Running lexical analyzer...\n");
    int tokenCount = 0;
    Token* tokens = lexicalAnalysis(testFile, &tokenCount);
    
    // Display the results
    displayTokens(tokens, tokenCount);
    
    // Count tokens by type for validation
    int keywordCount = 0, identifierCount = 0, constantCount = 0;
    int operatorCount = 0, symbolCount = 0;
    int charConstCount = 0, stringCount = 0;
    
    for (int i = 0; i < tokenCount; i++) {
        switch (tokens[i].type) {
            case TOKEN_KEYWORD: keywordCount++; break;
            case TOKEN_IDENTIFIER: identifierCount++; break;
            case TOKEN_CONSTANT: constantCount++; break;
            case TOKEN_OPERATOR: operatorCount++; break;
            case TOKEN_SPECIAL_SYMBOL: symbolCount++; break;
            case TOKEN_CHARACTER_CONSTANT: charConstCount++; break;
            case TOKEN_STRING_LITERAL: stringCount++; break;
            default: break;
        }
    }
    
    // Display statistics
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                   TOKEN STATISTICS                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("Keywords:           %d\n", keywordCount);
    printf("Identifiers:        %d\n", identifierCount);
    printf("Constants:          %d\n", constantCount);
    printf("Operators:          %d\n", operatorCount);
    printf("Special Symbols:    %d\n", symbolCount);
    printf("Character Constants:%d\n", charConstCount);
    printf("String Literals:    %d\n", stringCount);
    printf("Total Tokens:       %d\n", tokenCount);
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // Cleanup
    closeInputFile(testFile);
    freeTokens(tokens, tokenCount);
    
    printf("\nTest completed successfully!\n");
    return 0;
}
