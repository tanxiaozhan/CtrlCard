i header.inc
t <table width="100%" border="0" cellspacing="0" cellpadding="0">
t <tr><td align="center" valign="top">
t <form name="form1" method="get" action="">
t <table width="800" border="0" cellpadding="0" cellspacing="0">
t <tr><td height="84" align="center" valign="middle"><h3>系 统 校 时</h3></td></tr>
t <tr><td height="21"><input type="hidden" name="strDate" id="strDate" /></td></tr>
t <tr><td height="100" align="center">
t <button type="button" class="tbs" onclick=genTime(this.form);>　校　时　</button></td></tr>
t </table>
t </form></td></tr></table>
t <SCRIPT LANGUAGE="JavaScript">
t function genTime(f){
t var d = new Date();
t var strD;
t strD=d.getFullYear() + "-" + d.getMonth() + "-" + d.getDate() +"-"+ d.getDay()+"-";
t strD=strD + d.getHours() +"-" + d.getMinutes() +"-" + d.getSeconds();
t document.getElementById("strDate").value=strD;
t document.getElementById("result").innerHTML="校时完成！";
t f.submit();}
t </Script>
i footer.inc
. End of script must be closed with period