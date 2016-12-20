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
#include "LED_Display.h"
#include "rtc.h"

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

extern bool power_on;


char current_area=0;   //当前显示区号


// My structure of CGI status variable.
typedef struct {
  uint16_t xcnt;
  uint16_t unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)



//把显示屏各类参数保存到外部闪存，地址0-499
//地址分配详见Doc/extern_flash_address.txt
void save_display_para_to_flash(void){

	uint8_t buff[100];
	char i;
	//擦除地址0开始的4K存储扇区
	SPI_FLASH_SectorErase(0);


	
	//保存屏幕参数
	buff[0]=screen.width/256;
	buff[1]=screen.width%256;
	buff[2]=screen.height/256;
	buff[3]=screen.height%256;
	buff[4]=screen.light/256;
	buff[5]=screen.light%256;
	buff[6]=screen.color;
	buff[7]=screen.scan_type;
	//buff[8]=screen.area_number;
	buff[8]=MAX_AREA_NUMBER;
  SPI_FLASH_BufferWrite(buff, 0, 9);

//保存网站认证数据
  if (netHTTPs_LoginActive() == true) 
		buff[0]=1;
	else
		buff[0]=0;
	
	SPI_FLASH_BufferWrite(buff, 50, 1);

	strcpy((char *)buff,netHTTPs_GetPassword());
	
  SPI_FLASH_BufferWrite(buff, 0, strlen((char *)buff));
	
	//保存各显示区参数
	for(i=0;i<screen.area_number;i++){
		buff[0]=i;
		buff[1]=area[i].x/256;
		buff[2]=area[i].x%256;
		buff[3]=area[i].y/256;
		buff[4]=area[i].y%256;
		buff[5]=area[i].width/256;
		buff[6]=area[i].width%256;
		buff[7]=area[i].height/256;
		buff[8]=area[i].height%256;
		buff[9]=area[i].red;
		buff[10]=area[i].green;
		buff[11]=area[i].ani_in;
		buff[12]=area[i].ani_out;
		buff[13]=area[i].speed;
		buff[14]=area[i].content_type;
		buff[15]=area[i].length;

	  SPI_FLASH_BufferWrite(buff, (i+1)*100, 16);
		SPI_FLASH_BufferWrite(area[i].display_data, (i+1)*100+16, area[i].length);
		
	
	}
}

