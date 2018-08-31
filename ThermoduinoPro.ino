
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <EEPROMex.h>
#include <SPI.h>
#include <DHT.h>
#include "RTClib.h"
#include <Wire.h>
#include <SD.h>
#include <Udp.h>        // New from IDE 1.0
#include <avr/pgmspace.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

#include <ThingSpeak.h>                                                       //THINGSPEAK
unsigned long myChannelNumber = 16728;
const char * myWriteAPIKey = "ELJ26QMDNU5SYW2M";

ClickEncoder *encoder;
float last, value;
void timerIsr() {
  encoder->service();
}

#define DEBUG1 1//Init info
#define DEBUG2 1//Errors
#define DEBUG3 1  //Debug info
#define DEBUG4 1  //Debug Mirf
#define DEBUG5 1  //Debug Ethernet
#define DEBUG6 1  //Debug Cosm
// ++++++++++++++ DEFINIZIONE PIN SENSORI +++++
//#define BUTTON_UP    39             // button increase Td
//#define BUTTON_DOWN  35             // button decrease Td
#define DHTPIN1     (43)            // dht22 sensor pin
const int System =  34;             // original thermostat rele pin
const int Heater =  49;             // this thermostat rele pin
int pirPin = 37;                     //the digital pin connected to the PIR sensor's output

#include <OneWire.h>                  //
#include <DallasTemperature.h>        //
#define ONE_WIRE_BUS 31               //
OneWire oneWire(ONE_WIRE_BUS);        // DS18B20 external sensor pin
DallasTemperature sensors(&oneWire);  //
DeviceAddress tmp_address;            //
float ext = 0;                        //
float camera = 0;                     //

#define DHTTYPE DHT22
DHT dht1(DHTPIN1, DHTTYPE);

int sondaref = 0 ;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

byte pentoladef[8] = {
  0b00101, 0b01010, 0b00000, 0b00100, 0b11111, 0b01010, 0b01010, 0b01110
};
byte termometrodef[8] = {
  0b01011,  0b01000,  0b01011,  0b01000,  0b01011,  0b01000,  0b11100,  0b11100
};
byte termometroNEGdef[8] = {
  0b10100,  0b10111,  0b10100,  0b10111,  0b10100,  0b10111,  0b00011,  0b00011
};
byte gocciadef[8] = {
  0b00100,  0b00100,  0b01010,  0b01010,  0b10001,  0b10001,  0b10001,  0b01110
};
byte nottedef[8] = {
  0b11110, 0b00100, 0b01000, 0b11110, 0b00000, 0b00111, 0b00010, 0b00111
};

int pentola = 0;
int termometro = 1;
int goccia = 2;
int termometroNEG = 3;
int notte = 4;

int mac_byte = 239; //change this last byte of the mac adress for each board

String handshake = "THERMO_OK";

float Tc;                           // measured temperature
float Td;                           // disired temperature
float cucina;
float h0;                           // measured humidity
float Tt;                           //used to ckeck if measured temperature is OK
int address_temp = 250;            // eeprom address
float Tw;                          // eeprom write temperature
float Tr;                          // eeprom read temperature
boolean S = true;                  // system status
boolean H;                         // Heater status
boolean ER;                        // Error status
int adva = 0;
int x;
int y;
int Tmax = 30;                      //set maximum temperature
int Tmin = 0;                       //sets minimum temperature
long HeaterStatusDelay = 60;        //delay before heater changes state in seconds
long intervalNetworkCheck = 1500;   // each interval the system will check if the network is online
long intervalTempUpdate = 5000L;
boolean networkinput = false;
boolean networkonline = false;
boolean DNS = false;
boolean N = false;                          // network status
boolean T;
int Von;
int Voff;
int resetcounter;
long previousMillis = 0;
unsigned long currentMillis;
int lastTime1 = -1;
int lastTime2 = -1;
int lastTime3 = -1;
int lastTime4 = -1;
int lastTime5 = -1;
int nowtime1;
int nowtime2;
int nowTime3;
int nowTime4;
int nowTime5;          // used for lcd off using pir
long interval1 = 0;    //sensor reading interval
long interval2 = 300;  //web upload interval
long interval3 = 15000; //lcd off delay


RTC_Millis RTC;          //for software rtc
//RTC_DS1307 RTC;        //for hardware rtc
DateTime start;
DateTime up;


unsigned int localPort = 8888;      // local port to listen for UDP packets
IPAddress timeServer(193, 204, 114, 232);
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
EthernetUDP Udp;

String sketchname = String(30);             //skech name
String uptime;
String cosm_uptime;

