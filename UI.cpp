#include <Pololu3piPlus32U4.h>
#include "UI.h"

// Declaración de los objetos globales
extern Pololu3piPlus32U4::LineSensors lineSensors;
extern Pololu3piPlus32U4::OLED display;

void UI::inicializarPantalla() {
  display.setLayout21x8();
}

void UI::mostrarPantallaCarga() {
  display.noAutoDisplay();
  display.clear();

  // BIENVENIDA
  display.gotoXY(3, 0);
  display.print("BIENVENIDO A DOMA");
  display.gotoXY(8, 1);
  display.print("LINEAS");
  display.gotoXY(5, 7);
  display.print("B CAMBIA MODO");

  display.display();
}

void UI::mostrarCalibrando() {
  display.noAutoDisplay();
  display.clear();
  display.gotoXY(3, 4);
  display.print("Calibrando");
  display.display();
}

void UI::mostrarSensores() {
  display.noAutoDisplay();
  display.clear();
  unsigned int sensorValues[5];

  // Leer los valores de los sensores de línea
  lineSensors.read(sensorValues);

  // Mostrar los valores de los sensores en la pantalla OLED
  for (int i = 0; i < 5; i++) {
    display.gotoXY(0, i);
    display.print("Sensor ");
    display.print(i + 1);
    display.print(": ");
    display.print(sensorValues[i]);
  }

  display.display();
}

