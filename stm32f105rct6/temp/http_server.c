/**
*@brief		����http�����Ĳ�����http��Ӧ
*@param		s: http������socket
*@param		buf��������������
*@return	��
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
	int8 req_name[32]={0x00,};															/*����һ��http��Ӧ���ĵ�ָ��*/
	unsigned long file_len=0;																/*����http������ͷ�Ľṹ��ָ��*/
	uint16 send_len=0;
	uint8* http_response;
	st_http_request *http_request;
	memset(tx_buf,0x00,MAX_URI_SIZE);
	http_response = (uint8*)rx_buf;
	http_request = (st_http_request*)tx_buf;
	parse_http_request(http_request, buf);    							/*����http������ͷ*/

	switch (http_request->METHOD)		
  {
		case METHOD_ERR :																			/*������ͷ����*/
			memcpy(http_response, ERROR_REQUEST_PAGE, sizeof(ERROR_REQUEST_PAGE));
			//send(s, (uint8 *)http_response, strlen((int8 const*)http_response));
			break;
		
		case METHOD_HEAD:																			/*HEAD����ʽ*/
			
		case METHOD_GET:																			/*GET����ʽ*/
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
			
		case METHOD_POST:																			/*POST����*/
			mid(http_request->URI, "/", " ", req_name);					/*��ȡ��������ļ���*/
			if(strcmp(req_name,"updata_txt.cgi")==0)							  	
			{
				get_http_response(http_request);												/*��ÿͻ��ˣ�����������ص�����*/
				make_cgi_response(5,tx_buf);	/*������Ӧ���ı�����*/        
				//printf("%s\r\n",tx_buf);
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);

				/*����http��Ӧ*/
				ESP8266_SendString ( DISABLE, (char *)http_response, strlen((char const*)http_response), Multiple_ID_0);
				printf("%s\r\n",http_response);

				//send(s, (u_char *)http_response, strlen((char *)http_response));		
				//disconnect(s);																		/*�Ͽ�socket����*/				
				//reboot_flag=1;																		/*������־λ��1*/
				return;
			}
			break;
			
		default :
			break;
	}
}

 /**
*@brief		  ����ÿһ��http��Ӧ
*@param		  request�� ����һ��ָ��
*@return	  ��
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
*@brief		  ִ��һ���𸴣��� html, gif, jpeg,etc.
*@param		  buf- ������
*@param			type- ����������
*@param			len-  �����ݳ���
*@return	  ��
*/
void make_http_response_head(unsigned char * buf,char type,uint32 len)
{
  char * head;
  char tmp[10];
  memset(buf,0x00,MAX_URI_SIZE); 
  /* �ļ�����*/
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
*@brief	 	�ַ�������
*@param		srcĿ���ַ��� s1 s2�����ַ���
*@return	��
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
*@brief		��ÿͻ��ˣ�����������ص�����
*@param		http_request������һ��http����Ľṹ��ָ��
*@return	��
*/
void get_http_response(st_http_request *http_request)
{ 
  uint8 * param;
  param = get_http_param_value(http_request->URI,"displayText1");		/*��ȡ��������ص�����  */
  if(param)
  {
    printf("displayText = %s\r\n",param);	
  }
  param = get_http_param_value(http_request->URI,"xg");		/*��ȡ��������ص�����  */
	if(param)
	{
		printf("xg = %s\r\n",param);
	}
}

/**
*@brief		ִ��http��Ӧ
*@param		��  
*@return	��
*/
void make_cgi_response(uint16 delay, int8* cgi_response_buf)
{
  sprintf(cgi_response_buf,"<html><head><title>���ƿ����� - ���</title></head><body >"\
	"<div align='center'>������ɣ�</div></body></html>");
	return;
}

 /**
*@brief		  �õ���Ӧ�����е���һ������
*@param		  url����Ҫת����ҳ��ַ
*@param			param_name�� 
*@return	  ����һ������
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
*@brief	 	�ַ�תת��Ϊ8λ���ͺ���
*@param		str:Ҫת���ַ����� base:
*@return	num:����ת�����������
*/
uint16 atoi16(char* str,uint16 base	)
{
  unsigned int num = 0;
  while (*str !=0)
          num = num * base + c2d(*str++);
  return num;
}


/**
*@brief	 	��ʮ������ת��Ϊ�ַ���
*@param		c:Ҫת��ʮ��������
*@return	����һ���ַ�������
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
*@brief	 	���µ��ַ�ȥ�滻�ַ�����������ַ�
*@param		str:�滻���ַ�����oldchar:������ַ���newchar���µ��ַ�	
*@return	��
*/
void replacetochar(char * str,	char oldchar,char newchar	)
{
  int x;
  for (x = 0; str[x]; x++) 
    if (str[x] == oldchar) str[x] = newchar;	
}

 /**
*@brief		  ת��ת���ַ�Ϊascii charater 
*@param		  url����Ҫת����ҳ��ַ
*@return	  ��
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

