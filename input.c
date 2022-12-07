# include "my_mastermind.h"

/*
*   Checks whether a given code is valid and returns True if
*   the length of the code is equal to CODE_SIZE and
*   the code has valid pieces in it. Returns False otherwise.
*/
bool is_valid_code(int n, char* input) {
    if(!((n == CODE_SIZE && input[CODE_SIZE] == '\0')))
        return false;
    n = 0;
    while(input[n]) {
        if (input[n] < '0' || input[n++] > '8')
            return false;
    }
    return true;
}

/* 
*   Reads user input and replaces the newline character with '\0'
*   if the user pressed Enter to submit input. Otherwise,
*   print a newline and add a null terminator if the user pressed Ctrl + D.
*/
int read_input(char* guess) {
    int n = read(0, guess, 100);
    if (n && guess[n - 1] == '\n') {
        guess[n - 1] = '\0';
        n--;
    }
    else if(n) {
        printf("\n");
        guess[n] = '\0';
    }
    return n;
}