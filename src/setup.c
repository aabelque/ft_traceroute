/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:49:36 by aabelque          #+#    #+#             */
/*   Updated: 2022/01/03 16:17:10 by aabelque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void fill_icmp_pkt(struct s_env *e, struct s_icmp_pkt *pkt, struct in_addr sin_addr)
{
        ft_memset(pkt, 0, sizeof(*pkt));
	(pkt->ip).ip_off = 0;
	(pkt->ip).ip_hl = sizeof(pkt->ip) >> 2;
	(pkt->ip).ip_p = IPPROTO_ICMP;
	(pkt->ip).ip_len = e->packetlen;
	(pkt->ip).ip_ttl = e->ttl;
	(pkt->ip).ip_v = IPVERSION;
	(pkt->ip).ip_id = htons(e->pid + e->seq);
	(pkt->ip).ip_tos = 0;
	(pkt->ip).ip_dst = sin_addr;

        (pkt->icmp).icmp_type = ICMP_ECHO;
        (pkt->icmp).icmp_code = 0;
        (pkt->icmp).icmp_cksum = 0;
        (pkt->icmp).icmp_id = e->pid;
        (pkt->icmp).icmp_seq = e->seq;
        (pkt->icmp).icmp_cksum = checksum(&pkt->icmp, sizeof(struct icmp));
}

void fill_udp_pkt(struct s_env *e, struct s_udp_pkt *pkt, struct in_addr sin_addr)
{
	ft_memset(pkt, 0, sizeof(*pkt));
	(pkt->ip).ip_off = 0;
	(pkt->ip).ip_hl = sizeof(pkt->ip) >> 2;
	(pkt->ip).ip_p = IPPROTO_UDP;
	(pkt->ip).ip_len = e->packetlen;
	(pkt->ip).ip_ttl = e->ttl;
	(pkt->ip).ip_v = IPVERSION;
	(pkt->ip).ip_id = htons(e->pid + e->seq);
	(pkt->ip).ip_tos = 0;
	(pkt->ip).ip_dst = sin_addr;

	(pkt->udp).uh_sport = htons(e->pid);
	(pkt->udp).uh_dport = htons(e->port + e->seq);
	(pkt->udp).uh_ulen = htons((unsigned short)(e->packetlen - sizeof(struct ip)));
	(pkt->udp).uh_sum = 0;
}

void environment_setup(struct s_env *e)
{
        e->resolve_dns = true;
        e->options = 0;
        e->proto = 0;
        e->pid = (getpid() & 0xffff) | 0x8000;
        e->port = PORT;
        e->ttl = 1;
        e->seq = 0;
        e->pos_arg = 0;
        e->max_hops = NB_HOPS;
        e->max_probes = 3;
        e->packetlen = 0;
        e->snd_socket = 0;
        e->rcv_socket = 0;
        e->host = NULL;
        e->to = NULL;
        ft_memset(e->ipv4, 0, sizeof(e->ipv4));
        ft_memset(e->dns, 0, sizeof(e->dns));
        ft_memset(&e->from, 0, sizeof(e->from));
}

void environment_cleanup(struct s_env *e)
{
        e->resolve_dns = false;
        e->options = 0;
        e->proto = 0;
        e->pid = 0;
        e->port = 0;
        e->ttl = 0;
        e->seq = 0;
        e->pos_arg = 0;
        e->max_hops = 0;
        e->max_probes = 0;
        e->packetlen = 0;
        e->host = NULL;
        ft_memset(e->ipv4, 0, sizeof(e->ipv4));
        ft_memset(e->dns, 0, sizeof(e->dns));
        ft_memset(e->to, 0, sizeof(*e->to));
        ft_memset(&e->from, 0, sizeof(e->from));
        if (e->to)
                free(e->to);
        e->to = NULL;
        if (e->snd_socket) {
                close (e->snd_socket);
                e->snd_socket = 0;
        }
        if (e->rcv_socket) {
                close (e->rcv_socket);
                e->rcv_socket = 0;
        }
}

void sockets_setup(struct s_env *e)
{
        int opt = 1;
        struct sockaddr addr;
        socklen_t len = sizeof(addr);
        
        ft_memset(&addr, 0, sizeof(addr));
        if (e->options & OPT_ICMP) {
                e->snd_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
                e->proto = IPPROTO_ICMP;
        } else {
                e->snd_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
                e->proto = IPPROTO_UDP;
        }
        if (e->snd_socket == -1)
                exit_errors(SOCKET_ERROR, e->host, e->pos_arg, e);
        setsockopt(e->snd_socket, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));
        e->rcv_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (e->rcv_socket == -1)
                exit_errors(SOCKET_ERROR, e->host, e->pos_arg, e);
        setsockopt(e->rcv_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if (e->options & OPT_DEBUG) {
                setsockopt(e->rcv_socket, SOL_SOCKET, SO_DEBUG, &opt, sizeof(opt));
                printf("setsockopt SO_DEBUG: activate\n");
        }
        if (bind(e->snd_socket, &addr, len) < 0) {
                printf("%s\n", strerror(errno));
                exit_errors(BIND_ERROR, e->host, e->pos_arg, e);
        }
        if (bind(e->rcv_socket, &addr, len) < 0) {
                printf("%s\n", strerror(errno));
                exit_errors(BIND_ERROR, e->host, e->pos_arg, e);
        }
}
