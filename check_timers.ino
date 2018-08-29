/*************************** Check for timers *******************************************************************/
void check_timers()
{
  DateTime now = RTC.now();
  int weekend;
  int weekday;
  if (now.dayOfWeek() == 0 || now.dayOfWeek() == 6) {
    weekend = 8;
    weekday = 99;
  }
  else {
    weekday = 7;
    weekend = 99;
  }


  if (timer_1_enable == 1 && now.hour() == timer_1_hour && now.minute() == timer_1_minute && (now.dayOfWeek() == timer_1_day || timer_1_day == weekend || timer_1_day == weekday || timer_1_day == 9)) {
    if (timer1 == false) {
      timer1 = true;
      set_temp(timer1_action);
    }
  }
  else timer1 = false;

  if (timer_2_enable == 1 && now.hour() == timer_2_hour && now.minute() == timer_2_minute && (now.dayOfWeek() == timer_2_day || timer_2_day == weekend || timer_2_day == weekday || timer_2_day == 9)) {
    if (timer2 == false) {
      timer2 = true;
      set_temp(timer2_action);
    }
  }
  else timer2 = false;

  if (timer_3_enable == 1 && now.hour() == timer_3_hour && now.minute() == timer_3_minute && (now.dayOfWeek() == timer_3_day || timer_3_day == weekend || timer_3_day == weekday || timer_3_day == 9)) {
    if (timer3 == false) {
      timer3 = true;
      set_temp(timer3_action);
    }
  }
  else timer3 = false;

  if (timer_4_enable == 1 && now.hour() == timer_4_hour && now.minute() == timer_4_minute && (now.dayOfWeek() == timer_4_day || timer_4_day == weekend || timer_4_day == weekday || timer_4_day == 9)) {
    if (timer4 == false) {
      timer4 = true;
      set_temp(timer4_action);
    }
  }
  else timer4 = false;

  if (timer_5_enable == 1 && now.hour() == timer_5_hour && now.minute() == timer_5_minute && (now.dayOfWeek() == timer_5_day || timer_5_day == weekend || timer_5_day == weekday || timer_5_day == 9)) {
    if (timer5 == false) {
      timer5 = true;
      set_temp(timer5_action);
    }
  }
  else timer5 = false;

  if (timer_6_enable == 1 && now.hour() == timer_6_hour && now.minute() == timer_6_minute && (now.dayOfWeek() == timer_6_day || timer_6_day == weekend || timer_6_day == weekday || timer_6_day == 9)) {
    if (timer6 == false) {
      timer6 = true;
      set_temp(timer6_action);
    }
  }
  else timer6 = false;

  if (timer_7_enable == 1 && now.hour() == timer_7_hour && now.minute() == timer_7_minute && (now.dayOfWeek() == timer_7_day || timer_7_day == weekend || timer_7_day == weekday || timer_7_day == 9)) {
    if (timer7 == false) {
      timer7 = true;
      set_temp(timer7_action);
    }
  }
  else timer7 = false;

  if (timer_8_enable == 1 && now.hour() == timer_8_hour && now.minute() == timer_8_minute && (now.dayOfWeek() == timer_8_day || timer_8_day == weekend || timer_8_day == weekday || timer_8_day == 9)) {
    if (timer8 == false) {
      timer8 = true;
      set_temp(timer8_action);
    }
  }
  else timer8 = false;

  if (timer_9_enable == 1 && now.hour() == timer_9_hour && now.minute() == timer_9_minute && (now.dayOfWeek() == timer_9_day || timer_9_day == weekend || timer_9_day == weekday || timer_9_day == 9)) {
    if (timer9 == false) {
      timer9 = true;
      set_temp(timer9_action);
    }
  }
  else timer9 = false;
}

boolean set_temp(float Tt)
{
  if (Tt > Tmin && Tt < Tmax + 0.1) {
    Td = Tt;
    EEPROM.updateFloat(address_temp, Td);
    if (DEBUG5) Serial.println(Td);
    return true;
  }
  else
  {
    return false;
  }
}
