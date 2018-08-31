/*********************** Marque's Thermoduino *****************************/
/*DHT.cpp has an default delay of 250. This makes the system realy slow and must be adjusted to 50 to react faster
  Hardware used:
  Arduino uno
  Ethernet Shield W5100
  DFRobot LCD Keypad Shield
  DHT22 Humidity and Temperature sensor
  2x 2 channel 5v relay
  
__________1___     Room Thermostat pin 2
Relay 1___2__/     Heater pin 2
__________3___     Relay 2

                                        

__________1___     Mot Connected
Relay 2___2__/     Relay 1
__________3___     Heater pin 1 and Room Thermostat pin 1
  
  */
#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <HTTPClient.h>
#include <EEPROMex.h>
#include <Avviso.h>
#include <SPI.h>
#include <DHT.h>

#define DHTPIN1 (A1)                    // pin connected to temperature sensor
#define DHTPIN2 (A2)                    // optional second temperature sensor
#define DHTTYPE DHT22   
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

const int System =  2;     // pin connected to relay 1
const int Heater =  3;     // pin connected to relay 2
const int Buzzer =  A5;     // pin connected to buzzer
float Tc;               // measured temperature
float Td;               // disired temperature
float h1;
float h2;
float t2;
float Tt;
int softwareversion = 1191;
int address = 0;            // eeprom address
float Tw;
float Tr;
int S1;                    // system status
int R2;                    // Heater status
int adva = 0;
int x;
int y;
int Tmax = 24;                //set maximum temperature
int Tmin = 9;                 //sets minimum temperature
int button;
int buttonsensitivity = 150;      // sets the double press time (50-300)
long intervalNetworkCheck = 1500;
long intervalTempUpdate = 1500;
long previousMillis = 0;
unsigned long currentMillis; 
boolean networkinput = false;
boolean networkonline = false;
int N;
int T;
boolean tempread = false;

String readString = String(20); 

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);   // pin connected to lcd display
                                       // lcd keypad is connected to A0
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,68);
EthernetServer server(80);
EthernetClient client;

//IPAddress Router  ( 192,168,1,254 );   
//IPAddress Server   ( 192,168,1,12) ;   
//IPAddress Monster  (  192,168,1,14 );   
//IPAddress HTC   (192,168,1,103 );   
//IPAddress Ext   ( 82,157,141,10 );   
IPAddress Google1    (74,125,228,32 );   
//IPAddress Google2    ( 173,194,75,94);   
IPAddress ServerToCheck = Google1;             // set the name of the server to check

char content_main_top[] = "<body bgcolor=black><font color=white><center>";
char Uit[] = "<br>Systeem is uitgeschakeld<br>Stel de temperatuur in op de Kamerthermostaat<br>" ;
char Tk[] = "Kamer temperatuur: " ;
char C[] = "C";
char Aan[] = "Systeem is in werking<br>Temperatuur is ingesteld op: ";
char line[] = "<br>";



int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
/************************** Setup **********************/
void setup() {
     pinMode(System, OUTPUT);
     pinMode(Heater, OUTPUT); 
     pinMode(Buzzer, OUTPUT); 
     dht1.begin();
     dht2.begin();
     Serial.begin(9600); 
     lcd.begin(16, 2);
     lcd.setCursor(0,0);
     lcd.print("Getting IP......");
//     Serial.println("Getting IP......");
     Ethernet.begin(mac);

   
//    Serial.print("My IP address: ");
    Ethernet.localIP().printTo(Serial);
    Serial.println();
     /*  Serial.print("Gateway IP address is ");
    Ethernet.gatewayIP().printTo(Serial);
    Serial.println();
    Serial.print("DNS IP address is ");
    Ethernet.dnsServerIP().printTo(Serial);
    Serial.println();*/
    
    lcd.print("My IP address:  ");
    lcd.setCursor(0,1);
    Ethernet.localIP().printTo(lcd);
    delay (300);
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Thermoduino"); 
    lcd.print("  AAN"); 
    S1 = 2;                   // default system status
    Tc=18;                                        // default measured temperature
    h1 = dht1.readHumidity();
    Tc = dht1.readTemperature();
    networkcheck();
    
    Tw = EEPROM.readFloat(address);
    if (Tw > Tmin && Tw < Tmax)
    {
      Td = Tw;
   }
   else (Td = 17);
                  
    

/************************ Notify My Android settings *****************************
Avviso.begin(NOTIFY_MY_ANDROID);
Avviso.setApiKey("dbec96f09391f60605445597956fdd74821c2169a036067f");
Avviso.setApplicationName("Thermoduino");
int returnCode = Avviso.push("Reboot",(0) , 0);
*/
}

