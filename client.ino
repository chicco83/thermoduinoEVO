/************************* Store timers in EEPROM *************************************/
void save_timer(int timer, float Action, int Hour, int Minute, int Day, int Enable)
{
  time_date();
  if (timer == 1) {
    if (DEBUG5) Serial.println(Action);

    EEPROM.updateFloat (timer1_action_address, (timer1_action = Action));
    EEPROM.updateByte (timer_1_hour_address, (timer_1_hour = Hour));
    EEPROM.updateByte (timer_1_minute_address, (timer_1_minute = Minute));
    EEPROM.updateByte (timer_1_day_address, (timer_1_day = Day));
    EEPROM.updateByte (timer_1_enable_address, (timer_1_enable = Enable));
  }
  if (timer == 2) {
    EEPROM.updateFloat (timer2_action_address, (timer2_action = Action));
    EEPROM.updateByte (timer_2_hour_address, (timer_2_hour = Hour));
    EEPROM.updateByte (timer_2_minute_address, (timer_2_minute = Minute));
    EEPROM.updateByte (timer_2_day_address, (timer_2_day = Day));
    EEPROM.updateByte (timer_2_enable_address, (timer_2_enable = Enable));
  }
  if (timer == 3) {
    EEPROM.updateFloat (timer3_action_address, (timer3_action = Action));
    EEPROM.updateByte (timer_3_hour_address, (timer_3_hour = Hour));
    EEPROM.updateByte (timer_3_minute_address, (timer_3_minute = Minute));
    EEPROM.updateByte (timer_3_day_address, (timer_3_day = Day));
    EEPROM.updateByte (timer_3_enable_address, (timer_3_enable = Enable));
  }
  if (timer == 4) {
    EEPROM.updateFloat (timer4_action_address, (timer4_action = Action));
    EEPROM.updateByte (timer_4_hour_address, (timer_4_hour = Hour));
    EEPROM.updateByte (timer_4_minute_address, (timer_4_minute = Minute));
    EEPROM.updateByte (timer_4_day_address, (timer_4_day = Day));
    EEPROM.updateByte (timer_4_enable_address, (timer_4_enable = Enable));
  }
  if (timer == 5) {
    EEPROM.updateFloat (timer5_action_address, (timer5_action = Action));
    EEPROM.updateByte (timer_5_hour_address, (timer_5_hour = Hour));
    EEPROM.updateByte (timer_5_minute_address, (timer_5_minute = Minute));
    EEPROM.updateByte (timer_5_day_address, (timer_5_day = Day));
    EEPROM.updateByte (timer_5_enable_address, (timer_5_enable = Enable));
  }
  if (timer == 6) {
    EEPROM.updateFloat (timer6_action_address, (timer6_action = Action));
    EEPROM.updateByte (timer_6_hour_address, (timer_6_hour = Hour));
    EEPROM.updateByte (timer_6_minute_address, (timer_6_minute = Minute));
    EEPROM.updateByte (timer_6_day_address, (timer_6_day = Day));
    EEPROM.updateByte (timer_6_enable_address, (timer_6_enable = Enable));
  }
  if (timer == 7) {
    EEPROM.updateFloat (timer7_action_address, (timer7_action = Action));
    EEPROM.updateByte (timer_7_hour_address, (timer_7_hour = Hour));
    EEPROM.updateByte (timer_7_minute_address, (timer_7_minute = Minute));
    EEPROM.updateByte (timer_7_day_address, (timer_7_day = Day));
    EEPROM.updateByte (timer_7_enable_address, (timer_7_enable = Enable));
  }
  if (timer == 8) {
    EEPROM.updateFloat (timer8_action_address, (timer8_action = Action));
    EEPROM.updateByte (timer_8_hour_address, (timer_8_hour = Hour));
    EEPROM.updateByte (timer_8_minute_address, (timer_8_minute = Minute));
    EEPROM.updateByte (timer_8_day_address, (timer_8_day = Day));
    EEPROM.updateByte (timer_8_enable_address, (timer_8_enable = Enable));
  }
  if (timer == 9) {
    EEPROM.updateFloat (timer9_action_address, (timer9_action = Action));
    EEPROM.updateByte (timer_9_hour_address, (timer_9_hour = Hour));
    EEPROM.updateByte (timer_9_minute_address, (timer_9_minute = Minute));
    EEPROM.updateByte (timer_9_day_address, (timer_9_day = Day));
    EEPROM.updateByte (timer_9_enable_address, (timer_9_enable = Enable));
  }
  read_EEPROM();
}

