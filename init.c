# include "my_mastermind.h"

/*
*   Generates a randomized secret code,
*   using the "current time" as the seed
*   value for the random number generator.
*   Guaranteed to always generate a 4-digit
*   random integer that gets converted to a
*   string while replacing 9s with 0s.
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
*   Reads user input consisting of optional
*   secret code or number of attempts if any.
*   Otherwise, it generates a randomized secret
*   code and sets number of attempts to MAX_ATTEMPTS.
*   Lastly, it counts the number of secret code pieces
*   of the same "color" and initiliazes the rest to zero
*   to make it easier to keep track of misplaced pieces.
*/
bool init_game(char** argv, int argc, game_info* game) {
    if (argc == 1 || argc == 3 || argc == 5) {
        game->code = argv[1] && argv[2] && argv[1][1] == 'c' ? argv[2] :
                     argv[3] && argv[4] && argv[3][1] == 'c' ? argv[4] :
                     generate_secret_code(game->code);

        game->attempts = argv[3] && argv[4] && argv[3][1] == 't' ? atoi(argv[4]) :
                         argv[1] && argv[2] && argv[1][1] == 't' ? atoi(argv[2]) :
                         MAX_ATTEMPTS;

        if (game->attempts > 0 && is_valid_code(CODE_SIZE, game->code)) {
            for (int i = 0; i < CODE_SIZE; i++)
                game->pieces[game->code[i] - '0'] += 1;
            return true;
        }
    }
    return false;
}