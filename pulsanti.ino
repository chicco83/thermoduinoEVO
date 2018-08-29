void read_button()
{
  
   value += encoder->getValue();

  if (value != last) {
    Td += (value-last)/20;
    last = value;
    
    Serial.print("Encoder Value: ");
    Serial.println(value, 1);
    }
    
    ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
   
    switch (b) {

      case ClickEncoder::DoubleClicked:
if (S == true) {                          // se il termostato è acceso
S = false;                                // lo spengo
H = false;                                // metto lo status della caldaia su spento
digitalWrite(Heater, LOW);               // e spengo il rele


}
 else{
 
 S = true;
 
}
        break;
  }  
  }  
}    



//int buttonStateUP;             // the current reading from the input pin
//int lastButtonStateUP = HIGH;   // the previous reading from the input pin
//
//int buttonStateDOWN;             // the current reading from the input pin
//int lastButtonStateDOWN = HIGH;   // the previous reading from the input pin
//
//
//// the following variables are long's because the time, measured in miliseconds,
//// will quickly become a bigger number than can be stored in an int.
//long lastDebounceTime = 0;  // the last time the output pin was toggled
//long debounceDelay = 50;    // the debounce time; increase if the output flickers
    
    
//  int readingUP = digitalRead(BUTTON_UP);
//  if (readingUP != lastButtonStateUP) {
//    // reset the debouncing timer
//    lastDebounceTime = millis();
//  }
//
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    // whatever the reading is at, it's been there for longer
//    // than the debounce delay, so take it as the actual current state:
//
//    // if the button state has changed:
//    if (readingUP != buttonStateUP) {
//      buttonStateUP = readingUP;
//
//      // only toggle the LED if the new button state is HIGH
//      if (buttonStateUP == LOW) {
//        Td = Td + 0.1;
//        EEPROM.updateFloat(address_temp, Td); //  enable to store manual value set by buttons
//      }
//    }
//  }              
//  lastButtonStateUP = readingUP;                                    // save the reading.  Next time through the loop
//                                                                   // it'll be the lastButtonState:
//  int readingDOWN = digitalRead(BUTTON_DOWN);
//  if (readingDOWN != lastButtonStateDOWN) {
//    lastDebounceTime = millis();                                  // reset the debouncing timer
//  }
//
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    // whatever the reading is at, it's been there for longer
//    // than the debounce delay, so take it as the actual current state:
//
//    // if the button state has changed:
//    if (readingDOWN != buttonStateDOWN) {
//      buttonStateDOWN = readingDOWN;
//
//      // only toggle the LED if the new button state is HIGH
//      if (buttonStateDOWN == LOW) {
//        Td = Td - 0.1;
//        EEPROM.updateFloat(address_temp, Td);                    //enable to store manual value set by buttons
//      }
//    }
//  }
//  lastButtonStateDOWN = readingDOWN;                            // save the reading.  Next time through the loop,
//                                                                // it'll be the lastButtonState:


