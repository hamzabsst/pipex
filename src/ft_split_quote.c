/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 09:43:31 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/08 10:16:03 by hbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

static void	skip_quote(const char *s, char quote, int *pos, int *len)
{
	while (s[*pos] && s[*pos] != quote)
	{
		(*len)++;
		(*pos)++;
	}
	if (s[*pos] == quote)
		(*pos)++;
}

int	count_tokens(const char *s, char c)
{
	int (count), (i);
	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
		{
			if (s[i] == '\'' || s[i] == '\"')
			{
				while (s[i] && s[i] != s[i + 1])
					i++;
				if (s[i])
					i++;
			}
			else
				i++;
		}
	}
	return (count);
}

int	token_length(const char *s, char c, int start)
{
	int		len;
	int		pos;
	char	quote;

	len = 0;
	pos = start;
	while (s[pos] && s[pos] == c)
		pos++;
	while (s[pos] && s[pos] != c)
	{
		if (s[pos] == '\'' || s[pos] == '\"')
		{
			quote = s[pos++];
			skip_quote(s, quote, &pos, &len);
		}
		else
		{
			len++;
			pos++;
		}
	}
	return (len);
}

void	copy_token(const char *s, char c, int *start, char *token)
{
	int		i;
	char	quote;

	i = 0;
	while (s[*start] && s[*start] == c)
		(*start)++;
	while (s[*start] && s[*start] != c)
	{
		if (s[*start] == '\'' || s[*start] == '\"')
		{
			quote = s[(*start)++];
			while (s[*start] && s[*start] != quote)
				token[i++] = s[(*start)++];
			if (s[*start] == quote)
				(*start)++;
		}
		else
			token[i++] = s[(*start)++];
	}
	token[i] = '\0';
}

void	*free_split(char **str, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}
