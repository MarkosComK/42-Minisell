/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:39:51 by marsoare          #+#    #+#             */
/*   Updated: 2024/10/15 15:54:16 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	in_quotes(char *input, int i)
{
	if (input[i] == '"')
		return (true);
	return (false);
}

int	set_append(t_shell *sh, t_token *new_token, char *input, int i)
{
	new_token->value = ft_strndup(&input[i], 2);
	if (!new_token->value)
		exit_failure(sh, "handle_redir_1");
	new_token->type = APPEND;
	return (i + 2);
}

int	set_simple(t_shell *sh, t_token *new_token, char *input, int i)
{
	new_token->value = ft_strndup(&input[i], 1);
	if (!new_token->value)
		exit_failure(sh, "handle_redir_2");
	if (input[i] == '<')
		new_token->type = INFILE;
	else
		new_token->type = OUTFILE;
	return (i + 1);
}
