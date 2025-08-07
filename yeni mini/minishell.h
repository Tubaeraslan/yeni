/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:48:59 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 14:21:00 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h> 
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>

extern int	g_signal_status;

typedef struct s_mem_block
{
	void				*data;
	size_t				size;
	struct s_mem_block	**head;
	struct s_mem_block	*next;
}	t_mem_block;

typedef struct s_tokenizer
{
	int			i;
	int			j;
	int			inside_quotes;
	char		char_quote;
	char		buffer[1024];
	const char	*src;
}	t_tokenizer;

typedef struct s_heredoc
{
	char				*limiter;
	int					index;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_expand
{
	int		i;
	int		j;
	int		single_quote;
	int		double_quote;
	char	*input;
	char	*new_input;
}	t_expand;

typedef struct s_data
{
	char	**env;
	char	**export_list;
}	t_data;

typedef struct s_mini
{
	char			*input;
	char			*cmd;
	char			**args;
	char			**tokens;
	int				parsing_error;
	int				error_printed;
	int				last_exit_code;
	int				heredoc_fd;
	t_heredoc		*heredocs;
	t_data			*data;
	int				is_heredoc;
	t_tokenizer		tokenizer;
	int				token_count;
	int				is_pipe;
	char			*infile;
	char			*outfile;
	char			*heredoc_limiter;
	int				is_append;
	int				in_fd;
	int				out_fd;
	pid_t			*pids;
	struct s_mini	*next;
}	t_mini;

void		*mem_malloc(size_t size);
void		*mem_calloc(size_t type, size_t size);
void		*mem_realloc(void *old_data, size_t new_size);
void		mem_free(void);
void		*mem_absorb(void *addr);
t_mem_block	*mem_add_new_block(void *data, size_t size);
void		mem_clear_block(t_mem_block **head);
size_t		mem_find_size(void *data);
void		*mem_exit(void);
void		parse_input(t_mini *mini);
int			syntax_error(t_mini *mini);
int			check_pipe(char *input);
void		parse_error(t_mini *mini, const char *msg);
void		expand_variables(t_mini *mini);
void		expand_pid(t_expand *ex);
void		init_minishell(t_mini *mini, t_data *data, char **envp);
void		reset_mini_for_new_command(t_mini *mini);
int			read_varname(char *input, int *i, char *varname);
void		copy_env_value_to_input(t_expand *ex, char *value);
char		*get_env_value(char **env, char *key);
void		check_heredoc_and_setup(t_mini *mini);
void		add_heredoc_node(t_heredoc **h, t_heredoc **l, char *input, int *i);
char		*extract_limiter(char *input, int i);
void		process_heredoc_list(t_mini *mini, t_heredoc *heredocs);
void		heredoc_loop_custom(char *limiter, int write_fd);
void		ft_exit_gc(int status);
void		token(t_mini *mini);
int			process_quotes(t_tokenizer *tk);
void		process_space(t_mini *mini, t_tokenizer *tk);
int			process_operator(t_mini *mini, t_tokenizer *tk);
void		add_token(t_mini *mini, char *buffer);
void		parsing(t_mini *mini);
int			handle_infile(t_mini *mini, char **tokens, int *i);
int			handle_outfile(t_mini *mini, char **tokens, int *i, int append);
int			handle_heredoc(t_mini *mini, char **tokens, int *i);
t_mini		*create_next_command(t_mini *mini, char **tokens, int index);
int			check_heredoc_conditions(t_mini *mini);
void		handle_fork_error(t_mini *mini);
void		execute_commands(t_mini *mini);
void		execute_a_command(t_mini *mini);
int			execute_many_commands(t_mini *mini);
void		run_child(t_mini *mini);
char		*path_finder(char *cmd, char **env);
void		check_executable(char *path, struct stat *st, t_mini *mini);
void		exec_command(t_mini *mini, char *path);
int			is_built(char *arg);
void		execute_builtin(t_mini *mini);
void		execute_b(t_mini *mini);
int			handle_redirections(t_mini *mini);
void		ft_cd(t_mini *mini);
int			get_oldpwd(t_mini *mini, char *cwd, size_t c_size, char **oldpwd);
int			get_path(t_mini *mini, char **path);
void		update_env(char ***envp, char *arg);
void		ft_pwd(void);
void		ft_env(t_mini *mini);
void		ft_echo(t_mini *mini);
void		ft_export(t_mini *mini);
void		print_declare_line(char *env);
int			is_valid(char *arg);
void		update_export(t_mini *mini, char *var);
int			find_in_env(char **env, char *var);
char		**sort_env(char **env);
char		**ft_env_dup(char **envp);
int			env_len(char **env);
void		ft_unset(t_mini *mini);
int			ft_exit(t_mini *mini);
void		exit_control(t_mini	*mini);
void		exit_control2(t_mini	*mini);
void		exit_program(t_mini *mini, int exit_code);
int			numeric_control(char *arg);
int			validate_all_commands(t_mini *mini);
int			count_commands(t_mini *mini);
int			execute_pipeline(t_mini *mini, pid_t *pids);
pid_t		handle_fork(t_mini *mini, int prev_fd, int *fd);
void		print_err_and_exit(char *path, char *msg, int code, t_mini *mini);
void		execute_commands_pipe(char *path, t_mini *mini);
void		setup_stdin(t_mini *mini, int prev_fd);
void		setup_stdout(t_mini *mini, int *fd);
void		set_signal(int i);
void		sigint_handler(int sig);
void		handle_signals(void);
int			pipe_fork_step(t_mini *mini, pid_t *pids, int *i, int *prev_fd);
void		ft_close_fds(t_mini *mini);
int			parse_tokens_and_fill_args(t_mini *mini, char **tmp_args);
int			handle_tokens(t_mini *mini, char **tmp_args, int *count, int *i);
void		closer(int prev_fd);
#endif