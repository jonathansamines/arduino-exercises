// Arquitectura de Computadoras II
// @author: Jonathan Raí Samines Solares
// @carnet: 6590-13-3813
// Manipulación de LED's utilizando una fotocelda
// Se enciende/apaga un LED de acuerdo al nivel de exposicion 
// a la luz de una fotocelda

const int fotocelda = A1;
const int led = 5;

void setup() {
  // configuramos el LED como pin de salida
  pinMode(led, OUTPUT);

  // configuramos la impresion serial
  Serial.begin(9600);
}

void loop() {
  // leemos el valor analogico de intesidad de luz generado por la fotocelda
  int intensidadLuz = analogRead(fotocelda);

  // mapeamos el valor variable de la fotocelda a un valor valido para el LED
  int intesidadLuzLED = map(intensidadLuz, 0, 255, 0, 255);

  // mostramos el valor de intesidad de luz en el LED
  // encendiendolo o apagandolo de acuerdo al nivel de intesidad
  analogWrite(led, intesidadLuzLED);

  // mostramos los valores de la fotocelda y del LED
  // en el puerto de impresion serial del arduino
  Serial.print("Valor fotocelda: ");
  Serial.println(intensidadLuz);

  Serial.print("Valor LED: ");
  Serial.println(intesidadLuzLED);

  // esperamos por 1 segundo
  delay(1000);
}
