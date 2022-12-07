# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <time.h>

# define MAX_ATTEMPTS 10
# define CODE_SIZE 4
# define NUM_PIECES 9

#ifndef GAME_INFO_H
#define GAME_INFO_H
typedef struct s_game_info {
    unsigned int pieces[NUM_PIECES];
    char* code;
    int attempts;
} game_info;
#endif

bool init_game(char** argv, int argc, game_info* game);
bool is_valid_code(int n, char* input);
int read_input(char* guess);