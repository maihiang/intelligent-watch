#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/date/date.h"

// 判断是否为闰年
uint16_t isLeapYear(uint16_t year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 获取指定月份的天数
uint16_t daysInMonth(uint16_t year, uint16_t month) {
    switch (month) {
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 31;
    }
}

// 更新时间到下一秒
void updateTime(uint16_t *year, uint16_t *month, uint16_t *day, uint16_t *hour, uint16_t *minute, uint16_t *second) {
    (*second)++;
    if (*second >= 60) {
        *second = 0;
        (*minute)++;
        if (*minute >= 60) {
            *minute = 0;
            (*hour)++;
            if (*hour >= 24) {
                *hour = 0;
                (*day)++;
                if (*day > daysInMonth(*year, *month)) {
                    *day = 1;
                    (*month)++;
                    if (*month > 12) {
                        *month = 1;
                        (*year)++;
                    }
                }
            }
        }
    }
    if(*day > daysInMonth(*year, *month)) 
    {
        *day = daysInMonth(*year, *month);
    }
}

