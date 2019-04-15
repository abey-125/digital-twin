#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "Nokia1";       // WiFi SSID
String myPWD = "srydontknow"; // WiFi Password
String myAPI = "TRW15VI1EBOTMDIB";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
//String myFIELD = "field1"; 
int sendVal;
float temp;
int ir1;
int ir2;
int ir3;
int ir4;
int ir_res;
int ir_res1;
int set;
int set2;


void setup()
{
  Serial.begin(9600);
  espSerial.begin(115200);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  /*while(!esp.find("OK")) 
  {          
      //Wait for connection
  }*/
  delay(1000);
  
}

  void loop()
  {
    /* Here, I'm using the function random(range) to send a random value to the 
     ThingSpeak API. You can change this value to any sensor data
     so that the API will show the sensor data  
    */
    ir1= analogRead(A0);
    ir2= analogRead(A1);
    ir3= analogRead(A3);
    ir4= analogRead(A4);
    temp= analogRead(A2);
    temp = temp* 0.48828125;
    ir_res= (ir1+ir2)/2;
    ir_res1=(ir3+ir4)/2;
    
    if(ir_res<250)
    {
      set=1;
    }
    else
    {
      set=0;
    }

    if(ir_res1<250)
    {
      set2=1;
    }
    else
    {
      set2=0;
    }
    
    //sendVal = random(1000); // Send a random number between 1 and 1000
    String sendData = "GET /update?api_key="+ myAPI +"&field1="+String(set)+"&field2="+String(temp)+"&field3="+String(set2);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
}
