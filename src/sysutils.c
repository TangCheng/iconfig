/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * sysutils.c
 * Copyright (C) 2014 Watson Xu <xuhuashan@gmail.com>
 *
 * iconfig is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * iconfig is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <glib.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <errno.h>
#include "sysutils.h"

gboolean sysutils_datetime_get_datetime(gchar **str_value)
{
    time_t t;
    struct tm tm1;
    gchar buf[32];

    if (str_value) {
        t = time(NULL);
        if (localtime_r(&t, &tm1)) {
            strftime(buf, sizeof(buf), "%F %T", &tm1);
            *str_value = g_strdup(buf);

            return TRUE;
        }
        *str_value = NULL;
    }

    return FALSE;
}

gboolean sysutils_datetime_set_datetime(gchar *str_value)
{
    time_t timer;
    struct tm tm;

    timer = time(NULL);
    localtime_r(&timer, &tm);
    if (sscanf(str_value, "%d-%d-%d %d:%d:%d",
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec) < 6)
        return FALSE;

    tm.tm_year -= 1900;
    timer = mktime(&tm);
    if (timer == (time_t)-1)
        return FALSE;

    return(stime(&timer) == 0);
}

int sysutils_network_if_indextoname(unsigned int ifindex, char *ifname)
{
    int sockfd;
    struct ifreq ifr;
    int ret;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        return -1;

    ifr.ifr_ifindex = ifindex;
    ret = ioctl(sockfd, SIOCGIFNAME, &ifr);

    if (ret == 0)
        strncpy(ifname, ifr.ifr_name, IFNAMSIZ);

    close(sockfd);

    return 0;
}

