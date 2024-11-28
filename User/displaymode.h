#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H

void display_mode_0(void);
void display_mode_1(void);
void display_mode_2(void);
void display_mode_3(void);
void display_mode_4(void);

void display_main(void);//mode=0
void display_menu(void);//mode=1
void display_setting(void);//mode=2
void display_setting_year(void);//mode=21
void display_setting_month(void);//mode=22
void display_setting_day(void);//mode=23

#endif // DISPLAYMODE_H
