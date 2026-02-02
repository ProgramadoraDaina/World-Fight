#pragma once

#include <SFML/Graphics.hpp>
#include "fondo.h"

using namespace sf;

class Juego {
public:
    Juego(Fondo *s);

    void Run(); // Empieza el loop

    void ProcessEvents(); // Procesa los eventos (clicks, teclas presionadas, soltadas, etc.)
    void Update(); // Actualiza
    void Draw(); // Dibuja
    void SetFondo(Fondo *siguiente_escena); // Metodo para cambiar de escena
    void Exit();

	const RenderWindow &GetWindow();
	RenderWindow &GetWindow2();
	int GetRachaJugador1() const;
	int GetRachaJugador2() const;
	void ResetRachas();
	void IncrementarRachaJugador1();
	 void IncrementarRachaJugador2();
private:
    RenderWindow m_window;
    Fondo *m_fondo; // escena actual
    Fondo *m_siguiente_fondo = nullptr; // escena siguiente

 int rachaJugador1 = 0;
    int rachaJugador2 = 0;

};
