i header.inc
t <table width="100%" border="0" cellspacing="0" cellpadding="0">
t <tr><td align="center" valign="top">
t <form id="form1" name="form1" method="post" action="content.cgi">
t <h3>内　容　编 　辑</h3>
t <table width="100%" border="1" cellspacing="0" cellpadding="0">
t <tr><td>进入设置</td></tr>
t <tr><td>退出设置</td></tr>
t <tr><td>
t <table width="100%" border="0" cellspacing="0" cellpadding="0">
t <tr><td width="36%">字体
t <select name="fontselect" id="fontselect" class="tbs">
t <option value="0">宋体_16_常规</option>
t <option value="1">宋体_16_加粗</option>
t </select></td>
t <td width="28%"><input type="checkbox" name="single" id="single" class="tbs" style="width:40px;"/>单行文本</td>
t <td width="36%">水平
t <select name="select" id="select" class="tbs">
t <option value="0">左对齐</option>
t <option value="1">居中对齐</option>
t <option value="2">右对齐</option>
t </select></td></tr>
t <tr><td>颜色
t <select name="colorselect" id="colorselect" class="tbs">
t <option value="0">默认</option>
t <option value="1">红色</option>
t <option value="2">绿色</option>
t </select></td>
t <td><input type="checkbox" name="line" id="line" class="tbs" style="width:40px;" />自动换行</td>
t <td>垂直
t <select name="select2" id="select2" class="tbs">
t <option value="0">顶端对齐</option>
t <option value="1">居中对齐</option>
t <option value="2">底端对齐</option>
t </select></td></tr>
t </table></td></tr>
t <tr><td height="92">
c e t <textarea name=txt id=txt cols=30 rows=5 class=tbs style=height:200px>%s
t </textarea></td></tr>
t </table>
t <table width="90%" border="0" cellspacing="0" cellpadding="0">
t <tr><td height="57"><input type="hidden" name="delArea" id="delArea" value="F" /></td>
t <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
t <tr><td width="15%" height="57" align="center">&nbsp;</td>
t <td width="28%" height="57" align="center">
t <button class="tbs" type="button" onclick="check(this.form);">删 除</button></td>
t <td width="12%" height="57" align="center">&nbsp;</td>
t <td width="28%" height="57" align="center">
t <button class="tbs" type="submit">提 交</button></td>
t <td width="17%" height="57" align="center">&nbsp;</td></tr>
t </table><p>&nbsp;</p>
t </form></td></tr>
t </table>
t </body>
t </html>
t <SCRIPT  LANGUAGE="JavaScript">
t function check(f){
t if( confirm("确认删除该分区吗？") ){
t document.getElementById("delArea").value="T";
t f.submit();}}
t </Script>
i footer.inc
. End of script must be closed with period.