/*
 *
 * Compilation - 
 *
 *   # gcc -Wall -g -o rawping rawping.c -lpthread 
 *
 * How to run -
 * If you test DHCP client transaction for "wlan0" interface 
 *
 *   # sudo ifconfig wlan0 0.0.0.0   <-- Taking "wlan0" out of networking interfaces. 
 *   # sudo ./rawping wlan0          <-- Just see that it can print out elapsed time.
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>
#include <pthread.h>

/*
 * ---------------------------------------------------------------
 *
 */
static int debug = 1;

#define dprintf(fmt,args...)	{if (debug) printf(fmt,##args);}

#define DHCP_DISCOVER	0x1
#define DHCP_OFFER	0x2
#define DHCP_REQUEST	0x3
#define DHCP_ACK	0x5

/* SERVER PORT & CLIENT PORT */
#define DHCP_SPORT	67
#define DHCP_CPORT	68

//
// DHCP Options TLV Template ...
//
static int print_u8(void *container,  char *data);
static int print_u32(void *container, char *data);
static int print_dn(void *container,  char *data);
static int print_dns(void *container, char *data);
static int print_host(void *container,char *data);

static int set_u8(void *container,   char *data);
static int set_u32(void *container,  char *data);
static int set_dn(void *container,   char *data);
static int set_dns(void *container,  char *data);
static int set_string(void *container, char *data);
static int set_parm(void *container, char *data);

static void dhcp_send_message( void * container, int code );
static int  ping_req_message( void * container );
static int  arp_req_message( void * container );

#define TRANSID		0xBABEFACE

/* DO NOT CHANGE THIS */
#define DHCP_COOKIE	0x63825363

typedef struct {
	int code;
	int len;
	char * title; 
	int (*pfunc)(void *, char *);
	int (*sfunc)(void *, char *);
}__attribute__((aligned)) dhcp_tlv_t;
static dhcp_tlv_t dhcp_tlv[] = 
	{
		{ 53,	sizeof(u_int8_t)  , "type"  , print_u8  , set_u8  }, /* Message Type */
		{  1,	sizeof(u_int32_t) , "subnet", print_u32 , set_u32 }, /* Subnet */
		{ 55,	-1   ,              "param" , NULL,       set_parm}, /* Req Params */
		{ 58,	sizeof(u_int32_t) , "renew" , print_u32 , set_u32 }, /* Renewal Time */
		{ 59,	sizeof(u_int32_t) , "rebind", print_u32 , set_u32 }, /* Rebind Time */
		{ 51,   sizeof(u_int32_t) , "lease",  print_u32 , set_u32 }, /* IP Lease Time */
		{ 50,   sizeof(u_int32_t) , "reqip",  print_u32 , set_u32 }, /* Requested IP */
		{ 54,   sizeof(u_int32_t) , "server", print_u32 , set_u32 }, /* Server ID */
		{  3,   sizeof(u_int32_t) , "gw",     print_u32 , set_u32 }, /* Gateway */
		{ 15,   -1  ,               "domain", print_dn  , set_dn  }, /* Domain Name */
		{  6,   -1  ,               "dns",    print_dns , set_dns }, /* Domain Name Serves */
		{ 12,	-1  ,               "host",   print_host, set_string}, /* Host name */
		{255,   0   ,               NULL,     NULL, set_u8 },      /* END */
	};

static __inline__ dhcp_tlv_t * dhcp_tlv_info( u_int8_t code )
{
	int index;

	for (index = 0; dhcp_tlv[ index ].code != 255; index++ ) {
		if (dhcp_tlv[ index ].code == code) {
			return &dhcp_tlv[ index ];
		}
	}

	return NULL;
}

// broadcast address
static u_int8_t broad_addr[ ETH_ALEN ] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

//
// DHCP Options TLV ...
//
typedef struct {
	u_int8_t t;    /* type */
	u_int8_t l;    /* length */
	u_int8_t v[0]; /* value */
}__attribute__((packed)) tlv_t;

//
// Ethernet Header 
//
struct eth_header {
	u_int8_t daddr[ETH_ALEN];
	u_int8_t saddr[ETH_ALEN];
	u_int16_t type;
}__attribute__((packed));

//
// DHCP message ...
//
typedef struct {
        u_int8_t	op;
        u_int8_t	htype;
        u_int8_t	hlen;
        u_int8_t	hops;
        u_int32_t	xid;
        u_int16_t	secs;
        u_int16_t	flags;
        u_int32_t	ciaddr;
        u_int32_t	yiaddr;
        u_int32_t	siaddr;
        u_int32_t	giaddr;
        u_int8_t	chaddr[16];
        u_int8_t	sname[64];
        u_int8_t	file[128];
        u_int32_t	cookie;
        u_int8_t	options[0]; /* options */
}__attribute__((packed)) dhcpmsg_t;

typedef struct {
	pthread_t recv; // Packet receiver
	int done;
	int stage;	// Current DHCP stage
	int sock;       // DHCP socket 
	u_int32_t xid;	// transaction id
	u_int8_t hwaddr[ ETH_ALEN ]; // MAC 
	u_int32_t netmask;	// netmask
	u_int32_t router;	// router 
	u_int32_t server;	// DHCP server 
	u_int32_t givenip;	// Offered IP address
	u_int32_t renew;	// renew interval
	u_int32_t rebind;	// rebind interval
	u_int32_t lease;	// lease time for IP
	u_int8_t  dname[ 64 ];  // Domain Name
	u_int32_t dns[ 4 ];	// DNS IPs
	u_int8_t hostname[ 64 ]; // hostname 
}__attribute__((aligned)) dhcp_t;

