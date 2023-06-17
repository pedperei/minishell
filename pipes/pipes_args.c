#include "../Libft/libft.h"
#include "../minishell.h"

int	count_commands(t_shell *shell)
{
	int count;
	t_arg *arg;

	count = 1;
	arg = *shell->args;
	while (arg)
	{
		if (ft_strcmp(arg->name, "|") == 0 && !arg->quotes_perm)
			count++;
		arg = arg->next;
	}
	return (count);
}

int	count_arguments(t_shell *shell)
{
	int count_args;
	int flag_red;
	t_arg *arg;

	count_args = 0;
	flag_red = 0;
	arg = get_arg_byindex(shell, shell->index);
	while (arg && !(ft_strcmp(arg->name, "|") == 0 && !arg->quotes_perm))
	{
		if (flag_red)
			flag_red = 0;
		else if (is_reds(arg->name) && !arg->quotes_perm)
			flag_red = 1;
		else
			count_args++;
		arg = arg->next;
	}
	return (count_args);
}

char	**create_args_execve(t_shell *shell)
{
	char	**args_execve;
	int		count_args;
	int		flag_red;
	int		i;
	t_arg	*arg;

	count_args = count_arguments(shell);
	args_execve = ft_calloc(count_args + 1, sizeof(char *));
	arg = get_arg_byindex(shell, shell->index);
	if (!args_execve)
		return (0);
	flag_red = 0;
	i = 0;
	while (arg && !(!ft_strcmp(arg->name, "|") && !arg->quotes_perm))
	{
		if (flag_red)
			flag_red = 0;
		else if (is_reds(arg->name) && !arg->quotes_perm)
			flag_red = 1;
		else
			args_execve[i++] = ft_strdup(arg->name);
		arg = arg->next;
	}
	args_execve[i] = 0;
	return (args_execve);
}
