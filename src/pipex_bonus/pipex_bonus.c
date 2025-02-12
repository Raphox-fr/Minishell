/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaria <rafaria@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:34:07 by rafaria           #+#    #+#             */
/*   Updated: 2025/02/05 16:43:00 by rafaria          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(t_data_rule *struct_data, t_data_rule data,
		t_struc_process struct_exec)
{
	char	*pathname;
	int		k;

	k = 0;
	while (k < struct_exec.num_commands)
	{
		if (struct_exec.tab_heredoc[k] != -1
			&& struct_exec.tab_heredoc[k] != struct_exec.fd_heredoc)
			close(struct_exec.tab_heredoc[k]);
		k++;
	}
	free(struct_exec.tab_heredoc);
	(close(struct_exec.p_fd[0]), close(struct_exec.p_fd[1]));
	check_redirections_in_cmd(struct_data, data, struct_exec);
	check_cmd_in_cmd(&pathname, struct_data, data, &struct_exec);
	if ((check_if_in_builtins(data) == 0) && execve(pathname, struct_exec.cmd,
			struct_exec.env) == -1)
	{
		display_error(data.command, NULL, data.arguments, 1);
		if (pathname != NULL)
			free(pathname);
		free_execution_process(struct_data, struct_exec);
	}
	exit(EXIT_FAILURE);
}

void	execution_process(t_data_rule *struct_data, t_data_rule data,
		t_struc_process struct_exec)
{
	if (data.oper != NULL && data.oper[0] == 'h')
	{
		dup2(struct_exec.fd_heredoc, STDIN_FILENO);
		close(struct_exec.fd_heredoc);
		if (struct_exec.input_fd != NULL && *struct_exec.input_fd != -1)
		{
			close(*struct_exec.input_fd);
		}
	}
	else if (struct_exec.input_fd != NULL && *struct_exec.input_fd != -1)
	{
		dup2(*struct_exec.input_fd, STDIN_FILENO);
		close(*struct_exec.input_fd);
	}
	if (struct_exec.is_last_command != 1)
	{
		if (struct_exec.fd_heredoc != -1)
			close(struct_exec.fd_heredoc);
		close(struct_exec.p_fd[0]);
		dup2(struct_exec.p_fd[1], STDOUT_FILENO);
		close(struct_exec.p_fd[1]);
	}
	execute(struct_data, data, struct_exec);
}

void	second_process(t_struc_process struct_exec)
{
	if (struct_exec.input_fd != NULL && *struct_exec.input_fd != -1)
		close(*struct_exec.input_fd);
	if (struct_exec.is_last_command != 1)
	{
		if (struct_exec.fd_heredoc != -1)
		{
			close(struct_exec.fd_heredoc);
		}
		close(struct_exec.p_fd[1]);
		*struct_exec.input_fd = struct_exec.p_fd[0];
	}
	else
	{
		if (struct_exec.fd_heredoc != -1)
		{
			close(struct_exec.fd_heredoc);
		}
		close(struct_exec.p_fd[0]);
		close(struct_exec.p_fd[1]);
	}
}

void	do_pipe(t_data_rule *struct_data, t_data_rule data,
		t_struc_process struct_exec)
{
	int	p_fd[2];

	g_pid = 0;
	struct_exec.p_fd = p_fd;
	if (pipe(p_fd) == -1)
	{
		exit(1);
	}
	g_pid = fork();
	if (g_pid == -1)
	{
		exit(1);
	}
	if (g_pid == 0)
	{
		execution_process(struct_data, data, struct_exec);
	}
	else
	{
		second_process(struct_exec);
	}
}

char	**pipex(t_info **info)
{
	int				i;
	int				input_fd;
	int				*tab_heredoc;
	t_struc_process	struct_exec;

	i = 0;
	input_fd = -1;
	tab_heredoc = prepare_heredocs(info);
	struct_exec.env = (*info)->envv;
	if (tab_heredoc == NULL || (*info)->cmd->nb_command == 0)
		return (struct_exec.env);
	struct_exec.cmd = NULL;
	struct_exec.input_fd = &input_fd;
	struct_exec.fd_heredoc = tab_heredoc[i];
	struct_exec.tab_heredoc = tab_heredoc;
	struct_exec.num_commands = (*info)->cmd->nb_command;
	if (tchoupi((*info)->cmd, &struct_exec, (*info)->err) == -1)
		return (free(struct_exec.tab_heredoc), struct_exec.env);
	wait_for_children();
	g_pid = 0;
	return ((*info)->err->exit_value = ask_tmp_files(),
		free(struct_exec.tab_heredoc),
		struct_exec.env);
}
