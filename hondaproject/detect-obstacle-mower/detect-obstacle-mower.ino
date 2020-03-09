// Robot + censor de radiofrecuencia
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
const float umbral = 80.0; // distancia a dejar con los objetos en cm

// Variables de camino
int lado = 0; // 0=derecha 1=izquierda 
const int tGiro = 2000; // tiempo que tarda en girar 90º en milisegundos "Cambiar por el valor real"
boolean camino [] = {false,false,false,false}; // caminos posibles

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
  calibracion();
  parar();
  adelante(); //marcha hacia adelante constante hasta que encuentra objeto
}

void loop() {
  //iniciamos el sensor
  iniciarTrigger();
  // Obtenemos la distancia
  float distancia = calcularDistancia();
  if(distancia <= umbral){
    parar();
    esquivaObstaculo();
  }
}

// Método que sirve para calibrar y obtener los posbiles caminos por posibles objetos en el medio 
void calibracion(){
  for(int i = 0; i < 4; i++){
    derecha(); //giro de 90 grados
    parar();  
    // escaneo con sensor de radiofrecuencia
    iniciarTrigger();
    // Obtenemos la distancia
    float distancia = calcularDistancia();
    if(distancia > umbral){
      camino[i]=true; //camino posible  
    }
  }
}

// Método que inicia la secuencia del Trigger para comenzar a medir
void iniciarTrigger(){
  // Ponemos el Trigger en estado bajo y esperamos 2 ms
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
 
  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
 
  // Comenzamos poniendo el pin Trigger en estado bajo
  digitalWrite(TRIGGER, LOW);
}

// Método que calcula la distancia a la que se encuentra un objeto. Devuelve una variable tipo float que contiene la distancia
float calcularDistancia(){
  // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
  unsigned long tiempo = pulseIn(ECHO, HIGH);
 
  // Obtenemos la distancia en cm, hay que convertir el tiempo en segudos ya que está en microsegundos
  // por eso se multiplica por 0.000001
  float distancia = tiempo * 0.000001 * sonido / 2.0;
  return distancia ;
}

// Método que sirve para esquivar obstaculos
void esquivaObstaculo() {
  // escaneo con sensor de radiofrecuencia
  iniciarTrigger();
  // Obtenemos la distancia
  float distancia = calcularDistancia();
  int count = 0; //variable que usaremos para saltarnos la comprobación de hacia atras ya que volveria al mismo sitio si no encuentra salida tras una vuelta completa va para atras
  //Mientras se encuentre un obstaculo
  while(distancia <= umbral){
    if(lado==0){
      derecha();
    }else{
      izquierda();
    }
    parar();
    if(count != 1){ //si fuera uno significaria que el robot ha dado una vuelta de 180º
      iniciarTrigger();
      distancia = calcularDistancia();
    }
  }
  // cambio de lado
  if(lado==0){
    lado=1;
  } else{
    lado=0;
  }
  //continua la marcha por el camino libre
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
