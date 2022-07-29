
#ifndef ST7565R_H
	#define ST7565R_H


void lcd_init(void);

void lcd_clrscr();
void lcd_write_str(char* st1);
void lcd_Line1();
void lcd_goto(int8_t line, int8_t pos);
void lcd_write_char(int8_t ch1);
void lcd_set_cursor(int8_t line, int8_t pos);
void lcd_move_cursor_right();
void lcd_hide_cursor();

#endif