float ts1;
float vs1;
float hs1;
int nrs1;
float ts2;
float vs2;
float hs2;
int nrs2;

float ts3;
float vs3;
float hs3;
int nrs3;
float ts4;
float vs4;
float hs4;
int nrs4;
float BP;            //variabile sensore bmp180
//int PU;

boolean timer1;
boolean timer2;
boolean timer3;
boolean timer4;
boolean timer5;
boolean timer6;
boolean timer7;
boolean timer8;
boolean timer9;
int timer_1_hour;
int timer_1_minute;
int timer_1_enable;
int timer_1_day;
int timer1_channel;
float timer1_action;
int timer_2_hour;
int timer_2_minute;
int timer_2_enable;
int timer_2_day;
int timer2_channel;
float timer2_action;
int timer_3_hour;
int timer_3_minute;
int timer_3_enable;
int timer_3_day;
int timer3_channel;
float timer3_action;
int timer_4_hour;
int timer_4_minute;
int timer_4_enable;
int timer_4_day;
int timer4_channel;
float timer4_action;
int timer_5_hour;
int timer_5_minute;
int timer_5_enable;
int timer_5_day;
int timer5_channel;
float timer5_action;
int timer_6_hour;
int timer_6_minute;
int timer_6_enable;
int timer_6_day;
int timer6_channel;
float timer6_action;
int timer_7_hour;
int timer_7_minute;
int timer_7_enable;
int timer_7_day;
int timer7_channel;
float timer7_action;
int timer_8_hour;
int timer_8_minute;
int timer_8_enable;
int timer_8_day;
int timer8_channel;
float timer8_action;
int timer_9_hour;
int timer_9_minute;
int timer_9_enable;
int timer_9_day;
int timer9_channel;
float timer9_action;

int timer_1_hour_address = 74;
int timer_1_minute_address  = 76;
int timer_2_hour_address  = 25;
int timer_2_minute_address  = 26;
int timer_3_hour_address  = 29;
int timer_3_minute_address  = 30;
int timer_4_hour_address  = 33;
int timer_4_minute_address  = 34;
int timer_5_hour_address  = 37;
int timer_5_minute_address  = 38;
int timer_6_hour_address  = 42;
int timer_6_minute_address  = 44;
int timer_7_hour_address  = 50;
int timer_7_minute_address  = 52;
int timer_8_hour_address  = 58;
int timer_8_minute_address  = 60;
int timer_9_hour_address  = 66;
int timer_9_minute_address  = 68;
int timer_1_day_address  = 82;
int timer_2_day_address  = 84;
int timer_3_day_address  = 86;
int timer_4_day_address  = 88;
int timer_5_day_address  = 90;
int timer_6_day_address  = 92;
int timer_7_day_address  = 94;
int timer_8_day_address  = 96;
int timer_9_day_address  = 98;
int timer_1_enable_address  = 102;
int timer_2_enable_address  = 104;
int timer_3_enable_address  = 106;
int timer_4_enable_address  = 108;
int timer_5_enable_address  = 110;
int timer_6_enable_address  = 112;
int timer_7_enable_address  = 114;
int timer_8_enable_address  = 116;
int timer_9_enable_address  = 118;
int timer1_action_address  = 120;
int timer2_action_address  = 124;
int timer3_action_address  = 128;
int timer4_action_address  = 132;
int timer5_action_address  = 136;
int timer6_action_address  = 140;
int timer7_action_address  = 144;
int timer8_action_address  = 148;
int timer9_action_address  = 152;
int port_address = 154;

int Hour;
int Minute;
int Second;
int giorno;
int mese;
String time;
String date;
boolean tempread = false;
boolean receivingdata = false;

String readString = String(70);
String lastReading = String(40);

byte mac[6] = {
  0x90, 0xA2, 0xDA, 0x00, 0x00, 0x00
};

byte ip[] = { 192, 168, 0, 4 };                              //  comment to use dhcp

IPAddress temp_ip;
IPAddress dhcp_ip;
IPAddress gw_ip;
IPAddress dns_ip;
EthernetServer server(80);
int wan_port;
EthernetClient client;
IPAddress Google1    (74, 125, 228, 32 );
IPAddress ServerToCheck = Google1;             // set the name of the server to check
char dnsServerToCheck[] = "google.com";             // set the name of the server to check

