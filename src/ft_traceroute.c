/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:31:43 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/02 00:30:26 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void ft_traceroute(struct s_env *e)
{
        printf("uid = %d\n", getuid());
        printf("pid = %d\n", getpid());
}

int main(int argc, char **argv)
{
        int ret = 0;
        struct s_env e;

        environment_setup(&e);
        get_options(argc, argv, &e);
        if (e.options & OPT_H)
                print_usage();
        ret = resolve_host(&e);
        if (ret)
                exit_errors(ret, e.host, e.pos_arg, &e);
        sockets_setup(&e);
        ft_traceroute(&e);
        return 0;
}
