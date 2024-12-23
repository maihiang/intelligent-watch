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

// 获取二十四节气
uint16_t getSolarTerm(uint16_t month, uint16_t day) {
    // 简化算法，根据公历确定二十四节气
    static const uint16_t solarTermDates[24][2] = {
        {1, 5}, {1, 20}, {2, 3}, {2, 18}, {3, 5}, {3, 20},
        {4, 4}, {4, 20}, {5, 5}, {5, 21}, {6, 5}, {6, 21},
        {7, 7}, {7, 22}, {8, 7}, {8, 23}, {9, 7}, {9, 23},
        {10, 8}, {10, 23}, {11, 7}, {11, 22}, {12, 7}, {12, 21}
    };
    
    for (uint16_t i = 0; i < 24; i++) {
        if (solarTermDates[i][0] == month && solarTermDates[i][1] == day) {
            return i + 1;
        }
    }
    return 0;
}

// 更新时间到下一秒
void updateTime(uint16_t *year, uint16_t *month, uint16_t *day, uint16_t *hour, uint16_t *minute, uint16_t *second, uint16_t *solar_term)
{
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
    //更新二十四节气
    *solar_term = getSolarTerm(*month, *day);    
}

