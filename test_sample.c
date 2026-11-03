// This is a single-line comment
/* This is a
   multi-line comment */

#include <stdio.h>

// Function prototype
int add(int x, int y);

int main() {
    int a = 10;
    float b = 3.14;
    char ch = 'A';
    const char* str = "Hello, World!";
    
    if (a > 5) {
        printf("a is greater than 5\n");
    } else {
        printf("a is less than or equal to 5\n");
    }
    
    int result = add(a, 20);
    return 0;
}

int add(int x, int y) {
    return x + y;
}
