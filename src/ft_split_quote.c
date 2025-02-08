/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbousset <hbousset@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 09:43:31 by hbousset          #+#    #+#             */
/*   Updated: 2025/02/08 11:15:26 by hbousset         ###   ########.fr       */
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

static int	count_tokens(const char *s, char c)
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

static int	token_length(const char *s, char c, int start)
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

static void	copy_token(const char *s, char c, int *start, char *token)
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

char	**ft_split_quote(const char *s, char c)
{
	char	**results;
	int		tokens;
	int		i;
	int		start;
	int		len;

	if (!s)
		return (NULL);
	start = 0;
	i = 0;
	tokens = count_tokens(s, c);
	results = malloc(sizeof(char *) * (tokens + 1));
	if (!results)
		return (NULL);
	while (i < tokens)
	{
		len = token_length(s, c, start);
		results[i] = malloc(sizeof(char) * (len + 1));
		if (!results[i])
			return (free_split(results, i));
		copy_token(s, c, &start, results[i]);
		i++;
	}
	results[i] = NULL;
	return (results);
}
