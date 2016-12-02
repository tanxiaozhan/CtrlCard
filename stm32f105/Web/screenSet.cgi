i header.inc
t <table width="90%" border="0" cellspacing="0" cellpadding="0">
t <tr><td align="center" valign="top">
t <form id="form1" name="form1" method="get" action="">
t <table width="700" border="0" cellpadding="0" cellspacing="0">
t <tr><td height="84" colspan="3" align="center" valign="middle"><h3>屏 幕 参 数 设 置</h3></td></tr>
t <tr><td width="332" height="85" align="right" valign="middle"><img src=pabb.gif height="40px">屏宽：</td>
t <td width="180" valign="middle">
c s w <input class=tbs name=ScreenWidth type=text id=Width size=10 value=%d />
t </td><td width="188">&nbsp;</td></tr>
t <tr><td height="85" align="right" valign="middle"><img src=pabb.gif height="40px">屏高：</td><td>
c s h <input class=tbs name=ScreenHeight type=text id=Height size=10 value=%d />
t </td><td>&nbsp;</td></tr>
t <tr><td height="85" align="right" valign="middle"><img src=pabb.gif height="40px">亮度：</td><td>
c s l <input class=tbs name=ScreenLight type=text id=Light size=10 value=%d />
t </td><td align="left" valign="middle">1-100</td></tr>
t <tr><td height="100" colspan="2" align="center">
t <table width="90%" border="0" cellspacing="0" cellpadding="0">
t <tr><td width="22%" height="30" align="center">&nbsp;</td>
t <td width="28%" height="30" align="center" >
t <button class="tbs" type="button" onclick="window.location='screenSet.html'" >刷 新</button></td>
t <td width="10%" height="30" align="center">&nbsp;</td>
t <td width="27%" height="30" align="center" >
t <button class="tbs" type="button" onclick="check();">提 交</button></td>
t <td width="13%" height="30" align="center">&nbsp;</td></tr>
t </table></td><td>&nbsp;</td></tr>
t <tr><td height="50" colspan="3" align="center">&nbsp;</td></tr>
t </table>
t </form></td></tr></table>
t <SCRIPT  LANGUAGE="JavaScript">
t function check(){
t var num;
t if(document.getElementById("Light").value=="" ||document.getElementById("Width").value=="" 
t || document.getElementById("Height").value==""){alert("输入不完整!");return;}
t num=parseInt(document.getElementById("Light").value); 
t if((num) >100 || (num) < 1){alert("屏幕亮度只能输入1-100之间的整数！");
t document.getElementById("Light").focus();return;}
t document.getElementById("result").innerHTML="屏幕参数设置完成！";
t document.getElementById("form1").submit();}
t </Script>
i footer.inc
. End of script must be closed with period