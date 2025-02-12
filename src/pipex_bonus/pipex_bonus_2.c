/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaria <rafaria@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 02:57:57 by rafaria           #+#    #+#             */
/*   Updated: 2025/02/05 17:15:01 by rafaria          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	truc(t_data_rule *data, int i, t_struc_process *struct_exec,
		t_erreur *err)
{
	if (check_exit(data, data[i], *struct_exec, err) == -1)
		return ;
	else if (check_exit(data, data[i], *struct_exec, err) == -2)
		struct_exec->env = NULL;
	else if (struct_exec->num_commands == 1)
		struct_exec->env = exec_builtins(data[i], struct_exec->env);
	err->exit_value = ask_tmp_files();
}

int	tchoupi(t_data_rule *data, t_struc_process *struct_exec, t_erreur *err)
{
	int	i;

	i = 0;
	while (i < struct_exec->num_commands)
	{
		if (i == struct_exec->num_commands - 1)
			struct_exec->is_last_command = 1;
		else
			struct_exec->is_last_command = 0;
		struct_exec->fd_heredoc = struct_exec->tab_heredoc[i];
		if (check_if_in_builtins(data[i]) == -1
			|| check_if_in_builtins(data[i]) == 42)
		{
			truc(data, i, struct_exec, err);
			if (err->exit_value > 0)
				return (-1);
		}
		do_pipe(data, data[i], *struct_exec);
		i++;
	}
	return (0);
}

void	innit_tab_heredoc(int *tab_heredoc, int num_commands)
{
	int	pq;

	pq = 0;
	while (pq < num_commands)
	{
		tab_heredoc[pq] = 0;
		pq++;
	}
}

int	*prepare_heredocs(t_info **info)
{
	int	*tab_heredoc;
	int	i;

	tab_heredoc = malloc(sizeof(int) * (*info)->cmd->nb_command + 1);
	if (tab_heredoc == NULL)
		return (NULL);
	(innit_tab_heredoc(tab_heredoc, (*info)->cmd->nb_command), i = 0);
	while (i < (*info)->cmd->nb_command)
	{
		if ((*info)->cmd[i].oper != NULL
			&& (check_if_h_is_find((*info)->cmd[i].oper) != 0))
		{
			tab_heredoc[i] = give_latest_heredoc(info, (*info)->cmd[i]);
			if (tab_heredoc[i] == -1)
				return (free_heredoc(tab_heredoc, (*info)->cmd->nb_command),
					NULL);
		}
		else
			tab_heredoc[i] = -1;
		i++;
	}
	return (tab_heredoc);
}