char content_main_top[] = "<body bgcolor=black><font color=white><center>";
char Uit[] = "Il sistema è spento<br>Impostare la temperatura sul termostato<br>" ;
char Tk[] = "Temperatura ambiente: " ;
char C[] = "&degC";
char Aan[] = "Il sistema è accesso<br>La temperatura è impostata su: ";
char line[] = "<br>";
char Rf5[] = "<meta http-equiv=refresh content=2,url=/b?7>";
char Rf6[] = "<meta http-equiv=refresh content=25,url=/b?6>";
char Rf7[] = "<meta http-equiv=refresh content=15,url=/b?7>";

//int freeRam () {
//  extern int __heap_start, *__brkval;
//  int v;
//  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
//}

/********************* RTC start ****************************************************************************/
void start_rtc()
{
  Udp.begin(localPort);
  Wire.begin();
  RTC.begin(DateTime(__DATE__, __TIME__));
}
/********************* get start time ****************************************************************************/
void start_time()
{
  start =  RTC.now();
  if (DEBUG1) Serial.print("Start up time is ");
  if (DEBUG1) Serial.print(start.hour(), DEC);
  if (DEBUG1) Serial.print(':');
  Minute = (start.minute()); //display current minute
  if (Minute < 10)
  {
    if (DEBUG1) Serial.print("0"); //insert leading zero if minute value is less than 10
    if (DEBUG1) Serial.print(Minute);
  }
  else if (DEBUG1) Serial.print(Minute); //display current minute
  if (DEBUG1) Serial.print(":");
  Second = (start.second());
  if (Second < 10)
  {
    if (DEBUG1) Serial.print("0"); //insert leading zero if seconds value is less than 10
    if (DEBUG1) Serial.print(Second); //display current seconds
  }
  else if (DEBUG1) Serial.print(Second); //display current seconds
  if (DEBUG1) Serial.print("  Date is ");
  if (DEBUG1) Serial.print(start.day(), DEC);
  if (DEBUG1) Serial.print('/');
  if (DEBUG1) Serial.print(start.month(), DEC);
  if (DEBUG1) Serial.print('/');
  if (DEBUG1) Serial.println(start.year(), DEC);
}
/********************* RTC SYNC ****************************************************************************/
void synce_rtc()
{
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() )
  {
    if (DEBUG6) Serial.println("NTP Packet recieved");
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears + 1;
    DateTime now = RTC.now();
    if (DEBUG6)
    {
      Serial.print("UTC time is    ");       // UTC is the time at Greenwich Meridian (GMT)
      Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
      Serial.print(':');
      if ( ((epoch % 3600) / 60) < 10 )
      {
        // In the first 10 minutes of each hour, we'll want a leading '0'
        Serial.print('0');
      }
      Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
      Serial.print(':');
      if ( (epoch % 60) < 10 )
      {
        // In the first 10 seconds of each minute, we'll want a leading '0'
        Serial.print('0');
      }
      Serial.println(epoch % 60); // print the second
    }
    now = (epoch);
    RTC.adjust(DateTime(now));
    now = RTC.now();
    int Month = (now.month());
    if ((Month >= 4) && (Month <= 11)) {
      now = (epoch + 7200);
      if (DEBUG6) Serial.println("Summertime");

    }
    else {
      now = (epoch + 3600);    //one hour to adjust timezone
      if (DEBUG6) Serial.println("Wintertime");
    }
    RTC.adjust(DateTime(now));
    now = RTC.now();


    if (DEBUG6) Serial.print("Local time is  ");
    Hour = (now.hour());
    if (DEBUG6) Serial.print(Hour);
    if (DEBUG6) Serial.print(':');
    Minute = (now.minute()); //display current minute
    if (Minute < 10)
    {
      if (DEBUG6) Serial.print("0"); //insert leading zero if minute value is less than 10
      if (DEBUG6) Serial.print(Minute);
    }
    else if (DEBUG6) Serial.print(Minute); //display current minute
    if (DEBUG6) Serial.print(":");
    Second = (now.second());
    if (Second < 10)
    {
      if (DEBUG6) Serial.print("0"); //insert leading zero if seconds value is less than 10
      if (DEBUG6) Serial.print(Second); //display current seconds
      if (DEBUG6) Serial.println("");
    }
    else if (DEBUG6) Serial.println(Second); //display current seconds
    if (DEBUG6) Serial.println("RTC synced");
    lastTime3 = now.hour();
    lastTime4 = now.day();
  }
  else {
    Serial.println("RTC not synced");
  }
}
// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  if (DEBUG6) Serial.println("Request NTP Update");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
/***************************** start ethernet ***********************************************************************/
void restart_ethernet() {
  delay(1000);
  start_ethernet();
}

