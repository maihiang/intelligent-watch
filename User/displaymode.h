#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H

void display_main(void);           // mode=0
void display_menu(void);           // mode=1
void display_DHT11(void);          // mode=11
void display_WIFI(void);           // mode=12
void display_CORE(void);           // mode=13
void display_setting(void);        // mode=2
void display_setting_year(void);   // mode=21
void display_setting_month(void);  // mode=22
void display_setting_day(void);    // mode=23
void display_setting_hour(void);   // mode=24
void display_setting_minute(void); // mode=25
void display_setting_second(void); // mode=26

#endif // DISPLAYMODE_H
