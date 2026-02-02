#include <fstream>
#include <vector>

#include "Instrucciones.h"
#include "Combate.h"
#include "menu.h"

#include <cstring>
#include <algorithm>
#include <sstream>
#include <iostream>
using namespace sf;
using namespace std;

Instrucciones::Instrucciones()
{
    // Cargar recursos
    m_f1.loadFromFile("Src/fonts/arcade.ttf");
    m_tex_fondo.loadFromFile("src/Fondo degradado.png");
    m_spr_fondo.setTexture(m_tex_fondo);

    m_tex_logo.loadFromFile("Src/world fight titulo del juego.png");
    m_spr_logo.setTexture(m_tex_logo);

    // Obtener resolución y calcular escala dinámica
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    float screenAncho  = static_cast<float>(desktop.width);
    float screenAlto = static_cast<float>(desktop.height);

    // Base tomada del ancho del fondo (sin hardcode)
    sf::Vector2u bgSize = m_tex_fondo.getSize();
    float baseAncho = static_cast<float>(bgSize.x);

    float uiScale = screenAncho / baseAncho;

    // Escalar fondo para cubrir pantalla
    float scaleX = screenAncho / bgSize.x;
    float scaleY = screenAlto / bgSize.y;
    float finalScale = std::max(scaleX, scaleY);
    m_spr_fondo.setScale(finalScale, finalScale);

    // Escalar logo
    m_spr_logo.setScale(0.1859f * uiScale, 0.1859f * uiScale);
    m_spr_logo.setPosition(20.f * uiScale, 10.f * uiScale);

    // Configurar textos con tamaños relativos
    m_t1.setFont(m_f1);
    m_t1.setString("Como jugar");
    m_t1.setCharacterSize(static_cast<unsigned>(20 * uiScale));
    m_t1.setFillColor(Color::White);
    m_t1.setPosition(screenAncho - (330.f * uiScale), 25.f * uiScale);

    m_t2.setFont(m_f1);
    m_t2.setString("Movimientos");
    m_t2.setCharacterSize(static_cast<unsigned>(13 * uiScale));
    m_t2.setFillColor(Color::White);
    m_t2.setPosition(screenAncho - (305.f * uiScale), 50.f * uiScale);

    m_t3.setFont(m_f1);
    m_t3.setString("Jugador 1 | Jugador 2");
    m_t3.setCharacterSize(static_cast<unsigned>(16 * uiScale));
    m_t3.setFillColor(Color::White);
    m_t3.setPosition(screenAncho - (390.f * uiScale), 70.f * uiScale);

    m_t4.setFont(m_f1);
    m_t4.setString("Para Moverse: WASD | Flechas");
    m_t4.setCharacterSize(static_cast<unsigned>(12 * uiScale));
    m_t4.setFillColor(Color::White);
    m_t4.setPosition(screenAncho - (390.f * uiScale), 100.f * uiScale);

    m_t5.setFont(m_f1);
    m_t5.setString("Patear: F | K");
    m_t5.setCharacterSize(static_cast<unsigned>(12 * uiScale));
    m_t5.setFillColor(Color::White);
    m_t5.setPosition(screenAncho - (390.f * uiScale), 120.f * uiScale);

    m_t6.setFont(m_f1);
    m_t6.setString("HADUKEN: Space | I");
    m_t6.setCharacterSize(static_cast<unsigned>(12 * uiScale));
    m_t6.setFillColor(Color::White);
    m_t6.setPosition(screenAncho - (390.f * uiScale), 140.f * uiScale);

    m_t7.setFont(m_f1);
    m_t7.setString("Como seleccionar a los personajes (mouse)");
    m_t7.setCharacterSize(static_cast<unsigned>(9 * uiScale));
    m_t7.setFillColor(Color::White);
    m_t7.setPosition(220.f * uiScale, 175.f * uiScale);

    m_t8.setFont(m_f1);
    m_t8.setString("Click izquierdo | Click derecho");
    m_t8.setCharacterSize(static_cast<unsigned>(11 * uiScale));
    m_t8.setFillColor(Color::White);
    m_t8.setPosition(230.f * uiScale, 190.f * uiScale);

    m_t9.setFont(m_f1);
    m_t9.setString("El que gane mas de un round Gana el combate!");
    m_t9.setCharacterSize(static_cast<unsigned>(10 * uiScale));
    m_t9.setFillColor(Color::White);
    m_t9.setPosition(screenAncho - (600.f * uiScale), screenAlto - (110.f * uiScale));

// Configurar botón volver
    boton1.setFillColor(Color(212, 43, 43));
    boton1.setPosition(29.5f * uiScale, screenAlto - (87.f * uiScale));
    boton1.setSize(Vector2f(245.f * uiScale, 30.f * uiScale));

// Configurar texto dentro del botón
    m_t10.setFont(m_f1);
    m_t10.setString("Go back to menu");
    m_t10.setCharacterSize(static_cast<unsigned>(15 * uiScale));
    m_t10.setFillColor(Color::White);


    m_t10.setPosition(
        boton1.getPosition().x + (boton1.getSize().x - m_t10.getLocalBounds().width) * 0.5f,
        boton1.getPosition().y + (boton1.getSize().y - m_t10.getLocalBounds().height) * 0.5f
    );


}
Instrucciones::~Instrucciones()
{

}

void Instrucciones::ProcessEvents(Juego &juego, Event &event)
{
    // Click del mouse
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos(sf::Mouse::getPosition(juego.GetWindow()));

        if (boton1.getGlobalBounds().contains(mousePos))
        {
            juego.SetFondo(new Menu());
        }
    }
    // Tecla presionada
    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            juego.SetFondo(new Menu());
        }
    }
}


void Instrucciones::Update(Juego &juego)
{

}
void Instrucciones::Draw(RenderWindow &window)   // Mostramos los fondos y textos
{
    window.clear({0,0,0});
    window.draw(m_spr_fondo);
    window.draw(m_spr_logo);
    window.draw(boton1);
    window.draw(m_t1);
    window.draw(m_t2);
    window.draw(m_t3);
    window.draw(m_t4);
    window.draw(m_t5);
    window.draw(m_t6);
    window.draw(m_t7);
    window.draw(m_t8);
    window.draw(m_t9);
    window.draw(m_t10);
    window.display();
}
