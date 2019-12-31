/*
 * RtcDriver.c
 *
 *  Created on: Dec 30, 2019
 *      Author: janus
 */

#include "RtcDriver.h"
static RTC_HandleTypeDef rtc;
static rtcDriverCallback rtcDriverClb = NULL;

void initRTC(void) {

	__HAL_RCC_RTC_ENABLE();

	rtc.Instance = RTC;
	rtc.Init.HourFormat = RTC_HOURFORMAT_24;
	rtc.Init.AsynchPrediv = 127;
	rtc.Init.SynchPrediv = 255;
	rtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	rtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	rtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&rtc) != HAL_OK) {
		Error_Handler();
	}

	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

	setTime(0, 0, 0);
	setDate(monday, december, 30, 19);
}

void setDate(weekDays day, months month, uint8_t date, uint8_t year) {
	RTC_DateTypeDef sDate = { 0 };
	sDate.WeekDay = day;
	sDate.Month = month;
	sDate.Date = date % 32;
	sDate.Year = year % 100;
	if (HAL_RTC_SetDate(&rtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

void setTime(uint8_t hour, uint8_t minute, uint8_t second) {
	RTC_TimeTypeDef sTime = { 0 };
	sTime.Hours = hour % 24;
	sTime.Minutes = minute % 60;
	sTime.Seconds = second % 60;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&rtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

void set_alarm_every_second() {
	RTC_AlarmTypeDef sAlarm = { 0 };
	sAlarm.AlarmMask = every_second;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&rtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

void set_alarm_every_minute() {
	RTC_AlarmTypeDef sAlarm = { 0 };
	sAlarm.AlarmMask = every_minute;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&rtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

void set_alarm_every_hour() {
	RTC_AlarmTypeDef sAlarm = { 0 };
	sAlarm.AlarmMask = every_hour;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&rtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

void setAlarm(uint8_t hour, uint8_t minute, uint8_t second, AlarmTime t)
{
	RTC_AlarmTypeDef sAlarm = { 0 };
	sAlarm.AlarmTime.Hours = hour % 24;
	sAlarm.AlarmTime.Minutes = minute % 60;
	sAlarm.AlarmTime.Seconds = second % 60;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = t;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&rtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

RTC_TimeTypeDef getCurrentTime() {
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	HAL_RTC_GetTime(&rtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&rtc, &sDate, RTC_FORMAT_BIN);
	return sTime;
}

RTC_DateTypeDef getCurrentDate() {
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	HAL_RTC_GetTime(&rtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&rtc, &sDate, RTC_FORMAT_BIN);
	return sDate;
}

RTC_TimeTypeDef getAlarmValues(){
	RTC_AlarmTypeDef sAlarm = { 0 };
	HAL_RTC_GetAlarm(&rtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
	return sAlarm.AlarmTime;
}

void register_rtc_callback(rtcDriverCallback callback) {
	rtcDriverClb = callback;
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	if (rtcDriverClb != NULL)
		rtcDriverClb();
}

void RTC_Alarm_IRQHandler(void) {
	HAL_RTC_AlarmIRQHandler(&rtc);
}
