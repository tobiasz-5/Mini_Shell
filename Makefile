# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/25 14:16:49 by tschetti          #+#    #+#              #
#    Updated: 2024/11/25 14:16:49 by tschetti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g 
LDFLAGS = -lreadline
SRCS = 	libft/libft.c libft/libft2.c libft/libft3.c libft/libft4.c libft/libft_split.c \
		main.c minishell.c signals.c\
		builtins/builtin.c builtins/builtin2.c builtins/builtin3.c builtins/builtin4.c builtins/builtin5.c builtins/builtin_exit.c \
		errors_and_free/errs.c errors_and_free/errs2.c errors_and_free/errs3.c errors_and_free/free.c errors_and_free/free2.c\
		exec/exec_apply.c exec/exec_backup_fd.c exec/exec_child.c exec/exec_cmd.c exec/exec_env_utils.c exec/exec_handle_redir.c \
		exec/exec_heredoc.c exec/exec_init_args.c exec/exec_pipe.c exec/exec_pipe_utils.c exec/exec_pipe2.c exec/exec_process_heredoc.c \
		exec/exec_read_line_utils.c exec/exec_read_line.c exec/exec_redir2.c exec/exec_redirect.c exec/exec_signals.c exec/exec_tmp_heredoc.c \
		exec/exec_wait_pipe.c exec/exec.c exec/get_cmd_path.c \
		expanders/exp_token.c expanders/exp_utils.c expanders/exp_utils2.c expanders/exp_variable.c expanders/exp.c \
		lexer/lexer.c lexer/lexer_process_token.c lexer/lexer_process_special_token.c lexer/lexer_handle_token.c lexer/lexer_handle_operator.c \
		lexer/lexer_handle_token_utils.c lexer/lexer_create_token.c lexer/lexer_node_utils.c lexer/lexer_gen_utils.c \
		parsers/parser.c parsers/parser_args_and_redir.c parsers/parser_redir_utils.c \
		parsers/parser_redir_and_dollar.c parsers/parser_args_to_list.c parsers/token.c
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

