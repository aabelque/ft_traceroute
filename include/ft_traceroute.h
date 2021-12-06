/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:32:02 by aabelque          #+#    #+#             */
/*   Updated: 2021/12/06 18:15:53 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>

#define MAXHOST 1025
#define IP_SIZE sizeof(struct ip)
#define UDP_SIZE sizeof(struct udphdr)
#define HDR_LEN IP_SIZE + UDP_SIZE
#define NB_HOPS 30
#define PORT 33434

/* Errors */
#define BAD_OPT         0x10
#define ERROR_HOSTNAME  0x11
#define SOCKET_ERROR    0x12
#define EXTRA_ARG4      0x13
#define EXTRA_ARG3      0x14
#define SETSOCK_ERROR   0x15
#define BIND_ERROR      0x16
#define SUDO_ERROR      0x17
#define MALLOC_ERROR    0x18

/* Options for traceroute */
#define OPT_H 0x01

/* packet structure */
struct s_packet {
        unsigned int    ident;
        unsigned int    seq;
        struct timeval  tv;
};

/* environment structure */
struct s_env {
        bool                    resolve_dns;
        unsigned char           options;
        int                     pid;
        int                     ttl;
        int                     pos_arg;
        int                     max_hops;
        int                     max_probes;
        int                     packetlen;
        int                     data_size;
        int                     socket;
        char                    *host;
        char                    to[INET_ADDRSTRLEN];
        char                    dns[MAXHOST];
        struct addrinfo         *result;
        struct sockaddr_in      *addr;
};

void environment_setup(struct s_env *e);
void environment_cleanup(struct s_env *e);
void sockets_setup(struct s_env *e);
void get_options(int argc, char **argv, struct s_env *e);
void ft_traceroute(struct s_env *e);
int resolve_host(struct s_env *e);

/* Utils function */
void exit_errors(int error, char *arg, int position, struct s_env *e);
void print_usage(struct s_env *e);
void print_first_line(struct s_env *e);
void handle_errors(char **argv, int argc, int idx, struct s_env *e);
/* unsigned short checksum(void *addr, int len); */
/* int is_little_endian(void); */
void send_packet(char *packet, int ttl, int seq, struct s_env *e);

/* Libc */
int strisdigit(const char *s);
int ft_strlen(const char *s);
int ft_isdigit(char c);
void *ft_memset(void *s, int c, size_t n);

#endif
