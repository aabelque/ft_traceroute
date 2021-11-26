/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:49:55 by aabelque          #+#    #+#             */
/*   Updated: 2021/11/26 18:12:45 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void get_options(int argc, char **argv, struct s_env *env)
{
        char *s = NULL;
        int  i = 0;

        if (argc < 2)
                print_usage();
        while (++i < argc) {
                s = argv[i];
                if (*s == '-') {
                        s++;
                        if (*s == '\0') {
                                if ((argc - i) > 2) {
                                        handle_errors(argv, argc, i, env);
                                } else {
                                        if (env->host == NULL)
                                                env->host = argv[i];
                                }
                        } while (*s != '\0') {
                                switch (*s) {
                                case 'h':
                                        env->options |= OPT_H;
                                        break;
                                default:
                                        exit_errors(BAD_OPT, argv[i], i, env); 
                                        break;
                                }
                                s++;
                        }
                } else {
                        if (env->host == NULL) {
                                if (strisdigit(argv[i]))
                                        print_usage();
                                env->host = s;
                        }
                }
        }
}
