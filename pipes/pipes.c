/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:03:57 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/02 11:05:06 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

void	execute_command(t_shell *shell, t_env **env_vars)
{
	int		pipe_index;
	int		in_fd;
	char	**av;
	int		i;

	av = shell->args_str;
	in_fd = 0;
	i = 0;
	while ((pipe_index = find_pipe(shell, i)) != -1)
	{
		handle_pipe(shell, env_vars, &in_fd, pipe_index);
		i += pipe_index + 1;
	}
	run_commands_aux(shell, env_vars, in_fd, STDOUT_FILENO);
	if (in_fd != 0)
		close(in_fd);
	shell->args_str = av;
}

void	execute_absolute_path(t_shell *shell)
{
	execve(shell->args_str[0], shell->args_str, shell->envs_str);
	free_args(shell, shell->len_args);
	free(shell);
	perror("minishell");
	exit(1);
}

void	execute_relative_path(t_shell *shell, t_env **env_vars)
{
	char	*path_var;
	char	**path_dirs;

	path_var = get_env_value("PATH", env_vars);
	path_dirs = ft_split(path_var, ':');
	free(path_var);

	try_execve_at_each_path(shell, path_dirs);

	free_double_array(path_dirs);
	perror("minishell");
	exit(1);
}


void	execute_external_command(t_shell *shell, t_env **env_vars)
{
	if (ft_strchr(shell->args_str[0], '/'))
		execute_absolute_path(shell);
	else
		execute_relative_path(shell, env_vars);
}