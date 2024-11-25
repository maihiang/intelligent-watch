#ifndef DATE_H
#define DATE_H

// ...existing code...

uint16_t isLeapYear(uint16_t year);
uint16_t daysInMonth(uint16_t year, uint16_t month);
void updateTime(uint16_t *year, uint16_t *month, uint16_t *day, uint16_t *hour, uint16_t *minute, uint16_t *second); 

// ...existing code...

#endif // DATE_H
