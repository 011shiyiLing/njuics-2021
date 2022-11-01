#include <am.h>
#include <nemu.h>

void __am_timer_init() {
  //boot_time = inl(RTC_ADDR);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  //__am_timer_init();
  int now_time = inl(RTC_ADDR);
  uptime->us = (uint64_t)(now_time);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
