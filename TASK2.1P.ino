#include "DHT.h"
#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"


#define DHTPIN 2          
#define DHTTYPE DHT22     


DHT dht(DHTPIN, DHTTYPE);


char ssid[] = SECRET_SSID;   
char pass[] = SECRET_PASS;    
WiFiClient client;


unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(115200);  
  dht.begin();  
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
   
  ThingSpeak.begin(client);
}

void loop()
{

  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    int retryCount = 0;
    while(WiFi.status() != WL_CONNECTED && retryCount < 10)
    {
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);     
      retryCount++;
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nFailed to connect to WiFi.");
      return; 
    }
    Serial.println("\nConnected.");
  }
 
  delay(1000);

  float humidity = dht.readHumidity();
  float tempinC = dht.readTemperature();
  

  if (isnan(humidity) || isnan(tempinC)) {
    Serial.println("Failed to take readings check again!");
    return;
  }


  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(tempinC);
  Serial.print("°C ");
 

  ThingSpeak.setField(1, tempinC);
  ThingSpeak.setField(2, humidity);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

 
  delay(60000);//for next update
}