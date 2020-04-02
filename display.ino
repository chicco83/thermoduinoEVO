void display ()
{

  lcd.setCursor(0, 0);
//  orologio ();
  if (H == true) {
    lcd.setCursor (17, 0);
    lcd.print(" ON");
  }
  else {
    lcd.setCursor (17, 0);
    lcd.print("OFF");
  }
  lcd.setCursor(0, 1);
  lcd.write(pentola);
  lcd.print(" ");
  lcd.write(termometro);
  lcd.print(cucina, 1);         // measured temperature
  lcd.print(char( 223));    // Scrive il pallino dei gradi
  lcd.print("C ");
  lcd.write(goccia);
  lcd.print(" ");
  lcd.print(h0, 1);
  lcd.print("%");
  lcd.setCursor(0, 2);
  lcd.write(notte);
  lcd.print(" ");
  lcd.write(termometro);
  lcd.print(camera, 1);
  lcd.print(char( 223)); // Scrive il pallino dei gradi
  lcd.print("C ");
  lcd.print("EXT ");
if (ext < 10)
  {
    lcd.print(" "); //insert leading zero if ext temp value is less than 10
    lcd.print(ext,1);
  }
  else  lcd.print(ext,1); //display current ext temp
  lcd.print(char( 223)); // Scrive il pallino dei gradi
  lcd.print("C");
  lcd.setCursor(0, 3);   //quarta riga, primo carattere

  if (S == false) {
    lcd.print(" TERMOSTATO SPENTO");
    lcd.setCursor(0, 0);
    orologio();
  }
  else {
        lcd.print("MAN ");
        lcd.write(termometro);
        lcd.print(" ");
        lcd.print(Td, 1);
        lcd.print(char( 223)); // Scrive il pallino dei gradi
        lcd.print("C      ");

    if ((Td > Tc) && S && !ER && !H)                //
    { //
      lcd.setCursor(0, 0);
      lcd.print("ON tra: ");                        //
      lcd.print(HeaterStatusDelay - Von * 1 );      //    IF THE RELE STATUS IS GOING TO CHANGE IT WILL DISPLAY A COUNTDOWD ON THE LAS T LINE OF THE LCD
      lcd.print(" sec ");                            //
    }                                   //
    else {
      if (((Td < Tc) || !S || ER) && H)
      {
        lcd.setCursor(0, 0);
        lcd.print("OFF tra: ");
        lcd.print(HeaterStatusDelay - Voff * 1);
        lcd.print(" sec ");
      }
      else {
          lcd.setCursor(0, 0);
          orologio ();
      }
    }
  }
  DateTime now = RTC.now();
  if (digitalRead(pirPin) == HIGH)
  {
    lcd.on();
  }

  if (digitalRead(pirPin) == LOW)
  {

    lcd.off();

  }
}

void orologio()
{
  DateTime now = RTC.now();

  // start =  RTC.now();
  Hour = (now.hour());
  if (Hour < 10)
  {
    lcd.print("0"); //insert leading zero if hour value is less than 10
    lcd.print(Hour);
  }
  else  lcd.print(Hour); //display current hour
  lcd.print(':');
  Minute = (now.minute()); //display current hour
  if (Minute < 10)
  {
    lcd.print("0"); //insert leading zero if minute value is less than 10
    lcd.print(Minute);
  }
  else  lcd.print(Minute); //display current minute
  lcd.print(":");
  Second = (now.second());
  if (Second < 10)
  {
    lcd.print("0"); //insert leading zero if seconds value is less than 10
    lcd.print(Second); //display current seconds
  }
  else  lcd.print(Second); //display current seconds

  lcd.print(" ");

  giorno = (now.day());
  if (giorno < 10)
  {
    lcd.print("0"); //insert leading zero if day value is less than 10
    lcd.print(giorno);
  }
  else  lcd.print(giorno); //display current day

  // lcd.print(now.day(), DEC);
  lcd.print('-');

  mese = (now.month());
  if (mese < 10)
  {
    lcd.print("0"); //insert leading zero if month value is less than 10
    lcd.print(mese);
  }
  else  lcd.print(mese); //display current month
        lcd.print(" ");
}