typedef struct {
	pthread_t recv; // Packet receiver
	int done;
	int sock;       	// ICMP socket 
	u_int32_t ip;		// renew interval
	u_int32_t netmask;	// rebind interval
	u_int32_t dip;		// destination IP
	u_int8_t hwaddr[ ETH_ALEN ]; // MAC 
	u_int8_t ghwaddr[ ETH_ALEN ]; // MAC 
	u_int16_t id;		// ICMP ID
	u_int16_t seq;		// sequence number 
	u_int16_t replied;	// Reply counter
	u_int32_t req_time;	// Req  time
	u_int32_t resp_time;	// Resp time
	u_int32_t latency;	// (Resp time - Req time) ~ average
	u_int32_t tot_time;	// Total time
}__attribute__((aligned)) ping_t;

#define PINGIPID	0x1234
#define ICMPID		0xbabe

typedef struct {
	int done;
	int sock;       	// ARP socket 
	u_int32_t gip;		// gateway IP
	u_int32_t ip;		// my IP
	u_int8_t hwaddr[ ETH_ALEN ]; // MAC 
	u_int8_t ghwaddr[ ETH_ALEN ]; // MAC 
}__attribute__((aligned)) arp_t;

#define ARP_FAILURE 5

// current time 
static __inline__ unsigned int current_time( void )
{
	struct timeval tv;

	gettimeofday( &tv, NULL );
	return (unsigned int)( (tv.tv_sec*1000000 + tv.tv_usec) );
}

/*
 * ---------------------------------------------------------------
 *
 */
static int allproto_socket( char * ifname )
{
	int sock;
	struct ifreq ifr;
	struct sockaddr_ll addr;
	int n = 1;

	if (!ifname)
		return -1;

	sock = socket( AF_PACKET, SOCK_RAW, htons(ETH_P_ALL) );
	if (sock < 0) {
		fprintf(stderr,"socket( PF_INET, SOCK_RAW , ..)::failed\n");
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &n,
                       sizeof(n)) == -1) {
		close(sock);
                fprintf(stderr,"setsockopt[SOL_SOCKET,SO_REUSEADDR]\n");
                return -1;
        }

        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &n,
                       sizeof(n)) == -1) {
		close(sock);
                fprintf(stderr,"setsockopt[SOL_SOCKET,SO_BROADCAST]\n");
                return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_ifrn.ifrn_name, ifname, IFNAMSIZ);
        if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *) &ifr, sizeof(ifr)) < 0) {
		close(sock);
                fprintf(stderr,"setsockopt[SOL_SOCKET,SO_BINDTODEVICE]\n");
                return -1;
        }

	if ( ioctl(sock, SIOCGIFFLAGS, &ifr) ) {
		close(sock);
		fprintf(stderr,"ioctl(SIOCGIFFLAGS)::failed\n");
		return -1;
	}
	ifr.ifr_flags |= IFF_UP | IFF_BROADCAST | IFF_NOTRAILERS | IFF_RUNNING;
	if ( ioctl(sock, SIOCSIFFLAGS, &ifr) ) {
		close(sock);
		fprintf(stderr,"ioctl(SIOCSIFFLAGS)::failed\n");
		return -1;
	}

	memset( &addr, 0, sizeof(addr) );

	if ( ioctl(sock, SIOCGIFINDEX, &ifr) ) {
		close(sock);
		fprintf(stderr,"ioctl(SIOCGIFINDEX)::failed\n");
		return -1;
	}
	addr.sll_family = AF_PACKET;
	addr.sll_protocol = 0;
	addr.sll_ifindex = ifr.ifr_ifindex;
     
     	if (bind(sock, (void *)&addr, sizeof(addr)) == -1) {
		close(sock);
                fprintf(stderr,"bind::failed\n");
                return -1;
        }

	return sock;
}

