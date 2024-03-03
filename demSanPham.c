#include <REGX52.H>
#include <stdio.h>
#include <string.h>
#define LCD_RS  P0_0
#define LCD_RW  P0_1
#define LCD_EN  P0_2
#define LCD_D4  P0_3
#define LCD_D5  P0_4
#define LCD_D6  P0_5
#define LCD_D7  P0_6
#define R1 P2_0
#define R2 P2_1
#define R3 P2_2
#define R4 P2_3
#define C1 P2_4
#define C2 P2_5
#define C3 P2_6
#define C4 P2_7
#define out P3_2
#define led P1_2
#define lcd_clear 0x01

void delay_us(unsigned int t)
{
        unsigned int i;
        for(i=0; i < t ;i++);
}

void delay_ms(unsigned int t){
        unsigned int i;
	      while( t --){
				   for( i = 0 ; i< 123; i++){
					 }
				}
}

void LCD_Enable(void){
        LCD_EN =1;
        delay_us(3);
        LCD_EN =0;
        delay_us(50); 
}

void LCD_Send4Bit(unsigned char Data){                   //Ham Gui 4 Bit Du Lieu Ra LCD
        LCD_D4=Data & 0x01;
        LCD_D5=(Data>>1)&1;
        LCD_D6=(Data>>2)&1;
        LCD_D7=(Data>>3)&1;
}

void LCD_SendCommand(unsigned char command){            // Ham Gui 1 Lenh Cho LCD
        LCD_Send4Bit(command >>4);                      /* Gui 4 bit cao */
        LCD_Enable();
        LCD_Send4Bit(command);                          /* Gui 4 bit thap*/
        LCD_Enable();
}

void LCD_Init(){              // Ham Khoi Tao LCD
        LCD_Send4Bit(0x00);	  //turn on LCD
        
        LCD_RS=0;
        LCD_RW=0;
        
     LCD_Enable();
        LCD_Send4Bit(0x03);	  //function setting
          
     LCD_Enable();
	
        LCD_Send4Bit(0x02);	  //di chuyen con tro ve dau man hnh
     LCD_Enable();
        LCD_SendCommand( 0x28 ); //lua chon che do 4 bit
        LCD_SendCommand( 0x0c);  // bat hien thi va tat con tro di
        LCD_SendCommand( 0x06 ); // tang ID, khong dich khung
        LCD_SendCommand(0x01);  //xoa toan bo khung hinh
}

void LCD_Gotoxy(unsigned char x, unsigned char y){
        unsigned char address;
        if(!y)address=(0x80+x);
        else address=(0xc0+x);
        delay_us(1000);
        LCD_SendCommand(address);
        delay_us(50);
}

void LCD_Put(unsigned char Data){
        LCD_RS=1;
        LCD_SendCommand(Data);
        LCD_RS=0 ;
}

void LCD_Puts(char *s){
        while (*s){
                LCD_Put(*s);
                s++;
        }
}

idata int soSanPham=0;
idata int soLuong[10]={10,10,10,10,10,10,10,10,10,10};
idata int i=0;
idata int k;
idata int number;
idata int dem=0;
idata char Lcd_Buff[10];
idata int check_quet= 1;

void dem_so() interrupt 0
{
	dem++;
}

void nhap_phim(){
		dem=0;
		soSanPham=0;
		i=0;
    LCD_SendCommand( lcd_clear );
    LCD_Gotoxy(0,0);
    LCD_Puts("NHAP SO: ");
    while(1){
        int MP=100;
        R1 = 0; R2 = 1; R3 = 1; R4 = 1;
				while(C1==0){ MP = 7;}
				while(C2==0){ MP = 8;}
				while(C3==0){ MP = 9;}
				while(C4==0){ MP = 96;}
				R1 = 1; R2 = 0; R3 = 1; R4 = 1;
				while(C1==0){ MP = 4;}
				while(C2==0){ MP = 5;}
				while(C3==0){ MP = 6;}
				while(C4==0){ MP = 96;}
				R1 = 1; R2 = 1; R3 = 0; R4 = 1;
				while(C1==0){ MP = 1;}
				while(C2==0){ MP = 2;}
				while(C3==0){ MP = 3;}
				while(C4==0){ MP = 96;}
				R1 = 1; R2 = 1; R3 = 1; R4 = 0;
				while(C1==0){ MP = 42;}
				while(C2==0){ MP = 0;}
				while(C3==0){ MP = 96;}
				while(C4==0){ MP = 96;}
				if (MP!=100){
					if ((MP/10)==0){
						soLuong[i]=MP;
						i++;
						sprintf(Lcd_Buff,"%d",MP);
						LCD_Puts(Lcd_Buff);	
					}
					else if (MP==42) {
						LCD_SendCommand( lcd_clear );
						LCD_Gotoxy(0,0);
						LCD_Puts("NUMBER: ");
						number=i-1;
						for (i=number;i>=0;i--){
							for (k=0;k<=number-i;k++){
								soLuong[i]*=10;
							}
							soSanPham+=soLuong[i];
						}
						soSanPham=soSanPham/10;
						sprintf(Lcd_Buff,"%d",soSanPham);
						LCD_Puts(Lcd_Buff);			
						LCD_Gotoxy(0,1);
						LCD_Puts("COUNT : ");
						while (dem<=soSanPham){	
								LCD_Gotoxy(8,1);
								sprintf(Lcd_Buff,"%d",dem);
								LCD_Puts(Lcd_Buff);
								if (dem==soSanPham){
									led=1;
									break;
								}
						}
					}
					else {
						check_quet= 0;
						led=0;
            break;
					}
				}
		}
}


void main(){
	EA = 1;
	EX0 = 1;
	IT0=1;
	led=0;
	LCD_Init();
	nhap_phim();
	while(1){
		if (check_quet==0){
			check_quet=1;
			nhap_phim();
		}
	}
}