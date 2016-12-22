#include <stdio.h>
#include <stdlib.h>
#include "ascii.h"

int main()
{
    unsigned char ch,dot,temp;
    long n;
    int i,j;
    FILE *in;
    FILE *out;
    int scan,k;

    in = fopen("ascii.h","r");
    out = fopen("ascii.txt","w");
    ch = fgetc(in);
    n=0;
    dot=0;
    while( n<254 ){
       fputc(ch,out);
       ch=fgetc(in);
       printf("%c",ch);
       n++;
    }
    for(i=0;i<95;i++){
            fputc('{',out);
        for(j=0;j<16;j++){
            temp = ascii_Dot[i][j];
            scan =0x01;
            for(k=0;k<8;k++){
                if(temp & scan)
                    dot |= 0x80 >>k ;
                else
                    dot &= ~(0x80 >>k);

                scan <<= 1;
            }
            fputc('0',out);
            fputc('x',out);
            if(j==15)
                fprintf(out,"%02x},",dot);
            else
                fprintf(out,"%02x,",dot);
        }
            while( ch != '/' )  ch=fgetc(in);

            while( ch != '{' ){
                fputc(ch,out);
                ch=fgetc(in);
            }
            ch=' ';


    }




    fclose(in);
    fclose(out);
    return 0;
}


