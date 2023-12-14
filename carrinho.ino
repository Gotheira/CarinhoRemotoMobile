#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "iPhone Matos"; //Configure o nome da rede
const char* password = "vinimatos3"; //Configure a senha da rede acima
const char* mqtt_server = "broker.hivemq.com"; //Url do Broker público ou pago


WiFiClient espClient; //Instância do espClient WiFi
PubSubClient client(espClient); //Instância do client MQTT
unsigned long lastMsg=0; //Variável de armazenamento do envio das últimas mensagens
#define MSG_BUFFER_SIZE 50 //Definição do tamanho do array para envio de mensagens
char msg[MSG_BUFFER_SIZE]; //Definição do array para envio de mensagens


//** Declaração dos GPIOS do ESP 32 e variáveis auxiliares
int pinM1A = 33, pinM1B = 32, pinM2B = 25, pinM2A = 26, aux=0, pinLed = 14, pinLed2 = 27;

//** Declaração de flags
bool flagD = 0, flagU = 0, flagLed=0, flagL = 0, flagR=0;


//** Função de onfiguração e inicialização do WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi conectado");
  Serial.println("Endereço IP");
  Serial.println(WiFi.localIP());
}

//** Função de reconexão do cliente MQTT

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "CONTROLE";
    clientId+=String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())){
      Serial.println("Conectado");
      client.subscribe("controle/publisher");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//** Função de recebimento de dados - Topico - vetor de dados - tamanho

void callback(char*topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String data = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println();
  Serial.print("Message size :");
  Serial.println(length);
  Serial.println();
  Serial.println("-----------------------");
  Serial.println(data);

if(data == "D"){
    flagD = 1;
  }
  if(data == "d"){
    analogWrite(pinM1A, 0);
    analogWrite(pinM1B, 0);
  }
  if(data == "T"){
    analogWrite(pinM2A, 0);
    analogWrite(pinM2B, 0);
  }
  if(data == "U"){
    flagU=1;
  }

if(data=="L"){
  analogWrite(pinM2B, 200);
  analogWrite(pinM2A, 0);
}

if(data=="R"){
  analogWrite(pinM2A, 200);
  analogWrite(pinM2B, 0); 
}

if(data=="P"){
      if(aux < 5) aux++;
    String minhaString = String(aux);
    snprintf(msg, MSG_BUFFER_SIZE, minhaString.c_str());
    client.publish("controle/velo", msg);
    Serial.println(aux);
}

if(data=="M"){
    if(aux > 0) aux--;
    String minhaString = String(aux);
    snprintf(msg, MSG_BUFFER_SIZE, minhaString.c_str());
    client.publish("controle/velo", msg);
    Serial.println(aux);
}

if(data=="A"){
  if(flagLed == 0){
    digitalWrite(pinLed, HIGH);
    digitalWrite(pinLed2, HIGH);
    Serial.println("ON");
    String minhaString = String(flagLed);
    snprintf(msg, MSG_BUFFER_SIZE, minhaString.c_str());
    client.publish("controle/led", msg);
    flagLed = 1;
  }
  else if(flagLed == 1){
    digitalWrite(pinLed, LOW);
    digitalWrite(pinLed2, LOW);
    Serial.println("OFF");
    String minhaString = String(flagLed);
    snprintf(msg, MSG_BUFFER_SIZE, minhaString.c_str());
    client.publish("controle/led", msg);
    flagLed = 0;
  }
}

}

//Função de inicialização do ESP32, incialização dos pinos, chamada das funções de inicialização do WiFi, inicialização do MQTT
void setup() {

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
	
	pinMode(pinM1A, OUTPUT);
  pinMode(pinM1B, OUTPUT);
  pinMode(pinM2A, OUTPUT);
  pinMode(pinM2B, OUTPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinLed2, OUTPUT);

  digitalWrite(pinLed, HIGH);
  digitalWrite(pinLed2, HIGH);
	
	analogWrite(pinM1A, 0);
  analogWrite(pinM1B, 0);
  analogWrite(pinM2A, 0);
  analogWrite(pinM2B, 0);
}

//** Função main loop, define a velocidade dos motores de acordo com a velocidade escolhida no App Inventor, função de reconexão do cliente MQTT

void loop() {

  if(flagD == 1 && flagU == 0){
    switch (aux){
    case 0:
      analogWrite(pinM1A, 0);
      analogWrite(pinM1B, 0);
      break;
    case 1:
      analogWrite(pinM1A, 190);
      analogWrite(pinM1B, 0);
      break;
    case 2:
      analogWrite(pinM1A, 206);
      analogWrite(pinM1B, 0);
      break;
    case 3:
      analogWrite(pinM1A, 222);
      analogWrite(pinM1B, 0);
      break;
    case 4:
      analogWrite(pinM1A, 238);
      analogWrite(pinM1B, 0);
      break;
    case 5:
      analogWrite(pinM1A, 255);
      analogWrite(pinM1B, 0);
      break;
  }
    flagD=0;
  }
  if(flagU == 1 && flagD == 0){
    switch (aux){
    case 0:
      analogWrite(pinM1B, 0);
      analogWrite(pinM1A, 0);
      break;
    case 1:
      analogWrite(pinM1B, 190);
      analogWrite(pinM1A, 0);
      break;
    case 2:
      analogWrite(pinM1B, 206);
      analogWrite(pinM1A, 0);
      break;
    case 3:
      analogWrite(pinM1B, 222);
      analogWrite(pinM1A, 0);
      break;
    case 4:
      analogWrite(pinM1B, 238);
      analogWrite(pinM1A, 0);
      break;
    case 5:
      analogWrite(pinM1B, 255);
      analogWrite(pinM1A, 0);
      break;
  }
    flagU=0;
  }

  if (!client.connected()) { //if client is not connected
    reconnect(); //try to reconnect
    }
  client.loop();

	delay(300);
}
