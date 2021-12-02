/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 18:00:53 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/02 01:22:47 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

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
