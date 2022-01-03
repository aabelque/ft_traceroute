/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:32:02 by aabelque          #+#    #+#             */
/*   Updated: 2022/01/03 16:20:18 by aabelque         ###   ########.fr       */
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

/* Utils define */
#define PORT 33434
#define MAXHOST 1025
#define DATA_LEN 512
#define BYTES_PACKETS 32
#define NB_HOPS 30
#define IP_SIZE sizeof(struct iphdr)
#define ICMP_SIZE sizeof(struct icmphdr)
#define UDP_SIZE sizeof(struct udphdr)

/* Errors */
#define BAD_OPT         1
#define ERROR_HOSTNAME  2
#define SOCKET_ERROR    3
#define EXTRA_ARG4      4
#define EXTRA_ARG3      5
#define SETSOCK_ERROR   6
#define BIND_ERROR      7
#define SUDO_ERROR      8
#define MALLOC_ERROR    9
#define SELECT_ERROR    10
#define TTL_ERROR       11
#define HOPS_ERROR      12

/* Options for traceroute */
#define OPT_H           0x01
#define OPT_ICMP        0x02
#define OPT_DEBUG       0x04
#define OPT_N           0x08

/* UDP packet structure */
struct s_udp_pkt {
        struct ip       ip;
        struct udphdr   udp;
};

/* ICMP packet structure */
struct s_icmp_pkt {
        struct ip       ip;
        struct icmp     icmp;
};

/* environment structure */
struct s_env {
        bool                    resolve_dns;
        unsigned char           options;
        int                     pid;
        int                     proto;
        int                     port;
        int                     ttl;
        int                     seq;
        int                     pos_arg;
        int                     max_hops;
        int                     max_probes;
        int                     packetlen;
        int                     snd_socket;
        int                     rcv_socket;
        char                    *host;
        char                    ipv4[INET_ADDRSTRLEN];
        char                    dns[MAXHOST];
        fd_set                  readfs;
        struct addrinfo         *result;
        struct sockaddr_in      *to;
        struct sockaddr_in      from;
};

void environment_setup(struct s_env *e);
void environment_cleanup(struct s_env *e);
void sockets_setup(struct s_env *e);
void get_options(int argc, char **argv, struct s_env *e);
void ft_traceroute(struct s_env *e);
void send_packet(struct s_env *e);
void resolve_dns(struct sockaddr *addr, struct s_env *e);
void fill_udp_pkt(struct s_env *e, struct s_udp_pkt *pkt, struct in_addr sin_addr);
void fill_icmp_pkt(struct s_env *e, struct s_icmp_pkt *pkt, struct in_addr sin_addr);
int resolve_host(struct s_env *e);
int wait_for_packet(struct s_env *e);
int get_packet(struct s_env *e, int probe);

/* Utils function */
void exit_errors(int error, char *arg, int position, struct s_env *e);
void print_usage(struct s_env *e);
void print_first_line(struct s_env *e);
void print_addr(struct s_env *e);
void handle_errors(char **argv, int argc, int idx, struct s_env *e);
unsigned short checksum(void *addr, int len);
double gettimeval(struct timeval before, struct timeval after);

/* Libc */
void *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_memalloc(size_t size);
void *ft_memset(void *s, int c, size_t n);
int strisdigit(const char *s);
int ft_strlen(const char *s);
int ft_atoi(const char *str);
int ft_strcmp(const char *s1, const char *s2);

#endif
