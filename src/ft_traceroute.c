/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:31:43 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/02 01:22:19 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void ft_traceroute(struct s_env *e)
{
        //TODO
        //fill packet
        //send packet
        //recv packet
        //analyse packet
        printf("uid = %d\n", getuid());
        printf("pid = %d\n", getpid());
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
        return 0;
}
