// Arquitectura de Computadoras II
// Autor: Jonathan Rai Samines Solares
// Carnet: 6590-13-3813
// Alarma Casera
// Una alarma simple que detecta cuando un intruso (ladrón) ingreso sin autorización
// a una casa durante la noche por la ventana
// o sin encender la luz del pasillo de la casa cuando entra por la puerta

// Declaramos los pins de entrada que usaremos
const int pinPuerta = 2;
const int pinVentana = 3;
const int pinDetectorLuz = A0;

// Declaramos los pines de salida que usaremos
const int pinLuzAlarma = 5;
const int pinBocinaAlarma = 6;

// La alarma esta inicialmente apagada
bool estaAlarmaEncendida = false;

// Tanto la puerta como la ventana estan inicialmente cerradas
bool estaPuertaAbierta = false;
bool estaVentanaAbierta = false;

void setup() {
  // Se configura la puerta, la ventana y el indicador del día
  // como pines de entrada
  pinMode(pinPuerta, INPUT);
  pinMode(pinVentana, INPUT);
  pinMode(pinDetectorLuz, INPUT);

  // Se configura la luz y la bocina de la alarma como pines de salida
  pinMode(pinLuzAlarma, OUTPUT);
  pinMode(pinBocinaAlarma, OUTPUT);

  // Se inicializa el puerto serial
  Serial.begin(9600);
  Serial.println("Inicializando sistema de alarma");
}

bool determinarSiHayLuz() {
  // leemos el valor variable (analogo) del LDR
  int intensidadDeLuz = analogRead(pinDetectorLuz);

  // El rango de valores del LDR va de 0 a 512
  // Los valores de 0 al 200 consideran la luz como apagada (0)
  // Los valores de 201 en adelante la consideran como encendida (1)
  return intensidadDeLuz > 200;
}

void determinarSiDebeActivarseAlarma() {
  // Determinamos si es de día o de noche
  bool esDeNoche = !determinarSiHayLuz();

  // Determinamos si la luz de la casa esta encendida o apagada
  bool estaLuzApagada = !determinarSiHayLuz();

  // Determinamos si la puerta o la ventana fueron usadas
  bool ventanaFueUsada = (bool) digitalRead(pinVentana);
  bool puertaFueUsada = (bool) digitalRead(pinPuerta);

  // Cuando el botón es pulsado, entonces se considera que la puerta fue usada
  // Si estaba cerrada se abre, en el caso contrario se cierra
  if (puertaFueUsada) {
    estaPuertaAbierta = !estaPuertaAbierta;
  }

  // Cuando el botón es pulsado, entonces si considera que la ventana fue usada
  // Si estaba cerrada se abre, en el caso contrario se cierra
  if (ventanaFueUsada) {
    estaVentanaAbierta = !estaVentanaAbierta;
  }

  // Mostramos el estado actual de la alarma
  Serial.print(" - Estamos de : ");
  Serial.println(esDeNoche ? "Noche" : "Dia");

  Serial.print(" - La ventana esta: ");
  Serial.println(estaVentanaAbierta ? "Abierta" : "Cerrada");

  Serial.print(" - La puerta esta: ");
  Serial.println(estaPuertaAbierta ? "Abierta" : "Cerrada");

  // Si tanto la ventana como la puerta fueron abiertas simultaneamente entonces apagamos la alarma
  if (puertaFueUsada && ventanaFueUsada) {
    estaAlarmaEncendida = false;

    return;
  }
  

  // Si la puerta o la ventana fueron usadas, evaluamos
  // si la alarma debe de encenderse
  if (ventanaFueUsada || puertaFueUsada) {

    // Si la ventana fue abierta durante la noche o si la puerta fue abierta sin encender la luz
    // entonces consideramos que entro un ladrón y por tanto la alarma debe de encenderse
    if ((esDeNoche && estaVentanaAbierta) || (estaLuzApagada && estaPuertaAbierta)) {
      estaAlarmaEncendida = true;
    }
  }
}

void usarAlarma() {
  // Encendemos la luz y el sonido de la alarma
  digitalWrite(pinLuzAlarma, HIGH);
  tone(pinBocinaAlarma, 500);

  delay(20); // esperamos por 20ms

  // Apagamos la luz y el sonido del buzzer
  tone(pinBocinaAlarma, 0);
  digitalWrite(pinLuzAlarma, LOW);  
}

void loop() {
  // Determinamos si la alarma debe de encenderse o no
  determinarSiDebeActivarseAlarma();

  // Mostramos en pantalla si la alarma esta por encenderse o apagarse
  Serial.print("La alarma esta: ");
  Serial.println(estaAlarmaEncendida ? "Encendida" : "Apagada");

  // Esperamos 100ms para volver a verificar de nuevo el estado de la alarma
  delay(100);

  // Si se considera que la alarma debe de encenderse
  // entonces producimos un sonido tipo cirena intermitente con ella
  if (estaAlarmaEncendida) {
    usarAlarma();
    usarAlarma();
  }
}
