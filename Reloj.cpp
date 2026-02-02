#include "Reloj.h"                  // Incluye la definición de la clase Chrono
#include <sstream>                  // Para formatear el texto del cronómetro
#include <SFML/System/Time.hpp>     // Para manejar tiempo con SFML
#include <cmath>                    // Para usar std::round
#include <iostream>                 // Para depuración en consola

Reloj::Reloj() {}                 // Constructor por defecto (no hace nada)

// Constructor: recibe el tiempo que descontará el cronómetro en minutos
Reloj::Reloj(int timeMinutos) : minutos(timeMinutos-1) {
    /* ACLARACIÓN: La variable 'minutes' almacena los minutos de juego -1
       para que el cronómetro empiece en xx:59.
       Ejemplo: 5 minutos de juego → cronómetro inicia en 04:59 */

    // Carga y configura la fuente para el texto del cronómetro
    font.loadFromFile("src/fonts/Kanit.ttf");
    relojText.setFont(font);
    relojText.setString("00:00");                  // Texto inicial
    relojText.setFillColor(Color{204, 0, 0});      // Color rojo
    relojText.setCharacterSize(56);                // Tamaño grande

    // Posiciona el texto centrado en la parte inferior
    relojText.setPosition((1280 - relojText.getLocalBounds().width) / 2, 620);
}

// Devuelve los segundos restantes
int Reloj::SegundosRestantes() {
    /* Convierte los minutos a segundos y resta el tiempo transcurrido.
       minutes+1 por lo aclarado en el constructor. */
    return (minutos+1)*60 - std::round(clock.getElapsedTime().asSeconds());
}

// Actualiza el texto del cronómetro cada frame
void Reloj::Update() {
    std::stringstream timeString; // Para formatear el tiempo como "mm:ss"
    int elapsedTime = std::round(clock.getElapsedTime().asSeconds()); // Segundos transcurridos
    int relojMinutos = minutos - std::round(elapsedTime/60);         // Minutos restantes
    int relojSegundos = 59 - elapsedTime%60;                          // Segundos restantes

    // Formato "mm:ss" con ceros a la izquierda
    if (relojMinutos < 10) {
        timeString << "0" << relojMinutos;
    } else {
        timeString << relojMinutos;
    }
    timeString << ":";
    if (relojSegundos < 10) {
        timeString << "0" << relojSegundos;
    } else {
        timeString << relojSegundos;
    }

    // Actualiza el texto del cronómetro
    relojText.setString(timeString.str());
}

// Dibuja el cronómetro en la ventana
void Reloj::Draw(RenderWindow &window) {
    window.draw(relojText);
}

// Reinicia el cronómetro (empieza desde cero)
void Reloj::Start() {
    clock.restart();
}

void Reloj::setCenter(sf::View& camara)
{
    relojText.setPosition(camara.getCenter().x- relojText.getLocalBounds().width/2, 50);
}
