#include "lwip/tcp_impl.h"

void tcpCleanup()
{
  while (tcp_tw_pcbs != NULL)
  {
    tcp_abort(tcp_tw_pcbs);
  }
}

/*Robot 2018 WiFi UnP*/
#include <ESP8266WiFi.h>

/* Set these to your desired credentials. */
const char *ssid = "ROBOTIC";
const char *password = "eliandro";

WiFiServer server(80);
WiFiClient client;

/*Configuração de pinos de saídas*/
const int motorDiretoEsquerdo = 4; //vermelho  D2
const int motorInversoEsquerdo = 5; //verde     D1
const int motorDiretoDireito = 13; //verde    D7
const int motorInversoDireito = 15; //vermelho D8
const int PinoArma = 14;
int velocidade = 0;
int dir;
/*int lado1 = 5; //saida PWM para optoacoplador 1 (trás) - modifique aqui
int lado2 = 4; //saida PWM para optoacoplador 2 (Frente) - modifique aqui
int arma = 13; //saida digital - enviar comando para arma - modifique aqui*/


 void conectado(){
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("conexao Ok.");
}
void frente(){
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("comando para frente...");
          digitalWrite(motorInversoEsquerdo, LOW);
          digitalWrite(motorInversoDireito, LOW);
          delay(50);
          digitalWrite(motorDiretoEsquerdo, HIGH);
          digitalWrite(motorDiretoDireito, HIGH);
}
void tras(){
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("comando para tras...");
          digitalWrite(motorDiretoEsquerdo, LOW);
          digitalWrite(motorDiretoDireito, LOW);
          delay(50);
          digitalWrite(motorInversoEsquerdo, HIGH);
          digitalWrite(motorInversoDireito, HIGH);
}
void direita(){
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("comando para direita...");
          digitalWrite(motorInversoDireito, LOW);
          digitalWrite(motorDiretoDireito, LOW);
          delay(50);
          digitalWrite(motorDiretoEsquerdo, HIGH);
          digitalWrite(motorInversoEsquerdo, LOW);
}
void esquerda(){
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("comando para esquerda...");
          digitalWrite(motorDiretoEsquerdo, LOW);
          digitalWrite(motorInversoEsquerdo, LOW);
          delay(50);
          digitalWrite(motorInversoDireito, LOW);
          digitalWrite(motorDiretoDireito, HIGH);
}
void parar(){
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("comando para parar...");
          digitalWrite(motorDiretoEsquerdo, LOW);
          digitalWrite(motorInversoEsquerdo, LOW);
          delay(50);
          digitalWrite(motorInversoDireito, LOW);
          digitalWrite(motorDiretoDireito, LOW);
          delay(50);
}
void ligaarma(){
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("comando para arma...");
  //escrever aqui saída
  digitalWrite(PinoArma, HIGH);
}
void desligaarma(){
  client.print("HTTP://1.1 200 OK\n\n");
  Serial.println("comando para desliga arma...");
  //escrever aqui saída
  digitalWrite(PinoArma, LOW);
}

void velocidade0(){
  client.print("HTTP://1.1 200 OK\n\n");
  velocidade = 0;
  if(dir==1){
    //frente();
    }else if(dir==0){
    //tras();
  }
}
void velocidade170(){
  client.print("HTTP://1.1 200 OK\n\n");
  velocidade = 410;
  if(dir==1){
    //frente();
    }else if(dir==0){
    //tras();
  }
}
void velocidade200(){
  client.print("HTTP://1.1 200 OK\n\n");
  velocidade = 760;
  if(dir==1){
    //frente();
    }else if(dir==0){
    //tras();
  }
}
void velocidade255(){
  client.print("HTTP://1.1 200 OK\n\n");
  velocidade = 1024;
  if(dir==1){
    frente();
    }else if(dir==0){
    tras();
  }
}


void setup() {
   /*Inicializando as portas*/
  pinMode(motorInversoDireito, OUTPUT);
  pinMode(motorInversoEsquerdo, OUTPUT);
  pinMode(motorDiretoDireito, OUTPUT);
  pinMode(motorDiretoEsquerdo, OUTPUT);
  pinMode(PinoArma, OUTPUT);
  delay(10);

  /*Inicializar os dois optos com valor Zero*/
  analogWrite(motorDiretoEsquerdo, LOW);
  analogWrite(motorDiretoDireito, LOW);
  analogWrite(motorInversoEsquerdo, LOW);
  analogWrite(motorInversoDireito, LOW);
  digitalWrite(PinoArma,LOW);
  delay(50);

  velocidade=0;
  dir=1;

  
	Serial.begin(115200);
  /*Configurando IP Státicos*/
  IPAddress staticIP(192, 168, 4, 2); // IP set to Static
  IPAddress gateway(192, 168, 4, 1);// gateway set to Static
  IPAddress subnet(255, 255, 255, 0);// subnet set to Static

  WiFi.mode(WIFI_AP);// Working mode only as Acess Point 

  WiFi.softAP(ssid, password, 11, 0);//rede, senha, canal, visivel
  WiFi.config(staticIP, gateway, subnet);

  server.begin();
  Serial.println();
	Serial.print("AP com IP: ");
	Serial.println(WiFi.softAPIP());
	Serial.println("Servidor Iniciado!\n");

}

void loop() {
  tcpCleanup();
  client = server.available();

  if(!client){
    return;
  }

  while(!client.available()){
    delay(1);
  }
  String msg = client.readStringUntil('\r');
  msg = msg.substring(msg.indexOf("/")+1, msg.indexOf("HTTP")-1);
  //char req = client.read();
  char *req = const_cast<char*>(msg.c_str());
  Serial.println(req);
  client.print("HTTP://1.1 200 OK\n\n");
  client.flush();


  switch(*req){
    case '0': velocidade0();
              break;
    case '1': velocidade170();
              break;
    case '2': velocidade200();
              break;              
    case '3': velocidade255();
              break;              
    case 'u':  dir=1;
                frente();
                break;              
    case 'b':dir=0;
                tras(); 
                break;
    case 'e': esquerda();
              break;
    case 'r': direita();
              break;
    case 'w': ligaarma();
              break;
    case 'd': desligaarma();
              break;
    case 'c': conectado();
              break;
    case 'p': parar();
              break;    
              
  }

  
  client.print("HTTP://1.1 200 OK\n\n");
  client.flush();
    
  tcpCleanup();
}
