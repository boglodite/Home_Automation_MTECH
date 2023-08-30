#define BLYNK_TEMPLATE_ID "TMPL3LWCFc0Qr"
#define BLYNK_TEMPLATE_NAME "dht11"
#define BLYNK_AUTH_TOKEN "RD0ORTTV9yi0d7ssjmZntsCmMzv5GR0j"

#define BLYNK_PRINT Serial



#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#define DHTPIN 27 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Robin";
char pass[] = "12345670";
int Val=0;
float t=0;
float h=0;

BlynkTimer timer;


#define button1_pin 26
#define button2_pin 25


#define relay1_pin 19
#define relay2_pin 12



int relay1_state = 0;
int relay2_state = 0;



//Change the virtual pins according the rooms
#define button1_vpin    V2
#define button2_vpin    V3


//------------------------------------------------------------------------------
// This function is called every time the device is connected to the Blynk.Cloud
// Request the latest state from the server
BLYNK_CONNECTED() {
  Blynk.syncVirtual(button1_vpin);
  Blynk.syncVirtual(button2_vpin);
}


//--------------------------------------------------------------------------
// This function is called every time the Virtual Pin state change
//i.e when web push switch from Blynk App or Web Dashboard
BLYNK_WRITE(button1_vpin) {
  relay1_state = param.asInt();
  digitalWrite(relay1_pin, relay1_state);
}
BLYNK_WRITE(button2_vpin) {
  relay2_state = param.asInt();
  digitalWrite(relay2_pin, relay2_state);
}
BLYNK_WRITE(V4)
{
  Val = param.asInt(); // assigning incoming value from pin V4 to a variable
  Serial.print(" The Threshhold value is: ");
  Serial.println(Val);
  Serial.println();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------


void setup()
{
  // Debug console
  Serial.begin(115200);
  //--------------------------------------------------------------------
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  
  //--------------------------------------------------------------------
  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);
  
  //--------------------------------------------------------------------
  //During Starting all Relays should TURN OFF
  digitalWrite(relay1_pin, HIGH);
  digitalWrite(relay2_pin, HIGH);
  
  //--------------------------------------------------------------------
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  //--------------------------------------------------------------------
  //Blynk.virtualWrite(button1_vpin, relay1_state);
  //Blynk.virtualWrite(button2_vpin, relay2_state);
  //Blynk.virtualWrite(button3_vpin, relay3_state);
  //Blynk.virtualWrite(button4_vpin, relay4_state);
  //--------------------------------------------------------------------
  dht.begin();
  timer.setInterval(100L, sendSensor);
}
void sendSensor()
{
  h = dht.readHumidity();
  t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
}

void loop()
{
  Blynk.run();
  timer.run();
if (Val > t)
  {
    digitalWrite(relay2_pin, HIGH);

  }
  else {
    digitalWrite(relay2_pin, LOW);
 
    
  } 
  
  listen_push_buttons();
  
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void listen_push_buttons(){
    //--------------------------------------------------------------------------
    if(digitalRead(button1_pin) == LOW){
      delay(200);
      control_relay(1);
      Blynk.virtualWrite(button1_vpin, relay1_state); //update button state
    }
   
}

void control_relay(int relay){
  //------------------------------------------------
  if(relay == 1){
    relay1_state = !relay1_state;
    digitalWrite(relay1_pin, relay1_state);
    Serial.print("Relay1 State = ");
    Serial.println(relay1_state);
    delay(50);
  }
  
}







