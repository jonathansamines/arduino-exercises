// Nombre: Jonathan Rai Samines Solares
// Carnet: 6590-13-3813
// Control de la intesidad de un LED
// Controla un LED mediante 2 botones de acuerdo a las siguientes reglas
// El primer boton (de encendido) inicia el proceso de control del led
// Mientras el proceso esta activo, la intensidad de luz del LED aumenta y disminuye
// Al presionar el segundo boton, el LED aumenta su intensidad al máximo y tras un momento se apaga

// Pines de Control
const int pinBotonEncendido = 2;
const int pinBotonApagado = 3;
const int pinLED = 5;

// Limites de intensidad del LED
const int MIN_INTENSIDAD = 0;
const int MAX_INTENSIDAD = 255;

// Variables de Control
bool estaProcesoIniciado = false;
bool esProcesoAscendente = true;

void setup() {
  // Se declaran los pines de entrada
  pinMode(pinBotonEncendido, INPUT);
  pinMode(pinBotonApagado, INPUT);
  // pinMode(pinLED, OUTPUT);

  Serial.begin(9600);
}

void controlarIntensidadDelLED() {
  if (estaProcesoIniciado) {
    int diferencial = esProcesoAscendente ? MIN_INTENSIDAD : MAX_INTENSIDAD;

    // Modificamos el valor de intensidad del LED
    // Ascendente, recorremos todos los valores del 0 - 255
    // Descendente, recorremos los valores de 255 - 0.
    // Para hacerlo, a cada valor del intervalo ascendente le restamos 255, de modo que obtenemos su valor inverso en la escala
    // Por ejemplo el valor 3, se convierte en 252 (3-255) = -252, pero obtenemos su valor absoluto para manipular la escala descendente correctamente
    for (int intensidad = MIN_INTENSIDAD; intensidad <= MAX_INTENSIDAD; intensidad += 10) {
      int intesidadReal = intensidad - diferencial;
      intesidadReal = abs(intesidadReal);

      Serial.print("Intensidad del LED: ");
      Serial.println(intesidadReal);

      // Encendemos el LED con el valor de intensidad adecuado por 50ms
      analogWrite(pinLED, intesidadReal);
      delay(50);

      // Verificamos que si el boton de apagado esta presionado
      // Si lo esta, terminamos el ciclo inmediatamente
      if (verificarBotonApagado()) {
        break;
      }
    }

    // Una vez recorrimos cualquier de las escalas (ascendente o descendente)
    // Marcamos el estado para recorrer la otra escala
    esProcesoAscendente = !esProcesoAscendente;

    delay(100);

    Serial.println("");
    Serial.print("Cambiando de Modo a : ");
    Serial.println(esProcesoAscendente ? "Ascendente" : "Descendente");
  }
}

bool verificarBotonApagado() {
  bool estaBotonApagadoPresionado = (bool) digitalRead(pinBotonApagado);

  // Cuando se presiona el boton de fin
  // Se finaliza el proceso de control de intensidad del LED
  if (estaBotonApagadoPresionado) {
    Serial.println("Finalizando proceso");
    estaProcesoIniciado = false;

    // Encendemos el LED, esperamos al menos 200ms
    // y finalmente apagamos el lED
    digitalWrite(pinLED, HIGH);
    delay(1000);
    digitalWrite(pinLED, LOW);
  }  

  return estaBotonApagadoPresionado;
}

void loop() {
  // Leemos el valor actual de los botones
  bool estaBotonEncendidoPresionado = (bool) digitalRead(pinBotonEncendido);

  // Cuando se presionado el boton de inicio
  // Se inicia el proceso de control de intesidad del LED
  if (estaBotonEncendidoPresionado) {
    Serial.println("Iniciando proceso");
    estaProcesoIniciado = true;
  }

  controlarIntensidadDelLED();
}
