#include <Ethernet.h>
#include <SPI.h>
#include <JsonParser.h>

bool reading      = false;
String response   = "";
byte mac[]        = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[]         = { 192, 168, 1, 170 };
byte server[]     = { 159, 253, 42, 74 }; // Google
char serverName[] = "api.alitayfur.net";
String queryString = "/x.php?";

EthernetClient client;

void addQueryString(String key,String value){
  queryString = queryString + key + "=" + value + "&";
}

void doAction(){  
      char json[response.length()+60]; 
      response.toCharArray(json,(response.length()+60));
      JsonParser<32> parser;      
      JsonHashTable hashTable = parser.parseHashTable(json);     
      if (!hashTable.success())
      {
          Serial.println("Failed (Data Not Read) [hashTable]");
          return;
      }

      Serial.println(hashTable.getString("job"));
      Serial.println("Finish");
        
  
}




void connectServer(){
    Serial.println("Connecting...");
    if (client.connect(server, 80)) {
      Serial.println("connected");
      client.println("GET "+String(queryString)+" HTTP/1.0");
      client.println("HOST: "+String(serverName));
      client.println();
    } else {
      Serial.println("Connection Failed");
    }
}

void listenServer(){
  if (client.available()) {
    char c = client.read();
    if (reading)          { response += String(c);}
    if (String(c) == "*") { reading = true; }
  }

  if (!client.connected()) {
    Serial.println("Disconnecting.");
    client.stop();
    doAction();
    for(;;)
      ;
  }
  
}






void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  addQueryString("name","ali");
  addQueryString("desc","developer");
  connectServer();
}

void loop()
{
  listenServer(); 
}

