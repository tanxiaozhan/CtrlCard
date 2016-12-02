i header.inc 
t <tr><td height="339" colspan="3" align="center" valign="top">
t <table width="100%" border="0" cellspacing="0" cellpadding="0">
t <tr><td align="center" valign="top">
t <form id="form1" name="form1" method="get" action="">
t <table width="900" border="0" cellpadding="0" cellspacing="0">
t <tr><td height="84" align="center" valign="middle"><h3>扫 描 设 置</h3></td></tr>
t <tr><td height="46" valign="top"><table width="100%" border="0" align="center" cellpadding="0" cellspacing="2">
t <tr><td width="33%" height="75">颜　　色：</td>
t <td width="63%"><label for="select2"></label>
t <select name="select2" class="tbs" id="select2">
t <option value="0">单色</option>
t <option value="1">双色</option>
t </select></td></tr>
t <tr><td height="75">扫　　描：</td>
t <td><label for="select3"></label>
t <select name="select3" class="tbs" id="select3">
t <option value="0">静态扫描</option>
t <option value="1">1/2扫描</option>
t <option value="2">1/4扫描</option>
t <option value="3">1/8扫描</option>
t <option value="4">1/16扫描</option>
t </select></td></tr>
t <tr><td height="75">每区扫描行数：</td>
t <td><label for="textfield"></label>
t <input name="textfield" type="text" class="tbs" id="textfield" size="10" /></td></tr>
t <tr><td height="75">每区扫描列数：</td>
t <td><input name="textfield2" type="text" class="tbs" id="textfield2" size="10" /></td></tr>
t <tr><td height="75">走线线型：</td>
t <td><label for="select4"></label>
t <select name="select4" class="tbs" id="select4">
t <option value="0">Z型以右上角为起点</option>
t <option value="1">Z型以右下角为起点</option>
t <option value="2">Z型以左上角为起点</option>
t <option value="3">Z弄以左下角为起点</option>
t </select></td></tr>
t <tr><td height="75">行 译 码：</td>
t <td><label for="select5"></label>
t <select name="select5" class="tbs" id="select5">
t <option value="0">138译码</option>
t <option value="1">ABCD直译</option>
t </select></td></tr>
t <tr><td height="75" colspan="2">
t <input type="checkbox" name="checkbox" id="checkbox" style="width:30px; height:30px;" />OE极性&nbsp;
t <input type="checkbox" name="checkbox" id="checkbox" style="width:30px; height:30px;"/>数据极性&nbsp;
t <input type="checkbox" name="checkbox" id="checkbox" style="width:30px; height:30px;" />时钟反向&nbsp;
t <input type="checkbox" name="checkbox" id="checkbox" style="width:30px; height:30px;" />锁存反向
t </td></tr></table></td></tr>
t <tr><td height="100" align="center">
t <button type="button" class="tbs" onclick="javascript:location.reload();";> 刷 新 </button>
t &nbsp;&nbsp;&nbsp;&nbsp;
t <button type="submit" class="tbs" > 提 交 </button></td></tr></table></form></td></tr></table></td></tr>
t <tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table></body></html>
t <SCRIPT  LANGUAGE="JavaScript">
t function power1(f){
t f.submit();}
t </Script>
. End of script must be closed with period.