static void * dhcprecv( void * param )
{
	int done = 0;
	dhcp_t *dhcp = (dhcp_t *)param;
	#define PKTSZ 2048
	char * buf; // 4K bytes buffer;
	int len, option_len;
	char *trace;
	tlv_t *tlv;
	dhcp_tlv_t *tlv_info;
	struct sockaddr addr;
	int addrlen;
	int flags;
	fd_set rfds;
	struct timeval tv;
	struct eth_header *ethh;
	struct ip *iph;
	struct udphdr *udph;
	dhcpmsg_t *msg;

	if (!dhcp)
		return NULL;

	buf = (char *)malloc( PKTSZ );
	if (!buf)
		return NULL;

	// make socket to be non-block
	flags = fcntl( dhcp->sock, F_GETFL );
	fcntl(dhcp->sock, F_SETFL, flags | O_NONBLOCK);

	while (!done) {
		FD_ZERO(&rfds);
		FD_SET(dhcp->sock,&rfds);

		tv.tv_sec = 1; // every 1 sec.
		tv.tv_usec = 0;

		int ret = select(dhcp->sock+1,&rfds,NULL,NULL,&tv);
		if (ret == -1) {
			fprintf(stderr,"socket error\n");
			done = 1;
			continue;
		} else if (ret == 0) {
			continue; // timeout 
		} else {
			// data arrival !!
		}

		addrlen = sizeof(addr);
		len = recvfrom(dhcp->sock, buf, PKTSZ, 0, &addr, (socklen_t *)&addrlen );
		if (len < 0) {
			fprintf(stderr,"recv(sock=%d,)::error\n",dhcp->sock);
			done = 1;
			continue;
		}

		// DHCP packet smallest size 
		if (len < (sizeof(struct eth_header) + sizeof(struct ip) + sizeof(struct udphdr) + sizeof(dhcpmsg_t))) {
			fprintf(stderr,"received bytes is %d \n",len);
			continue;
		}

		// Ethernet Header Check
		ethh = (struct eth_header *)buf;
		if (ntohs(ethh->type) != ETHERTYPE_IP)
	       		continue;

		// IP Header Check 
		iph  = (struct ip *)((char *)ethh + sizeof(struct eth_header));
		if (iph->ip_p != IPPROTO_UDP)
	       		continue;

		// UDP Header Check 
		udph = (struct udphdr *)((char *)iph + sizeof(struct ip));
		if ((ntohs(udph->uh_sport) != DHCP_SPORT) || 
				(ntohs(udph->uh_dport) != DHCP_CPORT))
			continue;

		/* DHCP */
		msg = (dhcpmsg_t *)((char *)udph + sizeof(struct udphdr));

		/* CONDITION #1 */
		if (msg->op != 2 /* REPLY */ ||
				msg->htype != 1 /* ETHER */ ||
				msg->hlen != ETH_ALEN /* 6 */ ) {
			fprintf(stderr,"Message is wrong %d/%d/%d \n", 
						msg->op, msg->htype, msg->hlen );
			continue;
		}

		/* CONDITION #2 */
		if (ntohl(msg->xid) != dhcp->xid) {
			fprintf(stderr,"Wrong XID %08x \n", ntohl(msg->xid));
			continue;
		}

		/* CONDITION #3 */
		if (ntohl(msg->yiaddr) == 0x0) {
			fprintf(stderr,"IP address is empty \n");
			continue;
		}

		if (dhcp->givenip) {
			if (dhcp->givenip != ntohl( msg->yiaddr)) {
				dprintf("NEW IP ADDRESS : %d.%d.%d.%d -> %d.%d.%d.%d \n",
						((dhcp->givenip & 0xFF000000) >> 24) & 0xFF,
						((dhcp->givenip & 0x00FF0000) >> 16) & 0xFF,
						((dhcp->givenip & 0x0000FF00) >> 8 ) & 0xFF,
						((dhcp->givenip & 0x000000FF) >> 0 ) & 0xFF,
						((ntohl(msg->yiaddr) & 0xFF000000) >> 24) & 0xFF,
						((ntohl(msg->yiaddr) & 0x00FF0000) >> 16) & 0xFF,
						((ntohl(msg->yiaddr) & 0x0000FF00) >> 8 ) & 0xFF,
						((ntohl(msg->yiaddr) & 0x000000FF) >> 0 ) & 0xFF );
			}
		}
	
		// new IP address
		dhcp->givenip = ntohl( msg->yiaddr );

		//
		// Analysing DHCP options 
		//
		trace = (char *)&(msg->options[0]);
		do {
			tlv = (tlv_t *)trace;
			tlv_info = dhcp_tlv_info( tlv->t );
			if (!tlv_info)
				break;
			option_len = 0;
			if (tlv_info->pfunc)
				option_len = tlv_info->pfunc( (void *)dhcp, (char *)tlv );
			trace += (option_len + sizeof(tlv_t));
		} while (1);

		//
		// Responding to current message 
		//
		switch (dhcp->stage) {
		case DHCP_OFFER:
			dprintf("Recv DHCP Message(%d) %d bytes (%s) \n", len, dhcp->stage, strerror(errno) );

			//
			// Send Request message ...
			//
			dhcp_send_message( dhcp, DHCP_REQUEST );
			break;
		case DHCP_ACK:
			dprintf("Recv DHCP Message(%d) %d bytes (%s) \n", len, dhcp->stage, strerror(errno) );
			done = 1; /* DONE !! */
			dhcp->stage = 0xFF;  /* SUCCESS INDICATION */
			break;
		default:
			break;
		}
	}

	free( buf );

	dhcp->done = 1; // termination signal

	return NULL;
}

/* IP header checksum */
static u_int16_t in_csum(u_int8_t *bytes,int sz)
{
        u_int32_t chksum = 0;
	u_int16_t *p = (u_int16_t *)bytes;
	int i;

	for (i = 0; i < (sz/sizeof(u_int16_t)); i++)
		chksum += *p++ ;

	if (sz % sizeof(u_int16_t))
		chksum += *(unsigned char *)p;

        chksum = (chksum >> 16) + (chksum & 0xFFFF);
        chksum = (chksum >> 16) + chksum;
        chksum = ~chksum;

        return (u_int16_t)chksum;
}

