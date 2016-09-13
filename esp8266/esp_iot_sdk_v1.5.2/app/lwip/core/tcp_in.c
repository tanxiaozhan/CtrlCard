/**
 * @file
 * Transmission Control Protocol, incoming traffic
 *
 * The input processing functions of the TCP layer.
 *
 * These functions are generally called in the order (ip_input() ->)
 * tcp_input() -> * tcp_process() -> tcp_receive() (-> application).
 * 
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#define TXZDEBUG 1

#include "lwip/opt.h"

#if LWIP_TCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/tcp_impl.h"
#include "lwip/def.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/inet_chksum.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "arch/perf.h"
#include "osapi.h"

#ifdef MEMLEAK_DEBUG
static const char mem_debug_file[] ICACHE_RODATA_ATTR = __FILE__;
#endif

/* These variables are global to all functions involved in the input
   processing of TCP segments. They are set by the tcp_input()
   function. */
static struct tcp_seg inseg;		//tcp_seg锟结构锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷谋锟斤拷亩锟�
static struct tcp_hdr *tcphdr;		//锟斤拷锟侥讹拷锟斤拷TCP锟阶诧拷
static struct ip_hdr *iphdr;		//IP锟斤拷莅锟斤拷撞锟�
static u32_t seqno, ackno;		//TCP锟阶诧拷锟斤拷锟斤拷锟斤拷侄锟斤拷锟饺凤拷虾锟斤拷侄锟�
static u8_t flags;				//锟阶诧拷锟斤拷志锟街讹拷
static u16_t tcplen;				//TCP锟斤拷锟侥筹拷锟斤拷

static u8_t recv_flags;			//锟斤拷前锟斤拷锟侥达拷锟斤拷锟斤拷
static struct pbuf *recv_data;		//锟斤拷锟侥讹拷锟斤拷锟絧buf

struct tcp_pcb *tcp_input_pcb;	//锟斤拷前锟斤拷锟侥匡拷锟狡匡拷

/* Forward declarations. */
static err_t tcp_process(struct tcp_pcb *pcb)ICACHE_FLASH_ATTR;
static void tcp_receive(struct tcp_pcb *pcb)ICACHE_FLASH_ATTR;
static void tcp_parseopt(struct tcp_pcb *pcb)ICACHE_FLASH_ATTR;

static err_t tcp_listen_input(struct tcp_pcb_listen *pcb)ICACHE_FLASH_ATTR;
static err_t tcp_timewait_input(struct tcp_pcb *pcb)ICACHE_FLASH_ATTR;

/**
 * The initial input processing of TCP. It verifies the TCP header, demultiplexes
 * the segment between the PCBs and passes it on to tcp_process(), which implements
 * the TCP finite state machine. This function is called by the IP layer (in
 * ip_input()).
 *
 * @param p received TCP segment to process (p->payload pointing to the IP header)
 * @param inp network interface on which this segment was received
 */
 /** 
 *  TCP锟斤拷始锟斤拷锟斤拷锟诫处锟�?锟斤拷证锟斤拷TCP头锟斤拷锟斤拷锟斤拷锟斤拷锟絀P锟斤拷锟斤拷锟�
 
 * @锟斤拷锟斤拷p:锟斤拷锟斤拷锟斤拷盏锟絋CP锟斤拷(指锟斤拷IP头锟侥革拷锟斤拷)
 * @锟斤拷锟斤拷inp:锟斤拷锟秸段碉拷锟斤拷锟斤拷涌锟�
 */ 
void
tcp_input(struct pbuf *p, struct netif *inp)
{
  struct tcp_pcb *pcb, *prev;
  struct tcp_pcb_listen *lpcb;
#if SO_REUSE
  struct tcp_pcb *lpcb_prev = NULL;
  struct tcp_pcb_listen *lpcb_any = NULL;
#endif /* SO_REUSE */
  u8_t hdrlen;
  err_t err;

  PERF_START;

  TCP_STATS_INC(tcp.recv);	//状态锟斤拷1
  snmp_inc_tcpinsegs();			//tcp锟斤拷锟斤拷渭锟�1

  iphdr = (struct ip_hdr *)p->payload;// pointer to the actual data in the buffer
  /* 
  *锟斤拷头锟斤拷锟斤拷(IHL)锟斤拷4位锟斤拷IP协锟斤拷锟酵凤拷某锟斤拷龋锟街革拷锟絀Pv4协锟斤拷锟酵凤拷锟斤拷鹊锟斤拷纸锟斤拷锟斤拷锟斤拷俑锟�32位锟斤拷
  *锟斤拷锟斤拷IPv4锟侥帮拷头锟斤拷锟杰帮拷杀锟斤拷锟斤拷锟斤拷目锟窖� 锟筋，锟斤拷锟斤拷锟斤拷锟斤拷侄慰锟斤拷锟斤拷锟斤拷锟饺凤拷锟絀Pv4锟斤拷荼锟斤拷锟斤拷锟捷诧拷锟街碉拷偏锟斤拷锟斤拷锟斤拷
  *IPv4锟斤拷头锟斤拷锟斤拷小锟斤拷锟斤拷锟斤拷20锟斤拷锟街节ｏ拷锟斤拷锟絀HL锟斤拷锟斤拷侄蔚锟斤拷锟叫≈碉拷锟绞拷锟斤拷票锟绞撅拷锟斤拷锟�5 (5x4 = 20锟街斤拷)锟斤拷
  *锟斤拷锟斤拷说锟斤拷锟斤拷锟绞撅拷陌锟酵凤拷锟斤拷锟斤拷纸锟斤拷锟斤拷锟�4锟街节的憋拷锟斤拷
  */
  tcphdr = (struct tcp_hdr *)((u8_t *)p->payload + IPH_HL(iphdr) * 4);

#if TCP_INPUT_DEBUG
  tcp_debug_print(tcphdr);
#endif

  /* remove header from payload */
  if (pbuf_header(p, -((s16_t)(IPH_HL(iphdr) * 4))) || (p->tot_len < sizeof(struct tcp_hdr))) {
    /* drop short packets */
    LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: short packet (%"U16_F" bytes) discarded\n", p->tot_len));
    TCP_STATS_INC(tcp.lenerr);//锟斤拷锟襟长度硷拷锟斤拷
    TCP_STATS_INC(tcp.drop);//锟斤拷止锟斤拷锟斤拷
    snmp_inc_tcpinerrs();
    pbuf_free(p);//锟酵凤拷buffer
    return;
  }

  /* Don't even process incoming broadcasts/multicasts. */
  if (ip_addr_isbroadcast(&current_iphdr_dest, inp) ||
      ip_addr_ismulticast(&current_iphdr_dest)) {
    TCP_STATS_INC(tcp.proterr);//协锟斤拷锟斤拷锟斤拷锟斤拷
    TCP_STATS_INC(tcp.drop);
    snmp_inc_tcpinerrs();
    pbuf_free(p);
    return;
  }

#if CHECKSUM_CHECK_TCP
  /* Verify TCP checksum. */
  if (inet_chksum_pseudo(p, ip_current_src_addr(), ip_current_dest_addr(),
      IP_PROTO_TCP, p->tot_len) != 0) {
      LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: packet discarded due to failing checksum 0x%04"X16_F"\n",
        inet_chksum_pseudo(p, ip_current_src_addr(), ip_current_dest_addr(),
      IP_PROTO_TCP, p->tot_len)));
#if TCP_DEBUG
    tcp_debug_print(tcphdr);
#endif /* TCP_DEBUG */
    TCP_STATS_INC(tcp.chkerr);//校锟斤拷锟斤拷锟斤拷锟斤拷
    TCP_STATS_INC(tcp.drop);
    snmp_inc_tcpinerrs();
    pbuf_free(p);
    return;
  }
