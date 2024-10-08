/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erkoc <erkoc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:05:46 by fhosgor           #+#    #+#             */
/*   Updated: 2024/08/14 15:46:32 by erkoc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	i;

	i = 0;
	while (s1[i] && ft_strchr(set, s1[i]))
	{
		i++;
	}
	len = ft_strlen(s1 + i);
	while (len && ft_strchr(set, (s1 + i)[len - 1]))
	{
		len--;
	}
	return (ft_substr(s1, i, len));
}
