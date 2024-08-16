/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:58:01 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/16 17:04:45 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	count_redirects(t_main *mini)
{
	int	i;

	i = 0;
	mini->ac = 0;
	mini->oc = 0;
	mini->hc = 0;
	mini->ic = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == INPUT)
			mini->ic++;
		else if (mini->tokenized[i] == HEREDOC)
			mini->hc++;
		else if (mini->tokenized[i] == OUTPUT)
			mini->oc++;
		else if (mini->tokenized[i] == APPEND)
			mini->ac++;
		i++;
	}
	mini->ac = mini->ac / 2;
	mini->hc = mini->hc / 2;
}

void	allocate_for_redirects(t_main *mini)
{
	mini->append = malloc(sizeof(char *) * (mini->ac + 1));
	mini->output = malloc(sizeof(char *) * (mini->oc + 1));
	mini->heredoc = malloc(sizeof(char *) * (mini->hc + 1));
	mini->meta_input = malloc(sizeof(char *) * (mini->ic + 1));
	mini->append[mini->ac] = NULL;
	mini->output[mini->oc] = NULL;
	mini->heredoc[mini->hc] = NULL;
	mini->meta_input[mini->ic] = NULL;
	if (!mini->append || !mini->heredoc || !mini->input || !mini->output)
		return ; //malloc hatası
}

int	check_char(char c)
{
	if (c == CHAR || c == DOLLARINDBL || c == DOLLARINSGL
		|| c == DOUBLEQUOTE || c == SINGLEQUOTE)
		return (1);
	return (0);
}

