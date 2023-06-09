/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_aux2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 10:54:13 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/26 10:54:13 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

void	handle_child_process(t_shell *shell, int in_fd, int *pipe_fd,
		pid_t pid)
{
	int	built_in_command_executed;

	if (shell->current_cmd != 0)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (shell->current_cmd < shell->cmds - 1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (!pipe_fd)
		file_descriptor_handler(in_fd, STDOUT_FILENO);
	handle_redirection(shell);
	built_in_command_executed = is_builtin_command(shell);
	check_commands(shell, pid);
	if (built_in_command_executed || !shell->args_execve[0])
		exit(0);
	execute_external_command(shell);
	perror("execve");
	exit(1);
}

void	execute_absolute_path(t_shell *shell)
{
	execve(shell->args_str[0], shell->args_execve, shell->envs_str);
	free_args(shell, shell->len_args);
	free(shell);
	print_error_messages('F', shell->args_execve[0]);
	exit(127);
}

char	*construct_full_path(char *path_dir, char *command)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(path_dir, "/");
	full_path = ft_strjoin(temp, command);
	free(temp);
	return (full_path);
}

void	execute_relative_path(t_shell *shell)
{
	char	*path_var;
	char	**path_dirs;

	path_var = get_env_value("PATH", shell->envs);
	if (!path_var)
		path_dirs = 0;
	else
		path_dirs = ft_split(path_var, ':');
	free(path_var);
	try_execve_at_each_path(shell, path_dirs);
	free_double_array(path_dirs);
	print_error_messages('F', shell->args_execve[0]);
	exit(127);
}

void	execute_external_command(t_shell *shell)
{
	if (ft_strncmp(shell->args_str[0], "/", 1) == 0)
		execute_absolute_path(shell);
	else
		execute_relative_path(shell);
}
