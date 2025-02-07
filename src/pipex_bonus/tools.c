/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaria <rafaria@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 15:38:34 by rafaria           #+#    #+#             */
/*   Updated: 2025/02/07 15:41:41 by rafaria          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection(t_data_rule data)
{
	int	i;

	i = 0;
	if (data.oper != NULL)
		i = 0;
	else
		return (0);
	while (data.out[i] != NULL)
	{
		if (data.oper[i] == '<' && handle_entry_redirections(data, data.oper[i],
				data.input) == -1)
		{
			return (-1);
		}
		if ((data.oper[i] == '>' || data.oper[i] == 'r')
			&& handle_exit_redirections(data, data.oper[i], data.out[i]) == -1)
		{
			return (-1);
		}
		i++;
	}
	return (0);
}

int	handle_sig_heredoc(void)
{
	if (ask_tmp_files() == 1024)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

int	handle_heredoc(t_info **info, char *delimiter)
{
	char	*line;
	int		pipe_fds[2];

	(void)info;
	if (pipe(pipe_fds) == -1)
		return (perror("Erreur pipe"), -1);
	write_temp_file(TMP_FILES, 1023);
	rl_event_hook = handle_sig_heredoc;
	while (1)
	{
		line = readline("> ");
		if (ask_tmp_files() == 1024)
			return (close(pipe_fds[0]), close(pipe_fds[1]),
				rl_event_hook = NULL, -1);
		if (line == NULL)
			return (free(line), ctrl_d(line, delimiter), close(pipe_fds[1]), pipe_fds[0]);
		dollar_traitment(info, &line);
		if (line)
			if (line && ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
				return (free(line), close(pipe_fds[1]), rl_event_hook = NULL,
					pipe_fds[0]);
		if (line)
			write(pipe_fds[1], line, ft_strlen(line));
		write(pipe_fds[1], "\n", 1);
		free(line);
	}
}

void	ctrl_d(char *line, char *delimiter)
{
	write(1, "bash: warning: here-document delimited ", 39);
	write(1, "by end-of-file (wanted ", 23);
	ft_putstr_fd(delimiter, 1);
	write(1, ")\n", 2);
	write_temp_file(TMP_FILES, 0);
	free(line);
}

void	wait_for_children(void)
{
	int	status;
	int	exit_code;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status) != 0)
		{
			exit_code = WEXITSTATUS(status);
			write_temp_file(TMP_FILES, exit_code);
		}
	}
}

// int	handle_heredoc(t_data_rule *data, char *delimiter, char **envv)
// {
// 	char	*line;
// 	int		pipe_fds[2];

// 	if (pipe(pipe_fds) == -1)
// 	{
// 		perror("Erreur lors de la création du pipe");
// 		return (-1);
// 	}
// 	while (1)
// 	{
// 		if (ask_tmp_files(TMP_FILES) == 90)
// 		{
// 			is_tmpfile_90(pipe_fds[0], pipe_fds[1]);
// 			return (-1);
// 		}
// 		line = readline("Heredoc> ");
// 		if (line == NULL)
// 		{
// 			ctrl_d_heredoc(line, delimiter);
// 			break ;
// 		}
// 		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(pipe_fds[1], line, ft_strlen(line));
// 		write(pipe_fds[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pipe_fds[1]);
// 	return (pipe_fds[0]);
// }

// void (*old_sigint)(int);
// void (*old_sigquit)(int);

// old_sigint = signal(SIGINT, handle_sigint_heredoc);
// old_sigquit = signal(SIGQUIT, handle_sigint_heredoc);

// signal(SIGINT, old_sigint);
// signal(SIGQUIT, old_sigquit);

// volatile sig_atomic_t g_heredoc_interrupted = 0;

// void handle_sigint_heredoc(int sig)
// {
//     (void)sig;
//    	write_temp_file(TMP_FILES, 90);
//     write(1, "\n", 1);         // Ajoute un saut de ligne propre
// }

// int handle_heredoc(t_data_rule *data, char *delimiter, char **envv)
// {
//     char *line;
//     int pipe_fds[2];
//     pid_t pid;

//     if (pipe(pipe_fds) == -1)
//     {
//         perror("Pipe creation failed");
//         return (-1);
//     }

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("Fork failed");
//         return (-1);
//     }

//     if (pid == 0)
//     {
//         while (1)
//         {
//             line = readline("HEREDOC> ");
//             if (!line)
//             {
//                 write(1, "CTRL D PRESSED\n", 15);
// 					close(pipe_fds[1]);
// 					close(pipe_fds[0]);
// 				break ;
//             }
//             if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
//&& line[ft_strlen(delimiter)] == '\0')
//             {
//                 free(line);
//                 break ;
//             }
//             write(pipe_fds[1], line, ft_strlen(line));
//             write(pipe_fds[1], "\n", 1);
//             free(line);
//         }
//         close(pipe_fds[1]);
//         close(pipe_fds[0]);
//         killer_request(data);
//         free_env(envv);
//         exit(EXIT_SUCCESS);
//     }
//     else
//     {
//         close(pipe_fds[1]);
//         waitpid(pid, NULL, 0);
//     }
//     close(pipe_fds[0]);
//     return (0);
// }
