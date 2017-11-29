// Arquitectura de Computadoras II
// @author Jonathan Rai Samines Solares
// @carnet 6590-13-3813
// Gestión de led utilizando compuertas lógicas
// Este programa utiliza tres LED, y dos botones:
// El primer LED se enciende solo si ambos botones están presionados
// El segundo LED se enciende si cualquier de los botones está presionado
// El tercer LED se enciende solo si el primer LED no está presionado

const int primerBoton = 7;
const int segundoBoton = 2;

const int primerLed = 5;
const int segundoLed = 6;
const int tercerLed = 8;

bool estaPrimerBotonEncendido;
bool estaSegundoBotonEncendido;

void setup() {
    // se configuran las referencias a los botones como input
    pinMode(primerBoton, INPUT);
    pinMode(segundoBoton, INPUT);

    // se configuran las referencias a los LED como output
    pinMode(primerLed, OUTPUT);
    pinMode(segundoLed, OUTPUT);
    pinMode(tercerLed, OUTPUT);
} 

void loop() {
    estaPrimerBotonEncendido = (bool) digitalRead(primerBoton); // verificamos si el primer botón está encendido
    estaSegundoBotonEncendido = (bool) digitalRead(segundoBoton); // verificamos si el segundo botón está encendido

    // encendemos el primer LED si ambos botones están encendidos
    digitalWrite(primerLed, estaPrimerBotonEncendido && estaSegundoBotonEncendido);

    // encendemos el segundo LED si cualquiera de los botones esta encendido
    digitalWrite(segundoLed, estaPrimerBotonEncendido || estaSegundoBotonEncendido);

    // encendemos el tercer LED si el primer botón no está encendido
    digitalWrite(tercerLed, !estaPrimerBotonEncendido);
}
