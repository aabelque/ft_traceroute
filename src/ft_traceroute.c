/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:31:43 by aabelque          #+#    #+#             */
/*   Updated: 2022/01/03 15:17:53 by aabelque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static void check_code(int code, int *unreachable, int *end)
{
        switch (code) {
        case ICMP_HOST_UNREACH:
                printf(" !H");
                ++*unreachable;
                break;
        case ICMP_NET_UNREACH:
                printf(" !N");
                ++*unreachable;
                break;
        case ICMP_PROT_UNREACH:
                printf(" !P");
                ++*unreachable;
                break;
        case ICMP_SR_FAILED:
                printf(" !S");
                ++*unreachable;
                break;
        case ICMP_FRAG_NEEDED:
                printf(" !F");
                ++*unreachable;
                break;
        case ICMP_PORT_UNREACH:
                ++*end;
                break;
        case ICMP_UNREACH_PRECEDENCE_CUTOFF:
                printf(" !C");
                ++*unreachable;
                break;
        case ICMP_UNREACH_HOST_PRECEDENCE:
                printf(" !V");
                ++*unreachable;
                break;
        case ICMP_UNREACH_FILTER_PROHIB:
                printf(" !X");
                ++*unreachable;
                break;
        }
}

void ft_traceroute(struct s_env *e)
{
        int probe, cc;
        int code = 0, unreach = 0, end = 0;
        struct timeval t1, t2;
        in_addr_t lastaddr = 0;

        print_first_line(e);
        for (; e->ttl <= e->max_hops; ++e->ttl) {
                printf("%2d ", e->ttl);
                for (probe = 0; probe < e->max_probes; ++probe) {
                        e->seq++;
                        gettimeofday(&t1, NULL);
                        send_packet(e);
                        cc = wait_for_packet(e);
                        if (cc < 0) {
                                exit_errors(SELECT_ERROR, e->host, 0, e);
                        } else if (cc == 0) {
                                printf(" *");
                        } else if (FD_ISSET(e->rcv_socket, &e->readfs)) {
                                gettimeofday(&t2, NULL);
                                code = get_packet(e, probe);
                                if (e->from.sin_addr.s_addr != lastaddr) {
                                        print_addr(e);
                                        lastaddr = e->from.sin_addr.s_addr;
                                }
                                printf("  %g ms", gettimeval(t1, t2));
                                check_code(code, &unreach, &end);
                        }
                }
                printf("\n");
                if (code == -1 || end || (unreach > 0 \
                                        && unreach >= e->max_probes - 1))
                        return ;
        }
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
        if ((ret = resolve_host(&e)))
                exit_errors(ret, e.host, e.pos_arg, &e);
        sockets_setup(&e);
        ft_traceroute(&e);
        environment_cleanup(&e);
        return EXIT_SUCCESS;
}
