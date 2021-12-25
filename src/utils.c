/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 18:00:53 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/25 11:40:36 by aabelque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

inline void print_first_line(struct s_env *e)
{
        int len;
        len = e->proto == IPPROTO_UDP ? sizeof(struct s_udp_pkt):
                sizeof(struct s_icmp_pkt);
        printf("ft_traceroute to %s (%s), ", e->host, e->ipv4);
        printf("%d hops max, ", e->max_hops);
        printf("%d byte packets\n", len + BYTES_PACKETS);
}

inline void handle_errors(char **argv, int argc, int idx, struct s_env *e)
{
        if (argc > 4)
                exit_errors(EXTRA_ARG4, argv[idx + 2], 4, e);
        else if (argc > 3)
                exit_errors(EXTRA_ARG3, argv[idx + 2], 3, e);
}

void print_usage(struct s_env *e)
{
        printf("Usage:\n  ft_traceroute [ -hIdfm ]");
        printf(" [ -f first_ttl ] [ -m max_ttl ] host\n");
        printf("Options:\n  -h\t\tDisplay help\n");
        printf("  -I  --icmp\tUse ICMP ECHO for tracerouting\n");
        printf("  -d  --debug\tEnable socket level debugging\n");
        printf("  -f  first_ttl");
        printf("\tStart from the first_ttl hop (instead from 1)\n");
        printf("  -m  max_ttl");
        printf("\tSet the max number of hops (max TTL to be\n");
        printf("\t\treached). Default is 30\n");
        printf("  -n  \t\tDo not resolve IP addresses to their domain names\n");
        printf("Arguments:\n+    host\tThe host to traceroute to\n");
        environment_cleanup(e);
        exit(EXIT_SUCCESS);
}

void exit_errors(int error, char *arg, int position, struct s_env *e)
{
        switch (error) {
        case MALLOC_ERROR:
                fprintf(stderr, "Fatal: failed to allocate with malloc.\n");
        case SUDO_ERROR:
                fprintf(stderr, "You do not have enough privileges to use traceroute.\n");
                fprintf(stderr, "socket: Operation not permitted\n");
                break;
        case BAD_OPT:
                fprintf(stderr, "Bad option `%s' (argc %d)\n", arg, position);
                break;
        case EXTRA_ARG4:
                fprintf(stderr, "Extra arg `%s' (position %d, argc %d)\n", arg, position - 1, position);
                break;
        case EXTRA_ARG3:
                fprintf(stderr, "Extra arg `%s' (position %d, argc %d)\n", arg, position, position);
                break;
        case ERROR_HOSTNAME:
                fprintf(stderr, "%s: Temporary failure in name resolution\n", e->host);
                fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n", \
                                e->host, position, position);
                break;
        case SOCKET_ERROR:
                fprintf(stderr, "%s: Temporary failure in socket function\n", e->host);
                fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n", \
                                e->host, position, position);
                break;
        case SETSOCK_ERROR:
                fprintf(stderr, "%s: Temporary failure in setsockopt function\n", e->host);
                fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n", \
                                e->host, position, position);
                break;
        case BIND_ERROR:
                fprintf(stderr, "%s: Temporary failure in bind function\n", e->host);
                fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n", \
                                e->host, position, position);
                break;
        case SELECT_ERROR:
                fprintf(stderr, "%s: Temporary failure in select function\n", arg);
                break;
        case TTL_ERROR:
                fprintf(stderr, "first hop out of range\n");
                break;
        case HOPS_ERROR:
                fprintf(stderr, "max hops cannot be more than 255\n");
                break;
        }
        if (e)
                environment_cleanup(e);
        exit(EXIT_FAILURE);
}

double gettimeval(struct timeval before, struct timeval after)
{
        register double time;

	time = (double)(after.tv_sec - before.tv_sec) * 1000.0 +
	     (double)(after.tv_usec - before.tv_usec) / 1000.0;

	return time;
}

void print_addr(struct s_env *e)
{
        resolve_dns((struct sockaddr *)&e->from, e);
        if (e->options & OPT_N)
                e->resolve_dns = false;
        if (e->resolve_dns)
                printf(" %s (%s)", e->dns, inet_ntoa(e->from.sin_addr));
        else
                printf(" %s (%s)", inet_ntoa(e->from.sin_addr), \
                                inet_ntoa(e->from.sin_addr));
        e->resolve_dns = true;
}

unsigned short checksum(void *addr, int len)
{
        unsigned long checksum = 0;
        unsigned short *buf = addr;

        while (len > 1) {
                checksum += (unsigned short)*buf++;
                len -= sizeof(unsigned short);
        }
        if (len)
                checksum += *(unsigned char *)buf;
        checksum = (checksum >> 16) + (checksum & 0xffff);
        checksum = checksum + (checksum >> 16);
        return (unsigned short)(~checksum);
}
