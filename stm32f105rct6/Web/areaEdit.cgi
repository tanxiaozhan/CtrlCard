i header.inc
t <table width="100%" border="0" cellspacing="0" cellpadding="0">
t <tr><td align="center" valign="top">
t <form id="form1" name="form1" method="get" action="content.cgi">
t <table width="800" border="0" cellpadding="0" cellspacing="0">
t <tr><td height="75" colspan="3" align="center" valign="middle"><h3>分　区　设　置</h3></td></tr>
t <tr><td width="396" height="85" align="right" valign="middle"><img src=pabb.gif>分区号：</td>
t <td width="180" valign="middle">
c a n <input class=tbs name=areaNo type=text readonly=readonly size=10 value=%d />
t </td><td width="224">1-3</td></tr>
t <tr><td height="85" align="right" valign="middle"><img src=pabb.gif>X 坐标：</td><td>
c a x <input class=tbs name=X type=text id=X size=10 value=%d />
t </td><td>&nbsp;</td></tr>
t <tr><td height="85" align="right" valign="middle"><img src=pabb.gif>Y 坐标：</td><td>
c a y <input class=tbs name=Y type=text id=Y size=10  value=%d />
t </td><td align="left" valign="middle">&nbsp;</td></tr>
t <tr><td height="85" align="right" valign="middle"><img src=pabb.gif>宽　度：</td><td>
c a w <input class=tbs name=Width type=text id=Width size=10 value=%d />
t </td><td align="left" valign="middle">&nbsp;</td></tr>
t <tr><td height="85" align="right" valign="middle"><img src=pabb.gif>高　度：</td><td>
c a h <input class=tbs name=Height type=text id=Height size=10 value=%d />
t </td><td align="left" valign="middle">&nbsp;</td></tr>
t <tr><td height="100" colspan="3" align="center">
t <table width="90%" border="0" cellspacing="0" cellpadding="0">
t <tr><td width="25%" height="30" align="center">&nbsp;</td>
t <td width="30%" height="30" align="center">
t <input type="button" name="button" id="button" value=" 刷 新 " 
t class="tbs" onclick="window.location='areaEdit.cgi'" />
t </td><td width="5%" height="30" align="center">&nbsp;</td>
t <td width="27%" height="30" align="center">
t <input type="button" name="button2" id="button2" value=" 提 交 " class="tbs" onclick="check();" /></td>
t <td width="13%" height="30" align="center">&nbsp;</td></tr></table></td></tr>
t <tr><td height="50" colspan="3" align="center">&nbsp;</td></tr>
t </table></form></td></tr></table>
t <SCRIPT  LANGUAGE="JavaScript">
t function check(){
t var num;
t if(document.getElementById("X").value=="" || document.getElementById("Y").value =="" 
t ||document.getElementById("Width").value=="" || document.getElementById("Height").value=="")
t {alert("输入不完整!");return;}
t document.getElementById("form1").submit();}
t </Script>
i footer.inc
. End of script must be closed with period.