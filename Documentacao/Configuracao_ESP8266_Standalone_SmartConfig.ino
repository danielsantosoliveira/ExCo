const int pinoVM = 14; // pino onde está ligado o led vermelho (D5)
const int pinoAM = 12; // pino onde está ligado o led amarelo  (D6)
const int pinoVD = 13; // pino onde está ligado o led verde    (D7)
 
#define BLYNK_PRINT Serial    // Manda debug para a porta Serial
 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
// Pegue o Auth Token no App Blynk.
// Em Project Settings (icone da "porca").
char auth[] = "aNHSTyaF0v8fzOhfw-gBdsWl1ZMGpU4";
 
// conecte o terminal serial virtual em Pin V1
WidgetTerminal terminal(V1);
 
 
void setup()
{
  // configura os pinos como saida
  pinMode(pinoVM, OUTPUT);
  pinMode(pinoAM, OUTPUT);
  pinMode(pinoVD, OUTPUT);
 
  // configura a porta Serial
  Serial.begin(9600);
 
  // configura a conexão do NodeMCU com
  // o App Smart Config
  WiFi.mode(WIFI_STA);
  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (cnt++ >= 10) {
      WiFi.beginSmartConfig();
      while (1) {
        delay(1000);
        if (WiFi.smartConfigDone()) {
          Serial.println();
          Serial.println("SmartConfig: Successo");
          break;
        }
        Serial.print("|");
      }
    }
  }
 
  WiFi.printDiag(Serial);
 
  Blynk.config(auth);
}
 
// adaptado do exemplo do blynk
// Esta função conecta o pino virtual aos dados
BLYNK_WRITE(V1) // estamos usando o pino V1
{
  // Manda dados de volta para o terminal no App
  terminal.print("Recebido: ");
  terminal.write(param.getBuffer(), param.getLength());
  terminal.println();
  // para garantir que tudo seja enviado:
  terminal.flush();
 
  // faz um eco do que foi recebido na Serial
  String recebido = param.asStr();
  Serial.print("\nRecebido: ");
  Serial.println(recebido);
 
  // transforma em minúsculas para facilitar a comparação
  recebido.toLowerCase();
 
  // compara recebido com comandos esperados
  if (recebido.equals("liga vm")) digitalWrite(pinoVM, HIGH);
  if (recebido.equals("desl vm")) digitalWrite(pinoVM, LOW);
 
  if (recebido.equals("liga am")) digitalWrite(pinoAM, HIGH);
  if (recebido.equals("desl am")) digitalWrite(pinoAM, LOW);
 
  if (recebido.equals("liga vd")) digitalWrite(pinoVD, HIGH);
  if (recebido.equals("desl vd")) digitalWrite(pinoVD, LOW);
}
 
void loop()
{
  // aqui é importante chamar a função abaixo com frequência
  // se o seu código for mais complexo, evite o uso de "delay"
  Blynk.run();
}
