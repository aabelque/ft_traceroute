/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zizou </var/mail/zizou>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 18:04:35 by zizou             #+#    #+#             */
/*   Updated: 2021/11/26 18:34:09 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

inline void *ft_memset(void *s, int c, size_t n)
{
        while (n) {
                *(char *)s = (unsigned char)c;
                s++;
                n--;
        }
        return s;
}

inline int ft_isdigit(char c)
{
        if (c >= '0' && c <= '9')
                return 1;
        return 0;
}

inline int ft_strlen(const char *s)
{
        const char *b = s;

        while (*s)
                s++;
        return s - b;
}

inline int strisdigit(const char *s)
{
		if (!s || !ft_strlen(s))
				return 0;
		while (*s != '\0')
				if (!ft_isdigit(*s++))
						return 0;
		return 1;
}
