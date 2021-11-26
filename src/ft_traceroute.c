/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:31:43 by aabelque          #+#    #+#             */
/*   Updated: 2021/11/26 18:34:09 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <stdio.h>

int main(int argc, char **argv)
{
        int ret = 0;
        struct s_env env;

        environment_setup(&env);
        get_options(argc, argv, &env);
        if (env.options & OPT_H)
                print_usage();
        ret = resolve_host(&env);
        if (ret)
                exit_errors(ret, env.host, env.pos_arg, &env);
        sockets_setup(&env);
        return 0;
}
