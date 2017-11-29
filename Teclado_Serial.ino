// Teclado Serial
// Nombre: Jonathan Rai Samines Solares - 6590-13-3813
// Carnet: 6590-13-3813
// ---------------------------------------
// Funcionamiento:
// ---------------------------------------
// Un usuario puede ingresar caracteres mediante el Monitor Serial
// Si el caracter ingresado de acuerdo a la tabla siguiente es valido,
// entonces la frecuencia correspondiente a esa nota es entonada por el zumbador
// hasta que otro caracter valido cambie la nota o un caracter invalido sea ingresado
// en dicho caso el zumbador dejar de sonar

const int pinBocina = 10;

// A continuación declaramos tres arrays, cada uno contiene
// la de los códigos de tecla, caracteres de tecla y frecuencias asociadas
// de acuerdo a la siguiente tabla
// Tecla    ||   Codigo Tecla    ||    Frecuencia
// ------------------------------------------------
//   c      ||       099         ||       261
//   d      ||       100         ||       294
//   e      ||       101         ||       329
//   f      ||       102         ||       349
//   g      ||       103         ||       392
//   a      ||       097         ||       440
//   b      ||       098         ||       493
//   C      ||       067         ||       523

const char mapaTeclas[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
const int mapaCodigoTeclas[] = {99, 100, 101, 102, 103, 97, 98, 67};
const int mapaFrecuencias[] = {261, 294, 329, 349, 392, 440, 493, 523};

// Contiene el indice en el array de la tecla seleccionada
// Para que funcione, cada caracter, codigo de tecla y frecuencias asociadas
// deben de estar en la misma posicion en el array
// El valor -1 indica que la tecla actualmente seleccionada no es válida
int indiceTeclaSeleccionada = -1;

void setup() {
  // Inicializamos el monitor serial
  Serial.begin(9600);
}

void loop() {
  // Si hay datos disponibles ingresados por el usuario entonces validamos el caracter
  if (Serial.available()) {
    // Leemos el valor que el usuario ingresa en la pantalla
    byte codigoTeclaPresionada = Serial.read();

    // En caso de que la tecla ingresada no sea valido, mantenmos el valor inicial en -1 (invalido)
    indiceTeclaSeleccionada = -1;

    for (int posicion = 0; posicion < 8; posicion ++) {
      if (mapaCodigoTeclas[posicion] == codigoTeclaPresionada) {  
        // Asignamos la posicion de la tecla encontrada y terminamos la busqueda
        indiceTeclaSeleccionada = posicion;
        break;
      }
    }

    // Si la tecla es válida
    if (indiceTeclaSeleccionada != -1) {
      Serial.print("Tecla ingresada: ");
      Serial.println(mapaTeclas[indiceTeclaSeleccionada]);
    } else {
      Serial.println("Tecla no valida. Apagando bocina");
    }
  }
  

  // Si la tecla es valida
  if (indiceTeclaSeleccionada != -1) {
    // Hacemos sonar el zumbador de acuerdo a la frecuencia asociada al caracter seleccionado
    int frecuencia = mapaFrecuencias[indiceTeclaSeleccionada];

    analogWrite(pinBocina, frecuencia);
  } else {
    // En casos donde la tecla ingresada no sea valida, apagamos el buzzer
    analogWrite(pinBocina, LOW);
  }
}
