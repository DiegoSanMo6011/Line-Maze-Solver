#include <Pololu3piPlus32U4.h>
#include "UI.h"

// Declaración de los objetos globales
extern Pololu3piPlus32U4::LineSensors lineSensors;
extern Pololu3piPlus32U4::OLED display;

// Inicializa la pantalla OLED con el diseño de 21x8 caracteres
void UI::inicializarPantalla() {
    display.setLayout21x8();
}

// Muestra la pantalla de carga con un mensaje de bienvenida
void UI::mostrarPantallaCarga() {
    display.noAutoDisplay();  // Desactiva la actualización automática de la pantalla
    display.clear();          // Limpia la pantalla

    // Muestra el mensaje de bienvenida en diferentes posiciones de la pantalla
    display.gotoXY(3, 0);
    display.print("BIENVENIDO A DOMA");
    display.gotoXY(8, 1);
    display.print("LINEAS");
    display.gotoXY(5, 7);
    display.print("B CAMBIA MODO");

    display.display();        // Actualiza la pantalla para mostrar el contenido
}

// Muestra un mensaje de calibración en la pantalla OLED
void UI::mostrarCalibrando() {
    display.noAutoDisplay();  // Desactiva la actualización automática de la pantalla
    display.clear();          // Limpia la pantalla
    display.gotoXY(3, 4);
    display.print("Calibrando");
    display.display();        // Actualiza la pantalla para mostrar el contenido
}

// Muestra los valores de los sensores de línea en la pantalla OLED
void UI::mostrarSensores() {
    display.noAutoDisplay();  // Desactiva la actualización automática de la pantalla
    display.clear();          // Limpia la pantalla
    unsigned int sensorValues[5]; // Array para almacenar los valores de los sensores

    // Lee los valores de los sensores de línea
    lineSensors.read(sensorValues);

    // Muestra los valores de los sensores en la pantalla OLED
    for (int i = 0; i < 5; i++) {
        display.gotoXY(0, i);  // Posiciona el cursor en la pantalla
        display.print("Sensor ");
        display.print(i + 1);  // Imprime el número del sensor
        display.print(": ");
        display.print(sensorValues[i]);  // Imprime el valor del sensor
    }

    display.display();  // Actualiza la pantalla para mostrar el contenido
}