void start_ethernet()
{
  if (DEBUG5) Serial.println("Ethernet start");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print ("Acquisizione ip...");

  if (EEPROM.read(11) == '@' && networkonline == true) {
    if (DEBUG5) Serial.print("IP found ");
    int ip1 = EEPROM.read(13);
    int ip2 = EEPROM.read(14);
    int ip3 = EEPROM.read(15);
    int ip4 = EEPROM.read(16);
    IPAddress ip(ip1, ip2, ip3, ip4);
    if (DEBUG5) Serial.println(ip);
    int dns_ip1 = EEPROM.read(17);
    int dns_ip2 = EEPROM.read(18);
    int dns_ip3 = EEPROM.read(19);
    int dns_ip4 = EEPROM.read(20);
    IPAddress dns_ip(dns_ip1, dns_ip2, dns_ip3, dns_ip4);
    if (DEBUG5) Serial.println(dns_ip);
    int gw_ip1 = EEPROM.read(21);
    int gw_ip2 = EEPROM.read(22);
    int gw_ip3 = EEPROM.read(23);
    int gw_ip4 = EEPROM.read(24);
    IPAddress gw_ip(gw_ip1, gw_ip2, gw_ip3, gw_ip4);
    if (DEBUG5) Serial.println(gw_ip);
    Ethernet.begin(mac, ip, dns_ip, gw_ip);

  }
  else if (EEPROM.read(11) == '%' || networkonline == false) {
    // if (DEBUG5) Serial.println("DHCP IP ");
    //  Ethernet.begin(mac);                                         // comment to use static ip
    Ethernet.begin(mac, ip);                                       // comment to use dhcp
  }
  else {
    if (DEBUG5) Serial.println("IP not found ");
    //  Ethernet.begin(mac);                                         // comment to use static ip
    Ethernet.begin(mac, ip);                                       // comment to use dhcp
    temp_ip = (Ethernet.localIP());
    dhcp_ip = (Ethernet.localIP());
    gw_ip = (Ethernet.gatewayIP());
    dns_ip = (Ethernet.dnsServerIP());
    if (DEBUG5) Serial.println(temp_ip[0]);
    if (DEBUG5) Serial.println(temp_ip[1]);
    if (DEBUG5) Serial.println(temp_ip[2]);
    if (DEBUG5) Serial.println(temp_ip[3]);
    (temp_ip[3]) = (111);
    Ethernet.begin(mac, temp_ip, dns_ip, gw_ip);
  }
  server.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print ("IP acquisito");
  lcd.setCursor(0, 1);
  Ethernet.localIP().printTo(lcd);
  delay(1000);
  lcd.clear();
  if (DEBUG5) Serial.print("IP is ");
  if (DEBUG5) Serial.println(Ethernet.localIP());
  if (DEBUG5) Serial.print("Gateway is ");
  if (DEBUG5) Serial.println(Ethernet.gatewayIP());
  if (DEBUG5) Serial.print("DNS is ");
  if (DEBUG5) Serial.println(Ethernet.dnsServerIP());
  networkcheck();
}

/************************  check_first_run *****************************/
void check_first_run()
{
  if (DEBUG5) Serial.println("check_first_run ");
  if (EEPROM.read(1) == '#') {
    if (DEBUG5) Serial.println("mac found ");
    for (int i = 3; i < 6; i++) {
      mac[i] = EEPROM.read(i);
    }
  }
  else {
    if (DEBUG5) Serial.println("mac not found ");
    EEPROM.setMaxAllowedWrites(30000);

    // write a 0 to all 512 bytes of the EEPROM
    for (int i = 0; i < 512; i++)
      EEPROM.write(i, 0);

    mac[3] = 237;
    mac[4] = 254;
    mac[5] = mac_byte;
    for (int i = 3; i < 6; i++) {
      //      mac[i] = TrueRandom.randomByte();
      EEPROM.write(i, mac[i]);
    }
    EEPROM.write(1, '#');
  }

  if (DEBUG5) Serial.print("Mac is ");
  if (DEBUG5) Serial.print(mac[0]);
  if (DEBUG5) Serial.print(":");
  if (DEBUG5) Serial.print(mac[1]);
  if (DEBUG5) Serial.print(":");
  if (DEBUG5) Serial.print(mac[2]);
  if (DEBUG5) Serial.print(":");
  if (DEBUG5) Serial.print(mac[3]);
  if (DEBUG5) Serial.print(":");
  if (DEBUG5) Serial.print(mac[4]);
  if (DEBUG5) Serial.print(":");
  if (DEBUG5) Serial.println(mac[5]);
}

