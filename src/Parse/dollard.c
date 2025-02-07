/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollard.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaria <rafaria@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 12:10:34 by thodos-s          #+#    #+#             */
/*   Updated: 2025/02/07 15:41:04 by rafaria          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	add the last exit code	*/
int	dollard_interrogation(char **str, t_erreur *err)
{
	char	*tmp;
	char	*exit_value;

	tmp = NULL;
	(void)err;
	exit_value = ft_itoa(err->exit_value);
	tmp = ft_strjoin(*str, exit_value, 0);
	free(*str);
	free(exit_value);
	if (!tmp)
		return (0);
	*str = tmp;
	return (1);
}

/*	add an environment variable	*/
int	add_dollar(char *rqst, int *i, char **str, t_info **data)
{
	int	ty_dlr;

	ty_dlr = dollard_exist(rqst, data, i);
	if (ty_dlr == 2)
	{
		(*i) += 2;
		return (dollard_interrogation(str, (*data)->err));
	}
	else if (ty_dlr == 1)
		dollard_env(rqst, i, str, data);
	else
	{
		(*i)++;
		if (rqst[*i] && (ft_isdigit(rqst[*i])))
		{
			(*i)++;
			return (1);
		}
		while (rqst[*i] && (ft_isalpha(rqst[*i]) || rqst[*i] == '_'))
			(*i)++;
		while (rqst[*i] && ft_isdigit(rqst[*i]))
			(*i)++;
	}
	return (1);
}

/*	add a character	*/
int	add_char(char *c, char **str, int *itr, t_info **data)
{
	char	buff[2];
	char	*tmp;
	int		i;
	bool	bs;

	i = 0;
	bs = false;
	if (c[0] == '\\' && !(*data)->sq)
	{
		(*itr)++;
		bs = true;
		c++;
	}
	if (c[0] == '$' && !bs && !(*data)->sq && dollard_exist(c, data, &i))
		return (1);
	buff[0] = *c;
	buff[1] = '\0';
	tmp = ft_strjoin(*str, buff, 0);
	free(*str);
	if (!tmp)
		return (0);
	*str = tmp;
	(*itr)++;
	return (1);
}

static int	fuck_norm(t_info **data, char **str, char **rqst)
{
	int		i;
	bool	dq;

	(*data)->sq = false;
	dq = false;
	i = 0;
	while ((*rqst)[i])
	{
		quote_id(&(*data)->sq, &dq, NULL, (*rqst)[i]);
		if ((*rqst)[i] && (*rqst)[i + 1] && (*rqst)[i] == '$' && \
			((*rqst)[i + 1] != '\'' && (*rqst)[i + 1] != '"') && \
			(ft_isalnum((*rqst)[i + 1]) || (*rqst)[i + 1] == '?' || \
			(*rqst)[i + 1] == '_')
			&& !back_slash((*rqst), i) && !(*data)->sq && \
			!add_dollar((*rqst), &i, str, data))
			return (0);
		else if ((*rqst)[i] && !add_char((*rqst) + i, str, &i, data))
			return (0);
	}
	return (1);
}

/*	set and add environment variable in buffer	*/
int	dollar_traitment(t_info **data, char **rqst)
{
	char	*str;

	str = ft_strdup("");
	if (fuck_norm(data, &str, rqst) == 0)
		return (0);
	free(*rqst);
	(*rqst) = NULL;
	if (ft_strlen(str) <= 0)
	{
		free(str);
		return (0);
	}
	else
		*rqst = str;
	return (1);
}