static void dhcp_send_message( void * container, int code )
{
	dhcp_t *dhcp = (dhcp_t *)container;
	dhcpmsg_t *dhcpmsg;
	#define SENDSZ 2048
	char *buf;
	char *trace, *start;
	dhcp_tlv_t *tlv_info;
	tlv_t *tlv;
	int dhcp_len;
	int txlen;
	int ret;
	struct ip * iph;
	struct udphdr * udph;
	struct eth_header *ethh;

	if (!dhcp)
		return ;

	buf = (char *)malloc( SENDSZ );
	if (!buf)
		return ;

	memset(buf,0,SENDSZ);

	// ethernet header
	ethh = (struct eth_header *)buf;
	memcpy( ethh->daddr ,  broad_addr,  ETH_ALEN );
	memcpy( ethh->saddr , dhcp->hwaddr, ETH_ALEN );
	ethh->type = htons( ETHERTYPE_IP );

	// IP header 
	iph = (struct ip *)((char *)ethh + sizeof(struct eth_header));
	iph->ip_hl = sizeof(struct ip)/4; // Header length 
	iph->ip_v = 4; 			// IPv4
	iph->ip_tos = 0; 		// DSCP:CS0
	iph->ip_len = 0; 		// TO BE UPDATED
	iph->ip_id = 0; 		// It should be 0 for DHCP traffic 
	iph->ip_off = htons( IP_DF ); 	// do not fragment 
	iph->ip_ttl = 64; 		// usually
	iph->ip_p = IPPROTO_UDP;	// UDP 
	iph->ip_sum = 0;  		// TO BE UPDATED 
	iph->ip_src.s_addr = 0;		// TO BE UPDATED 
	iph->ip_dst.s_addr = 0; 	// TO BE UPDATED 

	// UDP header 
	udph = (struct udphdr *)((char *)iph + sizeof(struct ip));
	udph->uh_sport = htons( DHCP_CPORT );
	udph->uh_dport = htons( DHCP_SPORT );
	udph->uh_ulen  = 0;	// TO BE UPDATED 
	udph->uh_sum   = 0;	// TO BE UPDATED 

	dhcpmsg = (dhcpmsg_t *)((char *)udph + sizeof(struct udphdr));
	dhcpmsg->op = 1; /* Request */
      	dhcpmsg->htype = 1;
	dhcpmsg->hlen = ETH_ALEN;	
	dhcpmsg->xid = htonl( dhcp->xid );
	memcpy( dhcpmsg->chaddr, dhcp->hwaddr, ETH_ALEN );
	dhcpmsg->cookie = htonl( DHCP_COOKIE ); 

	// change stage 
	dhcp->stage = code; 

	trace = 
	start = (char *)&(dhcpmsg->options[0]);

	//
	// DHCP Option Fields...
	//
	switch( code ) {
	case DHCP_DISCOVER:

		// Message Type 
		tlv_info = dhcp_tlv_info( 53 );
		tlv = (tlv_t *)trace;
		tlv->t = tlv_info->code;
		tlv->l = tlv_info->sfunc( dhcp, (char *)tlv );
		trace += sizeof(tlv_t) + tlv->l;
		
		// Host name 
		memset( &dhcp->hostname, 0, 64 );
		if (gethostname( (char *)&dhcp->hostname, 64 ) != 0) {
			fprintf(stderr,"gethostname()::failed\n");
			free(buf);
			return ;
		}

		// Hostname 
		tlv_info = dhcp_tlv_info( 12 );
		tlv = (tlv_t *)trace;
		tlv->t = tlv_info->code;
		tlv->l = tlv_info->sfunc( dhcp, (char *)tlv );
		trace += sizeof(tlv_t) + tlv->l;

		// Param Request List 
		tlv_info = dhcp_tlv_info( 55 );
		tlv = (tlv_t *)trace;
		tlv->t = tlv_info->code;
		tlv->l = tlv_info->sfunc( dhcp, (char *)tlv );
		trace += sizeof(tlv_t) + tlv->l;

		// Param Request List 
		tlv = (tlv_t *)trace;
		tlv->t = 255;
		tlv->l = 0;
		trace += 1; /* exception case */

		break;

	case DHCP_REQUEST:

		// Message Type 
		tlv_info = dhcp_tlv_info( 53 );
		tlv = (tlv_t *)trace;
		tlv->t = tlv_info->code;
		tlv->l = tlv_info->sfunc( dhcp, (char *)tlv );
		trace += sizeof(tlv_t) + tlv->l;
		
		// Hostname 
		tlv_info = dhcp_tlv_info( 12 );
		tlv = (tlv_t *)trace;
		tlv->t = tlv_info->code;
		tlv->l = tlv_info->sfunc( dhcp, (char *)tlv );
		trace += sizeof(tlv_t) + tlv->l;

		// Requested IP Address 
		tlv_info = dhcp_tlv_info( 50 );
		tlv = (tlv_t *)trace;
		tlv->t = tlv_info->code;
		tlv->l = tlv_info->sfunc( dhcp, (char *)tlv );
		trace += sizeof(tlv_t) + tlv->l;

		// Server 
		tlv_info = dhcp_tlv_info( 54 );
		tlv = (tlv_t *)trace;
		tlv->t = tlv_info->code;
		tlv->l = tlv_info->sfunc( dhcp, (char *)tlv );
		trace += sizeof(tlv_t) + tlv->l;

		// Param Request List 
		tlv = (tlv_t *)trace;
		tlv->t = 255;
		tlv->l = 0;
		trace += 1; /* exception case */

		break;
	}

	//
	// total length 
	//
	dhcp_len = (int)(trace - start) + (int)sizeof(dhcpmsg_t);

	//
	// 32 bytes alignment
	//
	dhcp_len &= ~(32-1);
	dhcp_len += 32;

	//
	// UDP header
	//
	udph->uh_ulen  = htons( dhcp_len );
	udph->uh_sum   = 0; //ignore checksum 

	//
	// IP header
	//
	iph->ip_len    = htons( dhcp_len + sizeof(struct udphdr) + sizeof(struct ip) );
	// source IP address
	iph->ip_src.s_addr = htonl( dhcp->givenip ? dhcp->givenip : 0 );	
	// destination IP address
	iph->ip_dst.s_addr = htonl( dhcp->server ? dhcp->server : 0xFFFFFFFF );	
	// IP header checksum
	iph->ip_sum    = in_csum((u_int8_t *)iph,(int)sizeof(struct ip));

	// TX length 
	txlen = dhcp_len + sizeof(struct udphdr) + sizeof(struct ip) + sizeof(struct eth_header);

	/* DHCP Packet */
	ret = send( dhcp->sock , buf, txlen, 0 );
	dprintf("Send DHCP Message(%d) %d bytes - %s/%s \n", txlen, dhcp->stage, (ret == txlen) ? "OK" : "FAIL", strerror(errno) );

	free( buf );
}

static __inline__ void print_tlv( tlv_t *tlv )
{
	int i;
	char *p = (char *)&(tlv->v[0]);

	dprintf("[ %d %d ] : ",tlv->t,tlv->l);
	for (i = 0; i < tlv->l; i++ ) {
		dprintf("%02x ",*p++ & 0xFF);
		if ((i % 32) == 31)
			dprintf("\n");
	}
	dprintf("\n");
}

static int print_u8(void *container,  char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	dhcp_t *dhcp = (dhcp_t *)container;

	if (!tlv)
		return 0;
	//print_tlv( tlv );

	switch (tlv->t) {
	case 53:
		dhcp->stage = *(char *)&(tlv->v[0]);  // update stage 
		dprintf(" >> DHCP STAGE --> %d \n",dhcp->stage);
		break;
	}

	return tlv->l;
}

