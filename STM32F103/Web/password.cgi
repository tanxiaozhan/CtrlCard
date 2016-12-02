i header.inc
t <h3 align=center>修　改　密　码</h3>
t <form action="" method=post name=cgi>
t <table width=80% border=0 align="center">
t <tr><td width="45%" align="right">
t <img src=pabb.gif height="40px">用　　户：</TD><TD width="55%">admin</td></tr>
t <tr><td align="right"><img src=pabb.gif height="40px">密　　码：</td>
t <td><input name=pw0 type=password id=pw0 class=tbs size=10 maxlength=10></td></tr>
t <tr><td align="right"><img src=pabb.gif height="40px">确认密码：</td>
t <td><input type=password name=pw2 id=pw2 size=10 maxlength=10 class=tbs></td></tr>
t </table><p align=center>
t <input type=button name=set value="提 交" onClick="changeConfirm(this.form)" class="tbs">
t &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=reset value="清 除" class="tbs"></p></form>
t <script language=JavaScript>
t function changeConfirm(f){
t if(document.getElementById("pw0").value != document.getElementById("pw2").value){
t alert("二次输入的密码不相同，请重新输入。"); document.getElementById("pw0").focus();	return;}
t document.getElementById("result").innerHTML="密码修改完成！";
t f.submit();}
t </script>
i footer.inc
. End of script must be closed with period