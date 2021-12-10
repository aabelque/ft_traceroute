/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:31:43 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/09 13:18:05 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void ft_traceroute(struct s_env *e)
{
        int ttl, probe, cc, seq = 0;
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

                        /* while ((cc = recv_packet) > 0)*/
                }
        }
        //TODO
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
