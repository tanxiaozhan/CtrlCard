/**
*@brief		接收http请求报文并发送http响应
*@param		s: http服务器socket
*@param		buf：解析报文内容
*@return	无
*/
#include <stdio.h>
#include<string.h>
#include "http_server.h"
#include "webpage.h"
#include "bsp_esp8266.h"
#include "stm32f10x.h"

char tx_buf[MAX_URI_SIZE];
char rx_buf[MAX_URI_SIZE];
uint8 pub_buf[1024];

void proc_http(uint8 * buf)
{
	int8* name; 											
	int8 req_name[32]={0x00,};															/*定义一个http响应报文的指针*/
	unsigned long file_len=0;																/*定义http请求报文头的结构体指针*/
	uint16 send_len=0;
	uint8* http_response;
	st_http_request *http_request;
	memset(tx_buf,0x00,MAX_URI_SIZE);
	http_response = (uint8*)rx_buf;
	http_request = (st_http_request*)tx_buf;
	parse_http_request(http_request, buf);    							/*解析http请求报文头*/

	switch (http_request->METHOD)		
  {
		case METHOD_ERR :																			/*请求报文头错误*/
			memcpy(http_response, ERROR_REQUEST_PAGE, sizeof(ERROR_REQUEST_PAGE));
			//send(s, (uint8 *)http_response, strlen((int8 const*)http_response));
			break;
		
		case METHOD_HEAD:																			/*HEAD请求方式*/
			
		case METHOD_GET:																			/*GET请求方式*/
			name = http_request->URI;
			if(strcmp(name,"/index.htm")==0 || strcmp(name,"/")==0 || (strcmp(name,"/index.html")==0))
			{
				file_len = strlen(INDEX_HTML);
				make_http_response_head((uint8*)http_response, PTYPE_HTML,file_len);
				ESP8266_SendString ( DISABLE, (char *)http_response, strlen((char const*)http_response), Multiple_ID_0);
				printf("%s\r\n",http_response);
				
				//send(s,http_response,strlen((char const*)http_response));
				send_len=0;
				while(file_len)
				{
					if(file_len>1024)
					{
						//if(getSn_SR(s)!=SOCK_ESTABLISHED)
						//{
						//	return;
						//}
						//send(s, (uint8 *)INDEX_HTML+send_len, 1024);
				ESP8266_SendString ( DISABLE, (char *)INDEX_HTML+send_len, strlen((char const*)(INDEX_HTML + send_len)), Multiple_ID_0);

						send_len+=1024;
						file_len-=1024;
					}
					else
					{
						//send(s, (uint8 *)INDEX_HTML+send_len, file_len);
				ESP8266_SendString ( DISABLE, (char *)INDEX_HTML+send_len, file_len, Multiple_ID_0);

						send_len+=file_len;
						file_len-=file_len;
						printf("%s\r\n",(char *)INDEX_HTML+send_len);
					} 
				}
			}

			break;
			
		case METHOD_POST:																			/*POST请求*/
			mid(http_request->URI, "/", " ", req_name);					/*获取该请求的文件名*/
			if(strcmp(req_name,"updata_txt.cgi")==0)							  	
			{
				get_http_response(http_request);												/*获得客户端（浏览器）返回的数据*/
				make_cgi_response(5,tx_buf);	/*生成响应的文本部分*/        
				//printf("%s\r\n",tx_buf);
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);

				/*发送http响应*/
				ESP8266_SendString ( DISABLE, (char *)http_response, strlen((char const*)http_response), Multiple_ID_0);
				printf("%s\r\n",http_response);

				//send(s, (u_char *)http_response, strlen((char *)http_response));		
				//disconnect(s);																		/*断开socket连接*/				
				//reboot_flag=1;																		/*重启标志位置1*/
				return;
			}
			break;
			
		default :
			break;
	}
}

 /**
*@brief		  解析每一个http响应
*@param		  request： 定义一个指针
*@return	  无
*/
void parse_http_request(st_http_request * request,u_char * buf)
{
  char * nexttok;
  nexttok = strtok((char*)buf," ");
  if(!nexttok)
  {
    request->METHOD = METHOD_ERR;
    return;
  }
  if(!strcmp(nexttok, "GET") || !strcmp(nexttok,"get"))
  {
    request->METHOD = METHOD_GET;
    nexttok = strtok(NULL," ");
   			
  }
  else if (!strcmp(nexttok, "HEAD") || !strcmp(nexttok,"head"))	
  {
    request->METHOD = METHOD_HEAD;
    nexttok = strtok(NULL," ");
  		
  }
  else if (!strcmp(nexttok, "POST") || !strcmp(nexttok,"post"))
  {
    nexttok = strtok(NULL,"\0");//20120316
    //nexttok = strtok(NULL," ");
    request->METHOD = METHOD_POST;
 		
  }
  else
  {
    request->METHOD = METHOD_ERR;
  }	 
  if(!nexttok)
  {
    request->METHOD = METHOD_ERR; 			
    return;
  }
  strcpy(request->URI,nexttok); 					
}

 /**
*@brief		  执行一个答复，如 html, gif, jpeg,etc.
*@param		  buf- 答复数据
*@param			type- 答复数据类型
*@param			len-  答复数据长度
*@return	  无
*/
void make_http_response_head(unsigned char * buf,char type,uint32 len)
{
  char * head;
  char tmp[10];
  memset(buf,0x00,MAX_URI_SIZE); 
  /* 文件类型*/
  if 	(type == PTYPE_HTML) head = RES_HTMLHEAD_OK;
  else if (type == PTYPE_GIF)	head = RES_GIFHEAD_OK;
  else if (type == PTYPE_TEXT)	head = RES_TEXTHEAD_OK;
  else if (type == PTYPE_JPEG)	head = RES_JPEGHEAD_OK;
  else if (type == PTYPE_FLASH)	head = RES_FLASHHEAD_OK;
  else if (type == PTYPE_MPEG)	head = RES_MPEGHEAD_OK;
  else if (type == PTYPE_PDF)	head = RES_PDFHEAD_OK;

  sprintf(tmp,"%ld", len);	
  strcpy((char*)buf, head);
  strcat((char*)buf, tmp);
  strcat((char*)buf, "\r\n\r\n");
  //printf("%s\r\n", buf);
}

