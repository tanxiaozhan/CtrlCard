#include <stdio.h>
#include <stdlib.h>

#define fontHeigth 16
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	unsigned int dot[fontHeigth*4]={
	//0x00,0x00,0x00,0x78,0x0C,0x0C,0x0C,0x7E,0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00	//"f"
	0xff,0xff,0xff,0xc3,0x99,0x99,0xf9,0xe3,0xf9,0x99,0x99,0xc3,0xff,0xff,0xff,0xff,
	0xff,0x00,0xfe,0xfe,0xc0,0xce,0xd6,0xda,0xce,0xd6,0xda,0xde,0xde,0xde,0xde,0xde,
	0xff,0x0f,0xff,0xff,0x0f,0x7f,0xbf,0xdf,0x7f,0xbf,0xdf,0xff,0xff,0xff,0xdf,0xef
	//0xff,0x03,0xff,0xff,0x07,0x77,0xb7,0xd7,0x77,0xb7,0xd7,0xf7,0xf7,0xf7,0xf7,0xef
	//0xff,0x00,0x7f,0x7f,0x03,0x73,0x6b,0x5b,0x73,0x6b,0x5b,0x7b,0x7b,0x7b,0x7b,0xfb,
	//0xff,0x87,0xff,0xff,0xe7,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xef,0xf7
	};
	int i,j;
	int scan;
	FILE *fp;
    int count;
    char file[]="HZK16";
    long hzpos;
    int area,posi;
    //puts("Enter the name of the file to be processed: ");
    //gets(file);
    if( (fp = fopen(file, "rb")) == NULL )    //只读和二进制模式
    {
        printf("can't open %s\n", file);
        exit(1);
    }

    //fseek(fp, 0L, SEEK_SET);                //定位在文件开头处
    //last = ftell(fp);
    //printf("fseek(fp, 0L, SEEK_SET) , fteel(p): %d/n", last);

    //fseek(fp, 0L, SEEK_END);                //定位在文件结尾处
    //last = ftell(fp);
    //printf("fseek(fp, 0L, SEEK_END) , fteel(p): %d/n", last);
    for(;;){
        area=0;    //汉字所在区
        posi=0;    //汉字所在位
        while(area<1 || area>94){
            printf("input area(1-94,0=exit)=");
            scanf("%d",&area);
            if(area==0)
                exit(1);
        }
        while(posi<1 || posi>94){
            printf("input posi(1-94,0=exit)=");
            scanf("%d",&posi);
            if(posi==0)
                exit(1);
        }
        hzpos=((area-1)*94 + posi-1)*32;
        fseek(fp,hzpos,SEEK_SET);
        //fseek(fp,32,SEEK_SET);
        for(count = 0; count < fontHeigth*2; count++)
        {
            dot[count] = getc(fp);
            printf("%02x ",dot[count]);
        }

        printf("\n\n");

        for(i=0;i<fontHeigth;i++){
            scan=0x80;
            for(j=0;j<8;j++){
                if( (dot[i*2] & scan ) )
                    printf("* ");
                else
                    printf("  ");

                scan >>=  1 ;
            }
            scan=0x80;
            for(j=0;j<8;j++){
                if((dot[i*2+1] & scan))
                    printf("* ");
                else
                    printf("  ");

                scan>>=1;
            }
            printf("\n");
        }
        printf("\n\n");
    }
    fclose(fp);
    return 0;
}
