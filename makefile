NAME = minishell
SRC	= main.c parse/parse.c parse/parse1.c parse/parse2.c parse/parse3.c parse/parse4.c parse/parse5.c \
parse/error.c builtin/cd.c builtin/pwd.c builtin/echo.c builtin/exit.c builtin/env.c \
executer/executer.c parse/env.c executer/redirect.c builtin/export_utils.c builtin/export.c \
builtin/set_env.c executer/executer15.c parse/redirects.c  builtin/unset.c builtin/unset_utils.c \
executer/signal.c executer/executer_error.c
CC = gcc
CFLAGS = -g -Wall -Werror -Wextra #-fsanitize=address
RM = rm -rf
LIBFT = lib/libft/libft.a
READLINE = readline
OBJ = $(SRC:.c=.o)


RESET=\033[0m
RED=\033[31m

LIGHT_RED=\033[91m
GREEN=\033[32m
LIGHT_GREEN=\033[92m
YELLOW=\033[33m
LIGHT_YELLOW=\033[93m
BLUE=\033[34m
LIGHT_BLUE=\033[94m
MAGENTA=\033[35m
LIGHT_MAGENTA=\033[95m
CYAN=\033[36m
LIGHT_CYAN=\033[96m
WHITE=\033[37m
GREY=\033[90m
LIGHT_GREY=\033[37m

all : $(READLINE) $(LIBFT) $(NAME)

$(READLINE):
	@clear
	@echo "$(YELLOW)Compiling readline please wait$(GREEN)"
	@curl -s -O https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
	@tar -xvf readline-8.2.tar.gz 2>&1 | awk '{printf "."; fflush()}'
	@cd readline-8.2 && ./configure --prefix=${PWD}/readline 2>&1 | awk '{printf "."; fflush()}'
	@cd readline-8.2 && make install 2>&1 | awk '{printf "."; fflush()}'
	@echo "$(RESET)"
	


$(LIBFT):
	@echo "$(YELLOW)Compailing include please wait$(GREEN)"
	@make -C lib/libft | awk '{printf "."; fflush()}'
	@echo ""
	@echo "$(YELLOW)Compailing main proje please wait$(GREEN)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I${PWD}/readline/include/
	@echo "$(GREEN).$(RESET)" | tr -d '\n'

$(NAME): $(SRC) $(OBJ)
	@$(CC) $(CFLAGS) $(LIBFT) $(SRC) -L${PWD}/readline/lib  -I${PWD}/readline/include/ -lreadline -o $(NAME)
	@echo "$(GREEN)"
	@echo "Minishell Compiled"
	@echo "$(RESET)"
	
clean:
	@clear
	@$(RM) $(OBJ)
	@make -C lib/libft clean
	@echo "$(CYAN)Object files removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@make -C lib/libft fclean
	@rm -rf readline
	@rm -rf readline-8.2 readline-8.2.tar.gz
	@echo "$(CYAN)Readline files removed$(RESET)"
	@clear


tclean: clean
	@$(RM) $(NAME)
	@make -C lib/libft fclean
	@echo "$(CYAN)Readline files removed$(RESET)"
	@clear

re: tclean all

run: re 
	./minishell
.PHONY: all clean fclean re