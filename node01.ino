#include <UIPEthernet.h>

#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 21 };
EthernetServer server(80);

void setup()
{
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
  digitalWrite(5, LOW);
  digitalWrite(3, HIGH);
  dht.begin();
  Ethernet.begin(mac, ip);
  server.begin();
}
void loop()
{
  EthernetClient client = server.available();
  if (client) {
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && current_line_is_blank) {
          float h = dht.readHumidity();
          float t = dht.readTemperature();
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/xml");
          client.println();
          client.println("<?xml version=\"1.0\"?>");
          client.println("<root>");
          client.println("<node>");
          client.println("\t<name>Salon</name>");
          client.println("\t<sensor>");
          client.println("\t\t<name>Temperature</name>");
          client.print("\t\t<value>");
          client.print(t);
          client.println("</value>");
          client.println("\t\t<type>DHT22</type>");
          client.println("\t</sensor>");
          client.println("\t<sensor>");
          client.println("\t\t<name>Humidite</name>");
          client.print("\t\t<value>");
          client.print(h);
          client.println("</value>");
          client.println("\t\t<type>DHT22</type>");
          client.println("\t</sensor>");
          client.println("</node>");
          client.println("</root>");
          break;
        }
        if (c == '\n') {
          current_line_is_blank = true;
        } else if (c != '\r') {
          current_line_is_blank = false;
        }
      }
    }
    delay(5);
    client.stop();
  }
}
