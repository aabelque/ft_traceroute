/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_host.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zizou </var/mail/zizou>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 18:31:18 by zizou             #+#    #+#             */
/*   Updated: 2021/12/06 18:15:48 by zizou            ###   ########.fr       */
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
        hints.ai_family = PF_INET;
        hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

        ret = getaddrinfo(e->host, NULL, &hints, &e->result);
        if (ret)
                return ERROR_HOSTNAME;

        addr = (struct sockaddr_in *)e->result->ai_addr;
        e->addr = addr;
        e->addr->sin_port = PORT;
        inet_ntop(AF_INET, &addr->sin_addr, e->to, INET_ADDRSTRLEN);
        return 0;
}
