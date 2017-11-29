// Arquitectura de Computadoras II
// Nombre: Jonathan Raí Samines Solares
// Carnet: 6590-13-3813
// Generar 8 notas musicales por salida analógica

// Declaracion de variables asociados a pines del arduino
const int pinBocina = 10;
const int pinBoton = 9;

// Array que contiene las 8 notas a sacar por el pin 10
const int numeroDeNotas = 8;
const int notasMusicales[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
bool procesoIniciado = false;

// Definición de variables que contienen el tiempo de pausa entre
// las notas y el tiempo de pausa entre el fin de la secuencia de notas
const int tiempoPausaEntreNotas = 1000;
const int tiempoPausaEntreCiclo = 10;

void setup() {
  // Inicializar pines de de entrada
  pinMode(pinBoton, INPUT);
  pinMode(pinBocina, OUTPUT);

  // Inicializamos el puerto serial para mostrar datos en pantalla
  Serial.begin(9600);
}

void generarFrecuenciasMusicales() {
  // Recorremos la lista de notas disponibles
  for (int indiceNota = 0; indiceNota < numeroDeNotas; indiceNota++) {
    // Obtenemos la nota musical correspondiente al indice actual
    int notaMusical = notasMusicales[indiceNota];

    for (int segundo = 0; segundo < tiempoPausaEntreCiclo; segundo++) {
      verificarBotonInicioPresionado();
      if (!procesoIniciado) {
        break;
      }

      digitalWrite(pinBocina, HIGH);
      delayMicroseconds(notaMusical);
      digitalWrite(pinBocina, LOW);
      delayMicroseconds(notaMusical);

      Serial.print("Nota para la bocina: ");
      Serial.println(notaMusical);
    }

    if (!procesoIniciado) {
        break;
    }

    delay(tiempoPausaEntreNotas);
  }
}

void verificarBotonInicioPresionado() {
  bool botonInicioPresionado = (bool) digitalRead(pinBoton);

  Serial.println(botonInicioPresionado);

  if (botonInicioPresionado) {
      Serial.println("Boton encendido detectado");
      procesoIniciado = !procesoIniciado;
  }
}

void loop() {
  verificarBotonInicioPresionado();

  if (procesoIniciado) {
    Serial.println("Iniciando proceso");

    generarFrecuenciasMusicales(); 
  }
}
