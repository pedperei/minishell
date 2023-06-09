/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 20:16:42 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/26 10:53:29 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expansion_interrogation_mark(char *str_exp)
{
	char	*temp;
	char	*status;

	temp = str_exp;
	status = ft_itoa(g_check_exit[0]);
	str_exp = ft_strjoin(str_exp, status);
	free(temp);
	free(status);
	return (str_exp);
}

char	*expansion(char *input, char *str_exp, t_env **env, int *i)
{
	int		start;
	int		end;
	char	*search;
	t_env	*env_var;

	(*i)++;
	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_' || input[start] == '?')
	{
		(*i)++;
		if (input[start] == '?')
			return (expansion_interrogation_mark(str_exp));
	}
	end = *i;
	search = ft_substr(input, start, end - start);
	env_var = search_env_name(env, search);
	free(search);
	if (env_var)
	{
		search = str_exp;
		str_exp = ft_strjoin(str_exp, env_var->env_value);
		free(search);
	}
	return (str_exp);
}

void	free_expansion_aux(int *i, int *quotes_open, int *flag_type)
{
	free(i);
	free(quotes_open);
	free(flag_type);
}

void	exp_quote_update(char c, int *flag_type, int *quotes_open)
{
	if (quote_type(c))
	{
		if (*quotes_open == 0)
		{
			*flag_type = quote_type(c);
			*quotes_open = 1;
		}
		else if (*quotes_open == 1)
		{
			if (*flag_type == quote_type(c))
			{
				*quotes_open = 0;
				*flag_type = 0;
			}
		}
	}
}

char	*treat_expansion(char *input, t_env **env)
{
	int		*i;
	int		*quotes_open;
	int		*flag_type;
	char	*str_exp;
	char	*temp;

	quotes_open = ft_calloc(1, sizeof(int));
	flag_type = ft_calloc(1, sizeof(int));
	str_exp = ft_calloc(1, sizeof(char));
	i = ft_calloc(1, sizeof(int));
	while (input[*i])
	{
		exp_quote_update(input[*i], flag_type, quotes_open);
		if (input[*i] == '$' && *flag_type != 1)
			str_exp = expansion(input, str_exp, env, i);
		else
		{
			temp = str_exp;
			str_exp = ft_charjoin(str_exp, input[*i]);
			free(temp);
			(*i)++;
		}
	}
	free_expansion_aux(i, quotes_open, flag_type);
	return (str_exp);
}
