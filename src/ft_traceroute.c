/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:31:43 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/06 18:15:53 by zizou            ###   ########.fr       */
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
        printf("sin_addr = %s\n", inet_ntoa(e->addr->sin_addr));
        printf("sin_port = %u\n", e->addr->sin_port);

        if (setsockopt(e->socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
                exit_errors(SETSOCK_ERROR, e->host, e->pos_arg, e);
        do {
                err = sendto(e->socket, packet, sizeof(packet), 0, (struct sockaddr *)e->addr, sizeof(*e->addr));
        } while (err < 0 && errno == ECONNREFUSED);

        if (err < 0 || err != e->packetlen)
                fprintf(stderr, "Error sendto, wrote %d, ret=%d\n", e->packetlen, err);
        /* else */
        /*         printf("OK\n"); */
}

void ft_traceroute(struct s_env *e)
{
        int ttl, probe, seq = 0;
        char *packet = NULL;
        struct timeval t1, t2;

        print_first_line(e);
        packet = malloc(e->packetlen);
        if (!packet)
                exit_errors(MALLOC_ERROR, e->host, e->pos_arg, e);
        ft_memset(packet, 0, e->packetlen);

        for (ttl = 1; ttl <= e->max_hops; ++ttl) {
                printf("%2d ", ttl);
                for (probe = 0; probe < e->max_probes; ++probe) {
                        gettimeofday(&t1, NULL);
                        send_packet(packet, ttl, ++seq, e);
                }
        }
        //TODO
        //send packet
        //recv packet
        //analyse packet
        free(packet);
}

int main(int argc, char **argv)
{
        
        int ret = 0;
        struct s_env e;

        if (getuid() != 0)
                exit_errors(SUDO_ERROR, NULL, 0, NULL);
        environment_setup(&e);
        get_options(argc, argv, &e);
        if (e.options & OPT_H)
                print_usage(&e);
        if (resolve_host(&e))
                exit_errors(ret, e.host, e.pos_arg, &e);
        sockets_setup(&e);
        ft_traceroute(&e);
        environment_cleanup(&e);
        return 0;
}
