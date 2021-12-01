/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:49:36 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/02 00:44:55 by zizou            ###   ########.fr       */
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
        e->hops = NB_HOPS;
        e->packetlen = PACKET_LEN;
        e->socket = 0;
        e->host = NULL;
}

void environment_cleanup(struct s_env *e)
{
        e->resolve_dns = false;
        e->options = 0;
        e->pid = 0;
        e->ttl = 0;
        e->pos_arg = 0;
        e->hops = 0;
        e->packetlen = 0;
        e->socket = 0;
        e->host = NULL;
        close (e->socket);
}

void sockets_setup(struct s_env *e)
{
        int error = 0;
        struct sockaddr addr;
        socklen_t len = sizeof(addr);
        
        ft_memset(&addr, 0, sizeof(addr));
        e->socket = socket(e->result->ai_family, e->result->ai_socktype,
                        e->result->ai_protocol);
        if (e->socket == -1)
                exit_errors(SOCKET_ERROR, e->host, e->pos_arg, e);
        if (setsockopt(e->socket, IPPROTO_IP, IP_TTL, &e->ttl,
                                sizeof(e->ttl)) < 0)
                exit_errors(SETSOCK_ERROR, e->host, e->pos_arg, e);
        if (bind(e->socket, &addr, len) < 0) {
                printf("%s\n", strerror(errno));
                exit_errors(BIND_ERROR, e->host, e->pos_arg, e);
        }
}
