#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

//Written by Chris Konstad 7/26/12
//Released into the public domain

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
IPAddress broadcast(255,255,255,255);  //broadcast across all IP's

EthernetUDP Udp;

unsigned int localPort = 50000;      // local port to listen on
unsigned int topsidePort = 51000;  //remote port to send packet to

char packetBuffer[100];  //100 bytes to hold the incoming packet
char replyBuffer[100];  //100 bytes to hold the packet to send
char tempBuffer[50];

int rxDigital;
int rxAnalog;
int txDigital;
int txAnalog;

int pinDOut = 7;
int pinDIn = 8;
int pinAnalogOut = 9;
int pinAnalogIn = A2;

void setup()
{
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  
  pinMode(pinDOut, OUTPUT);
  pinMode(pinDIn, INPUT);
  pinMode(pinAnalogOut, OUTPUT);
}

void loop()
{
  int packetSize = Udp.parsePacket();  //see if there is data available
  if(packetSize)  //if there is a packet
  {
    memset(packetBuffer, 0, sizeof(packetBuffer));  //clear buffer
    memset(replyBuffer, 0, sizeof(replyBuffer));  //clear buffer
    Udp.read(packetBuffer, packetSize);  //read the packet into the buffer
    sscanf(packetBuffer, "%d %d", &rxDigital, &rxAnalog);
    if(rxDigital == 1)  //output digital value
      digitalWrite(pinDOut, HIGH);
    else
      digitalWrite(pinDOut, LOW);
    analogWrite(pinAnalogOut, rxAnalog);  //output analog value
    
    sendReplyPacket();
  }
}

void sendReplyPacket()
{
  //Read digital pin
  if(digitalRead(pinDIn) == HIGH)
    txDigital = 1;
  else
    txDigital = 0;
    
  //Read the analog pin and convert to a different range
  txAnalog = map(analogRead(pinAnalogIn), 0, 1023, 0, 255);
  
  //Convert the numbers to strings
  itoa(txDigital, tempBuffer, 10);  //convert INT to STRING
  strcat(replyBuffer, tempBuffer);  //add temp string to replyBuffer string
  strcat(replyBuffer, " ");  //add white-space deliminater
  
  itoa(txAnalog, tempBuffer, 10);
  strcat(replyBuffer, tempBuffer);
  strcat(replyBuffer, " ");
  
  Udp.beginPacket(broadcast, topsidePort);  //set the IP and Port of recipient
  Udp.write(replyBuffer);  //set the information to send
  Udp.endPacket();  //send the information
}