#endif

  /* Move the payload pointer in the pbuf so that it points to the
     TCP data instead of the TCP header. */
  hdrlen = TCPH_HDRLEN(tcphdr);//锟斤拷锟斤拷头锟侥筹拷锟斤拷
  if(pbuf_header(p, -(hdrlen * 4))){//锟斤拷锟絋CP头锟斤拷锟斤拷锟斤拷0为锟缴癸拷锟斤拷锟斤拷锟斤拷
    /* drop short packets */
    LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: short packet\n"));
    TCP_STATS_INC(tcp.lenerr);//tcp锟斤拷锟饺达拷锟斤拷锟斤拷锟�
    TCP_STATS_INC(tcp.drop);
    snmp_inc_tcpinerrs();
    pbuf_free(p);
    return;
  }

  /* Convert fields in TCP header to host byte order. */
  tcphdr->src = ntohs(tcphdr->src);				//转锟斤拷源锟斤拷址
  tcphdr->dest = ntohs(tcphdr->dest);				//转锟斤拷目锟侥碉拷址
  seqno = tcphdr->seqno = ntohl(tcphdr->seqno);	//转锟斤拷锟斤拷锟叫猴拷
  ackno = tcphdr->ackno = ntohl(tcphdr->ackno);	//转锟斤拷应锟斤拷锟�
  tcphdr->wnd = ntohs(tcphdr->wnd);				//转锟斤拷tcp锟斤拷锟斤拷

  flags = TCPH_FLAGS(tcphdr);//锟矫碉拷tcp header锟侥憋拷志
  /* 
  *锟斤拷志锟斤拷3位锟斤拷锟斤拷锟街段ｏ拷锟斤拷
  *    锟斤拷锟斤拷位锟斤拷1位
  *    锟斤拷锟街讹拷位锟斤拷1位锟斤拷取值锟斤拷0锟斤拷锟斤拷锟斤拷锟斤拷荼锟斤拷侄危锟斤拷锟�1锟斤拷锟斤拷荼锟斤拷锟斤拷芊侄危锟�
  *    锟斤拷锟斤拷位锟斤拷1位锟斤拷取值锟斤拷0锟斤拷锟斤拷莅锟斤拷锟斤拷没锟叫帮拷1锟斤拷锟斤拷莅锟斤拷锟斤拷锟叫革拷锟侥帮拷
  */ 
  tcplen = p->tot_len + ((flags & (TCP_FIN | TCP_SYN)) ? 1 : 0);//TCP_FIN 锟斤拷 TCP_SYN 锟斤拷位锟斤拷1锟斤拷锟斤拷锟斤拷锟�0

  /* Demultiplex an incoming segment. First, we check if it is destined
     for an active connection. 锟斤拷锟饺ｏ拷锟斤拷锟斤拷欠锟揭伙拷锟揭拷锟斤拷锟揭伙拷锟斤拷锟斤拷锟�*/
 ////////////////////////////////////////////////////////////////////////////////////////
  prev = NULL;  
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {//锟斤拷锟斤拷锟斤拷斜锟�
    LWIP_ASSERT("tcp_input: active pcb->state != CLOSED", pcb->state != CLOSED);
    LWIP_ASSERT("tcp_input: active pcb->state != TIME-WAIT", pcb->state != TIME_WAIT);
    LWIP_ASSERT("tcp_input: active pcb->state != LISTEN", pcb->state != LISTEN);
    if (pcb->remote_port == tcphdr->src &&
       pcb->local_port == tcphdr->dest &&
       ip_addr_cmp(&(pcb->remote_ip), &current_iphdr_src) &&
       ip_addr_cmp(&(pcb->local_ip), &current_iphdr_dest)) {//锟斤拷锟斤拷锟截的碉拷址

      /* Move this PCB to the front of the list so that subsequent
         lookups will be faster (we exploit locality in TCP segment
         arrivals). */
      LWIP_ASSERT("tcp_input: pcb->next != pcb (before cache)", pcb->next != pcb);
      if (prev != NULL) {//锟斤拷锟角耙伙拷锟斤拷诘悴晃拷锟�
        prev->next = pcb->next;
        pcb->next = tcp_active_pcbs;
        tcp_active_pcbs = pcb;//pcb锟斤拷锟斤拷锟斤拷前锟斤拷
      }
      LWIP_ASSERT("tcp_input: pcb->next != pcb (after cache)", pcb->next != pcb);
      break;
    }
    prev = pcb;//prev指锟斤拷pcb
  }

  if (pcb == NULL) {
    /* If it did not go to an active connection, we check the connections
       in the TIME-WAIT state. */
    for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {//锟斤拷锟斤拷却锟阶刺拷碌锟絧cb
      LWIP_ASSERT("tcp_input: TIME-WAIT pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
      if (pcb->remote_port == tcphdr->src &&
         pcb->local_port == tcphdr->dest &&
         ip_addr_cmp(&(pcb->remote_ip), &current_iphdr_src) &&
         ip_addr_cmp(&(pcb->local_ip), &current_iphdr_dest)) {
        /* We don't really care enough to move this PCB to the front
           of the list since we are not very likely to receive that
           many segments for connections in TIME-WAIT. */
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: packed for TIME_WAITing connection.\n"));
        tcp_timewait_input(pcb);//锟斤拷锟斤拷tcp timewait 锟侥帮拷
        pbuf_free(p);
        return;
      }
    }

    /* Finally, if we still did not get a match, we check all PCBs that
       are LISTENing for incoming connections. */
    prev = NULL;
    for(lpcb = tcp_listen_pcbs.listen_pcbs; lpcb != NULL; lpcb = lpcb->next) {//锟斤拷锟斤拷锟斤拷锟阶刺拷锟斤拷锟斤拷械锟絧cb
      if (lpcb->local_port == tcphdr->dest) {
#if SO_REUSE
        if (ip_addr_cmp(&(lpcb->local_ip), &current_iphdr_dest)) {
          /* found an exact match */
          break;
        } else if(ip_addr_isany(&(lpcb->local_ip))) {
          /* found an ANY-match */
          lpcb_any = lpcb;
          lpcb_prev = prev;
        }
#else /* SO_REUSE */
        if (ip_addr_cmp(&(lpcb->local_ip), &current_iphdr_dest) ||
            ip_addr_isany(&(lpcb->local_ip))) {
          /* found a match */
          break;
        }
#endif /* SO_REUSE */
      }
      prev = (struct tcp_pcb *)lpcb;
    }
#if SO_REUSE
    /* first try specific local IP */
    if (lpcb == NULL) {
      /* only pass to ANY if no specific local IP has been found */
      lpcb = lpcb_any;
      prev = lpcb_prev;
    }
#endif /* SO_REUSE */
    if (lpcb != NULL) {
      /* Move this PCB to the front of the list so that subsequent
         lookups will be faster (we exploit locality in TCP segment
         arrivals). */
      if (prev != NULL) {
        ((struct tcp_pcb_listen *)prev)->next = lpcb->next;
              /* our successor is the remainder of the listening list */
        lpcb->next = tcp_listen_pcbs.listen_pcbs;
              /* put this listening pcb at the head of the listening list */
        tcp_listen_pcbs.listen_pcbs = lpcb;
      }
    
      LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: packed for LISTENing connection.\n"));
      tcp_listen_input(lpcb);//锟斤拷锟斤拷tcp锟斤拷锟斤拷锟斤拷莅锟�
      pbuf_free(p);
      return;
    }
  }

#if TCP_INPUT_DEBUG
  LWIP_DEBUGF(TCP_INPUT_DEBUG, ("+-+-+-+-+-+-+-+-+-+-+-+-+-+- tcp_input: flags "));
  tcp_debug_print_flags(TCPH_FLAGS(tcphdr));
  LWIP_DEBUGF(TCP_INPUT_DEBUG, ("-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"));
#endif /* TCP_INPUT_DEBUG */


  if (pcb != NULL) {
    /* The incoming segment belongs to a connection. */
#if TCP_INPUT_DEBUG
#if TCP_DEBUG
    tcp_debug_print_state(pcb->state);
#endif /* TCP_DEBUG */
#endif /* TCP_INPUT_DEBUG */

    /* Set up a tcp_seg structure. */
    inseg.next = NULL;
    inseg.len = p->tot_len;
    inseg.p = p;
    inseg.tcphdr = tcphdr;

    recv_data = NULL;
    recv_flags = 0;

    /* If there is data which was previously "refused" by upper layer */
    if (pcb->refused_data != NULL) {
      /* Notify again application with data previously received. */
      LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: notify kept packet\n"));
      TCP_EVENT_RECV(pcb, pcb->refused_data, ERR_OK, err);//pcb锟斤拷锟斤拷锟斤拷锟�
      if (err == ERR_OK) {
        pcb->refused_data = NULL;
      } else if ((err == ERR_ABRT) || (tcplen > 0)) {
        /* if err == ERR_ABRT, 'pcb' is already deallocated */
        /* Drop incoming packets because pcb is "full" (only if the incoming
           segment contains data). */
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: drop incoming packets, because pcb is \"full\"\n"));
        TCP_STATS_INC(tcp.drop);//tcp锟斤拷锟斤拷锟斤拷锟�
        snmp_inc_tcpinerrs();
        pbuf_free(p);
        return;
      }
    }
    tcp_input_pcb = pcb;//锟斤拷录锟斤拷前锟斤拷锟侥达拷锟斤拷目锟斤拷瓶锟�
    err = tcp_process(pcb);//锟斤拷锟�?锟斤拷
    /* A return value of ERR_ABRT means that tcp_abort() was called
       and that the pcb has been freed. If so, we don't do anything. */
    if (err != ERR_ABRT) {
      if (recv_flags & TF_RESET) {
        /* TF_RESET means that the connection was reset by the other
           end. We then call the error callback to inform the
           application that the connection is dead before we
           deallocate the PCB. */
        TCP_EVENT_ERR(pcb->errf, pcb->callback_arg, ERR_RST);
        tcp_pcb_remove(&tcp_active_pcbs, pcb);//删锟斤拷锟絧cb锟叫憋拷锟叫碉拷pcb
        memp_free(MEMP_TCP_PCB, pcb);
      } else if (recv_flags & TF_CLOSED) {
        /* The connection has been closed and we will deallocate the
           PCB. */
        if (!(pcb->flags & TF_RXCLOSED)) {
          /* Connection closed although the application has only shut down the
             tx side: call the PCB's err callback and indicate the closure to
             ensure the application doesn't continue using the PCB. */
          TCP_EVENT_ERR(pcb->errf, pcb->callback_arg, ERR_CLSD);
        }
        tcp_pcb_remove(&tcp_active_pcbs, pcb);
        memp_free(MEMP_TCP_PCB, pcb);
      } else {
        err = ERR_OK;
        /* If the application has registered a "sent" function to be
           called when new send buffer space is available, we call it
           now. */
        if (pcb->acked > 0) {
          TCP_EVENT_SENT(pcb, pcb->acked, err);//锟斤拷锟斤拷荼锟饺凤拷希锟斤拷氐锟斤拷没锟斤拷锟絪end锟斤拷锟斤拷
          if (err == ERR_ABRT) {
            goto aborted;
          }
        }

        if (recv_data != NULL) {//锟斤拷锟斤拷萁锟斤拷盏锟�
          LWIP_ASSERT("pcb->refused_data == NULL", pcb->refused_data == NULL);
          if (pcb->flags & TF_RXCLOSED) {
            /* received data although already closed -> abort (send RST) to
               notify the remote host that not all data has been processed */
            pbuf_free(recv_data);
            tcp_abort(pcb);
            goto aborted;
          }

	//PSH锟斤拷志 PSH 锟斤拷锟斤拷位锟斤拷
	//锟斤拷PSH=1时锟斤拷要锟斤拷锟酵凤拷锟斤拷锟较凤拷锟酵该分段ｏ拷
	//锟斤拷锟斤拷辗锟斤拷锟斤拷锟侥斤拷锟斤拷锟侥斤拷锟斤拷应锟矫层，锟斤拷锟斤拷锟斤拷锟叫达拷锟�?
		  
          if (flags & TCP_PSH) {
            recv_data->flags |= PBUF_FLAG_PUSH;//锟斤拷锟絙uffer应锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
          }

          /* Notify application that data has been received. */
          TCP_EVENT_RECV(pcb, recv_data, ERR_OK, err);
          if (err == ERR_ABRT) {
            goto aborted;
          }

          /* If the upper layer can't receive this data, store it */
          if (err != ERR_OK) {
            pcb->refused_data = recv_data;
            LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: keep incoming packet, because pcb is \"full\"\n"));
          }
        }

        /* If a FIN segment was received, we call the callback
           function with a NULL buffer to indicate EOF. */
        if (recv_flags & TF_GOT_FIN) {
          /* correct rcv_wnd as the application won't call tcp_recved()
             for the FIN's seqno */
          if (pcb->rcv_wnd != TCP_WND) {
            pcb->rcv_wnd++;
          }
	 
          TCP_EVENT_CLOSED(pcb, err);
          if (err == ERR_ABRT) {
            goto aborted;
          }
        }

        tcp_input_pcb = NULL;//锟斤拷锟饺拷直锟斤拷锟�
        /* Try to send something out. */
        tcp_output(pcb);//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
