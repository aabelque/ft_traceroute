/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:49:55 by aabelque          #+#    #+#             */
/*   Updated: 2022/01/03 13:52:51 by aabelque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static void set_max_hops(struct s_env *e, char **argv, int idx)
{
        char *s = NULL;

        s = argv[idx];
        if (strisdigit(s))
                e->max_hops = ft_atoi(s);
        else
                print_usage(e);
        if (e->max_hops == 0)
                exit_errors(TTL_ERROR, NULL, 0, e);
        else if (e->max_hops < e->ttl)
                exit_errors(TTL_ERROR, NULL, 0, e);
        else if (0 > e->max_hops || e->max_hops > 255)
                exit_errors(HOPS_ERROR, NULL, 0, e);
}

static void set_ttl(struct s_env *e, char **argv, int idx)
{
        char *s = NULL;

        s = argv[idx];
        if (strisdigit(s))
                e->ttl = ft_atoi(s);
        else
                print_usage(e);
        if (0 >= e->ttl || e->ttl > e->max_hops) {
                exit_errors(TTL_ERROR, NULL, 0, e);
        }
}

void get_options(int argc, char **argv, struct s_env *e)
{
        char *s = NULL;
        int  i = 0;

        if (argc < 2)
                print_usage(e);
        while (++i < argc) {
                s = argv[i];
                if (!ft_strcmp(s, "--icmp")) {
                        e->options |= OPT_ICMP;
                } else if (!ft_strcmp(s, "--debug")) {
                        e->options |= OPT_DEBUG;
                } else if (*s == '-') {
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
                                case 'I':
                                        e->options |= OPT_ICMP;
                                        break;
                                case 'd':
                                        e->options |= OPT_DEBUG;
                                        break;
                                case 'f':
                                        ++i;
                                        set_ttl(e, argv, i);
                                        break;
                                case 'm':
                                        ++i;
                                        set_max_hops(e, argv, i);
                                        break;
                                case 'n':
                                        e->options |= OPT_N;
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
                                        print_usage(e);
                                e->host = s;
                                e->pos_arg = i;
                        }
                }
        }
}