static int print_u32(void *container, char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	dhcp_t *dhcp = (dhcp_t *)container;

	if (!tlv)
		return 0;
	//print_tlv( tlv );

	switch (tlv->t) {
	case 1:  // network mask 
		dhcp->netmask = ntohl( *(u_int32_t *)&(tlv->v[0]) ) ;  
		dprintf(" >> NETMASK --> %d.%d.%d.%d \n",
				((dhcp->netmask & 0xFF000000) >> 24 ) & 0xFF ,
				((dhcp->netmask & 0x00FF0000) >> 16 ) & 0xFF ,
				((dhcp->netmask & 0x0000FF00) >> 8  ) & 0xFF ,
				((dhcp->netmask & 0x000000FF) >> 0  ) & 0xFF 
			);
		break;
	case 3:  // router 
		dhcp->router = ntohl( *(u_int32_t *)&(tlv->v[0]) ) ;  
		dprintf(" >> ROUTER --> %d.%d.%d.%d \n",
				((dhcp->router & 0xFF000000) >> 24 ) & 0xFF ,
				((dhcp->router & 0x00FF0000) >> 16 ) & 0xFF ,
				((dhcp->router & 0x0000FF00) >> 8  ) & 0xFF ,
				((dhcp->router & 0x000000FF) >> 0  ) & 0xFF 
			);
		break;
	case 54:  // Server ID
		dhcp->server = ntohl( *(u_int32_t *)&(tlv->v[0]) ) ;  
		dprintf(" >> SERVER --> %d.%d.%d.%d \n",
				((dhcp->server & 0xFF000000) >> 24 ) & 0xFF ,
				((dhcp->server & 0x00FF0000) >> 16 ) & 0xFF ,
				((dhcp->server & 0x0000FF00) >> 8  ) & 0xFF ,
				((dhcp->server & 0x000000FF) >> 0  ) & 0xFF 
			);
		break;
	case 58: // renew 
		dhcp->renew = ntohl( *(u_int32_t *)&(tlv->v[0]) ) ;  
		dprintf(" >> RENEW --> %d sec\n", dhcp->renew );
		break;
	case 59: // rebind
		dhcp->rebind = ntohl( *(u_int32_t *)&(tlv->v[0]) ) ;  
		dprintf(" >> REBIND --> %d sec\n", dhcp->rebind );
		break;
	case 51: // leasse 
		dhcp->lease = ntohl( *(u_int32_t *)&(tlv->v[0]) ) ;  
		dprintf(" >> LEASE --> %d sec\n", dhcp->lease );
		break;
	}

	return tlv->l;
}

static int print_dns(void *container,  char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	dhcp_t *dhcp = (dhcp_t *)container;

	if (!tlv)
		return 0;
	//print_tlv( tlv );

	switch (tlv->t) {
	case 6:  // domain name servers 
		{
		int i, cnt = 0;
	        u_int32_t * p = (u_int32_t *)&(tlv->v[0]);

		while (((cnt*sizeof(u_int32_t)) < tlv->l) && 
				(cnt < 4))
			dhcp->dns[ cnt++ ]= ntohl( *p++ );

		dprintf(" >> DNS --> ");
		for (i = 0; i < cnt; i++) 
			dprintf("%d.%d.%d.%d ",
				((dhcp->dns[ i ] & 0xFF000000) >> 24 ) & 0xFF ,
				((dhcp->dns[ i ] & 0x00FF0000) >> 16 ) & 0xFF ,
				((dhcp->dns[ i ] & 0x0000FF00) >> 8  ) & 0xFF ,
				((dhcp->dns[ i ] & 0x000000FF) >> 0  ) & 0xFF 
			);
		dprintf("\n");

		}
		break;
	default:
		break;
	}

	return tlv->l;
}

static int print_dn(void *container, char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	dhcp_t *dhcp = (dhcp_t *)container;

	if (!tlv)
		return 0;
	//print_tlv( tlv );

	switch (tlv->t) {
	case 15:  // network mask
		{
		int cnt = 0;
	        u_int8_t * p = (u_int8_t *)&(tlv->v[0]);

		while ((cnt < tlv->l) && (cnt < 63)) 
			dhcp->dname[ cnt++ ] = *p++;
		dhcp->dname[ cnt ] = 0x0;
		dprintf(" >> DOMAIN --> %s \n", dhcp->dname);
		}
		break;
	default:
		break;
	}

	return tlv->l;
}

static int print_host(void *container, char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	if (!tlv)
		return 0;
	print_tlv( tlv );
	return tlv->l;
}

static int set_u8(void *container,  char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	dhcp_t *dhcp = (dhcp_t *)container;

	if (!tlv || !dhcp)
		return 0;

	switch(tlv->t) {
	case 53:
		*(u_int8_t *)&(tlv->v[0]) = dhcp->stage;
		break;
	case 255:
		return 0;
	default:
		break;
	}

	return sizeof(u_int8_t);
}

static int set_u32(void *container, char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	dhcp_t *dhcp = (dhcp_t *)container;

	if (!tlv)
		return 0;
	//print_tlv( tlv );

	switch (tlv->t) {
	case 50:  // Requested IP address
		*(u_int32_t *)&(tlv->v[0]) = htonl( dhcp->givenip );
		break;
	case 54:  // Server Identifier
		*(u_int32_t *)&(tlv->v[0]) = htonl( dhcp->server );
		break;
	default:
		break;
	}

	return sizeof(u_int32_t);
}

static int set_dn(void *container,  char *data)
{
	return 0;
}

static int set_dns(void *container, char *data)
{
	return 0;
}

static int set_string(void *container, char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	dhcp_t *dhcp = (dhcp_t *)container;
	int len;

	if (!tlv || !dhcp)
		return 0;

	switch(tlv->t) {
	case 12:
		strcpy( (char *)((u_int8_t *)&(tlv->v[0])), (char *)dhcp->hostname );
		len = strlen( (char *)dhcp->hostname );
		break;
	default:
		break;
	}

	return len;
}

static int set_parm(void *container, char *data)
{
	tlv_t *tlv = (tlv_t *)data;
	dhcp_t *dhcp = (dhcp_t *)container;
	int len;
	u_int8_t *p;

	if (!tlv || !dhcp)
		return 0;

	switch(tlv->t) {
	case 55:
		p  = (u_int8_t *)&(tlv->v[0]);
		*p++ = 1;  // subnet mask
		*p++ = 28; // broadcast address
		*p++ = 3;  // router 
		*p++ = 15; // domain name 
		*p++ = 6;  // dns 
		*p++ = 12; // hostname 
		len = 6;
		break;
	default:
		break;
	}

	return len;
}

