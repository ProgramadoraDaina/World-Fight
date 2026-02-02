#ifndef CONTROLESJUGADOR_H_INCLUDED
#define CONTROLESJUGADOR_H_INCLUDED
#pragma once
#include <SFML/Window/Keyboard.hpp>

class ControlesJugador {
public:

ControlesJugador();
    ControlesJugador(int presetIndex); // 0 = P1, 1 = P2

    // Elegir el preset activo (0 = P1, 1 = P2)
    void aplicarPreset(int presetIndex);

    // Sobrescribir manualmente el mapeo ACTIVO (si lo necesitás)
    void setTeclas(sf::Keyboard::Key up,
                   sf::Keyboard::Key down,
                   sf::Keyboard::Key left,
                   sf::Keyboard::Key right,
                   sf::Keyboard::Key patada,
                   sf::Keyboard::Key punio);

    // Consultas (idéntico comportamiento al actual)
    bool anyKey()    const;
    bool arriba()    const;
    bool abajo()     const;
    bool izquierda() const;
    bool derecha()   const;
    bool patada()    const;
    bool punio()     const;

    // (Opcional) Si querés cambiar los presets por instancia en runtime
    void setPresetP1(sf::Keyboard::Key up,
                     sf::Keyboard::Key down,
                     sf::Keyboard::Key left,
                     sf::Keyboard::Key right,
                     sf::Keyboard::Key patada,
                     sf::Keyboard::Key punio);

    void setPresetP2(sf::Keyboard::Key up,
                     sf::Keyboard::Key down,
                     sf::Keyboard::Key left,
                     sf::Keyboard::Key right,
                     sf::Keyboard::Key patada,
                     sf::Keyboard::Key punio);

private:
    // Mapeo activo para ESTE jugador
    sf::Keyboard::Key m_up;
    sf::Keyboard::Key m_down;
    sf::Keyboard::Key m_left;
    sf::Keyboard::Key m_right;
    sf::Keyboard::Key m_patada;
    sf::Keyboard::Key m_punio;

    // Presets por instancia (sin static, sin struct)
    // P1
    sf::Keyboard::Key m_p1_up;
    sf::Keyboard::Key m_p1_down;
    sf::Keyboard::Key m_p1_left;
    sf::Keyboard::Key m_p1_right;
    sf::Keyboard::Key m_p1_patada;
    sf::Keyboard::Key m_p1_punio;

    // P2
    sf::Keyboard::Key m_p2_up;
    sf::Keyboard::Key m_p2_down;
    sf::Keyboard::Key m_p2_left;
    sf::Keyboard::Key m_p2_right;
    sf::Keyboard::Key m_p2_patada;
    sf::Keyboard::Key m_p2_punio;
};


#endif // CONTROLESJUGADOR_H_INCLUDED