/************************  Network check *****************************/
void networkcheck()
{
  //  if (DEBUG1) Serial.println ("Network check");
  //  if (client.connect(dnsServerToCheck, 80))
  //  {
  //    delay(300);
  //    if (DEBUG1) Serial.print("connected to ");
  //    if (DEBUG1) Serial.println(dnsServerToCheck);
  //    client.println("GET / HTTP/1.1");
  //    client.println();
  //    while (client.connected())
  //    {
  //      if (client.available())
  //      {
  //        char c = client.read();
  //        if (readString.length() < 20)
  //          if (DEBUG1) Serial.print(c);
  //        {
  //          readString.concat(c);
  //        }
  //        if (c == '\n')
  //        {
  //          client.stop();
  //          if (DEBUG1) Serial.println();
  //          if (DEBUG1) Serial.println("DNS OK");
  DNS = true;
  networkonline = true;
  N = true;
  //          readString="";
  //        }
  //      }
  //      if (!client.connected())
  //      {
  //        client.stop();
  //        readString="";
  //      }
  //    }
  //  }
  //  if (!DNS)
  //  {
  //    if (DEBUG1) Serial.println("DNS Error");
  //    if (DEBUG1) Serial.println("Check for IP");
  //    if (client.connect(ServerToCheck, 80))
  //    {
  //      delay(300);
  //      if (DEBUG1) Serial.print("connected to ");
  //      if (DEBUG1) Serial.println(ServerToCheck);
  //      client.println("GET / HTTP/1.1");
  //      client.println();
  //      while (client.connected())
  //      {
  //        if (client.available())
  //        {
  //          char c = client.read();
  //          if (readString.length() < 20)
  //            if (DEBUG1) Serial.print(c);
  //          {
  //            readString.concat(c);
  //          }
  //          if (c == '\n')
  //          {
  //            client.stop();
  //            if (DEBUG1) Serial.println();
  //            if (DEBUG1) Serial.println("IP lookup OK");
  //            if (DEBUG1) Serial.println("DNS not available");
  //            networkonline = true;
  //            N = true;
  //            readString="";
  //          }
  //        }
  //        if (!client.connected())
  //        {
  //          if (DEBUG1) Serial.println("Disconnecting");
  //          client.stop();
  //          readString="";
  //        }
  //      }
  //    }
  //    else
  //    {
  //      if (DEBUG1) Serial.println("Network Error");
  //      resetcounter++;
  //      networkonline = false;
  //      N = false;
  //    }
  //  }
}
/************************  Read EEPROM *****************************/
const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;


// Returns true if the address is between the
// minimum and maximum allowed values, false otherwise.
//
// This function is used by the other, higher-level functions
// to prevent bugs and runtime errors due to invalid addresses.
boolean eeprom_is_addr_ok(int addr) {
  return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}


// Writes a sequence of bytes to eeprom starting at the specified address.
// Returns true if the whole array is successfully written.
// Returns false if the start or end addresses aren't between
// the minimum and maximum allowed values.
// When returning false, nothing gets written to eeprom.
boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes) {
  // counter
  int i;

  // both first byte and last byte addresses must fall within
  // the allowed range
  if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
    return false;
  }

  for (i = 0; i < numBytes; i++) {
    EEPROM.write(startAddr + i, array[i]);
  }

  return true;
}


// Writes a string starting at the specified address.
// Returns true if the whole string is successfully written.
// Returns false if the address of one or more bytes fall outside the allowed range.
// If false is returned, nothing gets written to the eeprom.
boolean eeprom_write_string(int addr, const char* string) {

  int numBytes; // actual number of bytes to be written

  //write the string contents plus the string terminator byte (0x00)
  numBytes = strlen(string) + 1;

  return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}


// Reads a string starting from the specified address.
// Returns true if at least one byte (even only the string terminator one) is read.
// Returns false if the start address falls outside the allowed range or declare buffer size is zero.
//
// The reading might stop for several reasons:
// - no more space in the provided buffer
// - last eeprom address reached
// - string terminator byte (0x00) encountered.
boolean eeprom_read_string(int addr, char* buffer, int bufSize) {
  byte ch; // byte read from eeprom
  int bytesRead; // number of bytes read so far

  if (!eeprom_is_addr_ok(addr)) { // check start address
    return false;
  }

  if (bufSize == 0) { // how can we store bytes in an empty buffer ?
    return false;
  }

  // is there is room for the string terminator only, no reason to go further
  if (bufSize == 1) {
    buffer[0] = 0;
    return true;
  }

  bytesRead = 0; // initialize byte counter
  ch = EEPROM.read(addr + bytesRead); // read next byte from eeprom
  buffer[bytesRead] = ch; // store it into the user buffer
  bytesRead++; // increment byte counter

  // stop conditions:
  // - the character just read is the string terminator one (0x00)
  // - we have filled the user buffer
  // - we have reached the last eeprom address
  while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) ) {
    // if no stop condition is met, read the next byte from eeprom
    ch = EEPROM.read(addr + bytesRead);
    buffer[bytesRead] = ch; // store it into the user buffer
    bytesRead++; // increment byte counter
  }

  // make sure the user buffer has a string terminator, (0x00) as its last byte
  if ((ch != 0x00) && (bytesRead >= 1)) {
    buffer[bytesRead - 1] = 0;
  }

  return true;
}
const int BUFSIZE = 25;
char buf[BUFSIZE];