#if TCP_INPUT_DEBUG
#if TCP_DEBUG
        tcp_debug_print_state(pcb->state);
#endif /* TCP_DEBUG */
#endif /* TCP_INPUT_DEBUG */
      }
    }
    /* Jump target if pcb has been aborted in a callback (by calling tcp_abort()).
       Below this line, 'pcb' may not be dereferenced! */
aborted:
    tcp_input_pcb = NULL;
    recv_data = NULL;

    /* give up our reference to inseg.p */
    if (inseg.p != NULL)
    {
      pbuf_free(inseg.p);//锟酵凤拷buffer
      inseg.p = NULL;
    }

    /*add processing queue segments that arrive out of order by LiuHan*/
#if TCP_QUEUE_OOSEQ
    extern char RxNodeNum(void);
    if (RxNodeNum() < 2){
    	extern void pbuf_free_ooseq_new(void* arg);
//    	os_printf("reclaim some memory from queued\n");
    	pbuf_free_ooseq_new(NULL);
    }
#endif
  } else {

    /* If no matching PCB was found, send a TCP RST (reset) to the
       sender. */
    LWIP_DEBUGF(TCP_RST_DEBUG, ("tcp_input: no PCB match found, resetting.\n"));
    if (!(TCPH_FLAGS(tcphdr) & TCP_RST)) {
      TCP_STATS_INC(tcp.proterr);//协锟斤拷锟斤拷锟斤拷锟斤拷
      TCP_STATS_INC(tcp.drop);//tcp锟斤拷锟斤拷锟斤拷锟�
      tcp_rst(ackno, seqno + tcplen,
        ip_current_dest_addr(), ip_current_src_addr(),
        tcphdr->dest, tcphdr->src);//锟斤拷锟斤拷TCP锟斤拷位
    }
    pbuf_free(p);
  }

  LWIP_ASSERT("tcp_input: tcp_pcbs_sane()", tcp_pcbs_sane());
  PERF_STOP("tcp_input");
}

/**
 * Called by tcp_input() when a segment arrives for a listening
 * connection (from tcp_input()).
 *
 * @param pcb the tcp_pcb_listen for which a segment arrived
 * @return ERR_OK if the segment was processed
 *         another err_t on error
 *
 * @note the return value is not (yet?) used in tcp_input()
 * @note the segment which arrived is saved in global variables, therefore only the pcb
 *       involved is passed as a parameter to this function
 */
 /*
*锟斤拷锟斤拷LISTEN状态锟侥匡拷锟狡匡拷锟斤拷酶煤锟斤拷锟�
*通锟斤拷锟角凤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷一锟斤拷锟剿口诧拷锟斤拷锟斤拷突锟斤拷锟絊YN锟斤拷锟斤拷锟斤拷锟斤拷
*
*/
static err_t
tcp_listen_input(struct tcp_pcb_listen *pcb)
{
  struct tcp_pcb *npcb;
  struct tcp_pcb *pactive_pcb;
  u8_t active_pcb_num = 0;
  err_t rc;

  /* In the LISTEN state, we check for incoming SYN segments,
     creates a new PCB, and responds with a SYN|ACK. */
  if (flags & TCP_ACK) {
    /* For incoming segments with the ACK flag set, respond with a
       RST. */
    LWIP_DEBUGF(TCP_RST_DEBUG, ("tcp_listen_input: ACK in LISTEN, sending reset\n"));
    tcp_rst(ackno + 1, seqno + tcplen,
      ip_current_dest_addr(), ip_current_src_addr(),
      tcphdr->dest, tcphdr->src);
  } else if (flags & TCP_SYN) {//锟秸碉拷SYN锟斤拷锟斤拷
    LWIP_DEBUGF(TCP_DEBUG, ("TCP connection request %"U16_F" -> %"U16_F".\n", tcphdr->src, tcphdr->dest));
#if TCP_LISTEN_BACKLOG
    if (pcb->accepts_pending >= pcb->backlog) {
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_listen_input: listen backlog exceeded for port %"U16_F"\n", tcphdr->dest));
      return ERR_ABRT;
    }
#endif /* TCP_LISTEN_BACKLOG */
    for(pactive_pcb = tcp_active_pcbs; pactive_pcb != NULL; pactive_pcb = pactive_pcb->next){
    	if (pactive_pcb->state == ESTABLISHED){
    		active_pcb_num ++;
    	}
    }
    if (active_pcb_num == MEMP_NUM_TCP_PCB){
    	LWIP_DEBUGF(TCP_DEBUG, ("tcp_listen_input: exceed the number of active TCP connections\n"));
    	TCP_STATS_INC(tcp.memerr);
    	return ERR_MEM;
    }
    npcb = tcp_alloc(pcb->prio);//锟斤拷锟斤拷锟斤拷锟狡匡拷
    /* If a new PCB could not be created (probably due to lack of memory),
       we don't do anything, but rely on the sender will retransmit the
       SYN at a time when we have more memory available. */
    if (npcb == NULL) {
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_listen_input: could not allocate PCB\n"));
      TCP_STATS_INC(tcp.memerr);//TCP锟节达拷锟斤拷锟斤拷锟斤拷
      return ERR_MEM;
    }

#if TCP_LISTEN_BACKLOG
    pcb->accepts_pending++;
#endif /* TCP_LISTEN_BACKLOG */
    /* Set up the new PCB. */
    //锟斤拷锟狡匡拷锟斤拷锟斤拷锟斤拷锟斤拷氐锟�4锟斤拷锟街讹拷
    ip_addr_copy(npcb->local_ip, current_iphdr_dest);
    npcb->local_port = pcb->local_port;
    ip_addr_copy(npcb->remote_ip, current_iphdr_src);
    npcb->remote_port = tcphdr->src;

	//锟斤拷锟狡匡拷锟斤拷锟斤拷锟斤拷锟街讹拷
    npcb->state = SYN_RCVD;//锟斤拷锟斤拷锟斤拷锟斤拷状态
    npcb->rcv_nxt = seqno + 1;//锟斤拷锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    npcb->rcv_ann_right_edge = npcb->rcv_nxt;
    npcb->snd_wnd = tcphdr->wnd;//锟斤拷锟矫凤拷锟酵达拷锟斤拷
    npcb->ssthresh = npcb->snd_wnd;
    npcb->snd_wl1 = seqno - 1;/* initialise to seqno-1 to force window update */
    npcb->callback_arg = pcb->callback_arg;
#if LWIP_CALLBACK_API
    npcb->accept = pcb->accept;
#endif /* LWIP_CALLBACK_API */
    /* inherit socket options */
    npcb->so_options = pcb->so_options & SOF_INHERITED;
    /* Register the new PCB so that we can begin receiving segments
       for it. */
    TCP_REG(&tcp_active_pcbs, npcb);

    /* Parse any options in the SYN. */
    tcp_parseopt(npcb);
#if TCP_CALCULATE_EFF_SEND_MSS
    npcb->mss = tcp_eff_send_mss(npcb->mss, &(npcb->remote_ip));
#endif /* TCP_CALCULATE_EFF_SEND_MSS */

    snmp_inc_tcppassiveopens();

    /* Send a SYN|ACK together with the MSS option. */
    rc = tcp_enqueue_flags(npcb, TCP_SYN | TCP_ACK);
    if (rc != ERR_OK) {//锟斤拷锟斤拷锟斤拷锟斤拷头锟斤拷驴锟斤拷瓶锟�
      tcp_abandon(npcb, 0);
      return rc;
    }
    return tcp_output(npcb);//锟斤拷锟酵憋拷锟斤拷
  }
  return ERR_OK;
}

