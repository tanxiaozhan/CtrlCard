i header.inc
t <table width="100%" border="0" cellspacing="0" cellpadding="0" style="font-size:30px">
t <tr><td align="center" valign="top">
t <form id="form1" name="form1" method="get" action=""><h3>内　容　管　理</h3>
t <table width="100%" cellspacing="0" cellpadding="0" style="border-top:1px solid">
t <tr><td width="8%" align="center" class="td" >选择</td>
t <td width="6%" align="center" class="td">ID</td>
t <td width="12%" align="center" class="td">X</td>
t <td width="12%" align="center" class="td">Y</td>
t <td width="12%" align="center" class="td">宽度</td>
t <td width="12%" align="center" class="td">高度</td>
t <td width="16%" align="center" class="td">内容类型</td>
t <td width="22%" align="center" style="border-bottom:1px solid #000080">编辑</td></tr>
c c 0 area 0
c c 1 area 1
c c 2 area 2
t </table>
t <table width="90%" border="0" cellspacing="0" cellpadding="0">
t <tr><td height="57">&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
t <tr><td width="15%" height="57" align="center">&nbsp;</td>
t <td width="28%" height="57" align="center">
t <button class="tbs" type="submit" value="delArea">删除分区</button></td>
t <td width="12%" height="57" align="center">&nbsp;</td>
t <td width="28%" height="57" align="center">
t <input type="button" class="tbs" onclick="window.location='areaSet.cgi?areaNo=0'" value="新增分区" /></td>
t <td width="17%" height="57" align="center">&nbsp;</td></tr>
t </table>
t </form></td></tr>
t </table>
i footer.inc
. End of script must be closed with period