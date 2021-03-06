/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rl_net.h"
#include "rl_net_lib.h"
#include "Board_LED.h"
#include "LED_Display.h"


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


extern screen_para screen;        //显示屏参数
extern area_para area[MAX_AREA_NUMBER];           //显示区参数


char IP_Addr[20];
extern bool LEDrun;

char current_area=0;   //当前显示区号


// Local variables.
static uint8_t P2;

// My structure of CGI status variable.
typedef struct {
  uint16_t xcnt;
  uint16_t unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)



//删除显示分区
//参数：area_no 显示分区号，0-2
void del_area(uint8_t area_no){
	area[area_no].width=0;
	area[area_no].height=0;
	area[area_no].x=0;
	area[area_no].y=0;
	area[area_no].content_type=TEXT;
	area[area_no].ani_in=0;
	area[area_no].ani_out=0;
	area[area_no].length=0;
	area[area_no].display_data[0]='\0';
}


// Process query string received by GET request.
void netCGI_ProcessQuery (const char *qstr) {
  char var[40];
	printf("cgi_process. qstr:%s\n",qstr);
  do {
    // Loop through all the parameters
    qstr = netCGI_GetEnvVar (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter
    //printf("var[]:%s\n",var);
		if (var[0] != 0) {
      // First character is non-null, string exists
      if (strncmp (var, "txt=", 4) == 0) {
        //更新显示数据
				strcpy((char *)area[current_area].display_data,&var[4]);
      }
			else if (strncmp (var, "line=", 5) == 0) {
				//自动换行显示
      }
			else if (strncmp (var, "areaNo=", 7) == 0) {
        //设置/修改当前显示分区
				current_area=(char)(var[7]-48);
      }
			else if (strncmp (var, "X=", 2) == 0) {
        //显示区起始X坐标
				area[current_area].x=atoi(&var[2]);
      }
      else if (strncmp (var, "Y=", 2) == 0) {
        //显示区起始Y坐标
				area[current_area].y=atoi(&var[2]);
      }
      else if (strncmp (var, "Width=", 6) == 0) {
        //显示区宽度
				area[current_area].width=atoi(&var[6]);
      }
      else if (strncmp (var, "Height=", 7) == 0) {
        //显示区高度
				area[current_area].x=atoi(&var[7]);
      }
      else if (strncmp (var, "ScreenWidth=", 12) == 0) {
        // 显示区起始X坐标
				screen.width=atoi(&var[12]);
      }
      else if (strncmp (var, "ScreenHeight=", 13) == 0) {
        // 显示区起始X坐标
				screen.height=atoi(&var[13]);
      }
      else if (strncmp (var, "ScreenLight=", 12) == 0) {
        // 显示区起始X坐标
				screen.light=atoi(&var[12]);
      }
      else if (strncmp (var, "delArea=T", 9) == 0) {
				  //删除显示分区
					del_area(current_area);
      }
      else if (strncmp (var, "delArea0=on", 11) == 0) {
        //删除显示分区0
				del_area(0);
      }
			else if (strncmp (var, "delArea1=on", 11) == 0) {
				//删除显示分区1
				del_area(1);
			}
			else if (strncmp (var, "delArea2=on", 11) == 0) {
				//删除显示分区1
				del_area(2);
			}
			else if (strncmp (var, "auth=true", 9) == 0) {
				//删除显示分区1
				netHTTPs_LoginOnOff (true);
			}
			else if (strncmp (var, "auth=false", 10) == 0) {
				//删除显示分区1
				netHTTPs_LoginOnOff (false);
			}
			else if (strncmp (var, "pw0=", 3) == 0) {
				//删除显示分区1
				netHTTPs_SetPassword (&var[3]);
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
  uint32_t len = 0;
  uint8_t id;
  static uint32_t adv;
	//printf("Generate dynamic web. \n");
	//printf("env=%s\n",env);
	
  switch (env[0]) {
    // Analyze a 'c' script line starting position 2
    case 'a' :
      // 显示区编辑 'areaEdit.cgi'
      switch (env[2]) {
        case 'n':
          // 显示区号
          len = sprintf (buf, &env[4], current_area+1);
          break;
        case 'x':
          // x坐标
          len = sprintf (buf, &env[4], area[current_area].x);
          break;
        case 'y':
          // y坐标
          len = sprintf (buf, &env[4], area[current_area].y);
          break;
        case 'w':
          // 宽
          len = sprintf (buf, &env[4], area[current_area].width);
          break;
        case 'h':
          // 高
          len = sprintf (buf, &env[4], area[current_area].height);
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
      //内容管理
			id=(uint8_t)env[2]-48;
			if( area[id].width<1 || area[id].height<1 )
				break;
				
				len += sprintf(buf+len,"<tr><td align=center>");
				len += sprintf(buf+len,"<input name=delArea%d type=checkbox class=tbs style=width:40px;height:40px/></td>",id);
				len += sprintf(buf+len,"<td align=center>%d</td>",id+1);
				len += sprintf(buf+len,"<td align=center>%d</td>",area[id].x);
				len += sprintf(buf+len,"<td align=center>%d</td>",area[id].y);
				len += sprintf(buf+len,"<td align=center>%d</td>",area[id].width);
				len += sprintf(buf+len,"<td align=center>%d</td>",area[id].height);
				len += sprintf(buf+len,"<td align=center>%d</td>",area[id].content_type);
				len += sprintf(buf+len,"<td align=center>");
				len += sprintf(buf+len,"<button class=tbs type=button onclick=\"window.location='edit.cgi?areaNo=%d'\">内容编辑</button>",id);
				len += sprintf(buf+len,"<p><button class=tbs type=button onclick=\"window.location='areaEdit.cgi?areaNo=%d'\">分区编辑</button></p></td></tr>",id);
				break;
/*		
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
*/


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
			switch (env[2]){
				case 't':
						len = sprintf(buf, &env[4],area[current_area].display_data);
				break;
				
			}
			
			break;

    case 'l':
      // 登录设置，是否启用身份认证 'auth.cgi'
			switch(env[2]){
				case '1':
					if (netHTTPs_LoginActive() == true) 
						len = sprintf(buf, &env[4],"checked=checked");
					else
						len = sprintf(buf, &env[4],"");
					break;
				
				case '2':
					if (netHTTPs_LoginActive() == true) 
						len = sprintf(buf, &env[4],"");
					else
						len = sprintf(buf, &env[4],"checked=checked");
					break;
			}
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

    case 's':
      //屏幕参数
      switch (env[2]) {
        case 'w':    //屏宽
          len = sprintf (buf, &env[4], screen.width);
          break;
				
        case 'h':    //屏高
          len = sprintf (buf, &env[4], screen.height);
          break;
        case 'l':   //屏幕亮度
          len = sprintf (buf, &env[4], screen.light);
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
