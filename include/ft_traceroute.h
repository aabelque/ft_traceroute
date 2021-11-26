/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:32:02 by aabelque          #+#    #+#             */
/*   Updated: 2021/11/26 21:23:05 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define EXTRA_ARG4 0x40
#define EXTRA_ARG3 0x50
#define ERROR_HOSTNAME 0x20

/* Options for traceroute */
#define OPT_H 0x01
#define BAD_OPT 0x30

/* environment structure */
struct s_env {
        unsigned char   options;
        int             pos_arg;
        char            *host;
        char            to[INET_ADDRSTRLEN];
        struct addrinfo *result;
};

extern void environment_setup(struct s_env *env);
extern void environment_cleanup(struct s_env *env);
extern void sockets_setup(struct s_env *env);
extern void get_options(int argc, char **argv, struct s_env *env);
extern int resolve_host(struct s_env *env);

/* Utils function */
extern void exit_errors(int error, char *arg, int position, struct s_env *env);
extern void print_usage(void);
extern void handle_errors(char **argv, int argc, int idx, struct s_env *env);

/* Libc */
extern int strisdigit(const char *s);
extern int ft_strlen(const char *s);
extern int ft_isdigit(char c);
extern void *ft_memset(void *s, int c, size_t n);

#endif
