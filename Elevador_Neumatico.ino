// Arquitectura de Computadoras II
// Author: Jonathan Rai Samines Solares
// Carnet: 6590-13-3813
// Elevador Neumatico
// Al presionar cualquiera de dos accionadores disponibles el motor del elevador se acciona
// si ambos botones están presionados se apaga por seguridad

const int primerBoton = 1;
const int segundoBoton = 2;
const int led = 5;

void setup() {
  pinMode(primerBoton, INPUT); // se configura el primer boton como entrada
  pinMode(segundoBoton, INPUT); // se configura el segundo boton como entrada
  pinMode(led, OUTPUT); // se configura el led como salida
}

void loop() {
  bool estaPrimerBotonPresionado = (bool) digitalRead(primerBoton); // es verdadero si el primer boton está encendido
  bool estaSegundoBotonPresionado = (bool) digitalRead(segundoBoton); // es verdadero si el segundo boton está encendido
  bool estaUnBotonPresionado = estaPrimerBotonPresionado || estaSegundoBotonPresionado; // es verdadero si cualquiera de los botones está encendido
  bool estanAmbosBotonesPresionados = estaPrimerBotonPresionado && estaSegundoBotonPresionado; // es verdadero si ambos botones están encendidos

  // encendemos el LED si cualquiera de los botones está encendido, pero no si ambos están encendidos
  digitalWrite(led, estaUnBotonPresionado && !estanAmbosBotonesPresionados);
}