/*************************** Check fot Ethernet Client *******************************************************************/
void run_eth()
{
  EthernetClient client = server.available();
  if (client)
  {
    boolean sentHeader = false;
    boolean currentLineIsBlank = true;
    while (client.connected())
    {
      if (client.available())
      {
        if (!sentHeader) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          sentHeader = true;
          readString = "";
          if (DEBUG5) Serial.println();
        }
        char c = client.read();
        // if (DEBUG5) Serial.write(c);
        if (readString.indexOf('\n') < 1)
        {
          readString.concat(c);
        }
        if (c == '\n' && currentLineIsBlank) {
          if (DEBUG5) Serial.print(readString);
          if (readString.indexOf("GET") >= 0) {

            if (readString.indexOf("/connect") >= 0) {
              client.println(handshake);
              if (DEBUG5) Serial.println("connect ");
            }

            if (readString.indexOf("/set_timer$") >= 0) {
              //timer, Temp, hour,  minute,  day, enable
              // 1/9,    0/30,  00/24,  00/59,  0/9,  0/1
              //  0=sunday, 6=saturday, 7=mon-fry, 8= weekend, 9= everyday
              client.println(handshake);
              String string(4);
              int dot;
              string = readString.substring((readString.indexOf("$") + 1), (dot = (readString.indexOf(","))));
              int TIMER = string.toInt();
              string = readString.substring((dot + 1), (dot = (readString.indexOf(",", (dot + 1)))));
              char carray[string.length() + 1];
              string.toCharArray(carray, sizeof(carray));
              float TEMP = atof(carray);
              string = readString.substring((dot + 1), (dot = (readString.indexOf(",", (dot + 1)))));
              int HOUR = string.toInt();
              string = readString.substring((dot + 1), (dot = (readString.indexOf(",", (dot + 1)))));
              int MINUTE = string.toInt();
              string = readString.substring((dot + 1), (dot = (readString.indexOf(",", (dot + 1)))));
              int DAY = string.toInt();
              string = readString.substring((dot + 1), (dot = (readString.indexOf(",", (dot + 1)))));
              int ENABLE = string.toInt();
              save_timer(TIMER, TEMP, HOUR, MINUTE, DAY, ENABLE);
            }

            if (readString.indexOf("/get_name") >= 0) {
              if (DEBUG5) Serial.print("get_name: ");
              if (DEBUG5) Serial.println(Controller_Name);
              client.print(Controller_Name);

            }

            if (readString.indexOf("/get_status") >= 0) {
              DateTime now = RTC.now();
              client.print(Controller_Name);
              client.print(",");
              client.print(S);                // system status
              client.print(",");
              client.print(H);                // heater status
              client.print(",");
              client.print(Td, 1);            // disired temperature
              client.print(",");
              client.print(Tc, 1);            // measured temperature
              client.print(",");
              client.print(now.hour());
              client.print(",");
              client.print(now.minute());
              client.print(",");
              client.print(now.dayOfWeek());
              client.print(",");
              client.print(wan_port);
              client.print(",");
              if (H == true) {
                client.print("CALDAIA ACCESA - sonda in uso ");
              }
              else {
                client.print("CALDAIA SPENTA - sonda in uso ");
              }
              if (sondaref == 1) {
                client.println("CAMERA");
              }
              if (sondaref == 2) {
                client.println("CUCINA");
              }

              if ((Td > Tc) && S && !ER && !H)
              {
                client.print("Attivazione in: ");
                client.print(HeaterStatusDelay - Von * 1 );
                client.print(" secondi");
              }
              if (((Td < Tc) || !S || ER) && H)
              {
                client.print("Disattivazione in: ");
                client.print(HeaterStatusDelay - Voff * 1);
                client.print(" secondi");
              }

              client.println(",");
              client.print("Temperatura cucina: ");
              client.print(cucina, 1);
              client.print("\u2103 - ");
              // client.print(",");
              client.print("Umidità cucina: ");
              client.print(h0, 1);
              client.print("%");
              client.print(",");
              client.print("Temperatura camera: ");
              client.print(camera, 1);
              client.print("\u2103");
              client.print(",");
              client.print("Temperatura esterna: ");
              client.print(ext, 1);
              client.print("\u2103");
              client.print(",");
              client.print("Pressione atmosferica: ");
              client.print(BP);
              client.println(" hPa");
              client.print(",");

              if (timer_1_enable == 1) {
                client.print("Timer 1: h ");
                client.print(timer_1_hour);
                client.print(":");
                client.print(timer_1_minute);
                if (timer_1_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_1_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_1_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_1_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_1_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_1_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_1_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_1_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_1_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_1_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer1_action == Td){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer1_action);
                client.print("\u2103");
                client.print(",");
              }

              if (timer_2_enable == 1) {
                client.print("Timer 2: h ");
                client.print(timer_2_hour);
                client.print(":");
                client.print(timer_2_minute);
                if (timer_2_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_2_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_2_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_2_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_2_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_2_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_2_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_2_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_2_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_2_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer2 == true){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer2_action);
                client.print("\u2103");
                client.print(",");
              }

              if (timer_3_enable == 1) {
                client.print("Timer 3: h ");
                client.print(timer_3_hour);
                client.print(":");
                client.print(timer_3_minute);
                if (timer_3_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_3_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_3_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_3_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_3_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_3_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_3_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_3_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_3_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_3_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer3 == true){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer3_action);
                client.print("\u2103");
                client.print(",");
              }

              if (timer_4_enable == 1) {
                client.print("Timer 4: h ");
                client.print(timer_4_hour);
                client.print(":");
                client.print(timer_4_minute);
                if (timer_4_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_4_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_4_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_4_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_4_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_4_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_4_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_4_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_4_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_4_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer4 == true){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer4_action);
                client.print("\u2103");
                client.print(",");
              }

              if (timer_5_enable == 1) {
                client.print("Timer 5: h ");
                client.print(timer_5_hour);
                client.print(":");
                client.print(timer_5_minute);
                if (timer_5_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_5_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_5_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_5_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_5_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_5_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_5_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_5_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_5_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_5_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer5 == true){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer5_action);
                client.print("\u2103");
                client.print(",");
              }

              if (timer_6_enable == 1) {
                client.print("Timer 6: h ");
                client.print(timer_6_hour);
                client.print(":");
                client.print(timer_6_minute);
                if (timer_6_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_6_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_6_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_6_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_6_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_6_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_6_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_6_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_6_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_6_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer6 == true){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer6_action);
                client.print("\u2103");
                client.print(",");
              }

              if (timer_7_enable == 1) {
                client.print("Timer 7: h ");
                client.print(timer_7_hour);
                client.print(":");
                client.print(timer_7_minute);
                if (timer_7_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_7_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_7_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_7_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_7_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_7_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_7_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_7_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_7_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_7_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer7 == true){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer7_action);
                client.print("\u2103");
                client.print(",");
              }

              if (timer_8_enable == 1) {
                client.print("Timer 8: h ");
                client.print(timer_8_hour);
                client.print(":");
                client.print(timer_8_minute);
                if (timer_8_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_8_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_8_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_8_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_8_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_8_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_8_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_8_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_8_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_8_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer8 == true){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer8_action);
                client.print("\u2103");
                client.print(",");
              }

              if (timer_9_enable == 1) {
                client.print("Timer 9: h ");
                client.print(timer_9_hour);
                client.print(":");
                client.print(timer_9_minute);
                if (timer_9_day == 0) {
                  client.print(" - Domenica - ");
                }
                else {
                  if (timer_9_day == 1) {
                    client.print(" - Lunedì - ");
                  }
                  else {
                    if (timer_9_day == 2) {
                      client.print(" - Martedì - ");
                    }
                    else {
                      if (timer_9_day == 3) {
                        client.print(" - Mercoledi - ");
                      }
                      else {
                        if (timer_9_day == 4) {
                          client.print(" - Giovedì - ");
                        }
                        else {
                          if (timer_9_day == 5) {
                            client.print(" - Venerdì - ");
                          }
                          else {
                            if (timer_9_day == 6) {
                              client.print(" - Sabato - ");
                            }
                            else {
                              if (timer_9_day == 7) {
                                client.print(" - Lunedì-Venerdì - ");
                              }
                              else {
                                if (timer_9_day == 8) {
                                  client.print(" - Sabato-Domenica - ");
                                }
                                else {
                                  if (timer_9_day == 9) {
                                    client.print(" - Ogni giorno - ");
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
                //                if (timer9 == true){
                //                  client.print(" - ATTIVO -");
                //                }
                //                else {
                //                  client.print(" - NON ATTIVO -");
                //                }
                client.print(" T: ");
                client.print(timer9_action);
                client.print("\u2103");
                client.print(",");
              }

              client.println(handshake);
            }
            if (readString.indexOf("/get_ip") >= 0) {
              client.println(dhcp_ip);
              if (DEBUG5) Serial.print("dhcp_ip: ");
              if (DEBUG5) Serial.println(dhcp_ip);
            }

            if (readString.indexOf("/get_port") >= 0) {
              client.println(wan_port);
              if (DEBUG5) Serial.print("wan_port: ");
              if (DEBUG5) Serial.println(wan_port);
            }

            if (readString.indexOf("/port$") >= 0) {
              char carray[6];
              readString.toCharArray( carray, 6, (readString.indexOf("$") + 1));
              int temp_wan_port = atoi(carray);
              if (temp_wan_port != wan_port) {
                if (DEBUG5) Serial.print("New Wan port: ");
                if (DEBUG5) Serial.println(temp_wan_port);
                EEPROM.updateInt (   port_address,  temp_wan_port );
                if ((wan_port = EEPROM.readInt(port_address)) == 255)wan_port = 0;
                if (DEBUG6) Serial.print("Wan port read: ");
                if (DEBUG6) Serial.println(wan_port);
              }
            }


            if (readString.indexOf("/set_ip$") >= 0) {
              client.println(handshake);
              String string(4);
              int dot;
              string = readString.substring((readString.indexOf("$") + 1), (dot = (readString.indexOf("."))));
              int ip1 = string.toInt();
              string = readString.substring((dot + 1), (dot = (readString.indexOf(".", (dot + 1)))));
              int ip2 = string.toInt();
              string = readString.substring((dot + 1), (dot = (readString.indexOf(".", (dot + 1)))));
              int ip3 = string.toInt();
              string = readString.substring((dot + 1), (dot = (readString.indexOf(".", (dot + 1)))));
              int ip4 = string.toInt();
              EEPROM.write(11, '@');
              EEPROM.write(13, ip1);
              EEPROM.write(14, ip2);
              EEPROM.write(15, ip3);
              EEPROM.write(16, ip4);
              EEPROM.write(17, dns_ip[0]);
              EEPROM.write(18, dns_ip[1]);
              EEPROM.write(19, dns_ip[2]);
              EEPROM.write(20, dns_ip[3]);
              EEPROM.write(21, gw_ip[0]);
              EEPROM.write(22, gw_ip[1]);
              EEPROM.write(23, gw_ip[2]);
              EEPROM.write(24, gw_ip[3]);
              if (DEBUG5) Serial.println("found");
              restart_ethernet();
            }
            if (readString.indexOf("/set_ip%") >= 0) {
              EEPROM.write(11, '%');
              start_ethernet();
            }

            if (readString.indexOf("/set_name$") >= 0) {
              int index_of_S;
              String substring = readString.substring((index_of_S = (readString.indexOf("$")) + 1), (readString.indexOf("$", index_of_S)));

              EEPROM.update(225, '@');
              Controller_Name = substring;
              substring.toCharArray(myStringChar, BUFSIZE); //convert string to char array
              strcpy(buf, myStringChar);
              eeprom_write_string(227, buf);

              if  (EEPROM.read(225) == '@') {
                Serial.print("set_name: ");
                eeprom_read_string(227, buf, BUFSIZE);
                Controller_Name = buf;
                Serial.println(Controller_Name);
                client.println(Controller_Name);
              }
            }


            if (readString.indexOf("/set_ip%") >= 0) {
              EEPROM.write(11, '%');
              start_ethernet();
            }


            if (readString.indexOf("/set_temp") >= 0)
            {
              if (S == false)
              {
                if (DEBUG5) Serial.println("off");
                client.print ("system_off");
              }
              if (S == true)
              {
                if (readString.indexOf("=") >= 0) {
                  //if (DEBUG5) Serial.print("new set temp");
                  char carray[5];
                  readString.toCharArray( carray, 5, (readString.indexOf("=") + 1));
                  Tt = atof(carray);
                  // if (DEBUG5) Serial.println(Tt);
                  if (Tt > Tmin && Tt < Tmax + 0.1) {
                    Td = Tt;
                    //if (DEBUG5) Serial.print("Temperature is nu ingesteld op: ");
                    //if (DEBUG5) Serial.print(Td,1);
                    // if (DEBUG5) Serial.println("C");
                    //client.print(Td,1);
                    EEPROM.updateFloat(address_temp, Td);
                    //if ((Tc > 7) && (Tc < 40)){sendData();}
                  }
                  else
                  {
                    client.print("wrong_setting");
                  }
                }
                /************************* Warmer *************************************/
                if (readString.indexOf("+") >= 0)
                {
                  Td = (Td + 0.1);
                  if (Td > Tmax) Td = Tmax;
                  if (DEBUG3) Serial.print(" up ");
                  if (DEBUG3) Serial.println(Td, 1);
                  EEPROM.updateFloat(address_temp, Td);
                  //  if ((Tc > 7) && (Tc < 40)){sendData();}
                }
                /************************* Kouder *************************************/
                if (readString.indexOf("-") >= 0)
                {
                  Td = (Td - 0.1);
                  if (Td < Tmin) Td = Tmin;              //
                  if (DEBUG3) Serial.print(" down ");
                  if (DEBUG3) Serial.println(Td, 1);
                  EEPROM.updateFloat(address_temp, Td);
                  //  if ((Tc > 7) && (Tc < 40)){sendData();}
                }
              }
            }
            /************************* acceso *************************************/
            if (readString.indexOf("system_state$1") > 1)
            {
              if (DEBUG3) Serial.println(" ACCESO");
              S = true;
              if ((Tc > 7) && (Tc < 40)) {
                //     sendData();
              }
            }
            /************************* spento *************************************/
            if (readString.indexOf("system_state$0") > 1)
            {
              if (DEBUG3) Serial.println(" SPENTO");
              S = false;
              if ((Tc > 7) && (Tc < 40)) {
                //     sendData();
              }
            }

            if (readString.indexOf("get_timers") > 1)
            {
              if (DEBUG3) Serial.println("get_timers");

              timer_1_hour = EEPROM.readByte(timer_1_hour_address);
              timer_1_minute = EEPROM.readByte(timer_1_minute_address);
              timer_1_day = EEPROM.readByte(timer_1_day_address);
              timer1_action = EEPROM.readFloat(timer1_action_address);
              timer_1_enable = EEPROM.readByte(timer_1_enable_address);
              client.print("1");
              client.print(",");
              client.print(timer_1_hour);
              client.print(",");
              client.print(timer_1_minute);
              client.print(",");
              client.print(timer_1_day);
              client.print(",");
              client.print(timer_1_enable);
              client.print(",");
              client.print(timer1_action);
              timer_2_hour = EEPROM.readByte(timer_2_hour_address);
              timer_2_minute = EEPROM.readByte(timer_2_minute_address);
              timer_2_day = EEPROM.readByte(timer_2_day_address);
              timer2_action = EEPROM.readFloat(timer2_action_address);
              timer_2_enable = EEPROM.readByte(timer_2_enable_address);
              client.print(",");
              client.print("2");
              client.print(",");
              client.print(timer_2_hour);
              client.print(",");
              client.print(timer_2_minute);
              client.print(",");
              client.print(timer_2_day);
              client.print(",");
              client.print(timer_2_enable);
              client.print(",");
              client.print(timer2_action);
              timer_3_hour = EEPROM.readByte(timer_3_hour_address);
              timer_3_minute = EEPROM.readByte(timer_3_minute_address);
              timer_3_day = EEPROM.readByte(timer_3_day_address);
              timer3_action = EEPROM.readFloat(timer3_action_address);
              timer_3_enable = EEPROM.readByte(timer_3_enable_address);
              client.print(",");
              client.print("3");
              client.print(",");
              client.print(timer_3_hour);
              client.print(",");
              client.print(timer_3_minute);
              client.print(",");
              client.print(timer_3_day);
              client.print(",");
              client.print(timer_3_enable);
              client.print(",");
              client.print(timer3_action);
              timer_4_hour = EEPROM.readByte(timer_4_hour_address);
              timer_4_minute = EEPROM.readByte(timer_4_minute_address);
              timer_4_day = EEPROM.readByte(timer_4_day_address);
              timer4_action = EEPROM.readFloat(timer4_action_address);
              timer_4_enable = EEPROM.readByte(timer_4_enable_address);
              client.print(",");
              client.print("4");
              client.print(",");
              client.print(timer_4_hour);
              client.print(",");
              client.print(timer_4_minute);
              client.print(",");
              client.print(timer_4_day);
              client.print(",");
              client.print(timer_4_enable);
              client.print(",");
              client.print(timer4_action);
              timer_5_hour = EEPROM.readByte(timer_5_hour_address);
              timer_5_minute = EEPROM.readByte(timer_5_minute_address);
              timer_5_day = EEPROM.readByte(timer_5_day_address);
              timer5_action = EEPROM.readFloat(timer5_action_address);
              timer_5_enable = EEPROM.readByte(timer_5_enable_address);
              client.print(",");
              client.print("5");
              client.print(",");
              client.print(timer_5_hour);
              client.print(",");
              client.print(timer_5_minute);
              client.print(",");
              client.print(timer_5_day);
              client.print(",");
              client.print(timer_5_enable);
              client.print(",");
              client.print(timer5_action);
              timer_6_hour = EEPROM.readByte(timer_6_hour_address);
              timer_6_minute = EEPROM.readByte(timer_6_minute_address);
              timer_6_day = EEPROM.readByte(timer_6_day_address);
              timer6_action = EEPROM.readFloat(timer6_action_address);
              timer_6_enable = EEPROM.readByte(timer_6_enable_address);
              client.print(",");
              client.print("6");
              client.print(",");
              client.print(timer_6_hour);
              client.print(",");
              client.print(timer_6_minute);
              client.print(",");
              client.print(timer_6_day);
              client.print(",");
              client.print(timer_6_enable);
              client.print(",");
              client.print(timer6_action);
              timer_7_hour = EEPROM.readByte(timer_7_hour_address);
              timer_7_minute = EEPROM.readByte(timer_7_minute_address);
              timer_7_day = EEPROM.readByte(timer_7_day_address);
              timer7_action = EEPROM.readFloat(timer7_action_address);
              timer_7_enable = EEPROM.readByte(timer_7_enable_address);
              client.print(",");
              client.print("7");
              client.print(",");
              client.print(timer_7_hour);
              client.print(",");
              client.print(timer_7_minute);
              client.print(",");
              client.print(timer_7_day);
              client.print(",");
              client.print(timer_7_enable);
              client.print(",");
              client.print(timer7_action);
              timer_8_hour = EEPROM.readByte(timer_8_hour_address);
              timer_8_minute = EEPROM.readByte(timer_8_minute_address);
              timer_8_day = EEPROM.readByte(timer_8_day_address);
              timer8_action = EEPROM.readFloat(timer8_action_address);
              timer_8_enable = EEPROM.readByte(timer_8_enable_address);
              client.print(",");
              client.print("8");
              client.print(",");
              client.print(timer_8_hour);
              client.print(",");
              client.print(timer_8_minute);
              client.print(",");
              client.print(timer_8_day);
              client.print(",");
              client.print(timer_8_enable);
              client.print(",");
              client.print(timer8_action);
              timer_9_hour = EEPROM.readByte(timer_9_hour_address);
              timer_9_minute = EEPROM.readByte(timer_9_minute_address);
              timer_9_day = EEPROM.readByte(timer_9_day_address);
              timer9_action = EEPROM.readFloat(timer9_action_address);
              timer_9_enable = EEPROM.readByte(timer_9_enable_address);
              client.print(",");
              client.print("9");
              client.print(",");
              client.print(timer_9_hour);
              client.print(",");
              client.print(timer_9_minute);
              client.print(",");
              client.print(timer_9_day);
              client.print(",");
              client.print(timer_9_enable);
              client.print(",");
              client.print(timer9_action);
              client.print(",");
              client.print(handshake);


              Serial.print("1");
              Serial.print(",");
              Serial.print(timer_1_hour);
              Serial.print(",");
              Serial.print(timer_1_minute);
              Serial.print(",");
              Serial.print(timer_1_day);
              Serial.print(",");
              Serial.print(timer_1_enable);
              Serial.print(",");
              Serial.print(timer1_action);
              Serial.println(",");
              Serial.print("2");
              Serial.print(",");
              Serial.print(timer_2_hour);
              Serial.print(",");
              Serial.print(timer_2_minute);
              Serial.print(",");
              Serial.print(timer_2_day);
              Serial.print(",");
              Serial.print(timer_2_enable);
              Serial.print(",");
              Serial.print(timer2_action);
              Serial.println(",");
              Serial.print("3");
              Serial.print(",");
              Serial.print(timer_3_hour);
              Serial.print(",");
              Serial.print(timer_3_minute);
              Serial.print(",");
              Serial.print(timer_3_day);
              Serial.print(",");
              Serial.print(timer_3_enable);
              Serial.print(",");
              Serial.print(timer3_action);
              Serial.println(",");
              Serial.print("4");
              Serial.print(",");
              Serial.print(timer_4_hour);
              Serial.print(",");
              Serial.print(timer_4_minute);
              Serial.print(",");
              Serial.print(timer_4_day);
              Serial.print(",");
              Serial.print(timer_4_enable);
              Serial.print(",");
              Serial.print(timer4_action);
              Serial.println(",");
              Serial.print("5");
              Serial.print(",");
              Serial.print(timer_5_hour);
              Serial.print(",");
              Serial.print(timer_5_minute);
              Serial.print(",");
              Serial.print(timer_5_day);
              Serial.print(",");
              Serial.print(timer_5_enable);
              Serial.print(",");
              Serial.print(timer5_action);
              Serial.println(",");
              Serial.print("6");
              Serial.print(",");
              Serial.print(timer_6_hour);
              Serial.print(",");
              Serial.print(timer_6_minute);
              Serial.print(",");
              Serial.print(timer_6_day);
              Serial.print(",");
              Serial.print(timer_6_enable);
              Serial.print(",");
              Serial.print(timer6_action);
              Serial.println(",");
              Serial.print("7");
              Serial.print(",");
              Serial.print(timer_7_hour);
              Serial.print(",");
              Serial.print(timer_7_minute);
              Serial.print(",");
              Serial.print(timer_7_day);
              Serial.print(",");
              Serial.print(timer_7_enable);
              Serial.print(",");
              Serial.print(timer7_action);
              Serial.println(",");
              Serial.print("8");
              Serial.print(",");
              Serial.print(timer_8_hour);
              Serial.print(",");
              Serial.print(timer_8_minute);
              Serial.print(",");
              Serial.print(timer_8_day);
              Serial.print(",");
              Serial.print(timer_8_enable);
              Serial.print(",");
              Serial.print(timer8_action);
              Serial.println(",");
              Serial.print("9");
              Serial.print(",");
              Serial.print(timer_9_hour);
              Serial.print(",");
              Serial.print(timer_9_minute);
              Serial.print(",");
              Serial.print(timer_9_day);
              Serial.print(",");
              Serial.print(timer_9_enable);
              Serial.print(",");
              Serial.print(timer9_action);
              Serial.print(",");
              Serial.println(handshake);
            }
          }


          else if (readString.startsWith("POST")) {
            if (DEBUG5) Serial.println("POST");
          }
          else {
            client.print (content_main_top);
            delay(1);
            client.stop();
            readString = "";
            client.read();
            client.read();
          }
          delay(1);
          client.stop();
          readString = "";
          client.read();
          client.read();
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }//      if (client.available())
      }//    while (client.connected())
    }
    client.stop();
  }
}
