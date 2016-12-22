#ifndef __FONT_H
#define __FONT_H 	   


/*
 * ����ASCII����ƫ����32����С:16���߶ȣ�* 8 �����ȣ�
 */
const unsigned char ascii_Dot [ 95 ] [ 16 ] = {       //Fixedsys����������ʽ������ȡģ	
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
{0x00,0x00,0x00,0x18,0x3c,0x3c,0x3c,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00},/*"!",1*/
{0x00,0x00,0x00,0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",2*/
{0x00,0x00,0x00,0x36,0x36,0x7f,0x36,0x36,0x36,0x7f,0x36,0x36,0x00,0x00,0x00,0x00},/*"#",3*/
{0x00,0x18,0x18,0x3c,0x66,0x60,0x30,0x18,0x0c,0x06,0x66,0x3c,0x18,0x18,0x00,0x00},/*"$",4*/
{0x00,0x00,0x70,0xd8,0xda,0x76,0x0c,0x18,0x30,0x6e,0x5b,0x1b,0x0e,0x00,0x00,0x00},/*"%",5*/
{0x00,0x00,0x00,0x38,0x6c,0x6c,0x38,0x60,0x6f,0x66,0x66,0x3b,0x00,0x00,0x00,0x00},/*"&",6*/
{0x00,0x00,0x00,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/
{0x00,0x00,0x00,0x0c,0x18,0x18,0x30,0x30,0x30,0x30,0x30,0x18,0x18,0x0c,0x00,0x00},/*"(",8*/
{0x00,0x00,0x00,0x30,0x18,0x18,0x0c,0x0c,0x0c,0x0c,0x0c,0x18,0x18,0x30,0x00,0x00},/*")",9*/
{0x00,0x00,0x00,0x00,0x00,0x36,0x1c,0x7f,0x1c,0x36,0x00,0x00,0x00,0x00,0x00,0x00},/*"*",10*/
{0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00},/*"+",11*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x1c,0x0c,0x18,0x00,0x00},/*",",12*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",13*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x1c,0x00,0x00,0x00,0x00},/*".",14*/
{0x00,0x00,0x00,0x06,0x06,0x0c,0x0c,0x18,0x18,0x30,0x30,0x60,0x60,0x00,0x00,0x00},/*"/",15*/
{0x00,0x00,0x00,0x1e,0x33,0x37,0x37,0x33,0x3b,0x3b,0x33,0x1e,0x00,0x00,0x00,0x00},/*"0",16*/
{0x00,0x00,0x00,0x0c,0x1c,0x7c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x00,0x00,0x00,0x00},/*"1",17*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x06,0x0c,0x18,0x30,0x60,0x7e,0x00,0x00,0x00,0x00},/*"2",18*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x06,0x1c,0x06,0x66,0x66,0x3c,0x00,0x00,0x00,0x00},/*"3",19*/
{0x00,0x00,0x00,0x30,0x30,0x36,0x36,0x36,0x66,0x7f,0x06,0x06,0x00,0x00,0x00,0x00},/*"4",20*/
{0x00,0x00,0x00,0x7e,0x60,0x60,0x60,0x7c,0x06,0x06,0x0c,0x78,0x00,0x00,0x00,0x00},/*"5",21*/
{0x00,0x00,0x00,0x1c,0x18,0x30,0x7c,0x66,0x66,0x66,0x66,0x3c,0x00,0x00,0x00,0x00},/*"6",22*/
{0x00,0x00,0x00,0x7e,0x06,0x0c,0x0c,0x18,0x18,0x30,0x30,0x30,0x00,0x00,0x00,0x00},/*"7",23*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x76,0x3c,0x6e,0x66,0x66,0x3c,0x00,0x00,0x00,0x00},/*"8",24*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x66,0x66,0x3e,0x0c,0x18,0x38,0x00,0x00,0x00,0x00},/*"9",25*/
{0x00,0x00,0x00,0x00,0x00,0x1c,0x1c,0x00,0x00,0x00,0x1c,0x1c,0x00,0x00,0x00,0x00},/*":",26*/
{0x00,0x00,0x00,0x00,0x00,0x1c,0x1c,0x00,0x00,0x00,0x1c,0x1c,0x0c,0x18,0x00,0x00},/*";",27*/
{0x00,0x00,0x00,0x06,0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x06,0x00,0x00,0x00,0x00},/*"<",28*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"=",29*/
{0x00,0x00,0x00,0x60,0x30,0x18,0x0c,0x06,0x0c,0x18,0x30,0x60,0x00,0x00,0x00,0x00},/*">",30*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x0c,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00},/*"?",31*/
{0x00,0x00,0x00,0x7e,0xc3,0xc3,0xcf,0xdb,0xdb,0xcf,0xc0,0x7f,0x00,0x00,0x00,0x00},/*"@",32*/
{0x00,0x00,0x00,0x18,0x3c,0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x00,0x00,0x00,0x00},/*"A",33*/
{0x00,0x00,0x00,0x7c,0x66,0x66,0x66,0x7c,0x66,0x66,0x66,0x7c,0x00,0x00,0x00,0x00},/*"B",34*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x60,0x60,0x60,0x66,0x66,0x3c,0x00,0x00,0x00,0x00},/*"C",35*/
{0x00,0x00,0x00,0x78,0x6c,0x66,0x66,0x66,0x66,0x66,0x6c,0x78,0x00,0x00,0x00,0x00},/*"D",36*/
{0x00,0x00,0x00,0x7e,0x60,0x60,0x60,0x7c,0x60,0x60,0x60,0x7e,0x00,0x00,0x00,0x00},/*"E",37*/
{0x00,0x00,0x00,0x7e,0x60,0x60,0x60,0x7c,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00},/*"F",38*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x60,0x60,0x6e,0x66,0x66,0x3e,0x00,0x00,0x00,0x00},/*"G",39*/
{0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00},/*"H",40*/
{0x00,0x00,0x00,0x3c,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00},/*"I",41*/
{0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x66,0x66,0x3c,0x00,0x00,0x00,0x00},/*"J",42*/
{0x00,0x00,0x00,0x66,0x66,0x6c,0x6c,0x78,0x6c,0x6c,0x66,0x66,0x00,0x00,0x00,0x00},/*"K",43*/
{0x00,0x00,0x00,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x7e,0x00,0x00,0x00,0x00},/*"L",44*/
{0x00,0x00,0x00,0x63,0x63,0x77,0x6b,0x6b,0x6b,0x63,0x63,0x63,0x00,0x00,0x00,0x00},/*"M",45*/
{0x00,0x00,0x00,0x63,0x63,0x73,0x7b,0x6f,0x67,0x63,0x63,0x63,0x00,0x00,0x00,0x00},/*"N",46*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x00,0x00,0x00,0x00},/*"O",47*/
{0x00,0x00,0x00,0x7c,0x66,0x66,0x66,0x7c,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00},/*"P",48*/
{0x00,0x00,0x00,0x3c,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x0c,0x06,0x00,0x00},/*"Q",49*/
{0x00,0x00,0x00,0x7c,0x66,0x66,0x66,0x7c,0x6c,0x66,0x66,0x66,0x00,0x00,0x00,0x00},/*"R",50*/
{0x00,0x00,0x00,0x3c,0x66,0x60,0x30,0x18,0x0c,0x06,0x66,0x3c,0x00,0x00,0x00,0x00},/*"S",51*/
{0x00,0x00,0x00,0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00},/*"T",52*/
{0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x00,0x00,0x00,0x00},/*"U",53*/
{0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00,0x00,0x00,0x00},/*"V",54*/
{0x00,0x00,0x00,0x63,0x63,0x63,0x6b,0x6b,0x6b,0x36,0x36,0x36,0x00,0x00,0x00,0x00},/*"W",55*/
{0x00,0x00,0x00,0x66,0x66,0x34,0x18,0x18,0x2c,0x66,0x66,0x66,0x00,0x00,0x00,0x00},/*"X",56*/
{0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x3c,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00},/*"Y",57*/
{0x00,0x00,0x00,0x7e,0x06,0x06,0x0c,0x18,0x30,0x60,0x60,0x7e,0x00,0x00,0x00,0x00},/*"Z",58*/
{0x00,0x00,0x00,0x3c,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3c,0x00},/*"[",59*/
{0x00,0x00,0x00,0x60,0x60,0x30,0x30,0x18,0x18,0x0c,0x0c,0x06,0x06,0x00,0x00,0x00},/*"\",60*/
{0x00,0x00,0x00,0x3c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3c,0x00},/*"]",61*/
{0x00,0x18,0x3c,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",62*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00},/*"_",63*/
{0x00,0x38,0x18,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/
{0x00,0x00,0x00,0x00,0x00,0x3c,0x06,0x06,0x3e,0x66,0x66,0x3e,0x00,0x00,0x00,0x00},/*"a",65*/
{0x00,0x00,0x00,0x60,0x60,0x7c,0x66,0x66,0x66,0x66,0x66,0x7c,0x00,0x00,0x00,0x00},/*"b",66*/
{0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0x60,0x60,0x60,0x66,0x3c,0x00,0x00,0x00,0x00},/*"c",67*/
{0x00,0x00,0x00,0x06,0x06,0x3e,0x66,0x66,0x66,0x66,0x66,0x3e,0x00,0x00,0x00,0x00},/*"d",68*/
{0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0x66,0x7e,0x60,0x60,0x3c,0x00,0x00,0x00,0x00},/*"e",69*/
{0x00,0x00,0x00,0x1e,0x30,0x30,0x30,0x7e,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00},/*"f",70*/
{0x00,0x00,0x00,0x00,0x00,0x3e,0x66,0x66,0x66,0x66,0x66,0x3e,0x06,0x06,0x7c,0x00},/*"g",71*/
{0x00,0x00,0x00,0x60,0x60,0x7c,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00},/*"h",72*/
{0x00,0x00,0x18,0x18,0x00,0x78,0x18,0x18,0x18,0x18,0x18,0x7e,0x00,0x00,0x00,0x00},/*"i",73*/
{0x00,0x00,0x0c,0x0c,0x00,0x3c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x78,0x00},/*"j",74*/
{0x00,0x00,0x00,0x60,0x60,0x66,0x66,0x6c,0x78,0x6c,0x66,0x66,0x00,0x00,0x00,0x00},/*"k",75*/
{0x00,0x00,0x00,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7e,0x00,0x00,0x00,0x00},/*"l",76*/
{0x00,0x00,0x00,0x00,0x00,0x7e,0x6b,0x6b,0x6b,0x6b,0x6b,0x63,0x00,0x00,0x00,0x00},/*"m",77*/
{0x00,0x00,0x00,0x00,0x00,0x7c,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00},/*"n",78*/
{0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0x66,0x66,0x66,0x66,0x3c,0x00,0x00,0x00,0x00},/*"o",79*/
{0x00,0x00,0x00,0x00,0x00,0x7c,0x66,0x66,0x66,0x66,0x66,0x7c,0x60,0x60,0x60,0x00},/*"p",80*/
{0x00,0x00,0x00,0x00,0x00,0x3e,0x66,0x66,0x66,0x66,0x66,0x3e,0x06,0x06,0x06,0x00},/*"q",81*/
{0x00,0x00,0x00,0x00,0x00,0x66,0x6e,0x70,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00},/*"r",82*/
{0x00,0x00,0x00,0x00,0x00,0x3e,0x60,0x60,0x3c,0x06,0x06,0x7c,0x00,0x00,0x00,0x00},/*"s",83*/
{0x00,0x00,0x00,0x30,0x30,0x7e,0x30,0x30,0x30,0x30,0x30,0x1e,0x00,0x00,0x00,0x00},/*"t",84*/
{0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x3e,0x00,0x00,0x00,0x00},/*"u",85*/
{0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00,0x00,0x00,0x00},/*"v",86*/
{0x00,0x00,0x00,0x00,0x00,0x63,0x6b,0x6b,0x6b,0x6b,0x36,0x36,0x00,0x00,0x00,0x00},/*"w",87*/
{0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x3c,0x18,0x3c,0x66,0x66,0x00,0x00,0x00,0x00},/*"x",88*/
{0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x0c,0x18,0xf0,0x00},/*"y",89*/
{0x00,0x00,0x00,0x00,0x00,0x7e,0x06,0x0c,0x18,0x30,0x60,0x7e,0x00,0x00,0x00,0x00},/*"z",90*/
{0x00,0x00,0x00,0x0c,0x18,0x18,0x18,0x30,0x60,0x30,0x18,0x18,0x18,0x0c,0x00,0x00},/*"{",91*/
{0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00},/*"|",92*/
{0x00,0x00,0x00,0x30,0x18,0x18,0x18,0x0c,0x06,0x0c,0x18,0x18,0x18,0x30,0x00,0x00},/*"}",93*/
{0x00,0x00,0x00,0x71,0xdb,0x8e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} /*"~",94*/
};  

#endif