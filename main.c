# include "my_mastermind.h"

/*
*   Generates a randomized secret code,
*   using the "current time" as the seed
*   value for the random number generator.
*   Guaranteed to always generate a 4-digit
*   random integer that gets converted to a
*   string while replacing 9s with 0s.
*/
char* generate_secret_code(char* secret_code) {
    unsigned int num;

    srand(time(0));
    do {
        num = rand() % 10000;
    } while (num < 1000);

    for (int i = CODE_SIZE - 1; i >= 0 ; i--) {
        secret_code[i] = '0' + (num % 10 % 9);
        num /= 10;
    }
    return secret_code;
}

/*
*   Checks whether a given code is valid and returns True if
*   the length of the code is equal to CODE_SIZE and
*   the code has valid pieces in it.
*   Returns False otherwise.
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
*   print a newline if the user pressed Ctrl + D.
*/
int read_input(char* guess) {
    int n = read(0, guess, 100);
    if (n && guess[n - 1] == '\n') {
        guess[n - 1] = '\0';
        n--;
    }
    else
        printf("\n");
    return n;
}

/*
*    Compares the player's guess against the secret code,
*    keep tracking of well placed and misplaced pieces.
*    Returns True if the guess matches the secret code,
*    or False otherwise.
*/
bool is_correct_guess(char* guess, game_info game) {
    unsigned int well_placed = 0;
    unsigned int misplaced = 0;

    for (int i = 0; guess[i]; i ++) {
        if (guess[i] == game.code[i]) {
            well_placed++;
            game.pieces[guess[i] - '0'] -= 1;
        }
    }
    for (int i = 0; guess[i]; i ++) {
        if (guess[i] != game.code[i] && game.pieces[guess[i] - '0']) {
            misplaced++;
            game.pieces[guess[i] - '0'] -= 1;
        }
    }
    if (well_placed == CODE_SIZE) {
        printf("Congratz! You did it!\n");
        return true;
    }
    else {
        printf("Well placed pieces: %d\n", well_placed);
        printf("Misplaced pieces: %d\n", misplaced);
        return false;
    }
}

/*
*   Reads user input consisting of optional
*   secret code or number of attempts if any.
*   Otherwise, it generates a randomized secret
*   code and sets number of attempts to MAX_ATTEMPTS.
*   Lastly, it counts the number of secret code pieces
*   of the same color and initiliazes the rest to zero
*   to make it easier to keep track of misplaced pieces.
*/
bool init_game(char** argv, int argc, game_info* game) {
    game->code = argv[1] && argv[2] && argv[1][1] == 'c' ? argv[2] :
                 argv[3] && argv[4] && argv[3][1] == 'c' ? argv[4] :
                 generate_secret_code(game->code);
    game->attempts = argv[3] && argv[4] && argv[3][1] == 't' ? atoi(argv[4]) :
                     argv[1] && argv[2] && argv[1][1] == 't' ? atoi(argv[2]) :
                     MAX_ATTEMPTS;

    for (int i = 0; i < CODE_SIZE; i++)
        game->pieces[game->code[i] - '0'] += 1;

    return (argc == 1 || argc == 3 || argc == 5) && is_valid_code(CODE_SIZE, game->code);
}

int main(int ac, char** argv) {
    char secret_code[] = "0000";
    game_info game = {{0,0,0,0,0,0,0,0,0}, 0, secret_code};
    char guess[100];
    unsigned int n = 1;
    unsigned int round = 0;
    bool is_next_round = true;

    if (init_game(argv, ac, &game)) {
        printf("Will you find the secret code?\n");
        printf("Please enter a valid guess\n");
        while(game.attempts) {
            if (is_next_round) {
                printf("---\n");
                printf("Round %d\n", round++);
            }
            if (!(n = read_input(guess))) {
                break;
            }
            if (!is_valid_code(n, guess)) {
                is_next_round = false;
                printf("Wrong input!\n");
                continue;
            }
            is_next_round = true;
            game.attempts = is_correct_guess(guess, game) ? 0 : game.attempts - 1;
        }
    }
    return 0;
}
