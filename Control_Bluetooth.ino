// Arquitectura de Computadoras II
// Ing. Mynor Fuentes
// Autor: Jonathan Rai Samines Solares - 6590-13-3813
// -----------------------------------------------------------
// Control de LEDs direcciones usando LEDs y un bluetooth
// Utilizando un control bluetooth controlamos el encendido
// de un conjunto de LEDS.
// U = Arriba   A = Abajo              R = Derecha  l = Izquierda     C = Centro
// P = Parpadeo T = Sentido del reloj  U = Sentido contrario al reloj O = Apagado


// Pines del programa
const int pinLedUp = 6;
const int pinLedDown = 2;
const int pinLedRight = 3;
const int pinLedLeft = 4;
const int pinLedCenter = 5;

// Constantes del programa
const int comandoUp = 85; // Letra U
const int comandoDown = 68; // Letra A
const int comandoLeft = 76; // Letra L
const int comandoRight = 82; // Letra R
const int comandoCenter = 67; // Letra C
const int comandoParpadeo = 80; // Letra P
const int comandoSentidoReloj = 84; // Letra T
const int comandoSentidoContraReloj = 81; // Letra U
const int comandoApagado = 79; // Letra O

// Variables del programa
int comandoBluetooth;

void setup() {
  // Declaramos los pines de entrada
  pinMode(pinLedUp, OUTPUT);
  pinMode(pinLedDown, OUTPUT);
  pinMode(pinLedRight, OUTPUT);
  pinMode(pinLedLeft, OUTPUT);
  pinMode(pinLedCenter, OUTPUT);

  // Inicializamos el puerto serial
  Serial.begin(9600);
}

void encenderLedUp(bool apagarLedCentral) {
  digitalWrite(pinLedUp, HIGH);
  digitalWrite(pinLedDown, LOW);
  digitalWrite(pinLedRight, LOW);
  digitalWrite(pinLedLeft, LOW);

  // Apagamos el led central solo cuando es necesario
  if (apagarLedCentral) digitalWrite(pinLedCenter, LOW);
}

void encenderLedDown(bool apagarLedCentral) {
  digitalWrite(pinLedDown, HIGH);
  digitalWrite(pinLedUp, LOW);
  digitalWrite(pinLedRight, LOW);
  digitalWrite(pinLedLeft, LOW);

  // Apagamos el led central solo cuando es necesario
  if (apagarLedCentral) digitalWrite(pinLedCenter, LOW);
}

void encenderLedLeft(bool apagarLedCentral) {
  digitalWrite(pinLedLeft, HIGH);
  digitalWrite(pinLedUp, LOW);
  digitalWrite(pinLedDown, LOW);
  digitalWrite(pinLedRight, LOW);

  // Apagamos el led central solo cuando es necesario
  if (apagarLedCentral) digitalWrite(pinLedCenter, LOW);
}

void encenderLedRight(bool apagarLedCentral) {
  digitalWrite(pinLedRight, HIGH);
  digitalWrite(pinLedUp, LOW);
  digitalWrite(pinLedDown, LOW);
  digitalWrite(pinLedLeft, LOW);

  // Apagamos el led central solo cuando es necesario
  if (apagarLedCentral) digitalWrite(pinLedCenter, LOW);
}

void encenderLedCenter() {
  digitalWrite(pinLedCenter, HIGH);
  digitalWrite(pinLedUp, LOW);
  digitalWrite(pinLedDown, LOW);
  digitalWrite(pinLedRight, LOW);
  digitalWrite(pinLedLeft, LOW);
}

void apagarTodosLosLed() {
  digitalWrite(pinLedUp, LOW);
  digitalWrite(pinLedRight, LOW);
  digitalWrite(pinLedDown, LOW);
  digitalWrite(pinLedLeft, LOW);
  digitalWrite(pinLedCenter, LOW);
}

void encenderTodosLosLed() {
  digitalWrite(pinLedUp, HIGH);
  digitalWrite(pinLedRight, HIGH);
  digitalWrite(pinLedDown, HIGH);
  digitalWrite(pinLedLeft, HIGH);
  digitalWrite(pinLedCenter, HIGH);
}

void loop() {
  // Leemos el comando proveniente del bluetooth
  // Si hay alguno disponible
  if (Serial.available() > 0) {
    comandoBluetooth = Serial.read();
  }

  // Escogemos la acción a realizar basados en el comando accionado
  // En caso de no encontrar un comando válido disponible
  // mostramos un mensaje de error indicando el problema
  switch(comandoBluetooth) {
    case comandoUp:
      encenderLedUp(true);
      break;
    case comandoDown:
      encenderLedDown(true);
      break;
    case comandoLeft:
      encenderLedLeft(true);
      break;
    case comandoRight:
      encenderLedRight(true);
      break;
    case comandoCenter:
      encenderLedCenter();
      break;
    case comandoSentidoReloj:
      digitalWrite(pinLedCenter, HIGH);
      encenderLedUp(false);
      delay(500);
      encenderLedRight(false);
      delay(500);
      encenderLedDown(false);
      delay(500);
      encenderLedLeft(false);
      delay(500);
      break;

    case comandoSentidoContraReloj:
      digitalWrite(pinLedCenter, HIGH);
      encenderLedUp(false);
      delay(500);
      encenderLedLeft(false);
      delay(500);
      encenderLedDown(false);
      delay(500);
      encenderLedRight(false);
      delay(500);
      break;

    case comandoParpadeo:
      encenderTodosLosLed();
      delay(100);
      apagarTodosLosLed();
      delay(1000);
      encenderTodosLosLed();
      break;

    case comandoApagado:
      apagarTodosLosLed();
      break;

    default:
      Serial.print("Comando blueooth no reconocido: ");
      Serial.println(comandoBluetooth);
  }
}
