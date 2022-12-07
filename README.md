# Welcome to My Mastermind
***

## Task
This project is an implementation of the game Mastermind in the C programming language.

By default, the player has 10 attempts to guess a randomly generated secret code made out
of 4 pieces from 9 different colors represented by '0' '1' '2' '3' '4' '5' '6' '7' '8'.

After each guess, the game provides feedback by displaying the number of correctly placed
pieces and the number of misplaced pieces. A piece that is present in the secret code, but
not in the correct position is called misplaced piece.

The game ends when the player correctly guesses the secret code or once the maximum number
of attempts is reached.

The program reads the player's keyboard input from standard input and accepts the following options:

-c [CODE]: to specify the secret code. Otherwise, a random code will be generated.  
-t [ATTEMPTS]: to specify the maximum number of attempts. Otherwise, the player has 10 attempts.

Only the following functions from the C standard library are used:  
`printf(3)`, `write(2)`, `read(2)`, `srand()`, `rand()`, `time()`, and `atoi()`.

## Description
The core functionality of this program is keeping track of the number of well placed
and misplaced pieces each time the player enters a valid guess. The key idea is to use an
array where the indeces correspond to each one of the game pieces and the value at each
index corresponds to the number of occurrences of each piece in the secret code.

The counter for well-placed pieces is incremented when a piece from the player's guess
matches a piece in the secret code exactly. The number of occurrences for that piece is
decremented, in the array described above, to prevent double-counting misplaced pieces.

If the number of occurrences for that piece can be decremented i.e. the value is greater than
zero, then the counter for misplaced pieces is incremented whenever a piece from the player's
guess does not match exactly, but it does exist in the secret code.

The rest of the program consists of prompting for user input and validating this input by making
sure the program does not crash whenever the input is considered to be invalid.

## Installation
Download the source code and compile it by running the `make my_mastermind` command within the
project's directory. This will generate an executable file called `my_mastermind`.

## Usage
Execute the program with the following command. The parts within square brackets are considered
optional arguments, where `CODE` must be a valid 4-digit code and `ATTEMPTS` must be a valid integer.
```
./my_mastermind [-c CODE] [-t ATTEMPTS]
```
If the `-c` option is not specified, the program will generate a random secret code.
If the `-t` option is not specified, the program will default to 10 attempts.

The program handles the keyboard sequence `Ctrl + D` i.e. End Of File in the following ways:
- Pressing `Ctrl + D` after entering a set of characters is considered normal execution.
- Pressing `Ctrl + D` by itself will end program execution.

### The Core Team

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
