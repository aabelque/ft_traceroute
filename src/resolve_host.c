/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_host.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 11:39:54 by aabelque          #+#    #+#             */
/*   Updated: 2022/01/03 16:22:05 by aabelque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void resolve_dns(struct sockaddr *addr, struct s_env *e)
{
        socklen_t len = sizeof(*addr);

        if (getnameinfo(addr, len, e->dns, sizeof(e->dns), NULL, 0, NI_NAMEREQD))
                e->resolve_dns = false;
}

int resolve_host(struct s_env *e)
{
        int ret = 0;
        struct addrinfo hints;
        struct sockaddr_in *addr;

        ft_memset(&hints, 0, sizeof(hints));
        hints.ai_flags = 0;
        hints.ai_family = AF_INET;
        hints.ai_socktype = 0;
	hints.ai_protocol = IPPROTO_UDP;
        hints.ai_addrlen = 0;
        hints.ai_addr = NULL;
        hints.ai_canonname = NULL;
        hints.ai_next = NULL;

        ret = getaddrinfo(e->host, NULL, &hints, &e->result);
        if (ret)
                return ERROR_HOSTNAME;
        addr = (struct sockaddr_in *)e->result->ai_addr;
        e->to = ft_memalloc(sizeof(*addr));
        if (!e->to)
                return ERROR_HOSTNAME;
        ft_memcpy(e->to, addr, sizeof(*addr));
        inet_ntop(AF_INET, &addr->sin_addr, e->ipv4, INET_ADDRSTRLEN);
        freeaddrinfo(e->result);
        return 0;
}