static int ping_req_message( void * container )
{
	ping_t *ping = (ping_t *)container;
	struct icmp *icmp;
	#define SENDSZ 2048
	char *buf;
	char *trace, *start;
	struct eth_header *ethh;
	struct ip * iph;
	int icmp_len;
	int txlen;
	int ret;

	if (!ping)
		return -1;

	buf = (char *)malloc( SENDSZ );
	if (!buf)
		return -1;

	memset(buf,0,SENDSZ);

	// ethernet header
	ethh = (struct eth_header *)buf;
	memcpy( ethh->daddr , ping->ghwaddr,   ETH_ALEN );
	memcpy( ethh->saddr , ping->hwaddr , ETH_ALEN );
	ethh->type = htons( ETHERTYPE_IP );

	// IP header 
	iph = (struct ip *)((char *)ethh + sizeof(struct eth_header));
	iph->ip_hl = sizeof(struct ip)/4; // Header length 
	iph->ip_v = 4; 			// IPv4
	iph->ip_tos = 0; 		// DSCP:CS0
	iph->ip_len = 0; 		// TO BE UPDATED
	iph->ip_id = 0; 		// Specific ... 
	iph->ip_off = htons( IP_DF ); 	// do not fragment 
	iph->ip_ttl = 64; 		// usually
	iph->ip_p = IPPROTO_ICMP;	// ICMP 
	iph->ip_sum = 0;  		// TO BE UPDATED 
	iph->ip_src.s_addr = htonl( ping->ip ) ;   // source IP	
	iph->ip_dst.s_addr = htonl( ping->dip );   // destination IP

	// ICMP header 
	icmp = (struct icmp *)((char *)iph + sizeof(struct ip));
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0; // TO BE UPDATED 
	icmp->icmp_id  = htons( ping->id );
	icmp->icmp_seq = htons( ping->seq ); 

	trace = start = (char *)((char *)icmp + sizeof(struct icmp));

	// data copy
	memset( trace, 0xAE, 64 ); // 64 bytes 
	trace += 64;

	//
	// total length 
	//
	icmp_len = (int)(trace - start) + (int)sizeof(struct icmp);

	//
	// 16 bytes alignment
	//
	icmp_len &= ~(16-1);
	icmp_len += 16;

	icmp->icmp_cksum = in_csum((u_int8_t *)icmp,icmp_len);  // ICMP checksum 

	// IP header
	//
	
	iph->ip_len    = htons( icmp_len + sizeof(struct ip) ); // IP header length 
	iph->ip_sum    = in_csum((u_int8_t *)iph,(int)sizeof(struct ip)); // IP header checksum

	// TX length 
	txlen = icmp_len + sizeof(struct ip) + sizeof(struct eth_header);

	/* DHCP Packet */
	ret = send( ping->sock , buf, txlen, 0 );
	dprintf("Send ICMP ECHO REQ Message %d bytes (%d) %s (%s) \n", txlen, ping->sock, (ret == txlen)? "OK": "FAIL", strerror(errno) );

	// Just system time 
	ping->req_time = current_time();

	free( buf );

	return 0;
}

static int get_hwaddr(int sock,char *ifname,char *macaddr)
{
	struct ifreq ifr;

	if (!ifname || !macaddr)
		return -1;

	memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
        if (ioctl(sock, SIOCGIFHWADDR, &ifr) != 0) {
                fprintf(stderr, "ioctl(SIOCGIFHWADDR)::failed");
                return -1;
        }

        if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER) {
                fprintf(stderr, "Invalid HW-addr family 0x%04x\n", ifr.ifr_hwaddr.sa_family);
                return -1;
        }

        memcpy( macaddr, ifr.ifr_hwaddr.sa_data, ETH_ALEN );

	return 0;
}

static void * pingrecv( void * param )
{
	ping_t *ping = (ping_t *)param;
	#define PKTSZ 2048
	char * buf; // 4K bytes buffer;
	int len;
	struct sockaddr addr;
	int addrlen;
	int flags;
	fd_set rfds;
	struct timeval tv;
	struct eth_header *ethh;
	struct ip *iph;
	struct icmp *icmp;

	if (!ping)
		return NULL;

	buf = (char *)malloc( PKTSZ );
	if (!buf)
		return NULL;

	// make socket to be non-block
	flags = fcntl( ping->sock, F_GETFL );
	fcntl(ping->sock, F_SETFL, flags | O_NONBLOCK);

	/* init time */
	ping->tot_time = 0;
	ping->latency = 0;

	while (!ping->done) {
		FD_ZERO(&rfds);
		FD_SET(ping->sock,&rfds);

		tv.tv_sec = 1; // every 1 sec.
		tv.tv_usec = 0;

		int ret = select(ping->sock+1,&rfds,NULL,NULL,&tv);
		if (ret == -1) {
			fprintf(stderr,"socket error\n");
			ping->done = 1;
			continue;
		} else if (ret == 0) {
			continue; // timeout 
		} else {
			// data arrival !!
		}

		addrlen = sizeof(addr);
		len = recvfrom(ping->sock, buf, PKTSZ, 0, &addr, (socklen_t *)&addrlen );
		if (len < 0) {
			fprintf(stderr,"recv(sock=%d,)::error\n",ping->sock);
			ping->done = 1;
			continue;
		}

		// Ping response packet smallest size 
		if (len < (sizeof(struct eth_header) + sizeof(struct ip) + sizeof(struct icmp))) {
			fprintf(stderr,"received bytes is %d \n",len);
			continue;
		}

		// Ethernet Header Check
		ethh = (struct eth_header *)buf;
		if (ntohs(ethh->type) != ETHERTYPE_IP)
	       		continue;

		// IP Header Check 
		iph  = (struct ip *)((char *)ethh + sizeof(struct eth_header));
		if (iph->ip_p != IPPROTO_ICMP)
	       		continue;

		/* ICMP */
		icmp = (struct icmp *)((char *)iph + sizeof(struct ip));

		/* Check */
		if ((ntohs(icmp->icmp_id) != ping->id) ||
				(ntohs(icmp->icmp_seq) != ping->seq)) 
			continue;

		dprintf("Send ICMP ECHO RESP %d bytes (%s) \n", len, strerror(errno) );

		ping->replied ++;

		/* statistics */
		ping->resp_time = current_time();

		ping->tot_time += (ping->resp_time - ping->req_time);
		ping->latency = ping->tot_time / ping->replied;

	}

	free( buf );

	return NULL;
}