/************************ Initial network check *****************************/
void networkcheck(){
  if (client.connect(ServerToCheck, 80)) 
 {
   delay(300);
//    Serial.print("connected to ");
//    Serial.println(ServerToCheck);
    client.println("GET / HTTP/1.1");
    client.println();
//   Serial.println("send");
    while (client.connected())
    {
//     Serial.println("con");
      if (client.available()) 
      {
//   Serial.println("ava");
       char c = client.read();
       if (readString.length() < 20)
//       Serial.print(c);
       {
         readString.concat(c);
       }
         if (c == '\n') 
         {
           client.stop();
 //          Serial.println("OK");
           networkonline = true;
           N =1;
           readString="";
         }
      }
      if (!client.connected()) {
//      Serial.println("Disconnecting");
      client.stop();
      readString="";
    }
  }
}
else {
//Serial.println("Er");
  networkonline = false;
  N = 2;
}



}
/************************ Loop Check for server or button press*****************************/
void loop() 
{
  EthernetClient client = server.available();
  if (client)
  {
    boolean sentHeader = false;
    while (client.connected())
    {
      if (client.available())
      {
        if(!sentHeader){
        client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          sentHeader = true;
        }
         char c = client.read(); 
         if (readString.length() < 20) 
         Serial.print(c);
         {readString.concat(c);}
         if (c == '\n')
         {
           int Is = readString.indexOf("/");
           int Iq = readString.indexOf("?");
           if(readString.indexOf("?") > 1)
           {
             int Ib = readString.indexOf("b");
             int It = readString.indexOf("t");
             if (It >1)
             {
               if (S1 == 2)
               {
                 client.print (content_main_top);
                 client.print (Uit);
                 client.print (Tk);
                 client.print (Tc,1);
                 client.print (C);
              /*  client.print (content_main_top);
                client.print(line);
                client.println("Systeem is uitgeschakeld");
                client.print(line);
                client.println("Stel de temperatuur in op de Kamerthermostaat");
                client.print(line);
                client.print("Kamer temperatuur: " );
                client.print(Tc);
                client.println("C");*/
          delay(1);
                 client.stop();
                readString="";
                client.read(); client.read();
               }
               if (S1 == 1)
               {
                 char carray[5];
                 readString.toCharArray( carray,5,(Iq+1));
                 Tt = atof(carray);
                 if (Tt > Tmin && Tt < Tmax + 0.1){
                  Td = Tt; 
//                 Serial.print("Temperature is nu ingesteld op: ");
//                 Serial.print(Td,1);
//                 Serial.println("C");
                  client.print (content_main_top);
                  client.print(line);      
                  client.print("Temperatuur is nu ingesteld op: ");
                  client.print(Td,1);
                  client.println(C);
                  client.print(line);
                  client.print(Tk);
                  client.print(Tc,1);
                  client.println(C);
          delay(1);
                 client.stop();
                 readString="";
                 client.read(); client.read();
                 EEPROM.updateFloat(address, Td); 
                 }
                 else
                 {
//                   Serial.print("Onjuiste invoer: ");
//                 Serial.print(Tt,1);
//                 Serial.println("C");
                 client.print (content_main_top);
                  client.print(line); 
                  client.print("Stel de temperatuur in tussen ");
                  client.print (Tmin);
                  client.print ("C en "); 
                  client.print (Tmax);
                  client.print (C);
                  client.print(line);
                 client.print (Tk);
                  client.print(Tc,1);
                  client.println(C);
          delay(1);
                 client.stop();
                 readString="";
                 client.read(); client.read();
                 }
               }
              }
             if (Ib > 1)
             {
               char carray[2];
               readString.toCharArray( carray,2,(Iq+1));
               button = atoi(carray);
               switch (button) {
               case 1: action(1501, client);break;
               case 2: action(1502, client);break;
               case 3: action(1503, client);break;
               case 4: action(1504, client);break;
               case 5: action(1505, client);break;
               case 6: action(1506, client);break;
               case 7: action(1507, client);break;

             }
          delay(1);
             client.stop();
             readString="";
             client.read(); client.read();
           }
         }
         if (Iq < 0)
         {
          delay(1);
         client.stop();
         readString="";
         client.read(); client.read();
         }
          delay(1);
         client.stop();
         readString="";
         client.read(); client.read();
       }//         if (c == '\n')

     }//      if (client.available())
   }//    while (client.connected())

 }
 y = analogRead (0);
 action( y ,(client) );
}