void	if_append(t_main *mini, int index, int temp, int j)
{
	mini->append[mini->ac - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
	{
		mini->append[mini->ac - 1][j] = mini->input[index];
		j++;
		index++;
	}
	mini->append[mini->ac - 1][j] = '\0';
}

void	if_heredoc(t_main *mini, int index, int temp, int j)
{
	mini->heredoc[mini->hc - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
	{
		mini->heredoc[mini->hc - 1][j] = mini->input[index];
		j++;
		index++;
	}
	mini->heredoc[mini->hc - 1][j] = '\0';
}

void if_output(t_main *mini, int index, int temp, int j)
{
	mini->output[mini->oc - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
	{
		mini->output[mini->oc - 1][j] = mini->input[index];
		j++;
		index++;
	}
	mini->output[mini->oc - 1][j] = '\0';
}

void	if_input(t_main *mini, int index, int temp, int j)
{
	mini->meta_input[mini->ic - 1] = malloc(sizeof(char) * (index - temp) + 1);
	index = temp;
	while (check_char(mini->tokenized[index]) && mini->tokenized[index])
	{
		mini->meta_input[mini->ic - 1][j] = mini->input[index];
		j++;
		index++;
	}
	mini->meta_input[mini->ic - 1][j] = '\0';
}

void	fill_red(t_main *mini, int index, int status)
{
	int	temp;
	int	j;

	temp = 0;
	j = 0;
	while (mini->input[index] == ' ')
		index++;
	temp = index;
	while (check_char(mini->tokenized[index] && mini->tokenized[index]))
		index++;
	if (status == APPEND)
		if_append(mini, index, temp, 0);
	else if (status == HEREDOC)
		if_heredoc(mini, index, temp, 0);
	else if (status == OUTPUT)
		if_output(mini, index, temp, 0);
	else
		if_input(mini, index, temp, 0);
}

void	take_redirects(t_main *mini)
{
	int	i;

	i = 0;
	count_redirects(mini);
	allocate_for_redirects(mini);
	mini->ac = 0;
	mini->hc = 0;
	mini->oc = 0;
	mini->ic = 0;
	while (mini->tokenized[i])
	{
		if (mini->tokenized[i] == APPEND)
		{
			i = i + 2;
			mini->ac++;
			fill_red(mini, i, APPEND);
		}
		else if (mini->tokenized[i] == HEREDOC)
		{
			i = i + 2;
			mini->hc++;
			fill_red(mini, i, HEREDOC);
		}
		else if (mini->tokenized[i] == OUTPUT)
		{
			mini->oc++;
			i += 1;
			fill_red(mini, i, OUTPUT);
		}
		else if (mini->tokenized[i] == INPUT)
		{
			mini->ic++;
			i += 1;
			fill_red(mini, i, INPUT);
		}
		else
			i++;
	}
}

void	fill_struct(t_main *mini, char **pipe_sub, int x)
{
	int		i;
	t_main	*temp;
	int		j;

	i = -1;
	j = 0;
	while (++x <= mini->pipecount)
	{
		if (mini->pipe_locs[j] == 0)
			pipe_sub[j] = ft_substr(mini->input, mini->pipe_locs[j - 1], ft_strlen(mini->input));
		pipe_sub[j] = ft_substr(mini->input, i, mini->pipe_locs[j] - i);
		i = mini->pipe_locs[j] + 1;
		pipe_sub[j] = ft_strtrim(pipe_sub[j], " ");
		j++;
	}
	i = -1;
	temp = mini;
	while (pipe_sub[++i])
	{
		mini->input = pipe_sub[i];
		mini = mini->next;
		mini = malloc(sizeof(mini));
		mini->next = NULL;
	}
	mini = temp;
}

int	check_builtin(t_main *mini)
{
	char	**splitted_input;

	splitted_input = ft_split(mini->inpwoutquotes, ' ');
	if (!ft_strncmp(splitted_input[0], "pwd", 3)
		&& !ft_strncmp(splitted_input[0], "pwd", ft_strlen(splitted_input[0])))
		return (BUILTIN);
	if (!ft_strncmp(splitted_input[0], "cd", 3)
		&& !ft_strncmp(splitted_input[0], "cd", ft_strlen(splitted_input[0])))
		return (BUILTIN);
	if (!ft_strncmp(splitted_input[0], "echo", 3)
		&& !ft_strncmp(splitted_input[0], "echo", ft_strlen(splitted_input[0])))
		return (BUILTIN);
	if (!ft_strncmp(splitted_input[0], "export", 3)
		&& !ft_strncmp(splitted_input[0], "export",
			ft_strlen(splitted_input[0])))
		return (BUILTIN);
	if (!ft_strncmp(splitted_input[0], "unset", 3)
		&& !ft_strncmp(splitted_input[0],
			"unset", ft_strlen(splitted_input[0])))
		return (BUILTIN);
	if (!ft_strncmp(splitted_input[0], "env", 3)
		&& !ft_strncmp(splitted_input[0], "env", ft_strlen(splitted_input[0])))
		return (BUILTIN);
	if (!ft_strncmp(splitted_input[0], "exit", 3)
		&& !ft_strncmp(splitted_input[0], "exit", ft_strlen(splitted_input[0])))
		return (BUILTIN);
	return (NONE);
}

void	not_builtin(t_main *mini, int fd_2[2])
{
	if (check_redirects(mini->tokenized) == 1)
	{
		take_redirects(mini);
		remove_quotes_from_append(mini, 0, 0, 0);
		remove_quotes_from_meta_input(mini, 0, 0, 0);
		remove_quotes_from_heredoc(mini, 0, 0, 0);
		remove_quotes_from_output(mini, 0, 0, 0);
		if (mini->heredoc[0])
		{
			run_heredoc(mini, fd_2);
		}
		open_files(mini);
		mini->token2 = ft_strdup(mini->tokenized);
		clean_unnecessary(mini, 0, 0);
	}
	mini->pid = fork();
	if (mini->pid == 0)
	{
		if (check_builtin(mini) == BUILTIN)
			return ;//buradan builtine yollucaz
		else
			one_cmd_exe(mini);
		exit(0);
	}
	waitpid(mini->pid, 0, 0);
}

void	split_cmd(t_main *mini)
{
	char	**pipe_sub;
	int		fd_2[2];

	fd_2[0] = dup(0);
	fd_2[1] = dup(1);
	pipe_sub = malloc(sizeof(char *) * (mini->pipecount + 2));
	pipe_sub[mini->pipecount + 1] = NULL;
	if (mini->pipecount > 0)
	{
		fill_struct(mini, pipe_sub, -1);
	}
	else
	{
		mini->inpwoutquotes = remove_quotes(mini);
		if (check_builtin(mini) == BUILTIN)
			return ; //buradan builtine yollucaz
		else
		{
			not_builtin(mini, fd_2);
		}
	}
	dup2(fd_2[0], 0);
	dup2(fd_2[1], 1);
}
