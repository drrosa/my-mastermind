# include "my_mastermind.h"

/*
*   Generates a randomized secret code,
*   using the "current time" as the seed
*   value for the random number generator.
*   Guaranteed to always generate a string
*   of 4 random digits between 0 and 8.
*/
static char* generate_secret_code(char* secret_code) {
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
*   Reads program arguments consisting of an optional
*   secret code and/or number of attempts.
*   Otherwise, it generates a randomized secret
*   code and sets number of attempts to MAX_ATTEMPTS.
*   Lastly, it stores the number of occurrences of each
*   piece in the secret code as it will be useful for
*   finding misplaced pieces in the player's guess.
*   Returns True if the secret code is a valid code and
*   the number of attempts is greater than zero.
*/
bool init_game(char** argv, int argc, game_info* game) {
    if (argc == 1 || argc == 3 || argc == 5) {
        game->code = argv[1] && argv[2] && argv[1][1] == 'c' ? argv[2] :
                     argv[3] && argv[4] && argv[3][1] == 'c' ? argv[4] :
                     generate_secret_code(game->code);

        game->attempts = argv[3] && argv[4] && argv[3][1] == 't' ? atoi(argv[4]) :
                         argv[1] && argv[2] && argv[1][1] == 't' ? atoi(argv[2]) :
                         MAX_ATTEMPTS;

        if (is_valid_code(CODE_SIZE, game->code) && game->attempts > 0) {
            for (int i = 0; i < CODE_SIZE; i++)
                game->pieces[game->code[i] - '0'] += 1;
            return true;
        }
    }
    return false;
}