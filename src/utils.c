/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 18:00:53 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/05 23:29:23 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void print_first_line(struct s_env *e)
{
        printf("ft_traceroute to %s (%s), ", e->host, e->to);
        printf("%d hops max, ", e->max_hops);
        printf("%d byte packets\n", e->packetlen);
}

void handle_errors(char **argv, int argc, int idx, struct s_env *e)
{
        if (argc > 4)
                exit_errors(EXTRA_ARG4, argv[idx + 2], 4, e);
        else if (argc > 3)
                exit_errors(EXTRA_ARG3, argv[idx + 2], 3, e);
}

void print_usage(struct s_env *e)
{
        fprintf(stdout, "Usage:\n  ft_traceroute [ -h ] host\n");
        fprintf(stdout, "Options:\n  -h\t\tDisplay help\n");
        fprintf(stdout, "Arguments:\n+    host\tThe host to traceroute to\n");
        environment_cleanup(e);
        exit(EXIT_SUCCESS);
}

void exit_errors(int error, char *arg, int position, struct s_env *e)
{
        switch (error) {
        case MALLOC_ERROR:
                fprintf(stderr, "Fatal: failed to allocate with malloc.\n");
        case SUDO_ERROR:
                fprintf(stderr, "Operation not permitted -> man sudo\n");
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
                fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n", e->host, position, position);
                break;
        case SOCKET_ERROR:
                fprintf(stderr, "%s: Temporary failure in socket function\n", e->host);
                fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n", e->host, position, position);
                break;
        case SETSOCK_ERROR:
                fprintf(stderr, "%s: Temporary failure in setsockopt function\n", e->host);
                fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n", e->host, position, position);
                break;
        case BIND_ERROR:
                fprintf(stderr, "%s: Temporary failure in bind function\n", e->host);
                fprintf(stderr, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n", e->host, position, position);
                break;
        }
        if (e)
                environment_cleanup(e);
        exit(EXIT_FAILURE);
}

/* int is_little_endian(void) */
/* { */
/*         int x = 1; */
/*         return *(char *)&x; */
/* } */

/* unsigned short checksum(void *addr, int len) */
/* { */
/*         unsigned long checksum = 0; */
/*         unsigned short *buf = addr; */

/*         while (len > 1) { */
/*                 checksum += (unsigned short)*buf++; */
/*                 len -= sizeof(unsigned short); */
/*         } */
/*         if (len) */
/*                 checksum += *(unsigned char *)buf; */
/*         checksum = (checksum >> 16) + (checksum & 0xffff); */
/*         checksum = checksum + (checksum >> 16); */
/*         return (unsigned short)(~checksum); */
/* } */
