
/**
******************************************************************************
* @file   webpge.h
* @author  WIZnet Software Team 
* @version V1.0
* @date    2015-xx-xx
* @brief   web���������ڵ�����
* @attention  
******************************************************************************
**/
#ifndef __WEBPAGE_H
#define __WEBPAGE_H
#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>STM32���ƿ�</title>"\
"</head>"\
"<body>"\
"<div align='center'>���ƿ�����"\
"<form id='frmSetting' method='POST' action='updata_txt.cgi'>"\
"<p><label for='txtIp'>����1��ʾ���ı���</label>"\
"<input type='text' id='txtIp' name='displayText1' size='16'/></p>"\
"<p><label for='xg'>��ʾЧ����</albel>"\
"<select name='xg' id='xg'>"\
    "<option value='1'>��������</option>"\
    "<option value='2'>��ת</option>"\
    "<option value='3'>����</option>"\
  "</select>"\
"<p><input type='submit' value='�ύ' /></p>"\
"</form>"\
"</div>"\
"</body>"\
"</html>"

#endif
