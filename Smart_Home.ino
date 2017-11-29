// Arquitectura de Computadoras II - Proyecto Elevador Automatizado
// Ing. Mynor Fuentes
// ------------------------------------------------------
// Autor: Jonathan Rai Samines Solares - 6590-13-3813
// Autor: David Armando Reyes Kanazawa - 6590-10-11896
// ------------------------------------------------------

// Libreria para leer información del sensor de temperatura
#include <SimpleDHT.h>

// Librerías para manipular la información de pantallas LCD utilizando la interfaz I2C
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

// Declaramos global de los pines utilizados
const int pinSensorTemperatura = 2;
const int pinVentilador = 3;
const int pinLucesCalle = 4;
const int pinLucesEdificio1 = 5;
const int pinLucesEdificio2 = 6;
const int pinLucesEdificio3 = 7;
const int pinMotorIN1 = 8;
const int pinMotorIN2 = 9;
const int pinMotorIN3 = 10;
const int pinMotorIN4 = 11;

// Constantes globales del programa
const int retardoMotor = 2;     // Tiempo de retardo en milisegundos (Velocidad del Motor)
const int temperaturaMaximaPermitida = 20; // Permitimos un maximo de 25 grados centigrados
const int comandoNivel1 = 49; // Numero 1
const int comandoNivel2 = 50; // Numero 2
const int comandoNivel3 = 51; // Numero 3
const int comandoEncenderLucesCalle = 52; // Numero 5
const int comandoEncenderLucesEdificio = 53; // Numero 4
const int numeroDePasosEnVuelta = 520; // 512 pasos es aproximadamente igual a 360 grados (1 vuelta)
const int numeroDeVueltasPorNivel = 3; // Numero de vueltas aproximadas que se necesitan para moverse de un nivel a otro

// Declaración de sensores
const SimpleDHT11 sensor;
const LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, 0);

// Variables globales del programa
int nivelActualElevador = 1; // Indica el nivel en el que el elevador se encuentra en todo momento
int direccionElevador = 1; // Indica si el elevador se mueve hacia arriba o hacia abajo
byte temperaturaAmbiente = 0; // Indica la temperatura del ambiente en grados centigrados
bool estanLucesEdificioEncendidas = false; // Indica si las luces del edificio estan encendidas
bool estanLucesCalleEncendidas = false; // Indica si las luces de la calle estan encendidas

// Representa el simbolo de grados centigrados como una matriz de datos
byte simboloGrados[8] = {
  0b00011110,
  0b00010010,
  0b00010010,
  0b00011110,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
};

// Representa el simbolo de una flecha hacia arriba como una matriz de datos
byte flechaHaciaArriba[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000100,
  0b00001110,
  0b00011111,
  0b00000000,
  0b00000000,
};

// Representa el simbolo de una flecha hacia abajo como una matriz de datos
byte flechaHaciaAbajo[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00011111,
  0b00001110,
  0b00000100,
  0b00000000,
  0b00000000,
};

void setup() {
  // Configuramos los pines de salida
  pinMode(pinVentilador, OUTPUT);
  pinMode(pinMotorIN1, OUTPUT);
  pinMode(pinMotorIN2, OUTPUT);
  pinMode(pinMotorIN3, OUTPUT);
  pinMode(pinMotorIN4, OUTPUT);
  pinMode(pinLucesEdificio1, OUTPUT);
  pinMode(pinLucesEdificio2, OUTPUT);
  pinMode(pinLucesEdificio3, OUTPUT);
  pinMode(pinLucesCalle, OUTPUT);
  
  // Inicializamos el puerto serial
  Serial.begin(9600);

  // Configuramos la pantalla LCD
  lcd.begin(16, 2); // Indicar a la libreria que tenemos conectada una pantalla de 16x2
  lcd.home(); // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.createChar(0, simboloGrados); // Crear el caracter personalizado para grados centigrados
  lcd.createChar(1, flechaHaciaArriba); // Crear el caracter personalizado para la flecha hacia arriba
  lcd.createChar(2, flechaHaciaAbajo); // Crear el caracter personalizado para la flecha hacia abajo

  // Mostramos la información de bienvenida del programa
  Serial.println("-----------------------------------------------");
  Serial.println(" Proyecto Arquitectura de Computadoras II      ");
  Serial.println(" + Jonathan Rai Samines Solares - 6590-13-3813 ");
  Serial.println(" + David Armando Reyes Kanazawa - 6590-10-11896");
  Serial.println("-----------------------------------------------");
}

void leerTemperaturaAmbiente() {
  // Leemos la información de temperatura y humedad del sensor
  bool hayErrorDeLectura = sensor.read(pinSensorTemperatura, &temperaturaAmbiente, NULL, NULL);

  if (hayErrorDeLectura) {
    Serial.print("Error al obtener información del sensor de tempratura");
    return;
  }

  // Esperamos por al menos 1 segundo dado que la velocidad de lectura
  // del sensor es de un hertz
  delay(1000);
}


