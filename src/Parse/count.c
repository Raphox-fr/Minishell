/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thodos-s <thodos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:24:07 by thodos-s          #+#    #+#             */
/*   Updated: 2024/12/04 12:24:30 by thodos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parsing.h"

/*	count occurence redirection symbol	*/
int	occ_rdir(char *buff)
{
	int	i;

	i = 0;
	while (buff[i] == '>' || buff[i] == '<')
		i++;
	return (i);
}

/*	identify elem	*/
int	state_in_view(const char *command, const int i)
{
	if (is_oper(command[i]))
		return (OPER);
	if (is_rdir(command[i]))
		return (RDIR);
	return (SPACES);
}

/*	set token to count the len word	*/
int	len_of_word(char *command, int i)
{
	t_token	token;

	token.len_word = 0;
	token.nb_word = 0;
	token.state = RESET;
	token.sep = ' ';
	return (r_value(command, i, &token));
}

/*	set token to count how much word	*/
int	nb_words(char *command)
{
	t_token	token;

	token.len_word = -1;
	token.nb_word = 0;
	token.state = RESET;
	token.sep = ' ';
	return (r_value(command, 0, &token));
}

/*	set how mush command	*/
int	nb_command(t_split *split, int count_word)
{
	int	count;
	int	k;

	count = 1;
	k = 0;
	if (!split || count_word == 0)
		return (0);
	while (k < count_word)
	{
		if ((ft_strncmp(split[k].word, "|", split[k].len_word) == 0)
			|| (ft_strncmp(split[k].word, ";", split[k].len_word) == 0))
			count++;
		k++;
	}
	return (count);
}