// Process query string received by GET request.
void netCGI_ProcessQuery (const char *qstr) {
  char var[40];
	/*时间结构体*/
	struct rtc_time real_time;
	//printf("cgi_process. qstr:%s\n",qstr);
  do {
    // Loop through all the parameters
    qstr = netCGI_GetEnvVar (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter
    //printf("var[]:%s\n",var);
		if (var[0] != 0) {
      // First character is non-null, string exists
			if (strncmp (var, "areaNo=", 7) == 0) {
        //设置/修改当前显示分区
				current_area=(char)(var[7]-49);
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
				area[current_area].height=atoi(&var[7]);
				save_display_para_to_flash();
      }
      else if (strncmp (var, "ScreenWidth=", 12) == 0) {
        // 屏幕宽
				screen.width=atoi(&var[12]);
      }
      else if (strncmp (var, "ScreenHeight=", 13) == 0) {
        // 屏幕高
				screen.height=atoi(&var[13]);
      }
      else if (strncmp (var, "ScreenLight=", 12) == 0) {
        // 屏幕亮度
				screen.light=atoi(&var[12]);
				save_display_para_to_flash();
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
				//删除显示分区2
				del_area(2);
			}
			else if (strncmp (var, "strDate=", 8) == 0) {
				//系统校时，网页返回日期时间格式：yyyy-mm-dd-ww-hh-mm-ss
				strcpy((char *)area[current_area].display_data,&var[8]);
				real_time.rt_year=atoi(strtok( var+8, "-" ));
				real_time.rt_mon=atoi(strtok( NULL,"-" ))+1;
				real_time.rt_date=atoi(strtok( NULL,"-" ));
				real_time.rt_day=atoi(strtok( NULL,"-" ));
				real_time.rt_hour=atoi(strtok( NULL,"-" ));
				real_time.rt_min=atoi(strtok( NULL,"-" ));
				real_time.rt_sec=atoi(strtok( NULL,"-" ));
				Time_Adjust(&real_time);    //校时
			}
			else if (strncmp (var, "power=OFF", 9) == 0) {
				//关机
				LED_display_power_off();				
			}
			else if (strncmp (var, "power=ON", 8) == 0) {
				//开机
				LED_display_start();
			}
			else if (strncmp (var, "chk=ALLON", 9) == 0) {
				//屏幕检测，红绿全亮
				check_screen(true,true,true);
			}
			else if (strncmp (var, "chk=RED", 7) == 0) {
				//屏幕检测，红色
				check_screen(true,true,false);
			}
			else if (strncmp (var, "chk=GREEN", 9) == 0) {
				//屏幕检测
				check_screen(true,false,true);
			}
			else if (strncmp (var, "chk=ALLOFF", 10) == 0) {
				//屏幕检测，全灭
				check_screen(true,false,false);
			}
			else if (strncmp (var, "chk=EXIT", 8) == 0) {
				//退出屏幕检测
				check_screen(false,false,false);
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
	//printf("Process data received by Post request.\n Type code:%d\n Post data:%s\n Post len:%d\n",code,data,len);
  if (code != 0) {
    // Ignore all other codes
    return;
  }

  if (len == 0) {
    // No data or all items (radio, checkbox) are off
    return;
  }
  passw[0] = 1;
  do {
    // Parse all parameters
    data = netCGI_GetEnvVar (data, var, sizeof (var));
    if (var[0] != 0) {
      // First character is non-null, string exists
      if (strncmp (var, "txt=",4) == 0) {
        //更新显示数据
				strcpy((char *)area[current_area].display_data,var+4);
				area[current_area].length=strlen((char *)area[current_area].display_data);
				save_display_para_to_flash();
      }
			else if (strncmp (var, "auth=true", 9) == 0) {
				//打开身份认证
				//http_EnAuth=true;
				netHTTPs_LoginOnOff (true);
				save_display_para_to_flash();
			}
			else if (strncmp (var, "auth=false", 10) == 0) {
				//关闭身份认证
				netHTTPs_LoginOnOff (false);
				save_display_para_to_flash();
			}
      else if ((strncmp (var, "pw0=", 4) == 0) ||
               (strncmp (var, "pw2=", 4) == 0)) {
        // 修改密码
        if (http_EnAuth) {
          if (passw[0] == 1) {
            strcpy (passw, var+4);
          }
          else if (strcmp (passw, var+4) == 0) {
            // 二次输入的密码相同
            strcpy (http_auth_passw, passw);
						save_display_para_to_flash();
          }
        }
      }
    }
  } while (data);
}

// Generate dynamic web data from a script line.
uint32_t netCGI_Script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  uint32_t len = 0;
  uint8_t id;
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
				case 'a':
					//给数组赋值，显示分区已存在=1，不存在=0
					for(id=0;id<MAX_AREA_NUMBER;id++)
						if(area[id].width>0)
							len += sprintf(buf+len,"areaFlag[%d]=1;",id);
						else
							len += sprintf(buf+len,"areaFlag[%d]=0;",id);
					break;
      }
      break;

    case 'c':
      //内容管理
			id=(uint8_t)env[2]-48;
			if( area[id].width<1 || area[id].height<1 )   //显示分区是否存在
				break;
				
				len += sprintf(buf+len,"<tr><td align=center class=td>");
				len += sprintf(buf+len,"<input name=delArea%d type=checkbox class=tbs style=width:40px;height:40px/></td>",id);
				len += sprintf(buf+len,"<td align=center class=td>%d</td>",id+1);
				len += sprintf(buf+len,"<td align=center class=td>%d</td>",area[id].x);
				len += sprintf(buf+len,"<td align=center class=td>%d</td>",area[id].y);
				len += sprintf(buf+len,"<td align=center class=td>%d</td>",area[id].width);
				len += sprintf(buf+len,"<td align=center class=td>%d</td>",area[id].height);
				len += sprintf(buf+len,"<td align=center class=td>%d</td>",area[id].content_type);
				len += sprintf(buf+len,"<td align=center style='border-bottom:1px solid #000080'>");
				len += sprintf(buf+len,"<button class=tbs type=button onclick=\"window.location='edit.cgi?areaNo=%d'\">内容编辑</button>",id+1);
				len += sprintf(buf+len,"<p><button class=tbs type=button onclick=\"window.location='areaEdit.cgi?areaNo=%d'\">分区编辑</button></p></td></tr>",id+1);
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
      //登录设置、修改密码 'auth.cgi, password.cgi'
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

    case 'p':
      //开机关机 'power.cgi'
      switch (env[2]) {
        case '1':
					if ( power_on ){
						len = sprintf (buf, &env[4], "OFF"); 
					}
					else{
						len = sprintf (buf, &env[4], "ON"); 
					}
          break;
        case '2':
					if ( power_on ){
						len = sprintf (buf, &env[4], "　关　机　"); //设置开关机页面提交按钮文字为关机
					}
					else{
						len = sprintf (buf, &env[4], "　开　机　"); //lcd_text[0]);
					}
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

  }

  return (len);
}
