# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/06 22:46:32 by tschetti          #+#    #+#              #
#    Updated: 2024/10/06 22:46:32 by tschetti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g 
LDFLAGS = -lreadline
SRCS = 	libft/a_libft.c libft/a_libft2.c libft/a_libft3.c libft/a_libft4.c libft/a_split.c \
		builtins/builtin.c builtins/builtin2.c builtins/builtin3.c builtins/builtin4.c builtins/builtin5.c builtins/builtin_exit.c \
		errors/errs.c errors/errs2.c errors/errs3.c \
		executor.c exec/exec2.c exec/exec2utils.c exec/exec2_signals.c exec/exec3_pipe.c \
		exec/exec3_pipe2.c exec/exec3_wait.c exec/exec4_pipe.c exec/exec5.c exec/exec5_builtin.c \
		exec/exec6_builtin.c exec/exec7_utils.c exec/exec7_child.c exec/exec8_child.c exec/exec10_heredoc.c exec/exec11.c \
		exec/exec12.c \
		get_cmd_path.c expanders/expander.c expanders/expander2_utils.c expanders/expander3.c expanders/expander4.c expanders/expander5_utils.c \
		free.c free2.c\
		lexer/lexer.c lexer/lexer2_process.c lexer/lexer3_special_process.c lexer/lexer4_handle_token.c lexer/lexer4_handle_operator.c \
		lexer/lexer5_handle_token_utils.c lexer/lexer6_create_token.c lexer/lexer7_node_utils.c lexer/lexer8_gen_utils.c \
		m_main.c m_minishell.c parsers/parser.c parsers/parser2_args_and_redir.c parsers/parser3_args_and_redir_utils.c \
		parsers/parser4_redir_and_dollar.c parsers/parser5_arg_list_to_array.c signals.c token.c
OBJS = $(SRCS:.c=.o)

all: $(NAME) mini_art

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

valgrind: all
	clear
	@valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --quiet --suppressions=zz_readline.supp ./minishell

.PHONY: all clean fclean re

mini_art:
	@echo "\033[38;5;51m 🐚 🦪 🐠 🐟 🦪 🐟 🦪 🦪 🐠 🐚\033[1;38;5;39m Compiled\033[0m"

test: $(NAME)
	@echo "Running tests..."
	@./test_script.sh
	@echo "All tests completed."