/************************* Actions on web or button input *************************************/
void action(int x, EthernetClient client)
{
  currentMillis = millis(); 
  if(currentMillis - previousMillis > intervalTempUpdate)
 {
  previousMillis = currentMillis;
  h1 = dht1.readHumidity();
  Tc = dht1.readTemperature();
  h2 = dht2.readHumidity();
  t2 = dht2.readTemperature();
  if ((Tc >7) && (Tc < 40)) {tempread = true;
  T = 1;}
  else {tempread = false;
  T = 2;}
 /*  Serial.print("H1: "); 
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("T1: "); 
    Serial.print(Tc);
    Serial.print(" *C\t");
    
    Serial.print("H2: "); 
    Serial.print(h2);
    Serial.print(" %\t");
    Serial.print("T2: "); 
    Serial.print(t2);
    Serial.println(" *C");
*/
 }
  
/************************* AAN *************************************/
  if (x < 50 || x == 1501) 
  {
  // dtostrf(Tc,5, 1, temp);               //float to char conversion
  //      dtostrf(Td,5, 1, inst);
    lcd.setCursor(0,0);
    lcd.print("Thermoduino"); 
    lcd.print("  AAN");
 //    Serial.println("AAN");
    if (x == 1501) {
    client.print (content_main_top);
    client.print(line);
    client.println(Aan);
    client.print(Td,1);
    client.println(C);
    client.print(line);
    client.print (Tk);
    client.print(Tc,1);
    client.println(C);
  }
   //Avviso.setApplicationName("Verwarming       INGESCHAKELD");
    //int returnCode = Avviso.push("Kamer Temperatuur:", temp, 0);
    //int returnCode1 = Avviso.push("Ingesteld op:", inst, 0);
    S1 = 1;
    delay (buttonsensitivity);
  }
  
/************************* Warmer *************************************/
  else if (x < 170 || x == 1502)
  {
    if (S1 == 1)
    {
      Td = (Td + 0.5);
      if (Td > Tmax) Td = Tmax; 
 //     Serial.print(Td,1);
      client.print (content_main_top);
      client.print(line);
      client.print("Temperatuur wordt verhoogd naar ");
      client.print(Td,1);
      client.println(C);
      client.print(line);
      client.print(Tk);
      client.print(Tc);
      client.println(C);
      EEPROM.updateFloat(address, Td);
    }
    if (S1 == 2)
    {
      
      // dtostrf(Tc,5, 1, temp);               //float to char conversion
      //      dtostrf(Td,5, 1, inst);
       client.print (content_main_top);
                 client.print (Uit);
                 client.print (Tk);
                 client.print (Tc,1);
                 client.print (C);
    }
    delay (buttonsensitivity);
  }

//Avviso.setApplicationName(Td);
//int returnCode = Avviso.push("up", (0), 0);
 //  delay (100);
    
/************************* Kouder *************************************/ 
  else if (x < 320 || x == 1503)
  {
    if (S1 == 1)
    {
      Td = (Td - 0.5);   
     if (Td < Tmin) Td = Tmin;              // 
  //    Serial.print(Td,1);
      client.print (content_main_top);
      client.print(line);      
      client.print("Temperatuur wordt verlaagd naar ");
      client.print(Td,1);
      client.println(C);
      client.print(line);
      client.print(Tk);
      client.print(Tc,1);
      client.println(C);
      EEPROM.updateFloat(address, Td);
    }
    if (S1 == 2)
    {
      // dtostrf(Tc,5, 1, temp);               //float to char conversion
      //      dtostrf(Td,5, 1, inst);
       client.print (content_main_top);
                 client.print (Uit);
                 client.print (Tk);
                 client.print (Tc,1);
                 client.print (C);
    }
    delay (buttonsensitivity);
  }
 
//Avviso.setApplicationName(Td);
//int returnCode = Avviso.push("down", (0), 0);
 //  delay (300);
  
/************************* Uit *************************************/   
  else if (x < 500 || x == 1504)
  {
  // dtostrf(Tc,5, 1, temp);               //float to char conversion
  //      dtostrf(Td,5, 1, inst);
   lcd.setCursor(0,0);
   lcd.print("Thermoduino"); 
   lcd.print("  UIT");
   // dtostrf(Tc,5, 1, temp);
   //    Serial.println("UIT");
     if (x == 1504)
    {
      client.print (content_main_top);
                 client.print (Uit);
                 client.print (Tk);
                 client.print (Tc,1);
                 client.print (C);
    }
 
   //Avviso.setApplicationName("Verwarming      UITGESCHAKELD");
   //int returnCode = Avviso.push("Kamer Temperatuur:",(Tc) , 0);

   
    S1 = 2;
    delay (buttonsensitivity);
  }
 
/************************* Info Menu *************************************/   
  else if (x < 800 )
  {
  // dtostrf(Tc,5, 1, temp);               //float to char conversion
  //      dtostrf(Td,5, 1, inst);
   lcd.setCursor(0,0);
   lcd.print("Thermoduino     ");
   lcd.setCursor(0,1);
   lcd.print("Heating System  ");
//   Serial.println("INFO");
   delay (buttonsensitivity + buttonsensitivity);
   
/*************************** Advanced Info Menu *************************************/
/*************************** Press Info for 0.2 seconds to enter *************************************/
    x = analogRead (0);
    if (x>500 && x<800)
    {
      delay (buttonsensitivity);
      adva = 1;
      lcd.setCursor(0,0);
      lcd.print("IP address:  ");
      lcd.setCursor(0,1);
      Ethernet.localIP().printTo(lcd);
      lcd.print("    ");
      delay (3000);
      lcd.setCursor(0,0);
      lcd.print("Uptime:         ");
      lcd.setCursor(0,1);
      int d,h4,h,m,s;
      s = millis() / 1000;
      m = s / 60;
      h = s / 3600;
      d = h / 24;
      s = s - m * 60;
      m = m - h * 60;      
      h = h - d * 24;
      if (d > 1 && d < 2) {lcd.print(d); lcd.print ("dag ");}
      if (d > 1) {lcd.print(d); lcd.print ("dagen ");}
      lcd.print(h);lcd.print("h ");
      lcd.print(m);lcd.print("m ");
      lcd.print(s);
      lcd.print("s               ");
      delay (3000);
      lcd.setCursor(0,0);      
      lcd.print("Software version");
      lcd.setCursor(0,1);
      lcd.print(softwareversion);
      lcd.print("            ");
      delay (3000);
      networkcheck();
      }
 
    /*************************** Standard Info Menu *************************************/
    else ;
    {
      delay (2000);
   /*   if (S1 == 1 && adva != 1)
      {
        lcd.setCursor(0,0);
        lcd.print("Thermoduino     ");
        lcd.setCursor(0,1);
        lcd.print("Ingeschakeld    ");
        delay (3000); 
        lcd.setCursor(0,0);
        lcd.print("Binnen  T: ");
        lcd.setCursor(11,0);
        lcd.print(Tc,1);
        lcd.print("C");
        lcd.setCursor(0,1);
        lcd.print("Binnen RV: ");
        lcd.setCursor(11,1);
        lcd.print(h1,1);
        lcd.print("%");
        delay (3000);
        lcd.setCursor(0,0);
        lcd.print("Buiten  T: ");
        lcd.setCursor(11,0);
        lcd.print(t2,1);
        lcd.print("C");
        lcd.setCursor(0,1);
        lcd.print("Buiten RV: ");
        lcd.setCursor(11,1);
        lcd.print(h2,1);
        lcd.print("%");
        delay (3000);
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("                ");
      }
      if (S1 == 2 && adva != 1)
      {
        lcd.setCursor(0,0);
        lcd.print("Thermoduino     ");
        lcd.setCursor(0,1);
        lcd.print("Uitgeschakeld   ");
        delay (3000);
        lcd.setCursor(0,0);
        lcd.print("Gebruik de      ");
        lcd.setCursor(0,1);
        lcd.print("kamerthermostaat");
        delay (3000);
        lcd.setCursor(0,0);
        lcd.print("Temperatuur:");
        lcd.setCursor(11,0);
        lcd.print(Tc);
        lcd.setCursor(0,1);
        lcd.print("Vochtigheid");
        lcd.setCursor(11,1);
        lcd.print(h1);lcd.print("%");
        delay (3000);
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("                ");
      }*/
    }
    adva = 0;
  }
  /*************************** Info Request *************************************/
  if (x == 1505)
  {
      client.print (content_main_top);
      client.print ("IP address: ");
      Ethernet.localIP().printTo(client);
      client.print(" Uptime: ");
      int d,h,m,s;
      s = millis() / 1000;
      m = s / 60;
      h = s / 3600;
      d = h / 24;
      s = s - m * 60;
      m = m - h * 60;
      h = h - d * 24;
      if (d > 1 && d < 2) {client.print(d); client.print ("dag ");}
      if (d > 1) {client.print(d); client.print ("dagen ");}
      client.print(h);client.print("h ");
      client.print(m);client.print("m ");
      client.print(s);
      client.print("s");
      client.print(line);
      client.print("Software version: ");
      client.print(softwareversion);
      client.print(" Free Ram: ");
      client.print(freeRam());
      client.print(line);
      client.print("Copyright mark@linnemann.tk");
      client.print ("<meta http-equiv=refresh content=5,url=/b?7>");

    }
    
 /*************************** Status Request *************************************/
  if (x == 1507)
  {
//Serial.println("Status request");
    if (S1 == 1)
    {
   //   Serial.println("AAN");
      client.print (content_main_top);
      client.println(Aan);
      client.print(Td,1);
      client.println(C);
      client.print(line);
      client.print (Tk);
      client.print(Tc,1);
      client.println(C);
    }
    if (S1 == 2)
    {
   //   Serial.println("UIT");
       client.print (content_main_top);
                 client.print (Uit);
                 client.print (Tk);
                 client.print (Tc,1);
                 client.print (C);
    }
    
  }
/*************************** Temperature Request *************************************/
  if (x == 1506)
  {
     client.print (content_main_top);     
    client.print("Binnen: "); 
    client.print(Tc,1);
    client.print("C ");
    //client.print("<p>");
    client.print(h1,1);
    client.print("% RV");
    client.print(line); 
    client.print("Buiten: ");
    client.print(t2,1);
    client.print("C "); 
   //client.print("<p>");
    client.print(h2,1);
    client.print("% RV");
    client.print ("<meta http-equiv=refresh content=3,url=/b?6>");

    
  }

  /***********************Reset button input*************************/
  x=1500;
          
  /**********Calculate if heater must turn on and print results *************/
  
  lcd.setCursor(7,1);
  lcd.print(Td,1);
  lcd.setCursor(14,1);
  
   
  if (T == 1 && N == 1)
 {
 lcd.print("OK");
 digitalWrite(Buzzer, LOW);
}
 else
 {
 lcd.print("ER");
   digitalWrite(Buzzer, HIGH);
 S1 = 2;
 }
lcd.setCursor(0,0);
  lcd.print("Thermoduino"); 
  if ((S1 == 1) && (networkonline = true) && (T = 1)){lcd.print ("  AAN");  digitalWrite(System, HIGH);}
  if (S1 == 2) {lcd.print ("  UIT"); digitalWrite(System, LOW);}
  lcd.setCursor(0,1);
  lcd.print(Tc,1);
  if ((Td > Tc) && (S1 == 1)) 
  {
  (R2 = 1);
  digitalWrite(Heater, HIGH);
  lcd.print(" * ");
 }
  if ((Td < Tc) || (S1 != 1))
  {
  (R2 = 2);
  digitalWrite(Heater, LOW);
  lcd.print(" _ ");
 }

/*****************Every x seconds check network conection*******/
if (networkonline == false) {
  currentMillis = millis(); 
  if(currentMillis - previousMillis > intervalNetworkCheck)
 {
   networkcheck();
 }
}
}

