#include <am.h>
#include <nemu.h>
//#include <stdio.h>

static uint32_t boot_time;
void __am_timer_init() {
  boot_time = (uint32_t)(inl(RTC_ADDR));
  //printf("%d\n" , boot_time);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  __am_timer_init();
  uint32_t now_time = (uint32_t)(inl(RTC_ADDR+4));
  //printf("%d\n",now_time);
  uptime->us = (uint64_t)(boot_time-now_time);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
