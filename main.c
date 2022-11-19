# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <time.h>

# define MAX_ATTEMPTS 10
# define CODE_SIZE 4
# define NUM_PIECES 9

typedef struct s_game_info {
    unsigned int pieces[NUM_PIECES];
    unsigned int attempts;
    char* code;
} game_info;

/*
*   Generates a randomized secret code,
*   using the "current time" as the seed
*   value for the random number generator.
*   Guaranteed to always generate a 4-digit
*   random integer that gets converted to a
*   string while replacing 9s with 0s.
*
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

void print_bit_map(unsigned int bits[NUM_PIECES]) {
    for (int i = 0; i < NUM_PIECES; i++)
        printf("%u ", bits[i]);
}

/*
    Compares the player's guess against the secret code,
    keep tracking of well placed and misplaced pieces.
    Returns True if the guess matches the secret code,
    or False otherwise.
*/
bool check_guess(char* guess, game_info game) {
    unsigned int well_placed = 0;
    unsigned int misplaced = 0;
    bool won = false;
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
    won = well_placed == CODE_SIZE;
    if (won) {
        printf("Congratz! You did it!\n");
    }
    else {
        printf("Well placed pieces: %d\n", well_placed);
        printf("Misplaced pieces: %d\n", misplaced);
    }
    return won;
}

// If the user pressed Ctrl + D, there will be a null terminator
// at the end of input string and n must be equal to CODE_SIZE.
// If the user pressed the Enter key, n must be equal to CODE_SIZE + 1.
// Returns False otherwise.
bool is_valid_input(int n, char* input) {
    if(!((n == CODE_SIZE && input[CODE_SIZE] == '\0')))
        return false;
    n = 0;
    while(input[n]) {
        if (input[n] < '0' || input[n++] > '8')
            return false;
    }
    return n == CODE_SIZE;
}

int read_input(char* guess) {
    // Replace the newline character with '\0' if
    // the user pressed Enter to submit input. Otherwise,
    // print a newline if the user pressed Ctrl + D.
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

    return (argc == 1 || argc == 3 || argc == 5) && is_valid_input(CODE_SIZE, game->code);
}

int main(int ac, char** argv) {
    char secret_code[] = "0000";
    game_info game = {{0,0,0,0,0,0,0,0,0}, 0, secret_code};
    char guess[100];
    unsigned int n = 1;
    unsigned int round = 0;
    bool next_round = true;

    // print_bit_map(game.pieces);
    // printf("\n%s\n", game.code);
    // printf("%d\n", game.attempts);

    if (init_game(argv, ac, &game)) {
        printf("Will you find the secret code?\n");
        printf("Please enter a valid guess\n");

        while(n && game.attempts) {
            if (next_round) {
                printf("---\n");
                printf("Round %d\n", round++);
            }
            n = read_input(guess);
            if (is_valid_input(n, guess)) {
                if (check_guess(guess, game)) {
                    break;
                }
                else {
                    next_round = true;
                    game.attempts--;
                }
            }
            else if (n) {
                next_round = false;
                printf("Wrong input!\n");
                continue;
            }
        }
    }
    return 0;
}
