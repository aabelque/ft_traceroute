/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 11:38:57 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/25 11:39:03 by aabelque         ###   ########.fr       */
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

inline int ft_strlen(const char *s)
{
        const char *b = s;

        while (*s)
                s++;
        return s - b;
}

static inline int ft_isdigit(char c)
{
        if (c >= '0' && c <= '9')
                return 1;
        return 0;
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

inline int ft_strcmp(const char *s1, const char *s2)
{
        while (*s1 == *s2) {
                if (*s1 == '\0')
                        return 0;
                s1++;
                s2++;
        }
        return *(unsigned char *)s1 - *(unsigned char *)s2;
}

static inline int ft_isspace(char c)
{
        if (c == ' ' || c == '\t' || c == '\f' || c == '\r' \
                        || c == '\n'|| c == '\v')
                return 1;
        return 0;
}

int ft_atoi(const char *str)
{
        int i, sign;
        long nbr;

        i = 0;
        sign = 1;
        nbr = 0;
        if (!str[i])
                return 0;
        while (ft_isspace(str[i]))
                i++;
        if (str[i] == '-' || str[i] == '+') {
                if (str[++i] == '-')
                        sign = -1;
        }
        while (str[i] < '0' || str[i] > '9')
                i++;
        while (str[i] >= '0' && str[i] <= '9')
                nbr = (nbr * 10) + (str[i++] - '0');
        return nbr * sign;
}
