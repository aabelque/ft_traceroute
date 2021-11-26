/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_host.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zizou </var/mail/zizou>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 18:31:18 by zizou             #+#    #+#             */
/*   Updated: 2021/11/26 23:54:42 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int resolve_host(struct s_env *env)
{
        int ret = 0;
        struct addrinfo hints;
        struct sockaddr_in *addr;

        ft_memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_CANONNAME;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

        ret = getaddrinfo(env->host, NULL, &hints, &env->result);
        if (ret)
                exit_errors(ERROR_HOSTNAME, env->host, 0, env);
                

        return 0;
}
