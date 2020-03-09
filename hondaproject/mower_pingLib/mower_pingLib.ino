#include <Pixy2.h> //Descargar y añadir libreria para poder usar (.zip)
#include<NewPing.h> //Descargar y añadir libreria para poder usar (.zip)
// Pines utilizados 
#define TRIGGER 5
#define ECHO 4
#define DERECHAP 13
#define DERECHAN 12
#define VEL1 A0
#define IZQUIERDAP 11
#define IZQUIERDAN 10
#define VEL2 A1

// Variables para la medición del ultrasonico
const float sonido = 34300.0; // Velocidad del sonido en cm/s
const int umbral = 80; // distancia a dejar con los objetos en cm

// Variables de camino
int lado = 0; // 0=derecha 1=izquierda 
const int tGiro = 2000; // tiempo que tarda en girar 90º en milisegundos "Cambiar por el valor real"
boolean camino [] = {false,false,false,false}; // caminos posibles
boolean apagado = false;
Pixy2 pixy; //Obejeto de la clase Pixy2
NewPing sonar(TRIGGER, ECHO, umbral); 

void setup() {
  // Iniciamos el monitor serie
  Serial.begin(9600);
  // Modo entrada/salida de los pines
  pinMode(DERECHAP, OUTPUT);
  pinMode(DERECHAN, OUTPUT);
  pinMode(IZQUIERDAP, OUTPUT);
  pinMode(IZQUIERDAN, OUTPUT);
  pinMode(VEL1,OUTPUT);
  pinMode(VEL2,OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pixy.init();
  pixy.ccc.getBlocks();
  calibracion();
}

void loop() {
  //iniciamos el sensor
  while(!apagado){
    int distancia = sonar.ping_cm();
    if(distancia <= umbral){
      parar();
      esquivaObstaculo();
    }
    pixy.ccc.getBlocks();
    if(!pixy.ccc.numBlocks){
      parar();
      cambiarRecorrido();
      if(lado==1){
        lado=0;
      } else {
        lado=1;
      }
    }
  }
}

// Método que sirve para calibrar y obtener los posbiles caminos por posibles objetos en el medio 
void calibracion(){
  for(int i = 0; i < 4; i++){
    derecha(); //giro de 90 grados
    parar();  

    // Obtenemos la distancia
    int distancia = sonar.ping_cm();
    pixy.ccc.getBlocks();
    //comprobación si hay distancia suficiente y cesped entonces es un camino posible
    if(distancia > umbral && pixy.ccc.numBlocks && pixy.ccc.blocks[0].m_signature==1){
      camino[i]=true; //camino posible  
    }
  }
  parar(); //simplemente para dar un tiempo apagado
  adelante(); //adelante empieza el camino constante hasta que encuentre algun objeto
}


// Método que sirve para esquivar obstaculos basico hace el patron [ donde el objeto estari a la zona de en medio 
void esquivaObstaculo() {
// ponemos el robot mirando a el lado por el que va a esquivar
  if(lado == 0) {
    derecha();
  } else {
    izquierda();
  }
  parar();
  adelante();
  delay(500); // avanza un cuadrante
  parar();
  // ponemos el robot mirando al frente pero por el lateral del objeto
  if(lado == 1) {
    derecha();
  } else {
    izquierda();
  }
  parar();
  adelante();
  delay(1000); //avanza dos cuadrantes para esquivar el obstaculo y ponerse por detras
 // ponemos nuevamente al robot mirando al sitio donde debe colocarse 
  if(lado == 1) {
    derecha();
  } else {
    izquierda();
  }
  parar();
  adelante();
  delay(500); //avanza dos cuadrantes para volver a su linea 
  //finalmente lo rotamos y lo dejamos en su sitio avanzando de forma normal
  parar();
  if(lado == 0) {
    derecha();
  } else {
    izquierda();
  }
  parar();
  avanzar();
}

// Encargado de propocionar un camino cuando este encuentra un obstaculo-salida de camino
void setCamino(){
  //giro hacia el lado que le toca en el camino
  if(lado==1){
    izquierda();
  } else {
    derecha();
  }
  parar();
  // inicio de grabación 
  pixy.ccc.getBlocks();
  //comprobamos si no se encuentra cesped al girar (esto significa que el recorrido a terminado)
  if(!pixy.ccc.numBlocks){ 
    apagado=true;
    return;
  }
  //comprobación de objeto en el lado al que se quiere girar
  int distancia = sonar.ping_cm();
  while(distancia <= umbralVivo){
    //giro a la posicion inicial
    if(lado==1){
      derecha();
    } else {
      izquierda();
    }
    parar();
    atras();
    parar();
  }
  adelante();
  delay(500); // movimiento de un cuadrante hacia adelante
  //colocamos el robot recto
  if(lado==1){
    izquierda();
  } else {
    derecha();
  }
  parar();
  adelante();
}
// METODOS MOVIMIENTO 
void adelante(){
  digitalWrite(DERECHAP, HIGH);
  digitalWrite(DERECHAN, LOW);
  digitalWrite(IZQUIERDAP, LOW);
  digitalWrite(IZQUIERDAN, HIGH);
  analogWrite(VEL1,255);
  analogWrite(VEL2,255);
}
void atras(){
  digitalWrite(DERECHAP, LOW);
  digitalWrite(DERECHAN, HIGH);
  digitalWrite(IZQUIERDAP, HIGH);
  digitalWrite(IZQUIERDAN, LOW);
  analogWrite(VEL1,255);
  analogWrite(VEL2,255);
  delay(500); //cambiar por si es necesario esto es volver un paso atras
}
void derecha(){
  digitalWrite(DERECHAP, LOW);
  digitalWrite(DERECHAN, HIGH);
  digitalWrite(IZQUIERDAP, LOW);
  digitalWrite(IZQUIERDAN, HIGH);
  analogWrite(VEL1,255);
  analogWrite(VEL2,255);
  delay(tGiro);
}
void izquierda(){
 digitalWrite(DERECHAP, HIGH);
 digitalWrite(DERECHAN, LOW);
 digitalWrite(IZQUIERDAP, HIGH);
 digitalWrite(IZQUIERDAN, LOW);
 analogWrite(VEL1,255);
 analogWrite(VEL2,255);
 delay(tGiro);
}
void parar(){
 digitalWrite(DERECHAP, LOW);
 digitalWrite(IZQUIERDAP, LOW); 
 analogWrite(VEL1,0);
 analogWrite(VEL2,0);
 delay(1000);
}