void verificarTemperaturaAmbiente() {
  bool hayDemasiadoCalor = temperaturaAmbiente > temperaturaMaximaPermitida;

  // Mostramos la información obtenida del ambiente
  Serial.println("");
  Serial.print(" Temperatura (Centigrados) : ");
  Serial.println(temperaturaAmbiente);

  Serial.print(" Ventilador encendido: ");
  Serial.println(hayDemasiadoCalor ? "Si" : "No");

  mostrarEstadoServicio();
  
  // Si la temperatura sobrepasa los 20 grados celcius
  // entonces encendemos el ventilador
  digitalWrite(pinVentilador, hayDemasiadoCalor);
}

void controlarLuces() {
  // De acuerdo a los comando ingresados por el usuario
  // encendemos o apagamos los LED
  digitalWrite(pinLucesEdificio1, estanLucesEdificioEncendidas);
  digitalWrite(pinLucesEdificio2, estanLucesEdificioEncendidas);
  digitalWrite(pinLucesEdificio3, estanLucesEdificioEncendidas);
  digitalWrite(pinLucesCalle, estanLucesCalleEncendidas);

  Serial.println("");
  Serial.print(" + Las luces del edificio estan: ");
  Serial.println(estanLucesEdificioEncendidas ? "Encendidas" : "Apagadas");
  Serial.print(" + Las luces de la calle estan: ");
  Serial.println(estanLucesCalleEncendidas ? "Encendidas" : "Apagadas");
}

void leerComandosBluetooth() {
  // Si hay informacion disponible en el puerto serial
  if(Serial.available() > 0){
    int comandoBluetooth = Serial.read(); // Leer el comando enviado a través de bluetooth

    // Si el comando que ingreso el usuario es valido
    // realizamos la acción correspondiente, en caso contrario mostramos un error
    switch(comandoBluetooth) {
      case comandoNivel1:
        moverElevador(1);
        
        break;
      case comandoNivel2:
        moverElevador(2);
  
        break;
      case comandoNivel3:
        moverElevador(3);
  
        break;
      case comandoEncenderLucesEdificio:
        if (estanLucesEdificioEncendidas) {
          Serial.println();
          Serial.println("Apagando luces del edificio");
          estanLucesEdificioEncendidas = false;
        } else {
          Serial.println();
          Serial.println("Encendiendo luces del edificio");
          estanLucesEdificioEncendidas = true;
        }
  
        break;
      case comandoEncenderLucesCalle:
        if (estanLucesCalleEncendidas) {
          Serial.println();
          Serial.println("Apagando luces de la calle");
          estanLucesCalleEncendidas = false;
        } else {
          Serial.println();
          Serial.println("Encendiendo luces de la calle");
          estanLucesCalleEncendidas = true;
        }
  
        break;
      default:
        Serial.println("");
        Serial.print("Comando de bluetooth no reconocido: ");
        Serial.println(comandoBluetooth);
    }
  }

  suspenderMotor(); // Apagado del Motor para que no se caliente
}

void mostrarEstadoServicio() {
  // Mostramos la informacion del ambiente en la pantalla LCD
  lcd.clear();
  lcd.print("Temperatura: ");
  lcd.print(temperaturaAmbiente);
  lcd.write((byte) 0);

  // Mostramos la información del elevador en la pantalla LCD
  lcd.setCursor(0, 1);
  lcd.write((byte) direccionElevador);
  lcd.print(" Numero Piso: ");
  lcd.print(nivelActualElevador);
}

void moverElevador(int nivelObjetivoElevador) {
  Serial.println("");
  Serial.print(" + Elevador actualmente en nivel: ");
  Serial.println(nivelActualElevador);
  Serial.print(" + Intentando mover elevador hacia nivel : ");
  Serial.println(nivelObjetivoElevador);

  int numeroNiveles;

  // Si el nivel al que quiero moverme está mas abajo
  // muevo el elevador en esa dirección
  if (nivelActualElevador > nivelObjetivoElevador) {
    numeroNiveles = nivelActualElevador - nivelObjetivoElevador;
    moverHaciaAbajo(numeroNiveles);


  // Si el nivel al que quiero moverme esta mas arriba
  // muevo el elevador en esa direccion
  } else if (nivelActualElevador < nivelObjetivoElevador) {
    numeroNiveles = nivelObjetivoElevador - nivelActualElevador;
    moverHaciaArriba(numeroNiveles);

  // En caso contrario, el elevador se encuentra en el nivel al que se desea ir
  // por tanto solo mostramos una advertencia
  } else {
    Serial.println(" - El elevador se encuentra actualmente en el nivel solicitado");
  }
}