int sysutils_network_get_address(const char *ifname,
                                 char **ipaddr,
                                 char **netmask,
                                 char **broadaddr)
{
    struct ifreq ifr;
    int sockfd;
    int ret = 0;

    assert(ifname);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        return -1;

    if (ipaddr) {
        *ipaddr = NULL;
        strcpy(ifr.ifr_name, ifname);
        ret = ioctl(sockfd, SIOCGIFADDR, &ifr);
        if (ret < 0)
            goto fail;
        *ipaddr = strdup(inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
    }

    if (netmask) {
        *netmask = NULL;
        strcpy(ifr.ifr_name, ifname);
        ret = ioctl(sockfd, SIOCGIFNETMASK, &ifr);
        if (ret < 0)
            goto fail;
        *netmask = strdup(inet_ntoa(((struct sockaddr_in*)&ifr.ifr_netmask)->sin_addr));
    }

    if (broadaddr) {
        *broadaddr = NULL;
        strcpy(ifr.ifr_name, ifname);
        ret = ioctl(sockfd, SIOCGIFBRDADDR, &ifr);
        if (ret < 0)
            goto fail;
        *broadaddr = strdup(inet_ntoa(((struct sockaddr_in*)&ifr.ifr_broadaddr)->sin_addr));
    }

    close(sockfd);
    return ret;

fail:
    if (ipaddr) {
        g_free(*ipaddr);
        *ipaddr = NULL;
    }
    if (netmask) {
        g_free(*netmask);
        *netmask = NULL;
    }
    if (broadaddr) {
        g_free(*broadaddr);
        *broadaddr = NULL;
    }

    close(sockfd);
    return ret;
}

int sysutils_network_set_address(const char *ifname,
                                 const char *ipaddr,
                                 const char *netmask,
                                 const char *broadaddr)
{
    struct ifreq ifr;
    struct sockaddr_in *addr_in = (struct sockaddr_in *)&(ifr.ifr_addr);
    int sockfd;
    int ret = -1;

    assert(ifname);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        return -1;

    if (ipaddr) {
        memset(&ifr, 0, sizeof(ifr));
        addr_in->sin_family = AF_INET;
        if (!inet_aton(ipaddr, &addr_in->sin_addr))
            goto finish;
        strcpy(ifr.ifr_name, ifname);
        if ((ret = ioctl(sockfd, SIOCSIFADDR, &ifr)) < 0)
            goto finish;
    }

    if (netmask) {
        memset(&ifr, 0, sizeof(ifr));
        addr_in->sin_family = AF_INET;
        if (!inet_aton(netmask, &addr_in->sin_addr))
            goto finish;
        strcpy(ifr.ifr_name, ifname);
        if ((ret = ioctl(sockfd, SIOCSIFNETMASK, &ifr)) < 0)
            goto finish;
    }

    if (broadaddr) {
        memset(&ifr, 0, sizeof(ifr));
        addr_in->sin_family = AF_INET;
        if (!inet_aton(broadaddr, &addr_in->sin_addr))
            goto finish;
        strcpy(ifr.ifr_name, ifname);
        if ((ret = ioctl(sockfd, SIOCSIFBRDADDR, &ifr)) < 0)
            goto finish;
    }

finish:
    close(sockfd);
    return ret;
}

int sysutils_network_set_gateway(const char *ifname, const char *gwaddr)
{
    int skfd;
    struct rtentry rt;
    struct in_addr addr;
    int err;

    if (!inet_aton(gwaddr, &addr))
        return -1;

    skfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (skfd < 0)
        return -1;

    /* Delete existing defalt gateway */
    memset(&rt, 0, sizeof(rt));

    rt.rt_dst.sa_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = 0;

    rt.rt_genmask.sa_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = 0;

    rt.rt_flags = RTF_UP;

    rt.rt_dev = (gchar *)ifname;

    err = ioctl(skfd, SIOCDELRT, &rt);

    if ((err == 0 || errno == ESRCH) && addr.s_addr) {
        /* Set default gateway */
        memset(&rt, 0, sizeof(rt));

        rt.rt_dst.sa_family = AF_INET;
        ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = 0;

        rt.rt_gateway.sa_family = AF_INET;
        ((struct sockaddr_in *)&rt.rt_gateway)->sin_addr = addr;

        rt.rt_genmask.sa_family = AF_INET;
        ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = 0;

        rt.rt_flags = RTF_UP | RTF_GATEWAY;

        rt.rt_dev = (gchar *)ifname;

        err = ioctl(skfd, SIOCADDRT, &rt);
    }

    close(skfd);

    return err;
}

int sysutils_network_get_gateway(const char *ifname, char **gwaddr)
{
    FILE *fp;
    char buf[256]; // 128 is enough for linux
    char iface[16];
    struct in_addr addr;
    unsigned long int dest_addr, gate_addr;

    *gwaddr = NULL;
    fp = fopen("/proc/net/route", "r");
    if (fp == NULL)
        return -1;
    /* Skip title line */
    fgets(buf, sizeof(buf), fp);
    while (fgets(buf, sizeof(buf), fp)) {
        if (sscanf(buf, "%s\t%lX\t%lX", iface, &dest_addr, &gate_addr) != 3)
            continue;
        if (dest_addr != 0)
            continue;
        if (strcmp(ifname, iface) != 0)
            continue;
        addr.s_addr = gate_addr;
        *gwaddr = strdup(inet_ntoa(addr));
        break;
    }

    fclose(fp);
    return 0;
}

int sysutils_network_set_dns(const char *ifname, const char **dns, int size)
{
    FILE *fp;
    char buf[128];
    int i;

    if (access(PACKAGE_SYSCONFDIR, F_OK) < 0)
        mkdir(PACKAGE_SYSCONFDIR, 0775);

    fp = fopen(PACKAGE_SYSCONFDIR "/resolv.conf", "w+");
    if (fp == NULL)
        return -1;

    fputs("# Generated by IConfig\n", fp);
    for (i = 0; i < size; i++) {
        if (dns[i] == NULL || strlen(dns[i]) == 0)
            continue;
        snprintf(buf, sizeof(buf), "nameserver %s\n", dns[i]);
        fputs(buf, fp);
    }

    fclose(fp);
    return i;
}
