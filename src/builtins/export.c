/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaria <rafaria@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:28:32 by raphox            #+#    #+#             */
/*   Updated: 2025/01/22 21:56:39 by rafaria          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_export_before_equal(char *args)
{
	int	i;
	int	j;
	int	found;

	i = 0;
	j = 0;
	found = 0;
	while (args[i] != '\0')
	{
		if (args[i] == '=')
		{
			found++;
			break ;
		}
		if ((args[0] >= '0' && args[0] <= '9') || (args[i] == '!'
				|| args[i] == '$' || args[i] == ' '
				|| args[i] == '#' || args[i] == '-'))
			return (write(2, "export: not a valid identifier\n", 31), -1);
		i++;
	}
	if ((i == 0 && found == 1))
		return (write(2, "export: not a valid identifier\n", 31), -1);
	if (found == 0)
		return (write_temp_file(TMP_FILES, 0), 0);
	return (0);
}

int	check_export(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		if (check_export_before_equal(args[i]) == -1)
		{
			write_temp_file(TMP_FILES, 1);
			return (-1);
		}
		i++;
	}
	return (0);
}

char	**export(char *command, char **arguments, char **envv)
{
	int		i;
	char	**result;
	char	**new_result;

	i = 0;
	result = envv;
	if (arguments != NULL && check_export(arguments) == -1)
	{
		return (result);
	}
	if (command != NULL && arguments == NULL)
		bubble_sort(result);
	else
	{
		while (arguments[i] != NULL)
		{
			new_result = cmd_export(arguments[i++], result);
			result = new_result;
		}
	}
	return (result);
}

// Fonction cmd_export qui ajoute un nouvel argument à l'environnement
char	**cmd_export(char *argument, char **env)
{
	int		check;
	char	**new_env;

	check = 0;
	while (env[check] != NULL)
	{
		if ((ft_strncmp(env[check], argument, ft_strlen(env[check])) == 0))
			return (env);
		check++;
	}
	if (check_var(argument) == 1)
		return (env);
	new_env = allocate_new_env_to_add_variable(env, argument);
	if (new_env == NULL)
		return (NULL);
	free_env(env);
	return (new_env);
}

// allouer nouvel environnement
char	**allocate_new_env_to_add_variable(char **env, char *arguments)
{
	int		i;
	int		size;
	char	**new_env;

	i = 0;
	size = 0;
	while (env[size] != NULL)
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 2));
	if (new_env == NULL)
	{
		return (NULL);
	}
	while (env[i] != NULL)
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = ft_strdup(arguments);
	new_env[++i] = NULL;
	return (new_env);
}
