/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:41:24 by marsoare          #+#    #+#             */
/*   Updated: 2024/10/22 20:59:21 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	tokenize_input(t_shell *shell, char *input)
{
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			i = handle_quotes(shell, input, i);
		else if (input[i] == '|')
			i = handle_pipe(shell, input, i);
		else if (input[i] == '>' || input[i] == '<')
			i = handle_redir(shell, input, i);
		else
			i = handle_word_token(shell, input, i);
		set_token_pos(shell->token_lst);
	}
}

int	join_strs(char **str, char *input, int i)
{
	char	*join;
	char	quote;
	int		start;
	char	*tmp;

	if (ft_isquote(input[i]))
	{
		quote = input[i++];
		start = i;
		while (input[i] != quote && input[i] != '\0')
			i++;
		join = ft_substr(input, start, i - start);
		tmp = *str;
		*str = ft_strjoin(*str, join);
		free(tmp);
		free(join);
		i++;
	}
	else
	{
		start = i;
		while (input[i] && !ft_isquote(input[i])
				&& !ft_isspace(input[i]) && !ft_ismeta(input, i))
			i++;
		join = ft_substr(input, start, i - start);
		tmp = *str;
		*str = ft_strjoin(*str, join);
		free(tmp);
		free(join);
		//i++;
	}
	return (i);
}

int	handle_word_token(t_shell *shell, char *input, int i)
{
	t_token	*new_token;
	char	*str = ft_strdup("");

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		exit_failure(shell, "handle_word_token");
	while (input[i] && !ft_isspace(input[i]) && !ft_ismeta(input, i))
	{
		i = join_strs(&str, input,  i);
	}
	new_token->value = str;
	if (!new_token->value)
		exit_failure(shell, "handle_word_token_1");
	new_token->type = WORD;
	new_token->state = GENERAL;
	ft_lstadd_back(&shell->token_lst, ft_lstnew(new_token));
	while (ft_isspace(input[i]))
		i++;
	return (i);
}

int	handle_redir(t_shell *shell, char *input, int i)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		exit_failure(shell, "handle_redir");
	if (input[i] == '>' && input[i + 1] == '>')
	{
		i = set_append(shell, new_token, input, i);
	}
	else
	{
		i = set_simple(shell, new_token, input, i);
	}
	new_token->state = GENERAL;
	ft_lstadd_back(&shell->token_lst, ft_lstnew(new_token));
	while (ft_isspace(input[i]))
		i++;
	return (i);
}

int	handle_pipe(t_shell *shell, char *input, int i)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		exit_failure(shell, "handle_pipe");
	new_token->value = ft_strndup(&input[i], 1);
	if (!new_token->value)
		exit_failure(shell, "handle_pipe_1");
	new_token->type = PIPE;
	new_token->state = GENERAL;
	ft_lstadd_back(&shell->token_lst, ft_lstnew(new_token));
	i++;
	while (ft_isspace(input[i]))
		i++;
	return (i);
}

int	handle_quotes(t_shell *shell, char *input, int i)
{
	t_token		*new_token;
	char	quote = input[i];
	char	*str = ft_strdup("");

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		exit_failure(shell, "handle_quotes");
	while (input[i] && !ft_isspace(input[i]) && !ft_ismeta(input, i))
	{
		i = join_strs(&str, input,  i);
	}
	new_token->value = str;
	if (!new_token->value)
		exit_failure(shell, "handle_quotes_1");
	new_token->type = WORD;
	if (quote == '"')
		new_token->state = IN_DQUOTES;
	else if (quote == '\'')
		new_token->state = IN_SQUOTES;
	ft_lstadd_back(&shell->token_lst, ft_lstnew(new_token));
	while (ft_isspace(input[i]))
		i++;
	return (i);
}
