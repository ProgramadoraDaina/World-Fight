#include "ControlesJugador.h"

ControlesJugador::ControlesJugador()
    : m_up(sf::Keyboard::Unknown),
      m_down(sf::Keyboard::Unknown),
      m_left(sf::Keyboard::Unknown),
      m_right(sf::Keyboard::Unknown),
      m_patada(sf::Keyboard::Unknown),
      m_punio(sf::Keyboard::Unknown),

      // Preset P1 (WASD, F, Space)
      m_p1_up(sf::Keyboard::W),
      m_p1_down(sf::Keyboard::S),
      m_p1_left(sf::Keyboard::A),
      m_p1_right(sf::Keyboard::D),
      m_p1_patada(sf::Keyboard::F),
      m_p1_punio(sf::Keyboard::Space),

      // Preset P2 (Flechas, K, I)
      m_p2_up(sf::Keyboard::Up),
      m_p2_down(sf::Keyboard::Down),
      m_p2_left(sf::Keyboard::Left),
      m_p2_right(sf::Keyboard::Right),
      m_p2_patada(sf::Keyboard::K),
      m_p2_punio(sf::Keyboard::I)
{
}

ControlesJugador::ControlesJugador(int presetIndex)
    : ControlesJugador() // inicializa presets P1/P2 por defecto
{
    aplicarPreset(presetIndex);
}

void ControlesJugador::aplicarPreset(int presetIndex)
{
    if (presetIndex == 0) {
        // P1
        m_up     = m_p1_up;
        m_down   = m_p1_down;
        m_left   = m_p1_left;
        m_right  = m_p1_right;
        m_patada = m_p1_patada;
        m_punio  = m_p1_punio;
    } else {
        // P2
        m_up     = m_p2_up;
        m_down   = m_p2_down;
        m_left   = m_p2_left;
        m_right  = m_p2_right;
        m_patada = m_p2_patada;
        m_punio  = m_p2_punio;
    }
}

void ControlesJugador::setTeclas(sf::Keyboard::Key up,
                                 sf::Keyboard::Key down,
                                 sf::Keyboard::Key left,
                                 sf::Keyboard::Key right,
                                 sf::Keyboard::Key patada,
                                 sf::Keyboard::Key punio)
{
    m_up     = up;
    m_down   = down;
    m_left   = left;
    m_right  = right;
    m_patada = patada;
    m_punio  = punio;
}

bool ControlesJugador::anyKey() const
{
    return sf::Keyboard::isKeyPressed(m_up)     ||
           sf::Keyboard::isKeyPressed(m_down)   ||
           sf::Keyboard::isKeyPressed(m_left)   ||
           sf::Keyboard::isKeyPressed(m_right)  ||
           sf::Keyboard::isKeyPressed(m_patada) ||
           sf::Keyboard::isKeyPressed(m_punio);
}

bool ControlesJugador::arriba() const
{
    return sf::Keyboard::isKeyPressed(m_up);
}
bool ControlesJugador::abajo() const
{
    return sf::Keyboard::isKeyPressed(m_down);
}
bool ControlesJugador::izquierda() const
{
    return sf::Keyboard::isKeyPressed(m_left);
}
bool ControlesJugador::derecha() const
{
    return sf::Keyboard::isKeyPressed(m_right);
}
bool ControlesJugador::patada() const
{
    return sf::Keyboard::isKeyPressed(m_patada);
}
bool ControlesJugador::punio() const
{
    return sf::Keyboard::isKeyPressed(m_punio);
}

void ControlesJugador::setPresetP1(sf::Keyboard::Key up,
                                   sf::Keyboard::Key down,
                                   sf::Keyboard::Key left,
                                   sf::Keyboard::Key right,
                                   sf::Keyboard::Key patada,
                                   sf::Keyboard::Key punio)
{
    m_p1_up = up; m_p1_down = down; m_p1_left = left; m_p1_right = right; m_p1_patada = patada; m_p1_punio = punio;
}

void ControlesJugador::setPresetP2(sf::Keyboard::Key up,
                                   sf::Keyboard::Key down,
                                   sf::Keyboard::Key left,
                                   sf::Keyboard::Key right,
                                   sf::Keyboard::Key patada,
                                   sf::Keyboard::Key punio)
{
    m_p2_up = up;
    m_p2_down = down;
    m_p2_left = left;
    m_p2_right = right;
    m_p2_patada = patada;
    m_p2_punio = punio;
}
