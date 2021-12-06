/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:49:36 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/06 23:38:02 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void environment_setup(struct s_env *e)
{
        e->resolve_dns = true;
        e->options = 0;
        e->pid = getpid() & 0xffff;
        e->ttl = 1;
        e->pos_arg = 0;
        e->max_hops = NB_HOPS;
        e->max_probes = 3;
        e->packetlen = sizeof(struct s_packet);
        e->socket = 0;
        e->host = NULL;
        ft_memset(e->to, 0, sizeof(e->to));
        ft_memset(e->dns, 0, sizeof(e->dns));
        ft_memset(e->addr, 0, sizeof(*e->addr));
}

void environment_cleanup(struct s_env *e)
{
        e->resolve_dns = false;
        e->options = 0;
        e->pid = 0;
        e->ttl = 0;
        e->pos_arg = 0;
        e->max_hops = 0;
        e->max_probes = 0;
        e->packetlen = 0;
        e->socket = 0;
        e->host = NULL;
        ft_memset(e->to, 0, sizeof(e->to));
        ft_memset(e->dns, 0, sizeof(e->dns));
        ft_memset(e->addr, 0, sizeof(*e->addr));
        freeaddrinfo(e->result);
        if (e->socket)
                close (e->socket);
}

void sockets_setup(struct s_env *e)
{
        int error = 0;
        struct sockaddr addr;
        socklen_t len = sizeof(addr);
        
        ft_memset(&addr, 0, sizeof(addr));
        e->socket = socket(e->result->ai_family, e->result->ai_socktype, e->result->ai_protocol);
        if (e->socket == -1)
                exit_errors(SOCKET_ERROR, e->host, e->pos_arg, e);
        if (bind(e->socket, &addr, len) < 0) {
                printf("%s\n", strerror(errno));
                exit_errors(BIND_ERROR, e->host, e->pos_arg, e);
        }
}
