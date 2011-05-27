
#ifndef ST7565R_H
	#define ST7565R_H

#define LCD1	0x20	// use lcd1  
#define LCD2 0x10       // use lcd2

void lcd_init(void);

void lcd_clrscr(int8_t Scr);
void lcd_write_str(char* st1, int8_t Scr);
void lcd_Line2(int8_t Scr);
void lcd_goto(int8_t line, int8_t pos,int8_t Scr);
void lcd_write_char(int8_t ch1, int8_t Scr);
void lcd_set_cursor(int8_t line, int8_t pos,int8_t Scr);
void lcd_hide_cursor();

#endif
