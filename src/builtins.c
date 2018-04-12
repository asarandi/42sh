/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 05:06:58 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/12 01:50:57 by ztisnes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *g_builtin_list[] = {
		"cd", "echo", "env",
		"exit", "setenv", "unsetenv"};
char *g_punani_list[] = {
		"Compact disc; access to other folders or directories",
		"Display given string", "Set environment and execute command, or print\
		environment", "Terminate program", "function inserts or resets the\
		environment variable name in the current environment list",
		"function deletes all instances of the variable name pointed to\
		by name from the list"
	};

int		builtin_cmd_index(char *cmd)
{
	int			i;
	const char	*builtin_list[] = {
		"echo", "cd", "setenv", "unsetenv", "env", "exit", "help"};

	i = 0;
	while (i < NUM_BUILTINS)
	{
		if (ft_strcmp(cmd, builtin_list[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int		builtin_echo(t_shell *sh, char **argv)
{
	int	dash_n;
	int	i;

	dash_n = 0;
	sh->exit_code += 0;	//spaghetti
	if (argv[1] != NULL)
	{
		if (ft_strcmp(argv[1], "-n") == 0)
			dash_n = 1;
		i = 1 + dash_n;
		while (argv[i] != NULL)
		{
			ft_printf(STDOUT_FILENO, "%s", argv[i]);
			if (argv[++i] != NULL)
				ft_printf(STDOUT_FILENO, " ");
		}
	}
	if (dash_n == 0)
		ft_printf(STDOUT_FILENO, "\n");
	return (0);
}

char	*builtin_cd_get_kv(t_shell *sh, char *variable)
{
	char	*result;

	result = NULL;
	result = kv_array_get_key_value(sh->envp, variable);
	if (result == NULL)
		(void)ft_printf(STDERR_FILENO, E_NOVARIABLE, variable);
	return (result);
}

int	builtin_exit(t_shell *sh, char **argv)
{
	int	exit_code;

	if ((argv[1] != NULL) && \
		((is_numeric_string(argv[1])) || \
		((argv[1][0] == '-') && \
		is_numeric_string(&argv[1][1]))))
			exit_code = ft_atoi(argv[1]);
	else
		exit_code = sh->exit_code;
	ft_printf(STDERR_FILENO, "exit\n");
	clean_up(sh);
	exit(exit_code);
	return (exit_code);
}

void	help_selection(char *cmd)
{
	int i;

	i = 0;
	while (g_builtin_list[i] != '\0')
	{
		if (g_builtin_list[i] && (i >= 0))
		{
			if (ft_strcmp(cmd ,g_builtin_list[i]) == 0)
				return (void)printf("Only if I'm %s -> %s\n", \
				g_builtin_list[i], g_punani_list[i]);
			i++;
		}
	}
}

void	help_list(void)
{
	int	i;

	i = 0;
	while (g_builtin_list[i] != '\0')
	{
		if (g_builtin_list[i] && (i >= 0))
		{
			printf("Only if im %s -> ", g_builtin_list[i]);
			if (i == 0)
				printf("\taccess directories/folder\n");
			else if (i == 1)
				printf("\tprint stuff\n");
			else if (i == 2)
				printf("\tenv stuff\n");
			else if (i == 3)
				printf("\texit stuff\n");
			else if (i == 4)
				printf("\tsetenv stuff\n");
			else if (i == 5)
				printf("\tunsetenv stuff\n");
			else
				break ;
			i++;
		}
	}
}

int	builtin_help(t_shell *sh, char **argv)
{
	sh->argc += 0;
	argv[0] += 0;
	if (argv[1] != NULL)
	{
		if (argv[2])
			return(int)printf("No more than 1 help command. Do `help [cmd]`\n");
		help_selection(argv[1]);
	}
	else
		help_list();
	return (0);
}