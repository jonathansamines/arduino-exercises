// Teclado Serial
// Nombre: Jonathan Rai Samines Solares - 6590-13-3813
// Carnet: 6590-13-3813
// ---------------------------------------
// Funcionamiento:
// ---------------------------------------
// Un usuario puede ingresar mediante el teclado serial
// números del 1 al 9.
// Dependiendo del numero ingresado, se enciende un LED un determinado numero de veces

// Mantenemos en memoria el conjunto de codigos de numeros validos
// así como los valores correspondientes a ellos de modo que podamos obtener
// el número basados en el caracter correspondiente
// Codigo |   Numero
// -----------------------------------
//  49    |  1
//  50    |  2
//  51    |  3
//  52    |  4
//  53    |  5
//  54    |  6
//  55    |  7
//  56    |  8
//  57    |  9
const int mapaCodigoTeclas[] = {49, 50, 51, 52, 53, 54, 55, 56, 57};
const int mapaNumeros[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
const int pinLed = 10;

void setup() {
  // Inicializamos el monitor serial
  Serial.begin(9600);

  // Inicializamos las salidas
  pinMode(pinLed, OUTPUT);

  Serial.println("  Arquitectura de Computadoras II  ");
  Serial.println("-----------------------------------");
}

void parpadeoLed() {
  Serial.println("  + LED Encendido");
  delay(500);
  digitalWrite(pinLed, HIGH);

  Serial.println("  - LED apagado");
  delay(500);
  digitalWrite(pinLed, LOW);
}

void parpadeoNumeroDeVeces(int numeroVeces) {
  Serial.print("Numero de parpadeos de LED: ");
  Serial.println(numeroVeces);

  for (int indice = 0; indice < numeroVeces; indice += 1) {
    parpadeoLed();
  }
}

void loop() {
  // Si hay datos disponibles ingresados por el usuario entonces validamos el caracter
  if (Serial.available()) {
    // Leemos el numero que el usuario ingresa en la pantalla
    int codigoTeclaPresionada = Serial.read();

    // Según el codigo que el usuario ingresa
    // buscamos el numero real correspondiente
    // Si al final lo encontramos, el ciclo del programa termina
    for (int indice = 0; indice <= 8; indice++) {
      int codigoTecla = mapaCodigoTeclas[indice];

      // Si el codigo de tecla es considerado valido
      if (codigoTecla == codigoTeclaPresionada) {
        // Obtenemos el valor del codigo correspondiente
        int numeroIntentos = mapaNumeros[indice];

        // Hacemos parpadear el led el numero de veces indicado
        parpadeoNumeroDeVeces(numeroIntentos);
        return;
      }
    }

    Serial.println("");
    Serial.print(" [x] Codigo de tecla invalido. Se esperaba un valor entre 1 y 9: ");
    Serial.println(codigoTeclaPresionada);
  }
}

