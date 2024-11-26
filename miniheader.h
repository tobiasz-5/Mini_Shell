/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniheader.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tschetti <tschetti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:17:06 by tschetti          #+#    #+#             */
/*   Updated: 2024/11/26 18:50:32 by tschetti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIHEADER_H
# define MINIHEADER_H

# define PATH_MAX 4096
# define COLOR_BLUE "\x1b[34m"
# define COLOR_RESET "\x1b[0m"
# define _POSIX_C_SOURCE 200809L

# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>

extern volatile sig_atomic_t	g_received_signal;

typedef enum e_token_type
{
	TOKEN_PIPE,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_DOLLAR,
	TOKEN_VARIABLE,
	TOKEN_WORD,
}	t_token_type;

typedef struct s_io_fds
{
	int	infile_fd;
	int	outfile_fd;
	int	heredoc_fd;
	int	stdin_backup;
	int	stdout_backup;
}	t_io_fds;

typedef struct s_env_var
{
	char				*name;
	char				*value;
	bool				exported_empty;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_shell_state
{
	bool		exit_shell;
	int			exit_code;
	int			last_exit_status;
	char		**envp;
	t_env_var	*env_list;
}	t_shell_state;

typedef struct s_var_expand
{
	const char	*cursor;
	const char	*dollar_sign;
	size_t		total_length;
	const char	*var_name;
	size_t		var_len;
	char		*var_value;
	char		*extracted_var_name;
	size_t		var_value_len;
	char		*result;
	char		*dest_ptr;
}	t_var_expand;

typedef struct s_token_node
{
	t_token_type		type;
	char				*token;
	bool				single_quote;
	bool				double_quote;
	struct s_token_node	*next;
}	t_token_node;

typedef struct s_token_state
{
	char	*token_str;
	bool	single_quote;
	bool	double_quote;
}	t_token_state;

typedef struct s_token_list
{
	t_token_node	**head;
	t_token_node	**tail;
	t_token_state	*state;
	int				i;
	int				result;
}	t_token_list;

typedef struct s_redirection
{
	char					*heredoc_filename;
	bool					is_quoted;
	char					*filename;
	t_token_type			type;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_list_args
{
	char				*arg;
	bool				single_quote;
	bool				double_quote;
	struct s_list_args	*next;
}	t_list_args;

typedef struct s_command
{
	int					last_exit_status;
	char				*cmd_name;
	bool				has_dollar;
	t_list_args			*args_list;
	t_redirection		*redirections;
	struct s_command	*next;
}	t_command;

typedef struct s_parser_state
{
	t_token_node	*current_token;
	bool			flg_error;
	char			*error_msg;
}	t_parser_state;

typedef struct s_expand_state
{
	size_t			i;
	size_t			j;
	bool			in_quotes;
	size_t			len;
	char			*output;
	t_shell_state	*shell_state;
}	t_expand_state;

typedef struct s_pipe_cmd
{
	t_command	*cmd;
	int			*prev_pipe_fd;
	int			*pipe_fd;
}	t_pipe_cmd;

typedef struct s_fork_info
{
	t_command	*cmd;
	char		**args_array;
	bool		*args_quote_flags;
}	t_fork_info;

typedef struct s_int_to_str_vars
{
	int		i;
	int		temp_num;
	char	temp_str[12];
	int		is_negative;
	int		j;
}	t_int_to_str_vars;

typedef struct s_filename_vars
{
	char	tmp_filename[PATH_MAX];
	char	unique_id_str[12];
	int		unique_id;
	int		fd;
}	t_filename_vars;

void			update_last_exit_status(t_shell_state *shell_state);
void			free_command_and_args(t_command *command);
void			clean_shell_state(t_shell_state *shell_state);
int				process_all_heredocs(t_command *command_list,
					t_shell_state *shell_state);
int				process_command_heredocs(t_command *current_cmd,
					t_shell_state *shell_state);
int				create_temp_heredoc_file(char **heredoc_filename);
void			read_and_expand_heredoc(const char *delimiter,
					int fd, bool is_quoted, t_shell_state *shell_state);
void			handle_heredoc(t_redirection *redirection,
					char **heredoc_filename, t_shell_state *shell_state);
void			clean_filename(t_redirection *redirection);
void			int_to_str(int num, char *str);
int				generate_unique_filename(t_filename_vars *vars);
void			restore_signals_after_command(void);
int				ft_isalnum(int c);
int				ft_isdigit(int c);
int				ft_isalpha(int c);
void			init_env_list(t_shell_state *shell_state);
void			exec_ext_cmd(t_command *all_cmds, t_command *command,
					char **args_array, t_shell_state *shell_state);
char			**build_envp(t_shell_state *shell_state);
char			**get_path_directories(t_shell_state *shell_state);
char			*get_command_path(const char *cmd_name,
					t_shell_state *shell_state);
size_t			ft_strlen(const char *s);
t_env_var		*find_local_env_var(t_env_var *env_list, const char *name);
char			*expand_var_in_string(char *str, t_shell_state *shell_state);
void			add_dollar_char(char **dest_ptr);
void			init_dasvidaniya_norminette(int *len, int *i);
void			init_io_fds(t_io_fds *fds);
void			copy_exit_status(char *result, int *j, char *exit_status_str);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			handle_unclosed_quote_error(t_token_state *state);
void			set_quote_flag(t_token_state *state, char quote);
void			print_command(t_command *command);
void			print_command_list(t_command *command_list);
void			execute_command(t_command *command, char **envp,
					t_shell_state *shell_state);
void			add_token_to_list(t_token_list *token_list, char *token_str);
void			add_token_node(t_token_list *token_list,
					t_token_node *new_node);
void			init_var_for_parsing_tokens(t_token_node *tokens,
					t_parser_state *state, t_command **cmd_list,
					t_command **last_command);
void			call_error_and_free(t_parser_state *state,
					t_command **cmd_list);
void			init_vars_for_parsing_command(t_command *command);
void			init_vars_for_not_cmd(t_parser_state *state);
void			init_vars_for_cmdname_error(t_parser_state *state,
					t_command *command);
void			init_vars_for_expctd_cmdname(t_parser_state *state,
					t_command *command);
void			init_vars_for_error_adding_arg(t_parser_state *state,
					t_command *command);
void			init_var_failed_add_arg(t_parser_state *state);
void			init_var_for_arg_value(char *arg_value, t_list_args *new_arg);
void			init_var_for_malloc_rdrctn_err(t_parser_state *state);
void			init_var_for_parse_redirection(t_redirection *redirection,
					t_parser_state *state);
void			init_vars_for_strdup_err(t_parser_state *state,
					t_redirection *redirection);
void			init_var_expctd_file_or_heredoc(t_parser_state *state,
					t_redirection *redirection);
void			free_command(t_command *command);
void			free_command_list(t_command *command_list);
void			*ft_memset(void *b, int c, size_t len);
void			close_prev_pipe_fd(int prev_pipe_fd[2]);
void			call_perror_exec_pipeline(t_command *command_list);
void			parent_cleanup(int prev_pipe_fd[2], int pipe_fd[2]);
void			call_perror_child_process_fd2(t_command *current_cmd);
void			call_perror_child_process_fd1(t_command *current_cmd);
void			execute_echo(char **args_array, t_shell_state *shell_state);
void			execute_exit(char **args_array, t_shell_state *shell_state);
void			execute_pwd(t_shell_state *shell_state);
void			execute_cd(char **args_array, t_shell_state *shell_state);
void			*ft_calloc(size_t nmemb, size_t size);
void			ft_bzero(void *s, size_t n);
void			init_sign(void);
void			process_input(char *input, t_shell_state *shell_state);
void			free_tokens(t_token_node *tokens);
void			free_token_list(t_token_node *tokens);
void			create_real_token(t_token_list *token_list);
void			restore_standard_fds(t_io_fds *fds);
void			free_args_array(char **args_array);
void			handle_sigint(int sig);
void			handle_sigquit(int sig);
void			handle_sigint_heredoc(int sig);
int				handle_child_redirections(t_command *command,
					t_io_fds *fds, t_shell_state *shell_state);
void			unset_env_var(t_env_var **env_list, const char *name);
void			builtin_env(t_shell_state *shell_state);
void			execute_single_command(t_command *all_cmds, t_command *command,
					t_shell_state *shell_state);
void			handle_export_command(t_env_var **env_list,
					const char *input, bool double_quote, bool single_quote);
void			execute_builtin(t_command *command, char **args_array,
					bool *args_quote_flags, t_shell_state *shell_state);
int				ft_atoi(const char *str);
int				skip_spaces(char *input, int i);
int				find_closing_quote(char *input, int i, char quote);
int				handle_token_error(t_token_state *state);
int				append_to_temp_token_str(t_token_state *state,
					char *temp_token);
int				parse_arguments_and_redirection(t_parser_state *state,
					t_command *command, t_shell_state *shell_state);
int				add_argument(t_command *command, char *arg_value,
					bool single_quote, bool double_quote);
int				parse_redirection(t_parser_state *state, t_command *command);
int				is_only_white_spaces(char *input);
int				prcs_nxt_tkn(char *input, t_token_list *token_list,
					t_shell_state *shell_state);
int				handle_dollar_token(char *input, t_token_list *token_list,
					t_shell_state *shell_state);
int				handle_operator_token(char *input, t_token_list *token_list);
int				is_redirection_token(t_token_type type);
int				create_operator_node(char *operator_str, int increment,
					t_token_list *token_list);
int				copy_literal_text(char **dest_ptr, const char *start,
					const char *end);
int				get_variable_name_length(const char *str);
int				get_variable_value_length(const char *var_name,
					int var_len, t_shell_state *shell_state);
int				hndl_out_redir(t_redirection *redirection,
					int flags);
int				handle_input_redirection(t_redirection *redirection);
void			handle_heredoc(t_redirection *redirection,
					char **heredoc_filename, t_shell_state *shell_state);
int				backup_fd(int old_fd, int *backup_fd, const char *error_msg);
int				handle_heredoc_redirection2(t_redirection *redirection,
					t_io_fds *fds, t_shell_state *shell_state);
int				process_single_redirection(t_redirection *redirection,
					t_io_fds *fds, t_shell_state *shell_state);
int				apply_dup2(int fd, int target_fd);
int				open_file(const char *filename, int flags, mode_t mode);
int				redirect_input(t_redirection *redirection,
					t_io_fds *fds);
int				redirect_output(t_redirection *redirection, t_io_fds *fds);
int				handle_heredoc_redirection(t_redirection *redirection,
					t_io_fds *fds, t_shell_state *shell_state);
int				apply_redirection(t_redirection *redirection,
					t_io_fds *fds, t_shell_state *shell_state);
int				handle_dollar_and_variable(t_parser_state *state,
					t_command *command, t_shell_state *shell_state);
int				process_quoted_token(char *input, char quote,
					t_token_list *token_list, t_shell_state *shell_state);
int				process_special_token(char *input, t_token_list *token_list,
					t_shell_state *shell_state);
int				is_operator(char c);
void			clean_filename(t_redirection *redirection);
int				handle_builtin_redirections(t_command *command,
					t_io_fds *fds, t_shell_state *shell_state);
int				is_builtin(char *cmd_name);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_ft_strlen(const char *s);
int				handle_builtins(char *input, char *cmd);
char			*ft_strcat(char *dest, const char *src);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strchr(const char *s, int c);
char			*ft_strndup(const char *s1, size_t n);
char			*ft_itoa(int n);
char			*ft_strjoin(const char *s1, const char *s2);
char			**convert_arglist_for_exc(t_command *command,
					bool **args_quote_flags_ptr);
char			*ft_strdup(const char *s);
char			**ft_split(const char *str, char delim);
char			*expand_variable_in_token(const char *token,
					t_var_expand *v_exp, t_shell_state *shell_state);
t_env_var		*find_env_var(t_env_var *env_list, const char *name);
t_token_type	determine_token_type(t_token_node *token_node);
t_token_node	*create_token_node(char *token_str, bool single_quote,
					bool double_quote);
t_token_node	*extract_token_str(char *input, int start, int end);
t_command		*parse_tokens(t_token_node *tokens, t_shell_state *shell_state);
t_command		*parse_command(t_parser_state *state,
					t_shell_state *shell_state);
t_token_node	*lexer(char *input, t_shell_state *shell_state);
int				expand_variable(char **dest_ptr, const char **cursor,
					t_shell_state *shell_state);
void			free_all(t_command *all_cmds, t_fork_info *finfo,
					t_shell_state *shell_state);
t_env_var		*find_local_env_var(t_env_var *env_list, const char *name);
char			*expand_var_in_heredoc(const char *input,
					t_shell_state *shell_state);
bool			update_exit_status(ssize_t bytes_read, size_t line_len);
bool			check_exit_condition(size_t line_len);
void			add_char_to_line(char buffer_char, char *line,
					size_t *line_len);
bool			is_newline_char(char buffer_char);
ssize_t			read_single_char(char *buffer);
void			read_and_expand_heredoc(const char *delimiter, int fd,
					bool is_quoted, t_shell_state *shell_state);
bool			read_input_line(char *line, size_t *line_len);
bool			handle_read_error(ssize_t bytes_read);
void			expand_and_write_line(const char *line, int fd,
					bool is_quoted, t_shell_state *shell_state);
bool			is_valid_var_name(const char *name);
void			setup_child_prcs(t_command *all_cmds, t_pipe_cmd *pcmd,
					t_shell_state *shell_state);
void			execute_pipeline_loop(t_command *current_cmd,
					int prev_pipe_fd[2], t_command *command_list,
					t_shell_state *shell_state);
void			execute_pipeline(t_command *command_list,
					t_shell_state *shell_state);
void			wait_for_all_children(t_shell_state *shell_state);
void			create_pipe_if_needed(t_command *current_cmd, int pipe_fd[2]);
void			init_pipeline(t_command **current_cmd,
					int prev_pipe_fd[2], t_command *command_list);
int				initialize_command_args(t_command *command, char ***args_array,
					bool **args_quote_flags);
void			handle_fork(t_command *all_cmds, t_fork_info *finfo,
					t_shell_state *shell_state);
int				prepare_arguments(t_command *command, char ***args_array,
					bool **args_quote_flags);
void			exec_in_child(t_command *all_cmds, t_fork_info *finfo,
					t_shell_state *shell_state);
void			handle_parent_after_fork(pid_t pid, t_shell_state *shell_state);

#endif
