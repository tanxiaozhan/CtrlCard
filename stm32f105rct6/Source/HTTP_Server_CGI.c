/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "rl_net.h"
#include "rl_net_lib.h"
#include "Board_LED.h"


// net_sys.c
extern  NET_LOCALM net_localm[];
#define LocM   net_localm[NETIF_ETH]

// Net_Config.c
extern struct net_tcp_cfg   net_tcp_config;
extern struct net_http_cfg  net_http_config;
#define tcp_NumSocks    net_tcp_config.NumSocks
#define tcp_socket      net_tcp_config.Scb
#define http_EnAuth     net_http_config.EnAuth
#define http_auth_passw net_http_config.Passw

#define buf_len 20     

char IP_Addr[20];
extern bool LEDrun;


// Local variables.
static uint8_t P2;

// My structure of CGI status variable.
typedef struct {
  uint16_t xcnt;
  uint16_t unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

// Process query string received by GET request.
void netCGI_ProcessQuery (const char *qstr) {
  char var[40];
	printf("cgi_process. qstr:%s\n",qstr);
  do {
    // Loop through all the parameters
    qstr = netCGI_GetEnvVar (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter
    printf("var[]:%s\n",var);
		if (var[0] != 0) {
      // First character is non-null, string exists
      if (strncmp (var, "ip=", 3) == 0) {
        // Local IP address
        netIP_aton (&var[3],NET_ADDR_IP4, LocM.IpAddr);
      }
      else if (strncmp (var, "msk=", 4) == 0) {
        // Local network mask
        netIP_aton (&var[4],NET_ADDR_IP4, LocM.NetMask);
      }
      else if (strncmp (var, "gw=", 3) == 0) {
        // Default gateway IP address
       netIP_aton (&var[3],NET_ADDR_IP4, LocM.DefGW);
      }
      else if (strncmp (var, "pdns=", 5) == 0) {
        // Primary DNS server IP address
        netIP_aton (&var[5],NET_ADDR_IP4, LocM.PriDNS);
      }
      else if (strncmp (var, "sdns=", 5) == 0) {
        // Secondary DNS server IP address
        netIP_aton (&var[5],NET_ADDR_IP4, LocM.SecDNS);
      }
    }
  } while (qstr);
}

// Process data received by POST request.
// Type code: - 0 = www-url-encoded form data.
//            - 1 = filename for file upload (null-terminated string).
//            - 2 = file upload raw data.
//            - 3 = end of file upload (file close requested).
//            - 4 = any XML encoded POST data (single or last stream).
//            - 5 = the same as 4, but with more XML data to follow.
void netCGI_ProcessData (uint8_t code, const char *data, uint32_t len) {
  char var[40],passw[12];
	printf("Process data received by Post request.\n Type code:%d\n Post data:%s\n Post len:%d\n",code,data,len);
  if (code != 0) {
    // Ignore all other codes
    return;
  }

  P2 = 0;
  LEDrun = true;
  if (len == 0) {
    // No data or all items (radio, checkbox) are off
    LED_SetOut (P2);
    return;
  }
  passw[0] = 1;
  do {
    // Parse all parameters
    data = netCGI_GetEnvVar (data, var, sizeof (var));
    if (var[0] != 0) {
      // First character is non-null, string exists
      if (strcmp (var, "led0=on") == 0) {
        P2 |= 0x01;
      }
      else if (strcmp (var, "led1=on") == 0) {
        P2 |= 0x02;
      }
      else if (strcmp (var, "led2=on") == 0) {
        P2 |= 0x04;
      }
      else if (strcmp (var, "led3=on") == 0) {
        P2 |= 0x08;
      }
      else if (strcmp (var, "led4=on") == 0) {
        P2 |= 0x10;
      }
      else if (strcmp (var, "led5=on") == 0) {
        P2 |= 0x20;
      }
      else if (strcmp (var, "led6=on") == 0) {
        P2 |= 0x40;
      }
      else if (strcmp (var, "led7=on") == 0) {
        P2 |= 0x80;
      }
      else if (strcmp (var, "ctrl=Browser") == 0) {
        LEDrun = false;
      }
      else if ((strncmp (var, "pw0=", 4) == 0) ||
               (strncmp (var, "pw2=", 4) == 0)) {
        // Change password, retyped password
        if (http_EnAuth) {
          if (passw[0] == 1) {
            strcpy (passw, var+4);
          }
          else if (strcmp (passw, var+4) == 0) {
            // Both strings are equal, change the password
            strcpy (http_auth_passw, passw);
          }
        }
      }
      else if (strncmp (var, "lcd1=", 5) == 0) {
        // LCD Module line 1 text
        //strcpy (lcd_text[0], var+5);
        //LCDupdate = true;
      }
      else if (strncmp (var, "lcd2=", 5) == 0) {
        // LCD Module line 2 text
        //strcpy (lcd_text[1], var+5);
        //LCDupdate = true;
      }
    }
  } while (data);
  LED_SetOut (P2);
}

// Generate dynamic web data from a script line.
uint32_t netCGI_Script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  NET_TCP_INFO *tsoc;
  const char *lang;
  uint32_t len = 0;
  uint8_t id;
  static uint32_t adv;
	printf("Generate dynamic web. \n");
	printf("env=%s\n",env);
	
  switch (env[0]) {
    // Analyze a 'c' script line starting position 2
    case 'a' :
      // Network parameters from 'network.cgi'
      switch (env[2]) {
        case 'i':
          // Write local IP address
          len = sprintf (buf, &env[4], netIP_ntoa (NET_ADDR_IP4,LocM.IpAddr,buf,buf_len));
          break;
        case 'm':
          // Write local network mask
          len = sprintf (buf, &env[4], netIP_ntoa (NET_ADDR_IP4,LocM.NetMask,buf,buf_len));
          break;
        case 'g':
          // Write default gateway IP address
          len = sprintf (buf, &env[4], netIP_ntoa (NET_ADDR_IP4,LocM.DefGW,buf,buf_len));
          break;
        case 'p':
          // Write primary DNS server IP address
          len = sprintf (buf, &env[4], netIP_ntoa (NET_ADDR_IP4,LocM.PriDNS,buf,buf_len));
          break;
        case 's':
          // Write secondary DNS server IP address
          len = sprintf (buf, &env[4], netIP_ntoa (NET_ADDR_IP4,LocM.SecDNS,buf,buf_len));
          break;
      }
      break;

    case 'b':
      // LED control from 'led.cgi'
      if (env[2] == 'c') {
        // Select Control
        len = sprintf (buf, &env[4], LEDrun ?     ""     : "selected",
                                     LEDrun ? "selected" :    ""     );
        break;
      }
      // LED CheckBoxes
      id = env[2] - '0';
      if (id > 7) {
        id = 0;
      }
      id = 1 << id;
      len = sprintf (buf, &env[4], (P2 & id) ? "checked" : "");
      break;

    case 'c':
      // TCP status from 'tcp.cgi'
      while ((len + 150) < buflen) {
        tsoc = &tcp_socket[MYBUF(pcgi)->xcnt];
        MYBUF(pcgi)->xcnt++;
        // 'sprintf' format string is defined here
        len += sprintf (buf+len,   "<tr align=\"center\">");
        if (tsoc->State <= netTCP_StateCLOSED) {
          len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                                   "<td>-</td><td>-</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt,net_tcp_ntoa(tsoc->State));
        }
        else if (tsoc->State == netTCP_StateLISTEN) {
          len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>%d</td><td>-</td>"
                                   "<td>-</td><td>-</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt, net_tcp_ntoa(tsoc->State), tsoc->LocPort);
        }
        else {
          netIP_ntoa (NET_ADDR_IP4,tsoc->Peer.addr,IP_Addr,buf_len);     //6.4.0-->7.0.0
					len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>%d</td>"
                                   "<td>%d</td><td>%s</td><td>%d</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt, net_tcp_ntoa(tsoc->State), tsoc->LocPort,
                                   tsoc->AliveTimer, buf, tsoc->Peer.port);
        }
        // Repeat for all TCP Sockets
        if (MYBUF(pcgi)->xcnt == tcp_NumSocks) {
          break;
        }
      }
      if (MYBUF(pcgi)->xcnt < tcp_NumSocks) {
        // Hi bit is a repeat flag
        len |= (1u << 31);
      }
      break;

    case 'd':
      // System password from 'system.cgi'
      switch (env[2]) {
        case '1':
          len = sprintf (buf, &env[4], http_EnAuth ? "Enabled" : "Disabled");
          break;
        case '2':
          len = sprintf (buf, &env[4], http_auth_passw);
          break;
      }
      break;

    case 'e':
      // Browser Language from 'language.cgi'
      lang = netHTTPs_GetLanguage ();
      if      (strncmp (lang, "en", 2) == 0) {
        lang = "English";
      }
      else if (strncmp (lang, "de", 2) == 0) {
        lang = "German";
      }
      else if (strncmp (lang, "fr", 2) == 0) {
        lang = "French";
      }
      else if (strncmp (lang, "sl", 2) == 0) {
        lang = "Slovene";
      }
      else {
        lang = "Unknown";
      }
      len = sprintf (buf, &env[2], lang, netHTTPs_GetLanguage());
      break;

    case 'f':
      // LCD Module control from 'lcd.cgi'
      switch (env[2]) {
        case '1':
          len = sprintf (buf, &env[4], 1); //lcd_text[0]);
          break;
        case '2':
          len = sprintf (buf, &env[4], 1);//lcd_text[1]);
          break;
      }
      break;

    case 'g':
      // AD Input from 'ad.cgi'
      switch (env[2]) {
        case '1':
//          adv = AD_in (0);
          len = sprintf (buf, &env[4], adv);
          break;
        case '2':
          len = sprintf (buf, &env[4], (float)adv*3.3f/4096);
          break;
        case '3':
          adv = (adv * 100) / 4096;
          len = sprintf (buf, &env[4], adv);
          break;
      }
      break;

    case 'x':
      // AD Input from 'ad.cgx'
//      adv = AD_in (0);
      len = sprintf (buf, &env[1], adv);
      break;

    case 'y':
      // Button state from 'button.cgx'
//      len = sprintf (buf, "<checkbox><id>button%c</id><on>%s</on></checkbox>",
//                     env[1], (get_button () & (1 << (env[1]-'0'))) ? "true" : "false");
      break;
  }
		printf("CGI:%s\n",buf);

  return (len);
}