String myString;
char myStringChar[BUFSIZE];
String Controller_Name;
String D_C_Name = "Controller 1";


void read_EEPROM()
{
  timer_1_hour = EEPROM.readByte(timer_1_hour_address);
  timer_1_minute = EEPROM.readByte(timer_1_minute_address);
  timer_1_day = EEPROM.readByte(timer_1_day_address);
  timer1_action = EEPROM.readFloat(timer1_action_address);
  timer_1_enable = EEPROM.readByte(timer_1_enable_address);
  if (DEBUG1) Serial.print("Timer 1: ");
  if (DEBUG1) Serial.print(timer_1_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_1_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_1_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_1_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer1_action);
  timer_2_hour = EEPROM.readByte(timer_2_hour_address);
  timer_2_minute = EEPROM.readByte(timer_2_minute_address);
  timer_2_day = EEPROM.readByte(timer_2_day_address);
  timer2_action = EEPROM.readFloat(timer2_action_address);
  timer_2_enable = EEPROM.readByte(timer_2_enable_address);
  if (DEBUG1) Serial.print("Timer 2: ");
  if (DEBUG1) Serial.print(timer_2_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_2_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_2_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_2_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer2_action);
  timer_3_hour = EEPROM.readByte(timer_3_hour_address);
  timer_3_minute = EEPROM.readByte(timer_3_minute_address);
  timer_3_day = EEPROM.readByte(timer_3_day_address);
  timer3_action = EEPROM.readFloat(timer3_action_address);
  timer_3_enable = EEPROM.readByte(timer_3_enable_address);
  if (DEBUG1) Serial.print("Timer 3: ");
  if (DEBUG1) Serial.print(timer_3_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_3_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_3_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_3_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer3_action);
  timer_4_hour = EEPROM.readByte(timer_4_hour_address);
  timer_4_minute = EEPROM.readByte(timer_4_minute_address);
  timer_4_day = EEPROM.readByte(timer_4_day_address);
  timer4_action = EEPROM.readFloat(timer4_action_address);
  timer_4_enable = EEPROM.readByte(timer_4_enable_address);
  if (DEBUG1) Serial.print("Timer 4: ");
  if (DEBUG1) Serial.print(timer_4_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_4_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_4_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_4_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer4_action);
  timer_5_hour = EEPROM.readByte(timer_5_hour_address);
  timer_5_minute = EEPROM.readByte(timer_5_minute_address);
  timer_5_day = EEPROM.readByte(timer_5_day_address);
  timer5_action = EEPROM.readFloat(timer5_action_address);
  timer_5_enable = EEPROM.readByte(timer_5_enable_address);
  if (DEBUG1) Serial.print("Timer 5: ");
  if (DEBUG1) Serial.print(timer_5_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_5_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_5_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_5_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer5_action);
  timer_6_hour = EEPROM.readByte(timer_6_hour_address);
  timer_6_minute = EEPROM.readByte(timer_6_minute_address);
  timer_6_day = EEPROM.readByte(timer_6_day_address);
  timer6_action = EEPROM.readFloat(timer6_action_address);
  timer_6_enable = EEPROM.readByte(timer_6_enable_address);
  if (DEBUG1) Serial.print("Timer 6: ");
  if (DEBUG1) Serial.print(timer_6_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_6_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_6_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_6_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer6_action);
  timer_7_hour = EEPROM.readByte(timer_7_hour_address);
  timer_7_minute = EEPROM.readByte(timer_7_minute_address);
  timer_7_day = EEPROM.readByte(timer_7_day_address);
  timer7_action = EEPROM.readFloat(timer7_action_address);
  timer_7_enable = EEPROM.readByte(timer_7_enable_address);
  if (DEBUG1) Serial.print("Timer 7: ");
  if (DEBUG1) Serial.print(timer_7_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_7_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_7_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_7_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer7_action);
  timer_8_hour = EEPROM.readByte(timer_8_hour_address);
  timer_8_minute = EEPROM.readByte(timer_8_minute_address);
  timer_8_day = EEPROM.readByte(timer_8_day_address);
  timer8_action = EEPROM.readFloat(timer8_action_address);
  timer_8_enable = EEPROM.readByte(timer_8_enable_address);
  if (DEBUG1) Serial.print("Timer 8: ");
  if (DEBUG1) Serial.print(timer_8_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_8_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_8_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_8_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer8_action);
  timer_9_hour = EEPROM.readByte(timer_9_hour_address);
  timer_9_minute = EEPROM.readByte(timer_9_minute_address);
  timer_9_day = EEPROM.readByte(timer_9_day_address);
  timer9_action = EEPROM.readFloat(timer9_action_address);
  timer_9_enable = EEPROM.readByte(timer_9_enable_address);
  if (DEBUG1) Serial.print("Timer 9: ");
  if (DEBUG1) Serial.print(timer_9_hour);
  if (DEBUG1) Serial.print(":");
  if (DEBUG1) Serial.print(timer_9_minute);
  if (DEBUG1) Serial.print(" Day: ");
  if (DEBUG1) Serial.print(timer_9_day);
  if (DEBUG1) Serial.print(" Enabled: ");
  if (DEBUG1) Serial.print(timer_9_enable);
  if (DEBUG1) Serial.print(" Action: ");
  if (DEBUG1) Serial.println(timer9_action);

  Tw = EEPROM.readFloat(address_temp);     //read last set temperature from EEPROM
  if (Tw > Tmin && Tw < Tmax)
  {
    Td = Tw;

    if (DEBUG1) Serial.print("Temperature in EEPROM data found: ");
    if (DEBUG1) Serial.println(Tw);
  }
  else
  {
    (Td = 17);
    if (DEBUG1) Serial.println("No temperature in EEPROM data found");
  }
  if  (EEPROM.read(225) == '@') {
    Serial.print("Stored name: ");
    eeprom_read_string(227, buf, BUFSIZE);
    Controller_Name = buf;
    if (DEBUG5)Serial.println(Controller_Name);

  }
  else {
    Controller_Name = D_C_Name ;
    if (DEBUG5) Serial.print("Default name: ");
    if (DEBUG5)Serial.println(Controller_Name);
  }
  if ((wan_port = EEPROM.readInt(port_address)) == 255)wan_port = 0;
  if (DEBUG6) Serial.print("Wan port: ");
  if (DEBUG6) Serial.println(wan_port);

}
/**********************************************************************************************/
/*********************************** Setup *************************************************************/
/*************************************************************************************************/
void setup()
{
  lcd.begin(20, 4);        // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.createChar(pentola, pentoladef);
  lcd.createChar(termometro, termometrodef);
  lcd.createChar(goccia, gocciadef);
  lcd.createChar(notte, nottedef);
  T = true;
  pinMode(Heater, OUTPUT);
  //pinMode(BUTTON_UP, INPUT_PULLUP);
  //pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(pirPin, INPUT);                    //   pir
  digitalWrite(pirPin, LOW);               //   sensor

  dht1.begin();

  sensors.begin();        //

  Serial.begin(9600);

  check_first_run();
  start_ethernet();
  start_rtc();
  synce_rtc();
  start_time();
  read_EEPROM();
  bmp180_check();

  encoder = new ClickEncoder(A1, A0, A2);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  EEPROM.setMaxAllowedWrites(200);

  if (DEBUG1) Serial.println("Setup complete");
  if (DEBUG1) Serial.println("");
  calculate();

  DateTime now = RTC.now();
  lastTime2  = now.unixtime();
  digitalWrite(Heater, LOW);

ThingSpeak.begin(client);

}
void(* resetFunc) (void) = 0; //declare reset function @ address 0


