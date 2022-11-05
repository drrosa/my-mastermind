# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <time.h>

# define MAX_ATTEMPTS 10
# define CODE_SIZE 4
# define NUM_PIECES 9

typedef struct s_game_info {
    bool pieces[NUM_PIECES];
    char* code;
    unsigned int attempts;
    unsigned int well_placed;
    unsigned int misplaced;
    bool won;
} game_info;

/*
*   Initializes values in a bit map to zero
*   and sets secret code pieces to one.
*/

void init_secret_pieces(char* code, bool* pieces) {
    unsigned int i = 0;
    while (*code) {
        pieces[i] = false;
        pieces[i + 1] = false;
        pieces[*code - '0'] = true;
        code++;
        i += 2;
    }
    pieces[i] = 0;
}

/*
*   Generates a randomized secret code.
*/
void generate_secret_code(char* secret_code) {
    unsigned int num;
    // uses the "current time" as the seed
    // value for the random number generator. 
    srand(time(0));
    // generates a 4-digit random integer.
    do {
        num = rand() % 10000;
    } while (num < 1000);
    // converts the given number to a string
    for (int i = 0; i < CODE_SIZE; i++) {
        secret_code[i] = '0' + num % 10;
        num /= 10;
    }
}

/*
*   Reads user input consisting of optional 
*   secret code or number of attempts if any.
*   Otherwise, it generates a randomized secret
*   code and sets number of attempts to MAX_ATTEMPTS.
*/
void init_game(char** argv, game_info* game) {
    if (argv[1] && argv[1][1] == 'c') {
        game->code = argv[2];
    }
    else if (argv[3] && argv[3][1] == 'c') {
        game->code = argv[4];
    }
    else {
        generate_secret_code(game->code);
    }

    init_secret_pieces(game->code, game->pieces);

    if (argv[3] && argv[3][1] == 't') {
        game->attempts = atoi(argv[4]);
    }
    else if (argv[1] && argv[1][1] == 't') {
        game->attempts = atoi(argv[2]);
    }
    else {
        game->attempts = MAX_ATTEMPTS;
    }
    game->well_placed = 0;
    game->misplaced = 0;
    game->won = false;
}

void print_bit_map(bool bits[NUM_PIECES]) {
    for (int i = 0; i < NUM_PIECES; i++)
        printf("%u ", bits[i]);
}

bool check_guess(char* guess, game_info* game) {
    // printf("guess: %s\n", guess);
    for (int i = 0; guess[i]; i ++) {
        if (guess[i] == game->code[i]) {
            game->well_placed++;
        }
        else if (game->pieces[guess[i] - '0']) {
            game->misplaced++;
        }
    }
    return game->well_placed == CODE_SIZE;
}

bool validate_input(char* input) {
    int i = 0;
    while(input[i]) {
        if (input[i] < '0' || input[i] > '8')
            return false;
        i++;
    }
    return (i == 4) && true;
}

int main(int ac, char** argv) {
    game_info game;
    char guess[100];
    char secret_code[5] = "0000";
    game.code = secret_code;
    unsigned int n = 1;
    unsigned int round = 0;
    bool valid_input = true;

    //TODO fix bug where 
    // If ./my_mastermind -c 4321
    // guess 1234 shows 3 misplaced pieces
    // instead of 4!
    
    init_game(argv, &game);
    // TODO validate game code
    // && validate_input(game.code)
    if (ac) {

        printf("Will you find the secret code?\n");
        printf("Please enter a valid guess\n");

        while(n && game.attempts) {
            if (valid_input) {
                printf("---\n");
                printf("Round %d\n", round++);
            }

            n = read(0, guess, 100);

            if (n == 4 && guess[CODE_SIZE] == 0) {
                printf("\n");
                n++;
            }

            guess[CODE_SIZE] = '\0';
            if (n == 5 && validate_input(guess)) {
                game.won = check_guess(guess, &game);
                valid_input = true;
            }
            else {
                printf("Wrong input!\n");
                valid_input = false;
                continue;
            }
            if (game.won) {
                printf("Congratz! You did it!\n");
                break;
            }
            else {
                printf("Well placed pieces: %d\n", game.well_placed);
                printf("Misplaced pieces: %d\n", game.misplaced);
                game.well_placed = 0;
                game.misplaced = 0;
                game.attempts--;
            }
        }
    }
    return 0;
}