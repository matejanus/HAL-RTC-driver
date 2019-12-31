#pragma once

#include "stm32f4xx_hal.h"

typedef enum{
	monday = 0x01,
	tuesday,
	wednesday,
	thursday,
	friday,
	saturday,
	sunday
}weekDays;

typedef enum{
	january = 0x01,
	february,
	march,
	april,
	may,
	june,
	july,
	august,
	september,
	october,
	november,
	december
}months;

typedef enum{
	repeat_hours = (RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_MINUTES | RTC_ALARMMASK_SECONDS),
	repeat_minutes = (RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_SECONDS),
	repeat_seconds = (RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES),
	every_hour  = (RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_MINUTES | RTC_ALARMMASK_SECONDS),
	every_minute  =  (RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES),
	every_second = RTC_ALARMMASK_ALL,
	single_time = RTC_ALARMMASK_DATEWEEKDAY
}AlarmTime;

typedef void (*rtcDriverCallback)(void);

void initRTC(void);
void setDate(weekDays day, months month, uint8_t date, uint8_t year);
void setTime(uint8_t hour, uint8_t minute, uint8_t second);
void set_alarm_every_second(void);
void set_alarm_every_minute(void);
void set_alarm_every_hour(void);
void setAlarm(uint8_t hour, uint8_t minute, uint8_t second, AlarmTime t);
RTC_TimeTypeDef getCurrentTime(void);
RTC_DateTypeDef getCurrentDate(void);
RTC_TimeTypeDef getAlarmValues(void);
void register_rtc_callback(rtcDriverCallback callback);