/****************************************************************************************************/
/****************************** Loop *****************************/
/****************************************************************************************************/
void loop()
{
  DateTime now = RTC.now();                      //
  check_timers();                                //
  run_eth();     //Check fot Ethernet Client     //   GROUP OF FUNCTIONS EXECUTED CONSTANTLY ON EACH LOOP
  read_button();                                 //
  display();                                     //

  nowtime1 = now.unixtime();                    //
  if (abs(nowtime1 - lastTime1) > interval1)    //
  {                                             //
    lastTime1 = nowtime1;                       //
    h0 = dht1.readHumidity();                   //  READING OF SENSORS EXECUTE EVERY "INTERVAL1" VALUE
    cucina = dht1.readTemperature();            //
    readDS18B20();
    if (now.hour() >= 0 && now.hour() < 9) {    //
      Tc = camera;
      sondaref = 1;
    }
    else {
      Tc = cucina  ;            //
      sondaref = 2 ;
    }
    pressione ();                               //
    calculate();                                //
    T = true;                                   //
    // Send data to Xively
  }

    nowtime2 = now.unixtime();                           //
    if (abs(nowtime2 - lastTime2) > interval2)           //
    {                                                    // WEB UPLOAD OF SENSOR
      lastTime2 = nowtime2;                              // READING MADE EVERY
                                                          // "INTERVAL2"
ThingSpeak.setField(1,cucina);
ThingSpeak.setField(2,camera);
ThingSpeak.setField(3,h0);
ThingSpeak.setField(4,BP);

  // Write the fields that you've set all at once.
 ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  

      
    }
//    if (networkonline == false)
//    {
//      resetcounter++;
//      currentMillis = millis();
//    if (currentMillis - previousMillis >= intervalNetworkCheck)
//    {
//      networkcheck();
//    }
//  }
//    if(resetcounter >= 4)
//    {
//      Serial.println("Resetting........");
//      delay(100);
//      resetFunc();
//    }

  nowTime3 = now.hour();
  if (nowTime3 != lastTime3)
  {
    synce_rtc();
    //    if (DEBUG2)  up_time();
    //    if (DEBUG2) Serial.println(uptime);
    if (N && (resetcounter > 0))
    {
      resetcounter = 0;
    }
  }
  nowTime4 = now.day();
  if (nowTime4 != lastTime4)
  {
    lastTime4 = now.day();
    //    Serial.println("Resetting........");
    //    delay(100);
    //    resetFunc();
  }
}

