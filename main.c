# include "my_mastermind.h"

/*
*    Compares the player's guess against the secret code,
*    while counting the number of well placed and misplaced
*    pieces which is then given as feedback to the player.
*    Returns True if the guess matches the secret code,
*    or False otherwise.
*/
static bool is_correct_guess(char* guess, game_info game) {
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

int main(int ac, char** argv) {
    char secret_code[] = "0000";
    game_info game = {.pieces = {0,0,0,0,0,0,0,0,0}, .code = secret_code, .attempts = 0};
    char guess[100];
    unsigned int n = 0;
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
                printf("exit\n");
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
