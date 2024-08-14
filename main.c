/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 17:49:13 by erkoc             #+#    #+#             */
/*   Updated: 2024/08/13 19:01:34 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_zero(t_main *mini)
{
	mini->ac = 0;
	mini->hc = 0;
	mini->oc = 0;
	mini->ic = 0;
}

void	start_cmd(void)
{
	char	*rcmd;
	t_main	cmd;
	t_env	*env;
	int		doublecount;
	int		singlecount;

	env = malloc(sizeof(t_env));
	if (!env)
		err_msg(2);
	take_env(&cmd);
	while (1)
	{
		doublecount = 0;
		singlecount = 0;
		set_zero(&cmd);
		//init_strc(&cmd, env);
		rcmd = readline("iboshell$> ");
		if (!rcmd)
		{
			free(env);
			printf("exit\n");
			exit(1);
		}
		add_history(rcmd);
		cmd.input = ft_strtrim(rcmd, "\t ");
		tab_to_space(cmd.input);
		free(rcmd);
		isquote_closed(cmd.input, -1, &doublecount, &singlecount);
		if (doublecount % 2 != 0 || singlecount % 2 != 0)
		{
			printf("Dquote!\n");
			free(cmd.input);
			continue ;
		}
		cmd.tokenized = tokenize(cmd.input);
		pipe_in_quotes(&cmd);
		empyt_pipe_check(&cmd);
		empty_inout_check(&cmd);
		split_cmd(&cmd);
		free(cmd.input);
	}
}

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
		err_msg(1);
	start_cmd();
}
