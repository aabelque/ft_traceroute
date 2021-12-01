/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:49:55 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/02 00:33:21 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void get_options(int argc, char **argv, struct s_env *e)
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
                                        handle_errors(argv, argc, i, e);
                                } else {
                                        if (e->host == NULL) {
                                                e->host = argv[i];
                                                e->pos_arg = i;
                                        }
                                }
                        } while (*s != '\0') {
                                switch (*s) {
                                case 'h':
                                        e->options |= OPT_H;
                                        break;
                                default:
                                        exit_errors(BAD_OPT, argv[i], i, e); 
                                        break;
                                }
                                s++;
                        }
                } else {
                        if (e->host == NULL) {
                                if (strisdigit(argv[i]))
                                        print_usage();
                                e->host = s;
                                e->pos_arg = i;
                        }
                }
        }
}
