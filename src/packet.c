/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabelque <aabelque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 11:39:20 by aabelque          #+#    #+#             */
/*   Updated: 2022/01/03 16:26:22 by aabelque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int get_packet(struct s_env *e, int probe)
{
        int cc = 0, hlen = 0;
        u_int8_t type, code;
        char packet[DATA_LEN];
        socklen_t len;
        struct icmp *ficmp, *icmp;
        struct ip *fip, *ip;
        struct udphdr *udp;

        ft_memset(&e->from, 0, sizeof(e->from));
        ft_memset(packet, 0, sizeof(packet));
        len = sizeof(e->from);

        cc = recvfrom(e->rcv_socket, (char *)&packet, sizeof(packet), \
                        0, (struct sockaddr *)&e->from, &len);
        if (cc > 0) {
                fip = (struct ip *)packet;
                hlen = fip->ip_hl << 2;

                ficmp = (struct icmp *)(packet + hlen);
                type = ficmp->icmp_type;
                code = ficmp->icmp_code;

                if ((type == ICMP_TIMXCEED && code == ICMP_TIMXCEED_INTRANS) \
                                || type == ICMP_UNREACH \
                                || type == ICMP_ECHOREPLY) {

                        ip = &ficmp->icmp_ip;
                        hlen = ip->ip_hl << 2;

                        if (ip->ip_p == IPPROTO_ICMP) {
                                icmp = (struct icmp *)((char *)ip + hlen);
                                if (icmp->icmp_seq == e->seq \
                                                && icmp->icmp_id == e->pid)
                                        return type == ICMP_TIME_EXCEEDED ? type : code;
                        } else {
                                udp = (struct udphdr *)((char *)ip + hlen);
                                if (ntohs(udp->uh_dport) == e->port + e->seq \
                                                && ntohs(udp->uh_sport) == e->pid)
                                        return type == ICMP_TIMXCEED ? type : code;
                        }
                }
        }
        return -1;
}

int wait_for_packet(struct s_env *e)
{
        struct timeval tv = {.tv_sec = 3, .tv_usec = 0};

        FD_ZERO(&e->readfs);
        FD_SET(e->rcv_socket, &e->readfs);

        return select(e->rcv_socket + 1, &e->readfs, (fd_set *)0, \
                        (fd_set *)0, &tv);
}

static int send_icmp_packet(struct s_env *e)
{
        struct s_icmp_pkt packet;
        struct sockaddr_in addr;

	ft_memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr = e->to->sin_addr;

        fill_icmp_pkt(e, &packet, addr.sin_addr);

        if (setsockopt(e->snd_socket, IPPROTO_IP, IP_TTL, &e->ttl, \
                                sizeof(e->ttl)) < 0)
                exit_errors(SETSOCK_ERROR, e->host, e->pos_arg, e);
        return sendto(e->snd_socket, (char *)&packet, e->packetlen, 0, \
                        (struct sockaddr *)&addr, sizeof(addr));
}

static int send_udp_packet(struct s_env *e)
{
        struct s_udp_pkt packet;
        struct sockaddr_in addr;

	ft_memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr = e->to->sin_addr;
	addr.sin_port = htons(e->port);

        fill_udp_pkt(e, &packet, addr.sin_addr);

        if (setsockopt(e->snd_socket, IPPROTO_IP, IP_TTL, &e->ttl, \
                                sizeof(e->ttl)) < 0)
                exit_errors(SETSOCK_ERROR, e->host, e->pos_arg, e);
        return sendto(e->snd_socket, (char *)&packet, e->packetlen, \
                        0, (struct sockaddr *)&addr, sizeof(addr));
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
                fprintf(stderr, "Error sendto, wrote %d, ret=%d\n", \
                                e->packetlen, cc);
}