/**
 * Called by tcp_input() when a segment arrives for a connection in
 * TIME_WAIT.
 *
 * @param pcb the tcp_pcb for which a segment arrived
 *
 * @note the segment which arrived is saved in global variables, therefore only the pcb
 *       involved is passed as a parameter to this function
 */
 /*
*锟斤拷锟斤拷TIME_WAIT状态锟侥匡拷锟狡匡拷锟斤拷酶煤锟斤拷锟斤拷锟斤拷盏锟斤拷谋锟斤拷亩危锟�
*锟斤拷状态锟铰ｏ拷锟截憋拷锟斤拷锟接碉拷锟斤拷锟街癸拷锟斤拷丫锟斤拷锟斤拷锟斤拷锟斤拷诘却锟�2MSL锟斤拷时锟斤拷
*锟斤拷状态锟铰的憋拷锟侥讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟叫的撅拷锟斤拷荩锟街憋拷锟缴撅拷锟缴★拷
*锟斤拷锟斤拷要锟斤拷锟酵凤拷锟斤拷锟斤拷ACK锟斤拷锟斤拷
*/
static err_t
tcp_timewait_input(struct tcp_pcb *pcb)
{
  
  if (flags & TCP_RST)  {		//RST锟斤拷位锟斤拷直锟接凤拷锟斤拷
    return ERR_OK;
  }
 
  if (flags & TCP_SYN) {		//锟斤拷SYN锟斤拷锟斤拷锟斤拷息锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷荼锟斤拷锟节斤拷锟秸达拷锟斤拷锟节ｏ拷锟斤拷锟酵凤拷锟斤拷锟斤拷RST锟斤拷锟斤拷
 
    if (TCP_SEQ_BETWEEN(seqno, pcb->rcv_nxt, pcb->rcv_nxt+pcb->rcv_wnd)) {
      
      tcp_rst(ackno, seqno + tcplen, ip_current_dest_addr(), ip_current_src_addr(),
        tcphdr->dest, tcphdr->src);
      return ERR_OK;
    }
  } else if (flags & TCP_FIN) {	//锟斤拷锟侥帮拷FIN锟斤拷锟斤拷锟斤拷息
  						    
    pcb->tmr = tcp_ticks;		  	//锟斤拷位锟饺达拷2MSL时锟戒，锟斤拷锟狡匡拷锟斤拷锟铰等达拷2MSL
  }

  if ((tcplen > 0))  {				//锟斤拷锟斤拷锟斤拷锟斤拷莸谋锟斤拷幕锟斤拷锟斤拷诮锟斤拷沾锟斤拷锟斤拷锟斤拷SYN锟斤拷锟斤拷
    pcb->flags |= TF_ACK_NOW;//锟斤拷锟斤拷一锟斤拷ACK锟斤拷锟斤拷
    return tcp_output(pcb);
  }
  return ERR_OK;
}

/**
 * Implements the TCP state machine. Called by tcp_input. In some
 * states tcp_receive() is called to receive data. The tcp_seg
 * argument will be freed by the caller (tcp_input()) unless the
 * recv_data pointer in the pcb is set.
 *
 * @param pcb the tcp_pcb for which a segment arrived
 *
 * @note the segment which arrived is saved in global variables, therefore only the pcb
 *       involved is passed as a parameter to this function
 */
