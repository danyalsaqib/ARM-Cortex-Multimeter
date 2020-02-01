#ifndef LCDINTERFACING_H_
#define LCDINTERFACING_H_




void delay_us(unsigned int d);
void lcdCommand(char cmnd);
void lcdData (unsigned char data);
void InitialisePortC();
void InitialisePortD();
void lcd_gotoxy(unsigned char x , unsigned char y);
void lcd_print(char*str);
void lcd_print_bottomLine(int initNum, int c);
void shifter(int c);
void lcd_init();
void lcd_send_integer(int a, int maxLength);
void lcd_send_float(float a, int maxLength);



#endif /* LCDINTERFACING_H_ */
