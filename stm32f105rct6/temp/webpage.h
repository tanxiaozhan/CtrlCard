
/**
******************************************************************************
* @file   webpge.h
* @author  WIZnet Software Team 
* @version V1.0
* @date    2015-xx-xx
* @brief   web服务器现在的内容
* @attention  
******************************************************************************
**/
#ifndef __WEBPAGE_H
#define __WEBPAGE_H
#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>STM32控制卡</title>"\
"</head>"\
"<body>"\
"<div align='center'>控制卡设置"\
"<form id='frmSetting' method='POST' action='updata_txt.cgi'>"\
"<p><label for='txtIp'>区域1显示的文本：</label>"\
"<input type='text' id='txtIp' name='displayText1' size='16'/></p>"\
"<p><label for='xg'>显示效果：</albel>"\
"<select name='xg' id='xg'>"\
    "<option value='1'>从左往右</option>"\
    "<option value='2'>旋转</option>"\
    "<option value='3'>渐显</option>"\
  "</select>"\
"<p><input type='submit' value='提交' /></p>"\
"</form>"\
"</div>"\
"</body>"\
"</html>"

#endif
