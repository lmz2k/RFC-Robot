//Incluindo 
#include <WiFiClient.h> 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define m1 16   // Roda 1 Frente
#define m2 5    // Roda 1 Trás
#define m3 4    // Roda 2 Frente
#define m4 0    // Roda 2 Trás

#define Arma 2  // Arma

//SSID e senha
const char *rede = "RonaldinhoOFICIAL"; // SSID
const char *senha = "ronaldinhoOFICIAL2018"; // Senha

//Porta do WebServer
WiFiServer server(5000); // Porta do Servidor

void setup() {
	delay(1000);
	Serial.println("\nConfigurando Ponto de acesso...");

   // IPAddress IP(192, 168, 2, 300);
   // IPAddress NMask(255, 255, 255, 0);
   // WiFi.softAPConfig(IP, IP, NMask);
    WiFi.softAP(rede, senha);
    Serial.printf("AP IP: [%s]\n", WiFi.softAPIP().toString().c_str());
    server.begin();
    Serial.println("Servidor iniciado.");
  
  pinMode(Arma, OUTPUT);
  digitalWrite(Arma, LOW);
}

void loop() {  
  WiFiClient client = server.available(); 
  if(client){ //Se o servidor estiver disponivel
     while(client.connected()){ //Enquanto o cliente estiver conectado ele fica ouvindo
         if(client.available()){ //se o cliente enviar algum dado
             String PWM1 = client.readStringUntil(';'); //Recebe uma palavra do cliente
             String PWM2 = client.readStringUntil(';'); //Recebe uma palavra do cliente
             String Atk = client.readStringUntil('\n'); //Recebe uma palavra do cliente   
             arma(Atk);
             rodas(PWM1, PWM2);
         }
      }
      client.stop(); //Cliente desconectado   
    }
}

 void arma(String status){

    if(status.equals("ligado")){
      digitalWrite(Arma, HIGH);
      Serial.println("Arma ligada");
    }else{
      digitalWrite(Arma, LOW);
    }
  
 }

 void rodas(String PWM1, String PWM2){
    int R1 = (PWM1.toInt())*10;
    int R2 = (PWM2.toInt())*10;
    
    if(R1 > 0){
      analogWrite(m1, R1);        //PWM de 10 bits, no pino do motor 1, com DutyCicle de valor = R1
      digitalWrite(m2, LOW);
      
    }else if(R1 < 0){
      unsigned int aux = R1;
      analogWrite(m2, aux);
      digitalWrite(m1, 0);
    }else{
      analogWrite(m1, 0);
      digitalWrite(m2, 0);
    }

    if(R2 > 0){
      analogWrite(m3, R2);
      digitalWrite(m4, 0);
    }else if(R2 < 0){
      unsigned int aux = R2;
      analogWrite(m4, aux);
      digitalWrite(m3, 0);
    }else{
      analogWrite(m3, 0);
      digitalWrite(m4, 0);
    }
}