static int arp_req_message( void * container )
{
	arp_t *arp = (arp_t *)container;
	#define SENDSZ 2048
	char *buf;
	struct eth_header *ethh;
	struct ether_arp *arph;
	int arp_len;
	int txlen;
	int ret;
	u_int32_t *v;

	if (!arp)
		return -1;

	buf = (char *)malloc( SENDSZ );
	if (!buf)
		return -1;

	memset(buf,0,SENDSZ);

	// ethernet header
	ethh = (struct eth_header *)buf;
	memcpy( ethh->daddr , broad_addr,  ETH_ALEN );
	memcpy( ethh->saddr , arp->hwaddr, ETH_ALEN );
	ethh->type = htons( ETHERTYPE_ARP );

	// ARP header 
	arph = (struct ether_arp *)((char *)ethh + sizeof(struct eth_header));
	arph->arp_hrd = htons( 1 );  /* Ethernet */
	arph->arp_pro = htons( ETHERTYPE_IP );
	arph->arp_hln = ETH_ALEN;
	arph->arp_pln = 4;
	arph->arp_op  = htons( 1 ); /* Request */

	/* MAC & IP address */
	memcpy( arph->arp_sha , arp->hwaddr, ETH_ALEN );
	v = (u_int32_t *)&(arph->arp_spa[0]);
	*v = htonl( arp->ip );
	memset( arph->arp_tha , 0 , ETH_ALEN );
	v = (u_int32_t *)&(arph->arp_tpa[0]);
	*v = htonl( arp->gip );

	//
	// total length 
	//
	arp_len = (int)(sizeof(struct ether_arp));

	// TX length 
	txlen = arp_len + sizeof(struct eth_header);

	/* DHCP Packet */
	ret = send( arp->sock , buf, txlen, 0 );
	dprintf("Send ARP Message %d bytes %s (%s) \n", txlen, (ret == txlen)? "OK" :"FAIL", strerror(errno) );

	free( buf );

	return 0;
}

/*
 *
 * ---------------------------------------------------------------
 *
 *
 * P  U  B  L  I  C      F  U  N  C  T  I  O  N  S
 *
 *
 * ---------------------------------------------------------------
 *
 */

int perform_dhcpc(char *ifname,u_int32_t *ip,u_int32_t *netmask,u_int32_t *gateway)
{
	int err;
	int sock;
	dhcp_t *dhcpjob;

	sock = allproto_socket( ifname );
	if (sock < 0)
		return -1;

	dhcpjob = (dhcp_t *)malloc(sizeof(dhcp_t));
	if (!dhcpjob) {
		close( sock );
		fprintf(stderr,"malloc(sizeof(dhcp_t))::failed\n");
		return -1;
	}
	memset( (char *)dhcpjob, 0, sizeof(dhcp_t) );

	// init ..
	dhcpjob->xid = TRANSID;  // transaction ID

	// Interface MAC address 
	if (get_hwaddr(sock,ifname, (char *)dhcpjob->hwaddr)) {
		close( sock );
		return -1;
	}

	// 
	// Receiver thread (DHCP protocol manager) 
	//
	dhcpjob->sock = sock;
	err = pthread_create(&(dhcpjob->recv),NULL,dhcprecv,(void *)dhcpjob);
	if (err) {
		close( sock );
		fprintf(stderr, "DHCP thread creation error\n");
		return -1;
	}
	sleep(1);

	// initialization 
	dhcpjob->givenip = 0;
	dhcpjob->server  = 0;

	// 
	// Send DHCP Discover message 
	//
	dhcp_send_message( dhcpjob , DHCP_DISCOVER );

	while (!dhcpjob->done)
		sleep(1);

	// pthread join 
	pthread_join( dhcpjob->recv , NULL );

	//
	// deleting socket 
	//
	close( sock );  

	err = (dhcpjob->stage == 0xFF) ? 0 : -1;

	//
	// Network Information 
	//
	if (!err) {
		if (ip) 	*ip = dhcpjob->givenip;
		if (netmask) 	*netmask = dhcpjob->netmask;
		if (gateway) 	*gateway = dhcpjob->router;
	}

	free( dhcpjob );

	dprintf("All forks done!!\n");

	return err;
}