/**********Calculate if heater must turn on and print results *************/
void calculate()
{
  DateTime now = RTC.now();
  if (T && N)
  {
    ER = false;
  }
  else
  {
    if (DEBUG3) Serial.println("Network or temperature read error!");
    ER = true;

  }
  if (S && networkonline && T && !ER) {
    digitalWrite(System, HIGH);
  }
  if (!S || ER) {
    digitalWrite(System, LOW);
    S = false;
  }
  if ((Td > Tc) && S && !ER && !H)
  {
    Voff = 0;
    Von ++;
    if (DEBUG3) Serial.print("Seconds before turning on Heater: ");
    if (DEBUG3) Serial.println(HeaterStatusDelay - Von * 1 );
    if (Von == HeaterStatusDelay / 1)
    {
      (H = true);
      if (DEBUG3) Serial.println("turning on Heater");
      if ((Tc > 0) && (Tc < 40))
      {
        digitalWrite(Heater, HIGH);
        //   sendData();
      }
      else
      {
        (H = false);
        if (DEBUG3) Serial.println("DHT error, turning off Heater");
        digitalWrite(Heater, LOW);
      }
      Von = 0;
    }
  }

  else
  {
    (Von = 0);
  }
  if (((Td < Tc) || !S || ER) && H)
  {
    Von = 0;
    Voff ++;
    if (DEBUG3) Serial.print("Seconds before turning off Heater: ");
    if (DEBUG3) Serial.println(HeaterStatusDelay - Voff * 1);
    if (Voff == HeaterStatusDelay / 1)
    {
      (H = false);
      if (DEBUG3) Serial.println("turning off Heater");
      digitalWrite(Heater, LOW);
      Voff = 0;
      if ((Tc > 7) && (Tc < 40)) {
        //   sendData();
      }
    }
  }
  else (Voff = 0);
}
/*************************** print time ****************************************************************************/
void time_date()
{
  DateTime now = RTC.now();  //Get the current data
  time = "";
  time.concat(now.hour());
  time.concat(':');
  Minute = (now.minute()); //display current minute
  if (Minute < 10)
  {
    time.concat("0"); //insert leading zero if minute value is less than 10
    time.concat(Minute);
  }
  else time.concat(Minute); //display current minute
  time.concat(":");
  Second = (now.second());
  if (Second < 10)
  {
    time.concat("0"); //insert leading zero if seconds value is less than 10
    time.concat(Second); //display current seconds
  }
  else time.concat(Second); //display current seconds
  Serial.print(time);
  Serial.print(' ');
  date = "";
  date.concat(now.day());
  date.concat('-');
  date.concat(now.month());
  date.concat('-');
  date.concat(now.year());
  date.concat(' ');
  date.concat(now.dayOfWeek());
  Serial.println(date);
}