void moverHaciaArriba(int numeroDeNiveles) {
  Serial.println("");
  Serial.println(" + Moviendo elevador hacia arriba");
  Serial.print(" + Numero de niveles: ");
  Serial.println(numeroDeNiveles);

  // Indicamos que la dirección del elevador es hacia arriba 
  // y actualizamos el estado del elevador en la pantalla LCD
  direccionElevador = 1;
  mostrarEstadoServicio();

  // Para mover el elevador un numero determinado de niveles
  // se calcularon un numero de vueltas necesarias para moverlo de un nivel a otro
  // ademas cada vuelta esta formada por un numero de pasos
  // Por tanto mover el elevador un nivel es igual = numeroDeNiveles * numeroDeVueltasPorNivel * numeroDePasosEnVuelta
  for (int nivel = 0; nivel < numeroDeNiveles; nivel += 1) {
    for (int vueltaActual = 0; vueltaActual < numeroDeVueltasPorNivel; vueltaActual += 1) {
      for (int paso = 0; paso < numeroDePasosEnVuelta; paso += 1) {
        aplicarPasoDerecha();
      }
    }

    // Actualizamos el nivel en el que el elevador se encuentra
    nivelActualElevador += 1;
    mostrarEstadoServicio();
    controlarLuces();
  }
}

void moverHaciaAbajo(int numeroDeNiveles) {
  Serial.println("");
  Serial.println(" + Moviendo elevador hacia abajo");
  Serial.print(" + Numero de niveles : ");
  Serial.println(numeroDeNiveles);

  // Indicamos que la dirección del elevador es hacia abajo
  // y actualizamos el estado del elevador en la pantalla LCD
  direccionElevador = 2;
  mostrarEstadoServicio();

  // Para mover el elevador un numero determinado de niveles
  // se calcularon un numero de vueltas necesarias para moverlo de un nivel a otro
  // ademas cada vuelta esta formada por un numero de pasos
  // Por tanto mover el elevador un nivel es igual = numeroDeNiveles * numeroDeVueltasPorNivel * numeroDePasosEnVuelta
  for (int nivel = 0; nivel < numeroDeNiveles; nivel += 1) {
    for (int vueltaActual = 0; vueltaActual < numeroDeVueltasPorNivel; vueltaActual += 1) {
      for (int paso = 0; paso < numeroDePasosEnVuelta; paso += 1) {
        aplicarPasoIzquierda();
      }
    }

    // Actualizamos el nivel en el que el elevador se encuentra
    nivelActualElevador -= 1;
    mostrarEstadoServicio();
    controlarLuces();
  }
}

void aplicarPasoDerecha(){
  digitalWrite(pinMotorIN4, LOW);
  digitalWrite(pinMotorIN3, LOW);
  digitalWrite(pinMotorIN2, HIGH);
  digitalWrite(pinMotorIN1, HIGH);
  delay(retardoMotor);

  digitalWrite(pinMotorIN4, LOW);
  digitalWrite(pinMotorIN3, HIGH);
  digitalWrite(pinMotorIN2, HIGH);
  digitalWrite(pinMotorIN1, LOW);
  delay(retardoMotor);

  digitalWrite(pinMotorIN4, HIGH);
  digitalWrite(pinMotorIN3, HIGH);
  digitalWrite(pinMotorIN2, LOW);
  digitalWrite(pinMotorIN1, LOW);
  delay(retardoMotor);

  digitalWrite(pinMotorIN4, HIGH);
  digitalWrite(pinMotorIN3, LOW);
  digitalWrite(pinMotorIN2, LOW);
  digitalWrite(pinMotorIN1, HIGH);
  delay(retardoMotor);
}

void aplicarPasoIzquierda() {
  digitalWrite(pinMotorIN4, HIGH);
  digitalWrite(pinMotorIN3, HIGH);
  digitalWrite(pinMotorIN2, LOW);
  digitalWrite(pinMotorIN1, LOW);
  delay(retardoMotor);

  digitalWrite(pinMotorIN4, LOW);
  digitalWrite(pinMotorIN3, HIGH);
  digitalWrite(pinMotorIN2, HIGH);
  digitalWrite(pinMotorIN1, LOW);
  delay(retardoMotor);

  digitalWrite(pinMotorIN4, LOW);
  digitalWrite(pinMotorIN3, LOW);
  digitalWrite(pinMotorIN2, HIGH);
  digitalWrite(pinMotorIN1, HIGH);
  delay(retardoMotor);

  digitalWrite(pinMotorIN4, HIGH);
  digitalWrite(pinMotorIN3, LOW);
  digitalWrite(pinMotorIN2, LOW);
  digitalWrite(pinMotorIN1, HIGH);
  delay(retardoMotor); 
}
        
void suspenderMotor() {
  digitalWrite(pinMotorIN4, LOW);
  digitalWrite(pinMotorIN3, LOW);
  digitalWrite(pinMotorIN2, LOW);
  digitalWrite(pinMotorIN1, LOW);
}

void loop() {
  // Leemos y actuamos de acuerdo a la temperatura ambiente
  leerTemperaturaAmbiente();
  verificarTemperaturaAmbiente();
  controlarLuces();

  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);

  // Leemos y actuamos de acuerdo al comando bluetooth recibido
  leerComandosBluetooth();
}
