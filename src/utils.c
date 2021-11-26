/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 18:00:53 by aabelque          #+#    #+#             */
/*   Updated: 2021/11/26 18:18:54 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void handle_errors(char **argv, int argc, int idx, struct s_env *env)
{
        if (argc > 4)
                exit_errors(EXTRA_ARG4, argv[idx + 2], 4, env);
        else if (argc > 3)
                exit_errors(EXTRA_ARG3, argv[idx + 2], 3, env);
}

void print_usage(void)
{
        fprintf(stderr, "Usage:\n  ft_traceroute [ -h ] host\n");
        fprintf(stderr, "Options:\n  -h\t\tDisplay help\n");
}

void exit_errors(int error, char *arg, int position, struct s_env *env)
{
        switch (error) {
        case BAD_OPT:
                fprintf(stderr, "Bad option `%s' (argc %d)\n", arg, position);
                break;
        case EXTRA_ARG4:
                fprintf(stderr, "Extra arg `%s' (position %d, argc %d)\n", arg, position - 1, position);
                break;
        case EXTRA_ARG3:
                fprintf(stderr, "Extra arg `%s' (position %d, argc %d)\n", arg, position, position);
                break;
        }
        environment_cleanup(env);
        exit(EXIT_FAILURE);
}
