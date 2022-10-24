NAME = my_mastermind
SRCS = main.c
WARNINGS = -Wall -Werror -Wextra

all: 	$(NAME)

$(NAME):
		@gcc -o $(NAME) $(WARNINGS) $(SRCS)

debug:
		@gcc -g -o $(NAME) $(SRCS)

clean:
		@rm -f *.o

fclean: clean
		@rm -f $(NAME)

re: 	fclean all