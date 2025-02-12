/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raphox <raphox@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:28:32 by raphox            #+#    #+#             */
/*   Updated: 2025/01/27 13:03:47 by raphox           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (check_if_in_envv(argument, env) != -1)
	{
		check = check_if_in_envv(argument, env);
		new_env = allocate_new_env_to_modify_variable(check, env, argument);
	}
	else
		new_env = allocate_new_env_to_add_variable(env, argument);
	if (new_env == NULL)
		return (NULL);
	free_env(env);
	return (new_env);
}

// alouer nouvel env + modif variable existante
char	**allocate_new_env_to_modify_variable(int pin, char **env,
		char *arguments)
{
	int		i;
	int		size;
	char	**new_env;

	i = 0;
	size = 0;
	while (env[size] != NULL)
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	if (new_env == NULL)
		return (NULL);
	while (env[i] != NULL)
	{
		if (i == pin)
			new_env[i] = ft_strdup(arguments);
		else
			new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

// allouer nouvel environnement + nouvel varaible
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