/**
*@brief	 	字符串处理
*@param		src目标字符串 s1 s2操作字符串
*@return	无
*/
void mid(int8* src, int8* s1, int8* s2, int8* sub)
{
	int8* sub1;
	int8* sub2;
	uint16 n;

  sub1=strstr(src,s1);
  sub1+=strlen(s1);
  sub2=strstr(sub1,s2);
  n=sub2-sub1;
  strncpy(sub,sub1,n);
  sub[n]=0;
}

/**
*@brief		获得客户端（浏览器）返回的数据
*@param		http_request：定义一个http请求的结构体指针
*@return	无
*/
void get_http_response(st_http_request *http_request)
{ 
  uint8 * param;
  param = get_http_param_value(http_request->URI,"displayText1");		/*获取浏览器返回的数据  */
  if(param)
  {
    printf("displayText = %s\r\n",param);	
  }
  param = get_http_param_value(http_request->URI,"xg");		/*获取浏览器返回的数据  */
	if(param)
	{
		printf("xg = %s\r\n",param);
	}
}

/**
*@brief		执行http响应
*@param		无  
*@return	无
*/
void make_cgi_response(uint16 delay, int8* cgi_response_buf)
{
  sprintf(cgi_response_buf,"<html><head><title>控制卡设置 - 完成</title></head><body >"\
	"<div align='center'>设置完成！</div></body></html>");
	return;
}

 /**
*@brief		  得到响应过程中的下一个参数
*@param		  url：需要转化网页地址
*@param			param_name： 
*@return	  返回一个数据
*/
uint8* get_http_param_value(char* uri, char* param_name)
{
  uint16 len;
  uint8* pos2;
  uint8* name=0; 
  uint8 *ret=pub_buf;
  uint16 content_len=0;
  int8 tmp_buf[10]={0x00,};
  if(!uri || !param_name) return 0;
  /***************/
  mid(uri,"Content-Length: ","\r\n",tmp_buf);
  content_len=atoi16(tmp_buf,10);
  //printf("content len=%d\r\n",content_len);
  uri = (int8*)strstr(uri,"\r\n\r\n");
  uri+=4;
  //printf("uri=%s\r\n",uri);
  uri[content_len]=0;
  /***************/	 
  name= (uint8*)strstr(uri,param_name);
  if(name)
  {
    name += strlen(param_name) + 1; 
    pos2=(uint8*)strstr((char*)name,"&");
    if(!pos2) 
    {
      pos2=name+strlen((char*)name);
    }
    len=0;
    len = pos2-name;
 
    if(len)
    {
      ret[len]=0;
      strncpy((char*)ret,(char*)name,len);
      unescape_http_url((char *)ret);
      replacetochar((char *)ret,'+',' ');
    }
    else
      ret[0]=0;
  }
  else
    return 0;
  return ret;		
}

/**
*@brief	 	字符转转化为8位整型函数
*@param		str:要转化字符串， base:
*@return	num:返回转化后的整型数
*/
uint16 atoi16(char* str,uint16 base	)
{
  unsigned int num = 0;
  while (*str !=0)
          num = num * base + c2d(*str++);
  return num;
}


/**
*@brief	 	把十进制数转化为字符型
*@param		c:要转化十进制数据
*@return	返回一个字符型数据
*/
char c2d(uint8 c	)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c -'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c -'A';

	return (char)c;
}


/**
*@brief	 	用新的字符去替换字符串中特殊的字符
*@param		str:替换后字符串，oldchar:特殊的字符，newchar：新的字符	
*@return	无
*/
void replacetochar(char * str,	char oldchar,char newchar	)
{
  int x;
  for (x = 0; str[x]; x++) 
    if (str[x] == oldchar) str[x] = newchar;	
}

 /**
*@brief		  转化转义字符为ascii charater 
*@param		  url：需要转化网页地址
*@return	  无
*/
void unescape_http_url(char * url)
{
  int x, y;
  
  for (x = 0, y = 0; url[y]; ++x, ++y) 
  {
    if ((url[x] = url[y]) == '%') 
	{
      url[x] = c2d(url[y+1])*0x10+c2d(url[y+2]);
      y+=2;
    }
  }
  url[x] = '\0';
}

