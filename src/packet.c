/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zizou <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 12:29:36 by zizou             #+#    #+#             */
/*   Updated: 2021/12/09 12:38:16 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void send_packet(char *packet, int ttl, int seq, struct s_env *e)
{
        int err = 0;
        struct s_packet *pkt = (struct s_packet *)packet;

        pkt->ident = e->pid;
        pkt->seq = seq;
        gettimeofday(&pkt->tv, NULL);

        if (setsockopt(e->socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
                exit_errors(SETSOCK_ERROR, e->host, e->pos_arg, e);
        do {
                err = sendto(e->socket, packet, sizeof(packet), 0, (struct sockaddr *)e->addr, sizeof(*e->addr));
        } while (err < 0 && errno == ECONNREFUSED);

        if (err < 0 || err != e->packetlen)
                fprintf(stderr, "Error sendto, wrote %d, ret=%d\n", e->packetlen, err);
}

int recv_packet(struct s_env *e)
{
        int cc = 0;
        struct msghdr msg;


        return (cc);
}
