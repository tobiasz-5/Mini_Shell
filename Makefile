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
SRCS = 	a_libft.c a_libft2.c a_libft3.c a_libft4.c a_split.c builtin.c builtin2.c builtin3.c builtin4.c builtin5.c builtin_exit.c \
		errs.c errs2.c errs3.c executor.c executor2.c executor2env.c executor2_signals.c executor3_pipe.c executor3_setup_child.c executor3_wait.c executor4_pipe_utils.c executor5_redir_builtin.c \
		executor6_redir_builtin_utils.c executor7_redir_child.c executor8_redir_child_utils.c executor9_heredoc.c \
		executor10_heredoc_expansion.c get_cmd_path.c executor11_read_write_heredoc.c \
		executor12_read_and_expand_utils.c expander.c expander2_utils.c expander3.c expander4.c expander5_utils.c \
		free.c free2.c lexer.c lexer2_process.c lexer3_special_process.c lexer4_handle_token.c lexer4_handle_operator.c \
		lexer5_handle_token_utils.c lexer6_create_token.c lexer7_node_utils.c lexer8_gen_utils.c \
		m_main.c m_minishell.c parser.c parser2_args_and_redir.c parser3_args_and_redir_utils.c \
		parser4_redir_and_dollar.c parser5_arg_list_to_array.c signals.c token.c
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

.PHONY: all clean fclean re

mini_art:
	@echo "\033[38;5;51m 🐚 🦪 🐠 🐟 🦪 🐟 🦪 🦪 🐠 🐚\033[1;38;5;39m Compiled\033[0m"

test: $(NAME)
	@echo "Running tests..."
	@./test_script.sh
	@echo "All tests completed."