int perform_arp(char *ifname, u_int32_t ip, u_int32_t gatewayip, u_int8_t *mac)
{
	int sock;
	int err;
	arp_t *arp;
	int failed, cnt;
	#define PKTSZ 2048
	char * buf; // 4K bytes buffer;
	int len;
	struct sockaddr addr;
	int addrlen;
	int flags;
	fd_set rfds;
	struct timeval tv;
	struct eth_header *ethh;
	struct ether_arp *arph;

	sock = allproto_socket( ifname );
	if (sock < 0)
		return -1;

	arp = (arp_t *)malloc( sizeof( arp_t ) );
	if (!arp) {
		close( sock );
		fprintf(stderr,"malloc( sizeof( arp_t ) )::failed\n");
		return -1;
	}

	// MAC address
	if (get_hwaddr(sock, ifname, (char *)arp->hwaddr)) {
		close( sock );
		return -1;
	}

	arp->sock = sock;
	arp->done = 0;
	arp->gip  = gatewayip;
	arp->ip   = ip;

	failed = 1;
	cnt = 0;

	// make socket to be non-block
	flags = fcntl( arp->sock, F_GETFL );
	fcntl(arp->sock, F_SETFL, flags | O_NONBLOCK);

	buf = (char *)malloc( PKTSZ );
	if (!buf) {
		close(sock);
		free(arp);
		fprintf(stderr,"malloc( PKTSZ )::failed\n");
		return -1;
	}

	while (!arp->done) {
		if (++cnt > ARP_FAILURE) {
			arp->done = 1;
			break;
		}

		err = arp_req_message( arp );
		if (err) {
			arp->done = 1;
			break;
		}
	
		FD_ZERO(&rfds);
		FD_SET(arp->sock,&rfds);

		tv.tv_sec = 1; // every 1 sec.
		tv.tv_usec = 0;

		int ret = select(arp->sock+1,&rfds,NULL,NULL,&tv);
		if (ret == -1) {
			fprintf(stderr,"socket error\n");
			arp->done = 1;
			continue;
		} else if (ret == 0) {
			continue; // timeout 
		} else {
			// data arrival !!
		}

		addrlen = sizeof(addr);
		len = recvfrom(arp->sock, buf, PKTSZ, 0, &addr, (socklen_t *)&addrlen );
		if (len < 0) {
			fprintf(stderr,"recv(sock=%d,)::error\n",arp->sock);
			arp->done = 1;
			continue;
		}

		sleep(1);

		/* Check ETH !! */
		ethh = (struct eth_header *)buf;
		if (memcmp(ethh->daddr,arp->hwaddr,ETH_ALEN))
			continue;

		if (ntohs(ethh->type) != ETHERTYPE_ARP)
			continue;

		/* CHeck ARP !! */
		arph = (struct ether_arp *)((char *)ethh + sizeof(struct eth_header));
		if (ntohs(arph->arp_op) != 2)  // REPLY
			continue;	

		/* TODO - Too much weak comparison */
		if (memcmp(arph->arp_tha,arp->hwaddr,ETH_ALEN))
			continue;

		dprintf("Recv ARP Message %d bytes (%s) \n", len, strerror(errno) );

		memcpy( mac, arph->arp_sha , ETH_ALEN );

		failed = 0;
		
		arp->done = 1;
	}

	close( sock );

	free( buf );
	free( arp );

	dprintf("All forks done at ARP !!\n");

	return failed;
}

int perform_ping(char *ifname, u_int32_t ip, u_int32_t netmask, u_int8_t *mac, u_int32_t dip, u_int32_t retry, u_int32_t *latency)
{
	int sock;
	int err, cnt, xcnt;
	ping_t *ping;
	int failed;

	sock = allproto_socket( ifname );
	if (sock < 0)
		return -1;

	ping = (ping_t *)malloc( sizeof( ping_t ) );
	if (!ping) {
		close( sock );
		fprintf(stderr,"malloc( sizeof( ping_t ) )::failed\n");
		return -1;
	}

	// MAC address
	if (get_hwaddr(sock,ifname, (char *)ping->hwaddr)) {
		close( sock );
		return -1;
	}

	ping->sock = sock;
	ping->done = 0;
	ping->ip = ip;
	ping->netmask = netmask;
	ping->dip = dip;
	ping->id  = ICMPID;
	ping->seq = 0;  // ICMP seq

	// gateway MAC address
	memcpy( ping->ghwaddr, mac, ETH_ALEN );

	err = pthread_create(&(ping->recv),NULL,pingrecv,(void *)ping);
	if (err) {
		close( sock );
		fprintf(stderr, "DHCP thread creation error\n");
		return -1;
	}
	sleep(1);

	failed = 0;

	cnt = 0;
	ping->replied = 0;
	
	while (!ping->done) {
		cnt = ping->replied;
		err = ping_req_message( ping );
		if (err) {
			ping->done = 1;
			break;
		}
		xcnt = 0;
		while ( 1 ) {
			if (ping->replied == (cnt+1)) {
				/* successfully replied */
				break;
			}
			usleep(1000);
			if (++xcnt >= 1000) {
				fprintf(stderr,"Timeout !!\n");
				failed = 1;
				break;
			}
		}
		ping->seq++;
		if (ping->seq > retry) {
			ping->done = 1;
			break;
		}
	}

	pthread_join( ping->recv, NULL );

	close( sock );

	if (latency)
		*latency = ping->latency;

	free( ping );

	dprintf("All forks done at ping!!\n");

	return failed;
}

/*
 * ---------------------------------------------------------------
 *
 */

int main(int argc, char *argv[])
{
	u_int32_t ip, netmask, gateway, latency;
	u_int8_t mac[ETH_ALEN];
	int err;

	if (argc < 2)
		exit(0);

	while (1) {
		err = perform_dhcpc( argv[1], &ip, &netmask, &gateway );
		if (!err) {
			printf("DHCP INFO : %d.%d.%d.%d|%d.%d.%d.%d|%d.%d.%d.%d \n",
				((ip & 0xFF000000) >> 24) & 0xFF ,
				((ip & 0x00FF0000) >> 16) & 0xFF ,
				((ip & 0x0000FF00) >> 8 ) & 0xFF ,
				((ip & 0x000000FF) >> 0 ) & 0xFF ,
				((netmask & 0xFF000000) >> 24) & 0xFF ,
				((netmask & 0x00FF0000) >> 16) & 0xFF ,
				((netmask & 0x0000FF00) >> 8 ) & 0xFF ,
				((netmask & 0x000000FF) >> 0 ) & 0xFF ,
				((gateway & 0xFF000000) >> 24) & 0xFF ,
				((gateway & 0x00FF0000) >> 16) & 0xFF ,
				((gateway & 0x0000FF00) >> 8 ) & 0xFF ,
				((gateway & 0x000000FF) >> 0 ) & 0xFF );

			err = perform_arp( argv[1], ip, gateway, mac );
			if (!err) {
				perform_ping( argv[1], ip, netmask, mac, ((8<<24)|(8<<16)|(8<<8)|8), 5, &latency );
				printf("Ping latency = %d.%dmsec \n",latency/1000,latency%1000);
			}
		}

		sleep(1);
	}
}
