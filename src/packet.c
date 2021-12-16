/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zizou <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 12:29:36 by zizou             #+#    #+#             */
/*   Updated: 2021/12/16 02:12:17 by zizou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int get_packet(struct s_env *e, int probe)
{
        int cc = 0, hlen = 0;
        u_int8_t type, code;
        char packet[DATA_LEN];
        socklen_t len;
        struct icmp *icmp;
        struct ip *fip, *ip;
        struct udphdr *udp;

        ft_memset(&e->from, 0, sizeof(e->from));
        len = sizeof(e->from);

        cc = recvfrom(e->rcv_socket, (char *)&packet, sizeof(packet), 0, (struct sockaddr *)&e->from, &len);
        if (cc > 0) {
                fip = (struct ip *)packet;
                hlen = fip->ip_hl << 2;

                icmp = (struct icmp *)(packet + hlen);
                type = icmp->icmp_type;
                code = icmp->icmp_code;

                if ((type == ICMP_TIMXCEED && code == ICMP_TIMXCEED_INTRANS) \
                                || type == ICMP_UNREACH || type == ICMP_ECHOREPLY) {

                        ip = &icmp->icmp_ip;

                        if (ip->ip_p == IPPROTO_ICMP) {
                                printf("icmp\n");
                        } else {
                                udp = (struct udphdr *)((char *)ip + hlen);
                                if (ntohs(udp->uh_dport) == e->port + e->seq \
                                                && ntohs(udp->uh_sport) == e->pid)
                                        return type == ICMP_TIMXCEED ? -1 : code;
                        }
                }
        }
        return 0;
}

int wait_for_packet(struct s_env *e)
{
        fd_set readfs;
        struct timeval tv = {.tv_sec = 3, .tv_usec = 0};

        FD_ZERO(&readfs);
        FD_SET(e->rcv_socket, &readfs);

        return select(e->rcv_socket + 1, &readfs, (fd_set *)0, (fd_set *)0, &tv);
}

static int send_icmp_packet(struct s_env *e)
{
        int cc = 0;
        /* struct s_icmp_pkt packet; */
        /* struct sockaddr_in addr; */

	/* ft_memset(&addr, 0, sizeof(addr)); */
	/* addr.sin_family = AF_INET; */
	/* addr.sin_addr.s_addr = e->to->sin_addr.s_addr; */
	/* addr.sin_port = e->port; */

        /* fill_icmp_pkt(e, &packet, addr.sin_addr); */

	/* packet.seq = e->seq; */
	/* packet.ttl = e->ttl; */
	/* gettimeofday(&packet.tv, NULL); */

        /* if (setsockopt(e->snd_socket, IPPROTO_IP, IP_TTL, &e->ttl, sizeof(e->ttl)) < 0) */
        /*         exit_errors(SETSOCK_ERROR, e->host, e->pos_arg, e); */
        /* cc = sendto(e->snd_socket, (char *)&packet, e->packetlen, 0, (struct sockaddr *)&addr, sizeof(addr)); */
        return cc;
}

static int send_udp_packet(struct s_env *e)
{
        struct s_udp_pkt packet;
        struct sockaddr_in addr;

	ft_memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = e->to->sin_addr.s_addr;
	addr.sin_port = e->port;

        fill_udp_pkt(e, &packet, addr.sin_addr);

	packet.seq = e->seq;
	packet.ttl = e->ttl;
	gettimeofday(&packet.tv, NULL);

        if (setsockopt(e->snd_socket, IPPROTO_IP, IP_TTL, &e->ttl, sizeof(e->ttl)) < 0)
                exit_errors(SETSOCK_ERROR, e->host, e->pos_arg, e);
        return sendto(e->snd_socket, (char *)&packet, e->packetlen, 0, (struct sockaddr *)&addr, sizeof(addr));
}

void send_packet(struct s_env *e)
{
        int cc = 0;

        if (e->proto == IPPROTO_ICMP) {
                e->packetlen = sizeof(struct s_icmp_pkt);
                cc = send_icmp_packet(e);
        } else {
                e->packetlen = sizeof(struct s_udp_pkt);
                cc = send_udp_packet(e);
        }
        if (cc < 0 || cc != e->packetlen)
                fprintf(stderr, "Error sendto, wrote %d, ret=%d\n", e->packetlen, cc);
}
