i header1.inc
t <tr><td height="339" colspan="3" align="center" valign="top">
t <table width="100%" border="0" cellspacing="0" cellpadding="0">
t <tr><td align="center" valign="top">
t <form id="form1" name="form1" method="get" action="">
t <table width="800" border="0" cellpadding="0" cellspacing="0">
t <tr><td height="70" align="center" valign="middle"><h3>屏 幕 检 测</h3></td></tr>
t <tr><td height="25"><input type="hidden" name="chk" id="chk"/></td></tr>
t <tr><td height="100" align="center">
t <button type=button class=tbs onclick="power1('ALL');" style="background:#FF0">红绿全亮</button>
t <button type=button class=tbs onclick="power1('RED');" style="background:#F00">红色全亮</button></td></tr>
t <tr><td height="100" align="center">
t <button type="button" class="tbs" onclick="power1('GREEN');" style="background:#0F0"> 绿色全亮 </button>
t <button type="button" class="tbs" onclick="power1('OFF');"> 退出检测 </button></td></tr>
t <tr><td height="100" align="center">&nbsp;</td></tr></table></form></td></tr></table></td></tr>
t <tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table></body></html>
t <SCRIPT  LANGUAGE="JavaScript">
t function power1(p){
t document.getElementById("chk").value=p;
t document.getElementById("form1").submit();}
t </Script>
. End of script must be closed with period.