static err_t
tcp_process(struct tcp_pcb *pcb)
{
  struct tcp_seg *rseg;
  u8_t acceptable = 0;
  err_t err;

  err = ERR_OK;

  /* Process incoming RST segments. */
  if (flags & TCP_RST) {
    /* First, determine if the reset is acceptable. */
    if (pcb->state == SYN_SENT) {
      if (ackno == pcb->snd_nxt) {
        acceptable = 1;
      }
    } else {
      if (TCP_SEQ_BETWEEN(seqno, pcb->rcv_nxt, 
                          pcb->rcv_nxt+pcb->rcv_wnd)) {
        acceptable = 1;
      }
    }

    if (acceptable) {
      LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_process: Connection RESET\n"));
      LWIP_ASSERT("tcp_input: pcb->state != CLOSED", pcb->state != CLOSED);
      recv_flags |= TF_RESET;
      pcb->flags &= ~TF_ACK_DELAY;
      return ERR_RST;
    } else {
      LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_process: unacceptable reset seqno %"U32_F" rcv_nxt %"U32_F"\n",
       seqno, pcb->rcv_nxt));
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_process: unacceptable reset seqno %"U32_F" rcv_nxt %"U32_F"\n",
       seqno, pcb->rcv_nxt));
      return ERR_OK;
    }
  }

  if ((flags & TCP_SYN) && (pcb->state != SYN_SENT && pcb->state != SYN_RCVD)) { 
    /* Cope with new connection attempt after remote end crashed */
    tcp_ack_now(pcb);
    return ERR_OK;
  }
  
  if ((pcb->flags & TF_RXCLOSED) == 0) {
    /* Update the PCB (in)activity timer unless rx is closed (see tcp_shutdown) */
    pcb->tmr = tcp_ticks;
  }
  pcb->keep_cnt_sent = 0;

  tcp_parseopt(pcb);

  /* Do different things depending on the TCP state. */
  switch (pcb->state) {
  case SYN_SENT:
    LWIP_DEBUGF(TCP_INPUT_DEBUG, ("SYN-SENT: ackno %"U32_F" pcb->snd_nxt %"U32_F" unacked %"U32_F"\n", ackno,
     pcb->snd_nxt, ntohl(pcb->unacked->tcphdr->seqno)));
    /* received SYN ACK with expected sequence number? */
    if ((flags & TCP_ACK) && (flags & TCP_SYN)
        && ackno == ntohl(pcb->unacked->tcphdr->seqno) + 1) {
      pcb->snd_buf++;
      pcb->rcv_nxt = seqno + 1;
      pcb->rcv_ann_right_edge = pcb->rcv_nxt;
      pcb->lastack = ackno;
      pcb->snd_wnd = tcphdr->wnd;
      pcb->snd_wl1 = seqno - 1; /* initialise to seqno - 1 to force window update */
      pcb->state = ESTABLISHED;

#if TCP_CALCULATE_EFF_SEND_MSS
      pcb->mss = tcp_eff_send_mss(pcb->mss, &(pcb->remote_ip));
#endif /* TCP_CALCULATE_EFF_SEND_MSS */

      /* Set ssthresh again after changing pcb->mss (already set in tcp_connect
       * but for the default value of pcb->mss) */
      pcb->ssthresh = pcb->mss * 10;

      pcb->cwnd = ((pcb->cwnd == 1) ? (pcb->mss * 2) : pcb->mss);
      LWIP_ASSERT("pcb->snd_queuelen > 0", (pcb->snd_queuelen > 0));
      --pcb->snd_queuelen;
      LWIP_DEBUGF(TCP_QLEN_DEBUG, ("tcp_process: SYN-SENT --queuelen %"U16_F"\n", (u16_t)pcb->snd_queuelen));
      rseg = pcb->unacked;
      pcb->unacked = rseg->next;

      /* If there's nothing left to acknowledge, stop the retransmit
         timer, otherwise reset it to start again */
      if(pcb->unacked == NULL)
        pcb->rtime = -1;
      else {
        pcb->rtime = 0;
//        pcb->nrtx = 0;
      }
      pcb->nrtx = 0;

      tcp_seg_free(rseg);

      /* Call the user specified function to call when sucessfully
       * connected. */
      TCP_EVENT_CONNECTED(pcb, ERR_OK, err);
      if (err == ERR_ABRT) {
        return ERR_ABRT;
      }
      tcp_ack_now(pcb);
    }
    /* received ACK? possibly a half-open connection */
    else if (flags & TCP_ACK) {
      /* send a RST to bring the other side in a non-synchronized state. */
      tcp_rst(ackno, seqno + tcplen, ip_current_dest_addr(), ip_current_src_addr(),
        tcphdr->dest, tcphdr->src);
    }
    break;
  case SYN_RCVD:
    if (flags & TCP_ACK) {
      /* expected ACK number? */
      if (TCP_SEQ_BETWEEN(ackno, pcb->lastack+1, pcb->snd_nxt)) {
        u16_t old_cwnd;
        pcb->state = ESTABLISHED;
        LWIP_DEBUGF(TCP_DEBUG, ("TCP connection established %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
#if LWIP_CALLBACK_API
        LWIP_ASSERT("pcb->accept != NULL", pcb->accept != NULL);
#endif
        /* Call the accept function. */
        TCP_EVENT_ACCEPT(pcb, ERR_OK, err);
        if (err != ERR_OK) {
          /* If the accept function returns with an error, we abort
           * the connection. */
          /* Already aborted? */
          if (err != ERR_ABRT) {
            tcp_abort(pcb);
          }
          return ERR_ABRT;
        }
        old_cwnd = pcb->cwnd;
        /* If there was any data contained within this ACK,
         * we'd better pass it on to the application as well. */
        tcp_receive(pcb);

        /* Prevent ACK for SYN to generate a sent event */
        if (pcb->acked != 0) {
          pcb->acked--;
        }

        pcb->cwnd = ((old_cwnd == 1) ? (pcb->mss * 2) : pcb->mss);

        if (recv_flags & TF_GOT_FIN) {
          tcp_ack_now(pcb);
          pcb->state = CLOSE_WAIT;
        }
      } else {
        /* incorrect ACK number, send RST */
        tcp_rst(ackno, seqno + tcplen, ip_current_dest_addr(), ip_current_src_addr(),
                tcphdr->dest, tcphdr->src);
      }
    } else if ((flags & TCP_SYN) && (seqno == pcb->rcv_nxt - 1)) {
      /* Looks like another copy of the SYN - retransmit our SYN-ACK */
      tcp_rexmit(pcb);
    }
    break;
  case CLOSE_WAIT:
    /* FALLTHROUGH */
  case ESTABLISHED:
    tcp_receive(pcb);
    if (recv_flags & TF_GOT_FIN) { /* passive close */
      tcp_ack_now(pcb);
      pcb->state = CLOSE_WAIT;
    }
    break;
  case FIN_WAIT_1:
    tcp_receive(pcb);
    if (recv_flags & TF_GOT_FIN) {
      if ((flags & TCP_ACK) && (ackno == pcb->snd_nxt)) {
        LWIP_DEBUGF(TCP_DEBUG,
          ("TCP connection closed: FIN_WAIT_1 %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
        tcp_ack_now(pcb);
        tcp_pcb_purge(pcb);
        TCP_RMV(&tcp_active_pcbs, pcb);
        pcb->state = TIME_WAIT;
        TCP_REG(&tcp_tw_pcbs, pcb);
      } else {
        tcp_ack_now(pcb);
        pcb->state = CLOSING;
      }
    } else if ((flags & TCP_ACK) && (ackno == pcb->snd_nxt)) {
      pcb->state = FIN_WAIT_2;
    }
    break;
  case FIN_WAIT_2:
    tcp_receive(pcb);
    if (recv_flags & TF_GOT_FIN) {
      LWIP_DEBUGF(TCP_DEBUG, ("TCP connection closed: FIN_WAIT_2 %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
      tcp_ack_now(pcb);
      tcp_pcb_purge(pcb);
      TCP_RMV(&tcp_active_pcbs, pcb);
      pcb->state = TIME_WAIT;
      TCP_REG(&tcp_tw_pcbs, pcb);
    }
    break;
  case CLOSING:
    tcp_receive(pcb);
    if (flags & TCP_ACK && ackno == pcb->snd_nxt) {
      LWIP_DEBUGF(TCP_DEBUG, ("TCP connection closed: CLOSING %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
      tcp_pcb_purge(pcb);
      TCP_RMV(&tcp_active_pcbs, pcb);
      pcb->state = TIME_WAIT;
      TCP_REG(&tcp_tw_pcbs, pcb);
    }
    break;
  case LAST_ACK:
    tcp_receive(pcb);
    if (flags & TCP_ACK && ackno == pcb->snd_nxt) {
      LWIP_DEBUGF(TCP_DEBUG, ("TCP connection closed: LAST_ACK %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
      /* bugfix #21699: don't set pcb->state to CLOSED here or we risk leaking segments */
      recv_flags |= TF_CLOSED;
    }
    break;
  default:
    break;
  }
  return ERR_OK;
}

#if TCP_QUEUE_OOSEQ
/**
 * Insert segment into the list (segments covered with new one will be deleted)
 *
 * Called from tcp_receive()
 */
static void ICACHE_FLASH_ATTR
tcp_oos_insert_segment(struct tcp_seg *cseg, struct tcp_seg *next)
{
  struct tcp_seg *old_seg;

  if (TCPH_FLAGS(cseg->tcphdr) & TCP_FIN) {
    /* received segment overlaps all following segments */
    tcp_segs_free(next);
    next = NULL;
  }
  else {
    /* delete some following segments
       oos queue may have segments with FIN flag */
    while (next &&
           TCP_SEQ_GEQ((seqno + cseg->len),
                      (next->tcphdr->seqno + next->len))) {
      /* cseg with FIN already processed */
      if (TCPH_FLAGS(next->tcphdr) & TCP_FIN) {
        TCPH_SET_FLAG(cseg->tcphdr, TCP_FIN);
      }
      old_seg = next;
      next = next->next;
      tcp_seg_free(old_seg);
    }
    if (next &&
        TCP_SEQ_GT(seqno + cseg->len, next->tcphdr->seqno)) {
      /* We need to trim the incoming segment. */
      cseg->len = (u16_t)(next->tcphdr->seqno - seqno);
      pbuf_realloc(cseg->p, cseg->len);
    }
  }
  cseg->next = next;
}
#endif /* TCP_QUEUE_OOSEQ */

/**
 * Called by tcp_process. Checks if the given segment is an ACK for outstanding
 * data, and if so frees the memory of the buffered data. Next, is places the
 * segment on any of the receive queues (pcb->recved or pcb->ooseq). If the segment
 * is buffered, the pbuf is referenced by pbuf_ref so that it will not be freed until
 * i it has been removed from the buffer.
 *
 * If the incoming segment constitutes an ACK for a segment that was used for RTT
 * estimation, the RTT is estimated here as well.
 *
 * Called from tcp_process().
 */
static void
tcp_receive(struct tcp_pcb *pcb)
{
  struct tcp_seg *next;
#if TCP_QUEUE_OOSEQ
  struct tcp_seg *prev, *cseg;
#endif /* TCP_QUEUE_OOSEQ */
  struct pbuf *p;
  s32_t off;
  s16_t m;
  u32_t right_wnd_edge;
  u16_t new_tot_len;
  int found_dupack = 0;

  if (flags & TCP_ACK) {//锟斤拷锟侥帮拷ACK
    right_wnd_edge = pcb->snd_wnd + pcb->snd_wl2;//锟斤拷锟酵达拷锟斤拷 + 锟斤拷锟斤拷应锟斤拷锟斤拷蟠翱诟锟斤拷锟�

    // first /* Update window. */
    /*seqno > snd_wl1锟斤拷锟斤拷锟斤拷锟斤拷锟街癸拷滩锟斤拷么锟斤拷指锟斤拷锟�;
    *seqno = snd_wl1锟斤拷锟斤拷ackno > snd_wl2;锟斤拷时锟斤拷锟皆凤拷没锟叫凤拷锟斤拷锟斤拷荩锟街伙拷锟斤拷盏锟斤拷锟捷碉拷确锟斤拷;
    *ackno = snd_wl2锟揭憋拷锟斤拷锟阶诧拷锟叫憋拷snd_wnd锟斤拷锟侥达拷锟斤拷.锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷应值
    */
    if (TCP_SEQ_LT(pcb->snd_wl1, seqno) ||
       (pcb->snd_wl1 == seqno && TCP_SEQ_LT(pcb->snd_wl2, ackno)) ||
       (pcb->snd_wl2 == ackno && tcphdr->wnd > pcb->snd_wnd)) {
      pcb->snd_wnd = tcphdr->wnd;
      pcb->snd_wl1 = seqno;
      pcb->snd_wl2 = ackno;
      if (pcb->snd_wnd > 0 && pcb->persist_backoff > 0) {
          pcb->persist_backoff = 0;//锟斤拷锟斤拷锟绞憋拷锟斤拷顺锟�
      }
      LWIP_DEBUGF(TCP_WND_DEBUG, ("tcp_receive: window update %"U16_F"\n", pcb->snd_wnd));
#if TCP_WND_DEBUG
    } else {
      if (pcb->snd_wnd != tcphdr->wnd) {
        LWIP_DEBUGF(TCP_WND_DEBUG, 
                    ("tcp_receive: no window update lastack %"U32_F" ackno %"
                     U32_F" wl1 %"U32_F" seqno %"U32_F" wl2 %"U32_F"\n",
                     pcb->lastack, ackno, pcb->snd_wl1, seqno, pcb->snd_wl2));
      }
#endif /* TCP_WND_DEBUG */
    }

    /* (From Stevens TCP/IP Illustrated Vol II, p970.) Its only a
     * duplicate ack if:
     * 1) It doesn't ACK new data 没锟斤拷确锟斤拷锟斤拷锟斤拷锟�
     * 2) length of received packet is zero (i.e. no payload) 锟斤拷锟侥讹拷锟斤拷锟轿猴拷锟斤拷锟�
     * 3) the advertised window hasn't changed 锟斤拷锟截达拷锟斤拷没锟叫革拷锟斤拷
     * 4) There is outstanding unacknowledged data (retransmission timer running)锟斤拷锟斤拷锟斤拷莸却锟饺凤拷锟�
     * 5) The ACK is == biggest ACK sequence number so far seen (snd_una)   ackno = lastack
     * 
     * If it passes all five, should process as a dupack: 
     * a) dupacks < 3: do nothing 
     * b) dupacks == 3: fast retransmit 
     * c) dupacks > 3: increase cwnd 
     * 
     * If it only passes 1-3, should reset dupack counter (and add to
     * stats, which we don't do in lwIP)
     *
     * If it only passes 1, should reset dupack counter
     *
     */

    /* Clause 1 */
    if (TCP_SEQ_LEQ(ackno, pcb->lastack)) {//锟斤拷锟截革拷ACK?
      pcb->acked = 0;
      /* Clause 2 */
      if (tcplen == 0) {
        /* Clause 3 */
        if (pcb->snd_wl2 + pcb->snd_wnd == right_wnd_edge){
          /* Clause 4 */
          if (pcb->rtime >= 0) {
            /* Clause 5 */
            if (pcb->lastack == ackno) {
              found_dupack = 1;
              if (pcb->dupacks + 1 > pcb->dupacks)
                ++pcb->dupacks;
              if (pcb->dupacks > 3) {
                /* Inflate the congestion window, but not if it means that
                   the value overflows. */
                if ((u16_t)(pcb->cwnd + pcb->mss) > pcb->cwnd) {
                  pcb->cwnd += pcb->mss;
                }
              } else if (pcb->dupacks == 3) {//锟斤拷锟截革拷ACK
                /* Do fast retransmit */
                tcp_rexmit_fast(pcb);
              }
            }
          }
        }
      }
      /* If Clause (1) or more is true, but not a duplicate ack, reset
       * count of consecutive duplicate acks */
      if (!found_dupack) {
        pcb->dupacks = 0;
      }
    } else if (TCP_SEQ_BETWEEN(ackno, pcb->lastack+1, pcb->snd_nxt)){//ackno锟斤拷lastack+1锟斤拷snd_nxt之锟戒，锟叫断凤拷锟酵达拷锟斤拷锟斤拷锟斤拷锟�
      /* We come here when the ACK acknowledges new data. */
	  
      if (pcb->flags & TF_INFR) {
        pcb->flags &= ~TF_INFR;// Reset the "IN Fast Retransmit" flag,since we are no longer in fast retransmit
        pcb->cwnd = pcb->ssthresh;//Reset the congestion window to the  "slow start threshold".       
      }

      /* Reset the number of retransmissions. */
      pcb->nrtx = 0;

      /* Reset the retransmission time-out. */
      pcb->rto = (pcb->sa >> 3) + pcb->sv;

      /* Update the send buffer space. Diff between the two can never exceed 64K? */
      pcb->acked = (u16_t)(ackno - pcb->lastack);

      pcb->snd_buf += pcb->acked;

      /* Reset the fast retransmit variables. */
      pcb->dupacks = 0;
      pcb->lastack = ackno;

      /* Update the congestion control variables (cwnd and
         ssthresh). */
      if (pcb->state >= ESTABLISHED) {//状态为锟斤拷锟斤拷锟斤拷锟接憋拷志
        if (pcb->cwnd < pcb->ssthresh) {
          if ((u16_t)(pcb->cwnd + pcb->mss) > pcb->cwnd) {
            pcb->cwnd += pcb->mss;
          }
          LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_receive: slow start cwnd %"U16_F"\n", pcb->cwnd));
        } else {
          u16_t new_cwnd = (pcb->cwnd + pcb->mss * pcb->mss / pcb->cwnd);
          if (new_cwnd > pcb->cwnd) {
            pcb->cwnd = new_cwnd;
          }
          LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_receive: congestion avoidance cwnd %"U16_F"\n", pcb->cwnd));
        }
      }
      LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_receive: ACK for %"U32_F", unacked->seqno %"U32_F":%"U32_F"\n",
                                    ackno,
                                    pcb->unacked != NULL?
                                    ntohl(pcb->unacked->tcphdr->seqno): 0,
                                    pcb->unacked != NULL?
                                    ntohl(pcb->unacked->tcphdr->seqno) + TCP_TCPLEN(pcb->unacked): 0));

      /* Remove segment from the unacknowledged list if the incoming
         ACK acknowlegdes them. 
         *锟酵凤拷unacked锟斤拷锟斤拷锟较憋拷确锟较的憋拷锟侥段ｏ拷
         *直锟斤拷unacked锟斤拷锟斤拷为锟斤拷停止*/
      while (pcb->unacked != NULL &&
             TCP_SEQ_LEQ(ntohl(pcb->unacked->tcphdr->seqno) +
                         TCP_TCPLEN(pcb->unacked), ackno)) {
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_receive: removing %"U32_F":%"U32_F" from pcb->unacked\n",
                                      ntohl(pcb->unacked->tcphdr->seqno),
                                      ntohl(pcb->unacked->tcphdr->seqno) +
                                      TCP_TCPLEN(pcb->unacked)));

        next = pcb->unacked;//pcb unacked锟斤拷志
        pcb->unacked = pcb->unacked->next;//pcb unacked 锟斤拷一锟斤拷锟斤拷志

        LWIP_DEBUGF(TCP_QLEN_DEBUG, ("tcp_receive: queuelen %"U16_F" ... ", (u16_t)pcb->snd_queuelen));
        LWIP_ASSERT("pcb->snd_queuelen >= pbuf_clen(next->p)", (pcb->snd_queuelen >= pbuf_clen(next->p)));
        /* Prevent ACK for FIN to generate a sent event */
        if ((pcb->acked != 0) && ((TCPH_FLAGS(next->tcphdr) & TCP_FIN) != 0)) {
          pcb->acked--;
        }

        pcb->snd_queuelen -= pbuf_clen(next->p);//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟絧bufs锟斤拷锟斤拷
        tcp_seg_free(next);//锟酵凤拷tcp锟斤拷

        LWIP_DEBUGF(TCP_QLEN_DEBUG, ("%"U16_F" (after freeing unacked)\n", (u16_t)pcb->snd_queuelen));
        if (pcb->snd_queuelen != 0) {
          LWIP_ASSERT("tcp_receive: valid queue length", pcb->unacked != NULL ||
                      pcb->unsent != NULL);
        }
      }

      /* If there's nothing left to acknowledge, stop the retransmit
         timer, otherwise reset it to start again */
      if(pcb->unacked == NULL)	//锟斤拷锟斤拷莸却锟饺凤拷锟�
        pcb->rtime = -1;			//停止锟截达拷锟斤拷时锟斤拷
      else
        pcb->rtime = 0;			//锟斤拷位锟截达拷锟斤拷时锟斤拷

      pcb->polltmr = 0;
    } else {
      /* Fix bug bug #21582: out of sequence ACK, didn't really ack anything */
      pcb->acked = 0;
    }

    /* We go through the ->unsent list to see if any of the segments
       on the list are acknowledged by the ACK. This may seem
       strange since an "unsent" segment shouldn't be acked. The
       rationale is that lwIP puts all outstanding segments on the
       ->unsent list after a retransmission, so these segments may
       in fact have been sent once. */
       /** unsent锟斤拷锟斤拷锟斤拷锟角凤拷锟杰憋拷ackno确锟较的憋拷锟侥段ｏ拷锟斤拷锟斤拷锟酵凤拷**/
    while (pcb->unsent != NULL &&
           TCP_SEQ_BETWEEN(ackno, ntohl(pcb->unsent->tcphdr->seqno) + 
                           TCP_TCPLEN(pcb->unsent), pcb->snd_nxt)) {
      LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_receive: removing %"U32_F":%"U32_F" from pcb->unsent\n",
                                    ntohl(pcb->unsent->tcphdr->seqno), ntohl(pcb->unsent->tcphdr->seqno) +
                                    TCP_TCPLEN(pcb->unsent)));

      next = pcb->unsent;//pcb未锟斤拷锟酵憋拷志
      pcb->unsent = pcb->unsent->next;//未锟斤拷锟酵碉拷锟斤拷一锟斤拷
      LWIP_DEBUGF(TCP_QLEN_DEBUG, ("tcp_receive: queuelen %"U16_F" ... ", (u16_t)pcb->snd_queuelen));
      LWIP_ASSERT("pcb->snd_queuelen >= pbuf_clen(next->p)", (pcb->snd_queuelen >= pbuf_clen(next->p)));
      /* Prevent ACK for FIN to generate a sent event */
      if ((pcb->acked != 0) && ((TCPH_FLAGS(next->tcphdr) & TCP_FIN) != 0)) {
        pcb->acked--;
      }
      pcb->snd_queuelen -= pbuf_clen(next->p);//锟斤拷锟斤拷锟斤拷pbuf锟侥革拷锟斤拷
      tcp_seg_free(next);//锟酵放讹拷
      LWIP_DEBUGF(TCP_QLEN_DEBUG, ("%"U16_F" (after freeing unsent)\n", (u16_t)pcb->snd_queuelen));
      if (pcb->snd_queuelen != 0) {//锟斤拷锟斤拷锟斤拷锟叫筹拷锟斤拷
        LWIP_ASSERT("tcp_receive: valid queue length",
          pcb->unacked != NULL || pcb->unsent != NULL);
      }
    }
    /* End of ACK for new data processing. */

    LWIP_DEBUGF(TCP_RTO_DEBUG, ("tcp_receive: pcb->rttest %"U32_F" rtseq %"U32_F" ackno %"U32_F"\n",
                                pcb->rttest, pcb->rtseq, ackno));

    /* RTT estimation calculations. This is done by checking if the
       incoming segment acknowledges the segment we use to take a
       round-trip time measurement. */
    if (pcb->rttest && TCP_SEQ_LT(pcb->rtseq, ackno)) {//RTT锟斤拷锟节斤拷锟斤拷锟揭该憋拷锟侥段憋拷确锟斤拷
      /* diff between this shouldn't exceed 32K since this are tcp timer ticks
         and a round-trip shouldn't be that long... */
      m = (s16_t)(tcp_ticks - pcb->rttest);//锟斤拷锟斤拷M值

      LWIP_DEBUGF(TCP_RTO_DEBUG, ("tcp_receive: experienced rtt %"U16_F" ticks (%"U16_F" msec).\n",
                                  m, m * TCP_SLOW_INTERVAL));

      /* This is taken directly from VJs original code in his paper 锟斤拷锟斤拷锟絉TT锟斤拷锟姐公式*/
      m = m - (pcb->sa >> 3);
      pcb->sa += m;
      if (m < 0) {
        m = -m;
      }
      m = m - (pcb->sv >> 2);
      pcb->sv += m;
      pcb->rto = (pcb->sa >> 3) + pcb->sv;

      LWIP_DEBUGF(TCP_RTO_DEBUG, ("tcp_receive: RTO %"U16_F" (%"U16_F" milliseconds)\n",
                                  pcb->rto, pcb->rto * TCP_SLOW_INTERVAL));

      pcb->rttest = 0;
    }
  }

  /* If the incoming segment contains data, we must process it
     further. */
  if (tcplen > 0) {
    /* This code basically does three things:

    +) If the incoming segment contains data that is the next
    in-sequence data, this data is passed to the application. This
    might involve trimming the first edge of the data. The rcv_nxt
    variable and the advertised window are adjusted.

    +) If the incoming segment has data that is above the next
    sequence number expected (->rcv_nxt), the segment is placed on
    the ->ooseq queue. This is done by finding the appropriate
    place in the ->ooseq queue (which is ordered by sequence
    number) and trim the segment in both ends if needed. An
    immediate ACK is sent to indicate that we received an
    out-of-sequence segment.

    +) Finally, we check if the first segment on the ->ooseq queue
    now is in sequence (i.e., if rcv_nxt >= ooseq->seqno). If
    rcv_nxt > ooseq->seqno, we must trim the first edge of the
    segment on ->ooseq before we adjust rcv_nxt. The data in the
    segments that are now on sequence are chained onto the
    incoming segment so that we only need to call the application
    once.
    */

    /* First, we check if we must trim the first edge. We have to do
       this if the sequence number of the incoming segment is less
       than rcv_nxt, and the sequence number plus the length of the
       segment is larger than rcv_nxt. */
    /*    if (TCP_SEQ_LT(seqno, pcb->rcv_nxt)){
          if (TCP_SEQ_LT(pcb->rcv_nxt, seqno + tcplen)) {*/
    if (TCP_SEQ_BETWEEN(pcb->rcv_nxt, seqno + 1, seqno + tcplen - 1)){// seqno < rcv_nxt < seqno + tcplen
      /* Trimming the first edge is done by pushing the payload
         pointer in the pbuf downwards. This is somewhat tricky since
         we do not want to discard the full contents of the pbuf up to
         the new starting point of the data since we have to keep the
         TCP header which is present in the first pbuf in the chain.

         What is done is really quite a nasty hack: the first pbuf in
         the pbuf chain is pointed to by inseg.p. Since we need to be
         able to deallocate the whole pbuf, we cannot change this
         inseg.p pointer to point to any of the later pbufs in the
         chain. Instead, we point the ->payload pointer in the first
         pbuf to data in one of the later pbufs. We also set the
         inseg.data pointer to point to the right place. This way, the
         ->p pointer will still point to the first pbuf, but the
         ->p->payload pointer will point to data in another pbuf.

         After we are done with adjusting the pbuf pointers we must
         adjust the ->data pointer in the seg and the segment
         length.*/
	//去锟斤拷锟斤拷锟侥讹拷锟斤拷锟斤拷荼锟脚碉拷锟斤拷rcv_nxt锟斤拷锟斤拷锟�
      off = pcb->rcv_nxt - seqno;
      p = inseg.p;
      LWIP_ASSERT("inseg.p != NULL", inseg.p);
      LWIP_ASSERT("insane offset!", (off < 0x7fff));
      if (inseg.p->len < off) {
        LWIP_ASSERT("pbuf too short!", (((s32_t)inseg.p->tot_len) >= off));
        new_tot_len = (u16_t)(inseg.p->tot_len - off);
        while (p->len < off) {
          off -= p->len;
          /* KJM following line changed (with addition of new_tot_len var)
             to fix bug #9076
             inseg.p->tot_len -= p->len; */
          p->tot_len = new_tot_len;
          p->len = 0;
          p = p->next;
        }
        if(pbuf_header(p, (s16_t)-off)) {
          /* Do we need to cope with this failing?  Assert for now */
          LWIP_ASSERT("pbuf_header failed", 0);
        }
      } else {
        if(pbuf_header(inseg.p, (s16_t)-off)) {
          /* Do we need to cope with this failing?  Assert for now */
          LWIP_ASSERT("pbuf_header failed", 0);
        }
      }
      inseg.len -= (u16_t)(pcb->rcv_nxt - seqno);
      inseg.tcphdr->seqno = seqno = pcb->rcv_nxt;
    }
    else {
      if (TCP_SEQ_LT(seqno, pcb->rcv_nxt)){//seqno < rcv_nxt
        /* the whole segment is < rcv_nxt */
        /* must be a duplicate of a packet that has already been correctly handled */
	//锟斤拷锟侥讹拷锟斤拷锟斤拷锟斤拷锟斤拷荼锟脚撅拷小锟斤拷rcv_nxt锟斤拷锟斤拷吮锟斤拷锟斤拷锟斤拷馗锟斤拷锟斤拷模锟�
	//直锟斤拷锟斤拷源锟斤拷锟斤拷应ACK锟斤拷锟侥达拷锟斤拷

        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_receive: duplicate seqno %"U32_F"\n", seqno));
        tcp_ack_now(pcb);
      }
    }

    /* The sequence number must be within the window (above rcv_nxt
       and below rcv_nxt + rcv_wnd) in order to be further
       processed. */
    if (TCP_SEQ_BETWEEN(seqno, pcb->rcv_nxt, 
                        pcb->rcv_nxt + pcb->rcv_wnd - 1)){//rcv_nxt <  seqno < rcv_nxt + rcv_wnd - 1,锟斤拷锟斤拷锟斤拷诮锟斤拷辗锟轿э拷锟�
      if (pcb->rcv_nxt == seqno) {
        /* The incoming segment is the next in sequence. We check if
           we have to trim the end of the segment and update rcv_nxt
           and pass the data to the application. */
        tcplen = TCP_TCPLEN(&inseg);//锟斤拷锟姐报锟侥段筹拷锟斤拷

        if (tcplen > pcb->rcv_wnd) {//锟斤拷锟斤拷锟斤拷沾锟斤拷诖锟叫★拷锟斤拷锟斤拷锟斤拷锟轿诧拷锟斤拷囟锟�
          LWIP_DEBUGF(TCP_INPUT_DEBUG, 
                      ("tcp_receive: other end overran receive window"
                       "seqno %"U32_F" len %"U16_F" right edge %"U32_F"\n",
                       seqno, tcplen, pcb->rcv_nxt + pcb->rcv_wnd));
          if (TCPH_FLAGS(inseg.tcphdr) & TCP_FIN) {
            /* Must remove the FIN from the header as we're trimming 
             * that byte of sequence-space from the packet */
            TCPH_FLAGS_SET(inseg.tcphdr, TCPH_FLAGS(inseg.tcphdr) &~ TCP_FIN);
          }
          /* Adjust length of segment to fit in the window. */
          inseg.len = pcb->rcv_wnd;
          if (TCPH_FLAGS(inseg.tcphdr) & TCP_SYN) {
            inseg.len -= 1;
          }
          pbuf_realloc(inseg.p, inseg.len);
          tcplen = TCP_TCPLEN(&inseg);
          LWIP_ASSERT("tcp_receive: segment not trimmed correctly to rcv_wnd\n",
                      (seqno + tcplen) == (pcb->rcv_nxt + pcb->rcv_wnd));
        }
#if TCP_QUEUE_OOSEQ
        /* Received in-sequence data, adjust ooseq data if:
           - FIN has been received or
           - inseq overlaps with ooseq */
        if (pcb->ooseq != NULL) {
          if (TCPH_FLAGS(inseg.tcphdr) & TCP_FIN) {
            LWIP_DEBUGF(TCP_INPUT_DEBUG, 
                        ("tcp_receive: received in-order FIN, binning ooseq queue\n"));
            /* Received in-order FIN means anything that was received
             * out of order must now have been received in-order, so
             * bin the ooseq queue */
            while (pcb->ooseq != NULL) {
              struct tcp_seg *old_ooseq = pcb->ooseq;
              pcb->ooseq = pcb->ooseq->next;
              tcp_seg_free(old_ooseq);
            }
          }
          else {
            next = pcb->ooseq;
            /* Remove all segments on ooseq that are covered by inseg already.
             * FIN is copied from ooseq to inseg if present. */
            while (next &&
                   TCP_SEQ_GEQ(seqno + tcplen,
                               next->tcphdr->seqno + next->len)) {
              /* inseg cannot have FIN here (already processed above) */
              if (TCPH_FLAGS(next->tcphdr) & TCP_FIN &&
                  (TCPH_FLAGS(inseg.tcphdr) & TCP_SYN) == 0) {
                TCPH_SET_FLAG(inseg.tcphdr, TCP_FIN);
                tcplen = TCP_TCPLEN(&inseg);
              }
              prev = next;
              next = next->next;
              tcp_seg_free(prev);
            }
            /* Now trim right side of inseg if it overlaps with the first
             * segment on ooseq */
            if (next &&
                TCP_SEQ_GT(seqno + tcplen,
                           next->tcphdr->seqno)) {
              /* inseg cannot have FIN here (already processed above) */
              inseg.len = (u16_t)(next->tcphdr->seqno - seqno);
              if (TCPH_FLAGS(inseg.tcphdr) & TCP_SYN) {
                inseg.len -= 1;
              }
              pbuf_realloc(inseg.p, inseg.len);
              tcplen = TCP_TCPLEN(&inseg);
              LWIP_ASSERT("tcp_receive: segment not trimmed correctly to ooseq queue\n",
                          (seqno + tcplen) == next->tcphdr->seqno);
            }
            pcb->ooseq = next;
          }
        }
#endif /* TCP_QUEUE_OOSEQ */

        pcb->rcv_nxt = seqno + tcplen;

        /* Update the receiver's (our) window. */
        LWIP_ASSERT("tcp_receive: tcplen > rcv_wnd\n", pcb->rcv_wnd >= tcplen);
        pcb->rcv_wnd -= tcplen;

        tcp_update_rcv_ann_wnd(pcb);

        /* If there is data in the segment, we make preparations to
           pass this up to the application. The ->recv_data variable
           is used for holding the pbuf that goes to the
           application. The code for reassembling out-of-sequence data
           chains its data on this pbuf as well.

           If the segment was a FIN, we set the TF_GOT_FIN flag that will
           be used to indicate to the application that the remote side has
           closed its end of the connection. */
        if (inseg.p->tot_len > 0) {
          recv_data = inseg.p;
          /* Since this pbuf now is the responsibility of the
             application, we delete our reference to it so that we won't
             (mistakingly) deallocate it. */
          inseg.p = NULL;
        }
        if (TCPH_FLAGS(inseg.tcphdr) & TCP_FIN) {
          LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_receive: received FIN.\n"));
          recv_flags |= TF_GOT_FIN;
        }

#if TCP_QUEUE_OOSEQ
        /* We now check if we have segments on the ->ooseq queue that
           are now in sequence. */
        while (pcb->ooseq != NULL &&
               pcb->ooseq->tcphdr->seqno == pcb->rcv_nxt) {

          cseg = pcb->ooseq;
          seqno = pcb->ooseq->tcphdr->seqno;

          pcb->rcv_nxt += TCP_TCPLEN(cseg);
          LWIP_ASSERT("tcp_receive: ooseq tcplen > rcv_wnd\n",
                      pcb->rcv_wnd >= TCP_TCPLEN(cseg));
          pcb->rcv_wnd -= TCP_TCPLEN(cseg);

          tcp_update_rcv_ann_wnd(pcb);

          if (cseg->p->tot_len > 0) {
            /* Chain this pbuf onto the pbuf that we will pass to
               the application. */
            if (recv_data) {
              pbuf_cat(recv_data, cseg->p);
            } else {
              recv_data = cseg->p;
            }
            cseg->p = NULL;
          }
          if (TCPH_FLAGS(cseg->tcphdr) & TCP_FIN) {
            LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_receive: dequeued FIN.\n"));
            recv_flags |= TF_GOT_FIN;
            if (pcb->state == ESTABLISHED) { /* force passive close or we can move to active close */
              pcb->state = CLOSE_WAIT;
            } 
          }

          pcb->ooseq = cseg->next;
          tcp_seg_free(cseg);
        }
#endif /* TCP_QUEUE_OOSEQ */


        /* Acknowledge the segment(s). */
        tcp_ack(pcb);

      } else {
        /* We get here if the incoming segment is out-of-sequence. */
        tcp_send_empty_ack(pcb);
#if TCP_QUEUE_OOSEQ
        /* We queue the segment on the ->ooseq queue. */
        if (pcb->ooseq == NULL) {
          pcb->ooseq = tcp_seg_copy(&inseg);
        } else {
          /* If the queue is not empty, we walk through the queue and
             try to find a place where the sequence number of the
             incoming segment is between the sequence numbers of the
             previous and the next segment on the ->ooseq queue. That is
             the place where we put the incoming segment. If needed, we
             trim the second edges of the previous and the incoming
             segment so that it will fit into the sequence.

             If the incoming segment has the same sequence number as a
             segment on the ->ooseq queue, we discard the segment that
             contains less data. */

          prev = NULL;
          for(next = pcb->ooseq; next != NULL; next = next->next) {//锟斤拷ooseq取锟铰碉拷M锟斤拷锟斤拷锟侥段ｏ拷锟矫憋拷锟侥段非空ｏ拷M++
            if (seqno == next->tcphdr->seqno) {//锟矫憋拷锟侥讹拷锟斤拷始锟斤拷锟�== 要锟斤拷锟斤拷谋锟斤拷亩伪锟斤拷
              /* The sequence number of the incoming segment is the
                 same as the sequence number of the segment on
                 ->ooseq. We check the lengths to see which one to
                 discard. */
              if (inseg.len > next->len) {//要锟斤拷锟斤拷谋锟斤拷亩伪锟脚革拷
                /* The incoming segment is larger than the old
                   segment. We replace some segments with the new
                   one. */
                cseg = tcp_seg_copy(&inseg);//要锟斤拷锟斤拷谋锟斤拷亩未锟斤拷锟斤拷M锟斤拷锟斤拷锟侥讹拷
                if (cseg != NULL) {
                  if (prev != NULL) {
                    prev->next = cseg;
                  } else {
                    pcb->ooseq = cseg;
                  }
                  tcp_oos_insert_segment(cseg, next);
                }
                break;
              } else {
                /* Either the lenghts are the same or the incoming
                   segment was smaller than the old one; in either
                   case, we ditch the incoming segment. */
                break;
              }
            } else {
              if (prev == NULL) {
                if (TCP_SEQ_LT(seqno, next->tcphdr->seqno)) {
                  /* The sequence number of the incoming segment is lower
                     than the sequence number of the first segment on the
                     queue. We put the incoming segment first on the
                     queue. */
                  cseg = tcp_seg_copy(&inseg);
                  if (cseg != NULL) {
                    pcb->ooseq = cseg;
                    tcp_oos_insert_segment(cseg, next);
                  }
                  break;
                }
              } else {
                /*if (TCP_SEQ_LT(prev->tcphdr->seqno, seqno) &&
                  TCP_SEQ_LT(seqno, next->tcphdr->seqno)) {*/
                if (TCP_SEQ_BETWEEN(seqno, prev->tcphdr->seqno+1, next->tcphdr->seqno-1)) {
                  /* The sequence number of the incoming segment is in
                     between the sequence numbers of the previous and
                     the next segment on ->ooseq. We trim trim the previous
                     segment, delete next segments that included in received segment
                     and trim received, if needed. */
                  cseg = tcp_seg_copy(&inseg);
                  if (cseg != NULL) {
                    if (TCP_SEQ_GT(prev->tcphdr->seqno + prev->len, seqno)) {
                      /* We need to trim the prev segment. */
                      prev->len = (u16_t)(seqno - prev->tcphdr->seqno);
                      pbuf_realloc(prev->p, prev->len);
                    }
                    prev->next = cseg;
                    tcp_oos_insert_segment(cseg, next);
                  }
                  break;
                }
              }
              /* If the "next" segment is the last segment on the
                 ooseq queue, we add the incoming segment to the end
                 of the list. */
              if (next->next == NULL &&
                  TCP_SEQ_GT(seqno, next->tcphdr->seqno)) {
                if (TCPH_FLAGS(next->tcphdr) & TCP_FIN) {
                  /* segment "next" already contains all data */
                  break;
                }
                next->next = tcp_seg_copy(&inseg);
                if (next->next != NULL) {
                  if (TCP_SEQ_GT(next->tcphdr->seqno + next->len, seqno)) {
                    /* We need to trim the last segment. */
                    next->len = (u16_t)(seqno - next->tcphdr->seqno);
                    pbuf_realloc(next->p, next->len);
                  }
                  /* check if the remote side overruns our receive window */
                  if ((u32_t)tcplen + seqno > pcb->rcv_nxt + (u32_t)pcb->rcv_wnd) {
                    LWIP_DEBUGF(TCP_INPUT_DEBUG, 
                                ("tcp_receive: other end overran receive window"
                                 "seqno %"U32_F" len %"U16_F" right edge %"U32_F"\n",
                                 seqno, tcplen, pcb->rcv_nxt + pcb->rcv_wnd));
                    if (TCPH_FLAGS(next->next->tcphdr) & TCP_FIN) {
                      /* Must remove the FIN from the header as we're trimming 
                       * that byte of sequence-space from the packet */
                      TCPH_FLAGS_SET(next->next->tcphdr, TCPH_FLAGS(next->next->tcphdr) &~ TCP_FIN);
                    }
                    /* Adjust length of segment to fit in the window. */
                    next->next->len = pcb->rcv_nxt + pcb->rcv_wnd - seqno;
                    pbuf_realloc(next->next->p, next->next->len);
                    tcplen = TCP_TCPLEN(next->next);
                    LWIP_ASSERT("tcp_receive: segment not trimmed correctly to rcv_wnd\n",
                                (seqno + tcplen) == (pcb->rcv_nxt + pcb->rcv_wnd));
                  }
                }
                break;
              }
            }
            prev = next;
          }
        }
#endif /* TCP_QUEUE_OOSEQ */

      }
    } else {
      /* The incoming segment is not withing the window. */
      tcp_send_empty_ack(pcb);
    }
  } else {
    /* Segments with length 0 is taken care of here. Segments that
       fall out of the window are ACKed. */
    /*if (TCP_SEQ_GT(pcb->rcv_nxt, seqno) ||
      TCP_SEQ_GEQ(seqno, pcb->rcv_nxt + pcb->rcv_wnd)) {*/
    if(!TCP_SEQ_BETWEEN(seqno, pcb->rcv_nxt, pcb->rcv_nxt + pcb->rcv_wnd-1)){
      tcp_ack_now(pcb);//锟斤拷源锟剿凤拷锟斤拷一锟斤拷锟斤拷锟斤拷确锟较憋拷锟斤拷
    }
  }
}

/**
 * Parses the options contained in the incoming segment. 
 *
 * Called from tcp_listen_input() and tcp_process().
 * Currently, only the MSS option is supported!
 *
 * @param pcb the tcp_pcb for which a segment arrived
 */
static void
tcp_parseopt(struct tcp_pcb *pcb)
{
  u16_t c, max_c;
  u16_t mss;
  u8_t *opts, opt;
#if LWIP_TCP_TIMESTAMPS
  u32_t tsval;
#endif

  opts = (u8_t *)tcphdr + TCP_HLEN;

  /* Parse the TCP MSS option, if present. */
  if(TCPH_HDRLEN(tcphdr) > 0x5) {
    max_c = (TCPH_HDRLEN(tcphdr) - 5) << 2;
    for (c = 0; c < max_c; ) {
      opt = opts[c];
      switch (opt) {
      case 0x00:
        /* End of options. */
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: EOL\n"));
        return;
      case 0x01:
        /* NOP option. */
        ++c;
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: NOP\n"));
        break;
      case 0x02:
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: MSS\n"));
        if (opts[c + 1] != 0x04 || c + 0x04 > max_c) {
          /* Bad length */
          LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: bad length\n"));
          return;
        }
        /* An MSS option with the right option length. */
        mss = (opts[c + 2] << 8) | opts[c + 3];
        /* Limit the mss to the configured TCP_MSS and prevent division by zero */
        pcb->mss = ((mss > TCP_MSS) || (mss == 0)) ? TCP_MSS : mss;
        /* Advance to next option */
        c += 0x04;
        break;
#if LWIP_TCP_TIMESTAMPS
      case 0x08:
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: TS\n"));
        if (opts[c + 1] != 0x0A || c + 0x0A > max_c) {
          /* Bad length */
          LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: bad length\n"));
          return;
        }
        /* TCP timestamp option with valid length */
        tsval = (opts[c+2]) | (opts[c+3] << 8) | 
          (opts[c+4] << 16) | (opts[c+5] << 24);
        if (flags & TCP_SYN) {
          pcb->ts_recent = ntohl(tsval);
          pcb->flags |= TF_TIMESTAMP;
        } else if (TCP_SEQ_BETWEEN(pcb->ts_lastacksent, seqno, seqno+tcplen)) {
          pcb->ts_recent = ntohl(tsval);
        }
        /* Advance to next option */
        c += 0x0A;
        break;
#endif
      default:
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: other\n"));
        if (opts[c + 1] == 0) {
          LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: bad length\n"));
          /* If the length field is zero, the options are malformed
             and we don't process them further. */
          return;
        }
        /* All other options have a length field, so that we easily
           can skip past them. */
        c += opts[c + 1];
      }
    }
  }
}

#endif /* LWIP_TCP */
