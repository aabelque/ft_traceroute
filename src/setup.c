/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:49:36 by aabelque          #+#    #+#             */
/*   Updated: 2021/11/26 18:26:20 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void environment_setup(struct s_env *env)
{
        env->options = 0;
}

void environment_cleanup(struct s_env *env)
{
        env->options = 0;
}

void sockets_setup(struct s_env *env)
{
        int error = 0;
}
