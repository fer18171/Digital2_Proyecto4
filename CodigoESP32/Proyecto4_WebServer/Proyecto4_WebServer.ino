/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "Nexxt_456560";  // Enter your SSID here
const char* password = "WadUFE3n";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)


uint8_t suma;
unsigned char Disp;
String disponible = "\t  <td  style=\"background-color: #5BB564;\"> <h2>Disponible &#128513</h2> </td>\t\n";
String ocupado = "\t  <td  style=\"background-color: #E64545;\"><h2>Ocupado &#128546</h2></td>\t\n";

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  while (! Serial2);

  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  pinMode(LED1pin, OUTPUT);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  //Primero se lee el dato recibido del UART2 que viene de la tiva c
  if (Serial2.available() > 0) { //Solo entra si hay datos en el buffer serial
    Disp = Serial2.read(); 
  }
  suma = 0;

  //Codigo para sumar la cantidad de parqueos disponibles
  if (1 & Disp) {  //Si el bit 0 de Disp esta encendido, se suma 1
    suma += 1;
  }
  if (2 & Disp) {  //Si el bit 1 de Disp esta encendido, se suma 1
    suma += 1;
  }
  if (4 & Disp) {  //Si el bit 2 de Disp esta encendido, se suma 1
    suma += 1;
  }
  if (8 & Disp) {  //Si el bit 3 de Disp esta encendido, se suma 1
    suma += 1;
  }
  
  server.handleClient();
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
//Cada vez que se refresque la pagina, se enviara de nuevo el codigo del html
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML2());
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML2() {
  String code = "<!DOCTYPE html> \n";
  code += "\t<html>\n";
  code += "  <head><meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0, user-scalable=no\\\">\n";
  code += "  <title>Parqueomatic</title>\n";
  code += "  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  code += "  body{margin-top: 50px;} h1 {color: #141C87;margin: 50px auto 30px;} h3 {color: #141C87;margin-bottom: 50px;}\n";
  code += "  p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  code += "  div {\n";
  code += "  width: 180px;\n";
  code += "  padding: 30px;\n";
  code += "  font-family: sans-serif;\n";
  code += "  background-color: gold;\n";
  code += "    }\n";
  code += "  \n";
  code += "table {\n";
  code += "  width:70%;\n";
  code += "  \n";
  code += "}\n";
  code += "table, th, td {\n";
  code += "  border: 3px solid black;\n";
  code += "  border-collapse: collapse;\n";
  code += "}\n";
  code += "th, td {\n";
  code += "  padding: 5px;\n";
  code += "  text-align: center;\n";
  code += "}\n";
  code += "#t01 td:nth-child(odd) {\n";
  code += "  background-color: #FDF580;\n";
  code += "}\n";
  code += "#t01 th {\n";
  code += "  background-color: black;\n";
  code += "  color: white;\n";
  code += "}\n";
  code += "  </style>\n";

//Codigo para que la pagina se refresque cada 0.5 segundos
code += "<script>\n";
code += "<!--\n";
code += "function timedRefresh(timeoutPeriod) {\n";
code += "\tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n";
code += "}\n";
code += "\n";
code += "window.onload = timedRefresh(500);\n";
code += "\n";
code += "//   -->\n";
code += "</script>";

  //Codigo del visualizador principal de la pagina web
  code += "  </head>\n";
  code += "  <body>\n";
  code += "  <h1>Parqueo-matic 3000 &#128664</h1>\n";
  code += "  <h3>Santiago Fernandez 18171</h3>\n";
  code += "  <table id=\"t01\" align=\"center\">\n";
  code += "\t<tr>\n";

//Aqui se muestra disponibilidad del parqueo 1
  code += "\t  <td> <h2> Parqueo 1 </h2></td>\n";
  if ((Disp & 1) == 1) { //Verifica que el bit 0 de Disp este encendido
    code += disponible;
  }
  else {
    code += ocupado;
  }
  code += "\t</tr>\n";
  code += "\t<tr>\n";

//Aqui se muestra disponibilidad del parqueo 2
  code += "\t  <td><h2> Parqueo 2 </h2></td>\n";
  if ((Disp & 2) == 2) { //Verifica que el bit 1 de Disp este encendido
    code += disponible;
  }
  else {
    code += ocupado;
  }

  code += "\t</tr>\n";
  code += "\t<tr>\n";

//Aqui se muestra disponibilidad del parqueo 3
  code += "\t  <td><h2> Parqueo 3 </h2></td>\n";
  if ((Disp & 4) == 4) { //Verifica que el bit 2 de Disp este encendido
    code += disponible;
  }
  else {
    code += ocupado;
  }
  code += "\t</tr>\n";
  code += "\t<tr>\n";

//Aqui se muestra disponibilidad del parqueo 4
  code += "\t  <td><h2> Parqueo 4 </h2></td>\n";
  if ((Disp & 8) == 8) { //Verifica que el bit 3 de Disp este encendido
    code += disponible;
  }
  else {
    code += ocupado;
  }
  code += "\t</tr>\n";
  code += "  </table>\n";
  code += "  <h2>Parqueos disponibles:</h2>\n";
  //Para evaluar los parqueos disponibles, se despliega en letra grande el valor de suma convertido en string
  code += "  <font size=7><span style=\"border: 3px solid black\">" + String(suma) + "</span></font>\n";
  code += "\t</body>\n";
  code += "    </html>";
  return code;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
