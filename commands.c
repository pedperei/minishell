/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:11:40 by joao-per          #+#    #+#             */
/*   Updated: 2023/05/01 17:25:19 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

void cd_command(char **av, t_env **env_vars)
{
	char	cwd[MAX_LINE];
	char	*oldwd;
	t_env 	*temp;

    if (av[1] == NULL)
	{
		chdir(getenv("HOME"));
	}
	else
	{
		if (chdir(av[1]) != 0)
		{
			perror("minishell");
			return;
		}
	}
	
	getcwd(cwd, sizeof(cwd));
	temp = search_env_name(env_vars, "PWD=");
	oldwd = ft_strdup(temp->env_var);
	free(temp->env_var);
	temp->env_var = ft_strjoin("PWD=", cwd);
	temp = search_env_name(env_vars, "OLDPWD=");
	free(temp->env_var);
	temp->env_var = ft_strjoin("OLD", oldwd);
	free(oldwd);
}

int	check_commands(char **av, t_env **child_env_vars)
{
	char	cwd[MAX_LINE];
	if (strcmp(av[0], "cd") == 0)
	{
		cd_command(av, child_env_vars);
	}
	if (strcmp(av[0], "pwd") == 0)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			perror("minishell");
		else
		{
			write(STDOUT_FILENO, cwd, ft_strlen(cwd));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (0);
	}
	if (strcmp(av[0], "export") == 0)
	{
		export_variable(child_env_vars, av[1]);
		return (0);
	}
	if (strcmp(av[0], "unset") == 0)
	{
		unset_variable(child_env_vars, av[1]);
		return (0);
	}
	if (strcmp(av[0], "echo") == 0)
	{
		echo_command(av);
		return (0);
	}
	return (check_commands2(av, child_env_vars));
}

int	check_commands2(char **av, t_env **child_env_vars)
{
	struct dirent	*entry;
	DIR				*dir;
	char			*dir_path;

	if (strcmp(av[0], "ls") == 0)
	{
		if (av[1] == NULL)
			dir_path = ".";
		else
			dir_path = av[1];
		if ((dir = opendir(dir_path)) == NULL)
			perror("minishell");
		else
		{
			while ((entry = readdir(dir)) != NULL)
			{
				write(1, entry->d_name, ft_strlen(entry->d_name));
				write(STDOUT_FILENO, "\n", 1);
			}
			closedir(dir);
		}
		return (0);
	}
	execute_external_command(av, child_env_vars);
	return (1);
}

void	export_variable(t_env **env_vars, const char *new_var)
{
	t_env	*new_env_var;
	t_env	*curr;

	// If no argument is provided, display all environment variables
	if (!new_var)
	{
		print_env_vars(env_vars);
		return ;
	}
	new_env_var = (t_env *)malloc(sizeof(t_env));
	if (!new_env_var)
	{
		perror("minishell: export: malloc");
		return ;
	}
	new_env_var->env_var = ft_strdup(new_var);
	new_env_var->next = NULL;
	if (*env_vars == NULL)
	{
		*env_vars = new_env_var;
		return ;
	}
	curr = *env_vars;
	while (curr->next)
		curr = curr->next;
	curr->next = new_env_var;
	//free(new_env_var);
}

void	unset_variable(t_env **env_vars, const char *var_name)
{
	t_env	*curr;
	t_env	*prev;
	char	*equal_pos;

	curr = *env_vars;
	prev = NULL;
	if (!var_name)
	{
		printf("minishell: unset: Invalid format. Usage: unset KEY\n");
		return ;
	}
	while (curr)
	{
		equal_pos = ft_strchr(curr->env_var, '=');
		if (equal_pos && strncmp(curr->env_var, var_name,
				equal_pos - curr->env_var) == 0
			&& ft_strlen(var_name) == (size_t)(equal_pos - curr->env_var))
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_vars = curr->next;

			free(curr->env_var);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	printf("minishell: unset: Variable not found: %s\n", var_name);
}

void	echo_command(char **av)
{
	int	new_line;
	int	i;

	new_line = 1;
	i = 1;
	if (av[i] && strcmp(av[i], "-n") == 0)
	{
		new_line = 0;
		i++;
	}
	while (av[i])
	{
		write(STDOUT_FILENO, av[i], ft_strlen(av[i]));
		if (av[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (new_line)
		write(STDOUT_FILENO, "\n", 1);
}
