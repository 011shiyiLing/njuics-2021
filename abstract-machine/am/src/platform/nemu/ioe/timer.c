#include <am.h>
#include <nemu.h>

static uint64_t boot_time;
void __am_timer_init() {
  boot_time = (uint64_t)(inl(RTC_ADDR));
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint64_t now_time = (uint64_t)(inl(RTC_ADDR + 4));
  uptime->us = boot_time - now_time;
  //uptime->us = ((uint64_t)(inl(RTC_ADDR+4))<<32 | inl(RTC_ADDR)) - boot_time;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
