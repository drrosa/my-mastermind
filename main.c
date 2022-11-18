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
    unsigned int well_placed;
    unsigned int misplaced;
    char* code;
    bool won;
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

/*
*   Reads user input consisting of optional
*   secret code or number of attempts if any.
*   Otherwise, it generates a randomized secret
*   code and sets number of attempts to MAX_ATTEMPTS.
*   Lastly, it counts the number of secret code pieces
*   of the same color and initiliazes the rest to zero
*   to make it easier to keep track of misplaced pieces.
*/
void init_game(char** argv, game_info* game) {
    game->code = argv[1] && argv[2] && argv[1][1] == 'c' ? argv[2] :
                 argv[3] && argv[4] && argv[3][1] == 'c' ? argv[4] :
                 generate_secret_code(game->code);
    game->attempts = argv[3] && argv[4] && argv[3][1] == 't' ? atoi(argv[4]) :
                     argv[1] && argv[2] && argv[1][1] == 't' ? atoi(argv[2]) :
                     MAX_ATTEMPTS;
    game->well_placed = 0;
    game->misplaced = 0;
    game->won = false;

    for (int i = 0; i < CODE_SIZE; i++)
        game->pieces[game->code[i] - '0'] += 1;
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
bool check_guess(char* guess, game_info* game_ptr, game_info game) {
    for (int i = 0; guess[i]; i ++) {
        if (guess[i] == game.code[i]) {
            game_ptr->well_placed++;
            game.pieces[guess[i] - '0'] -= 1;
        }
    }
    for (int i = 0; guess[i]; i ++) {
        if (guess[i] != game.code[i] && game.pieces[guess[i] - '0']) {
            game_ptr->misplaced++;
            game.pieces[guess[i] - '0'] -= 1;
        }
    }
    return game_ptr->well_placed == CODE_SIZE;
}

bool validate_input(char* input, int n) {
    int i = 0;

    
    // Replace the newline character with '\0' if
    // the user pressed Enter to submit input. Otherwise,
    // print a newline if the user pressed Ctrl + D.
    if (input[n - 1] == '\n')
        input[n - 1] = '\0';
    else
        printf("\n");

    // If the user pressed Ctrl + D, there will be a null terminator
    // at the end of input string and n must equal to CODE_SIZE.
    // If the user pressed the Enter key, n must be equal to CODE_SIZE + 1.
    // Returns False otherwise.
    if(!((n == CODE_SIZE && input[CODE_SIZE] == '\0') || n == CODE_SIZE + 1))
        return false;

    while(input[i]) {
        if (input[i] < '0' || input[i] > '8')
            return false;
        i++;
    }
    return i == CODE_SIZE;
}

int main(int ac, char** argv) {
    char secret_code[] = "0000";
    game_info game = {{0,0,0,0,0,0,0,0,0}, 0, 0, 0, secret_code, false};
    char guess[100];
    unsigned int n = 1;
    unsigned int round = 0;
    bool valid_input = true;

    init_game(argv, &game);
    print_bit_map(game.pieces);
    // printf("\n%s\n", game.code);
    // printf("%d\n", game.attempts);

    if ((ac == 1 || ac == 3 || ac == 5) && validate_input(game.code, CODE_SIZE)) {

        printf("Will you find the secret code?\n");
        printf("Please enter a valid guess\n");

        while(game.attempts) {
            if (valid_input) {
                printf("---\n");
                printf("Round %d\n", round++);
            }

            if ((n = read(0, guess, 100)) && validate_input(guess, n)) {
                if (check_guess(guess, &game, game)) {
                    printf("Congratz! You did it!\n");
                    break;
                }
                else {
                    valid_input = true;
                    printf("Well placed pieces: %d\n", game.well_placed);
                    printf("Misplaced pieces: %d\n", game.misplaced);
                    game.well_placed = 0;
                    game.misplaced = 0;
                    game.attempts--;
                }
            }
            else {
                printf("Wrong input!\n");
                valid_input = false;
                continue;
            }
        }
    }
    return 0;
}
