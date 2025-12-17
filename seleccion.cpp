#include "seleccion.h"
#include "Combate.h"
#include "menu.h"
#include <iostream>       // Incluye la librería iostream (para depuración si se usa)

using namespace sf;       // Permite usar clases de SFML sin prefijo sf::


Selector::Selector() {
    m_f1.loadFromFile("Src/fonts/arcade.ttf");

    // Fondo
    m_tex_fondo.loadFromFile("src/Fondo degradado.png");
    m_spr_fondo.setTexture(m_tex_fondo);

    m_tex_logo.loadFromFile("src/world fight titulo del juego.png");
    m_spr_logo.setTexture(m_tex_logo);

    // Resolución actual
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    float screenAncho  = static_cast<float>(desktop.width);
    float screenAlto = static_cast<float>(desktop.height);

    // Escala del fondo (sin deformación)
    float scaleX = screenAncho / m_tex_fondo.getSize().x;
    float scaleY = screenAlto / m_tex_fondo.getSize().y;
    float finalScale = std::max(scaleX, scaleY);
    m_spr_fondo.setScale(finalScale, finalScale);

    // Base tomada del ancho del fondo (sin hardcode)
    sf::Vector2u bgSize = m_tex_fondo.getSize();
    float baseWidth = static_cast<float>(bgSize.x);
    float uiScale = screenAncho / baseWidth;

    // Logo
    m_spr_logo.setScale(0.1859f * uiScale, 0.1859f * uiScale);
    m_spr_logo.setPosition(20.f * uiScale, 10.f * uiScale);

    // Cargar texturas de personajes
    m_ryu.loadFromFile("src/ryu estatico.png");
    m_joseph.loadFromFile("src/joseph.png");
    m_moon.loadFromFile("src/Moon estatico.png");
    m_camila.loadFromFile("src/camila estatico.png");

    ryu_sprite.setTexture(m_ryu);
    ryu_sprite.setTextureRect(sf::IntRect(55, 40, 425, 425));

    joseph_sprite.setTexture(m_joseph);
    joseph_sprite.setTextureRect(sf::IntRect(12, 0, 140, 148));

    moon_sprite.setTexture(m_moon);
    moon_sprite.setTextureRect(sf::IntRect(72, 5, 188, 192));

    camila_sprite.setTexture(m_camila);
    camila_sprite.setTextureRect(sf::IntRect(82, 5, 192, 192));

    // Textos
    m_t1.setFont(m_f1);
    m_t1.setFillColor(sf::Color::White);
    m_t1.setString("Jugar");
    m_t1.setCharacterSize(static_cast<unsigned>(15 * uiScale));

    m_t2.setFont(m_f1);
    m_t2.setFillColor(sf::Color::White);
    m_t2.setString("Volver");
    m_t2.setCharacterSize(static_cast<unsigned>(15 * uiScale));

    // Botones
    float buttonWidth  = 200.f * uiScale;
    float buttonHeight = 30.f * uiScale;

    boton1.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    boton1.setFillColor(sf::Color(212, 43, 43));
    boton1.setPosition(30.f * uiScale, 220.f * uiScale);

    boton2.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    boton2.setFillColor(sf::Color(212, 43, 43));
    boton2.setPosition(30.f * uiScale, 260.f * uiScale);

    // Centrar textos en botones
    m_t1.setPosition(
        boton1.getPosition().x + (boton1.getSize().x - m_t1.getLocalBounds().width) * 0.5f,
        boton1.getPosition().y + (boton1.getSize().y - m_t1.getLocalBounds().height) * 0.5f
    );

    m_t2.setPosition(
        boton2.getPosition().x + (boton2.getSize().x - m_t2.getLocalBounds().width) * 0.5f,
        boton2.getPosition().y + (boton2.getSize().y - m_t2.getLocalBounds().height) * 0.5f
    );

    // Posiciones proporcionales (cuadrícula 2x2)
    ryu_sprite.setPosition(screenAncho * 0.47f, screenAlto * 0.20f);
    joseph_sprite.setPosition(screenAncho * 0.65f, screenAlto * 0.20f);
    moon_sprite.setPosition(screenAncho * 0.65f, screenAlto * 0.5f);
    camila_sprite.setPosition(screenAncho * 0.47f, screenAlto * 0.5f);

    // Escala proporcional usando uiScale (sin hardcode 1920x1080)
    ryu_sprite.setScale(uiScale * 0.23f, uiScale * 0.23f);
    joseph_sprite.setScale(uiScale * 0.65f, uiScale * 0.65f);
    moon_sprite.setScale(uiScale * 0.48f, uiScale * 0.5f);
    camila_sprite.setScale(uiScale * 0.51f, uiScale * 0.5f);

}
Selector::~Selector()
{
    // Destructor vacío
}

// Manejo de eventos en la pantalla de selección
void Selector::ProcessEvents(Juego &juego, sf::Event &event)
{
    if (event.type == Event::MouseButtonPressed || event.type == sf::Event::KeyPressed)
    {
        Vector2f mousePos = Vector2f(Mouse::getPosition(juego.GetWindow())); // Posición del mouse

        // Áreas de los botones
        FloatRect button1Bounds = boton1.getGlobalBounds();
        FloatRect button2Bounds = boton2.getGlobalBounds();

        // Áreas de los personajes
        FloatRect ryuBounds = ryu_sprite.getGlobalBounds();
        FloatRect josephBounds = joseph_sprite.getGlobalBounds();
        FloatRect moonBounds = moon_sprite.getGlobalBounds();
        FloatRect camilaBounds = camila_sprite.getGlobalBounds();

        // Resetear opacidad (todos blancos)
        ryu_sprite.setColor(sf::Color(255, 255, 255, 255));
        joseph_sprite.setColor(sf::Color(255, 255, 255, 255));
        moon_sprite.setColor(sf::Color(255, 255, 255, 255));
        camila_sprite.setColor(sf::Color(255, 255, 255, 255));

        // Colores para selección
        Color red(255, 0, 0, 200);   // Jugador 1
        Color blue(3, 74, 252, 200); // Jugador 2

        if (event.type == sf::Event::KeyPressed)
        {
            //Jugador 1
            if (event.key.code == sf::Keyboard::A)
            {
                posj1[0] = false;
            }
            else if (event.key.code == sf::Keyboard::D)
            {
                posj1[0] = true;
            }
            else if (event.key.code == sf::Keyboard::W)
            {
                posj1[1] = false;
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                posj1[1] = true;
            }

            //Jugador 2
            if (event.key.code == sf::Keyboard::Left)
            {
                posj2[0] = false;
            }
            else if (event.key.code == sf::Keyboard::Right)
            {
                posj2[0] = true;
            }
            else if (event.key.code == sf::Keyboard::Up)
            {
                posj2[1] = false;
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                posj2[1] = true;
            }

            //Calcular posicion del seleccionado
            //Jugador 1
            if (!posj1[0] && !posj1[1])
            {
                Jugador_uno = "Ryu";
                seleccionadoP1 = &ryu_sprite;
            }
            if (posj1[0] && !posj1[1])
            {
                Jugador_uno = "Joseph";
                seleccionadoP1 = &joseph_sprite;
            }
            if (!posj1[0] && posj1[1])
            {
                Jugador_uno = "Camila";
                seleccionadoP1 = &camila_sprite;
            }
            if (posj1[0] && posj1[1])
            {
                Jugador_uno = "Moon";
                seleccionadoP1 = &moon_sprite;
            }

            //Jugador 2
            if (!posj2[0] && !posj2[1])
            {
                Jugador_dos = "Ryu";
                seleccionadoP2 = &ryu_sprite;
            }
            if (posj2[0] && !posj2[1])
            {
                Jugador_dos = "Joseph";
                seleccionadoP2 = &joseph_sprite;
            }
            if (!posj2[0] && posj2[1])
            {
                Jugador_dos = "Camila";
                seleccionadoP2 = &camila_sprite;
            }
            if (posj2[0] && posj2[1])
            {
                Jugador_dos = "Moon";
                seleccionadoP2 = &moon_sprite;
            }
        }

        if (event.type == Event::MouseButtonPressed)
        {
            // Selección de personajes según clic izquierdo (Jugador 1) o derecho (Jugador 2)
            if (ryuBounds.contains(mousePos))
            {
                if(event.mouseButton.button == Mouse::Left)
                {
                    Jugador_uno = "Ryu";
                    seleccionadoP1 = &ryu_sprite;
                    posj1[0] = false;
                    posj1[1] = false;
                }
                else if(event.mouseButton.button == Mouse::Right)
                {
                    Jugador_dos = "Ryu";
                    seleccionadoP2 = &ryu_sprite;
                    posj2[0] = false;
                    posj2[1] = false;
                }
            }
            else if (josephBounds.contains(mousePos))
            {
                if(event.mouseButton.button == Mouse::Left)
                {
                    Jugador_uno = "Joseph";
                    seleccionadoP1 = &joseph_sprite;
                    posj1[0] = true;
                    posj1[1] = false;
                }
                else if(event.mouseButton.button == Mouse::Right)
                {
                    Jugador_dos = "Joseph";
                    seleccionadoP2 = &joseph_sprite;
                    posj2[0] = true;
                    posj2[1] = false;
                }
            }
            else if (camilaBounds.contains(mousePos))
            {
                if(event.mouseButton.button == Mouse::Left)
                {
                    Jugador_uno = "Camila";
                    seleccionadoP1 = &camila_sprite;
                    posj1[0] = false;
                    posj1[1] = true;
                }
                else if(event.mouseButton.button == Mouse::Right)
                {
                    Jugador_dos = "Camila";
                    seleccionadoP2 = &camila_sprite;
                    posj2[0] = false;
                    posj2[1] = true;
                }
            }
            else if (moonBounds.contains(mousePos))
            {
                if(event.mouseButton.button == Mouse::Left)
                {
                    Jugador_uno = "Moon";
                    seleccionadoP1 = &moon_sprite;
                    posj1[0] = true;
                    posj1[1] = true;
                }
                else if(event.mouseButton.button == Mouse::Right)
                {
                    Jugador_dos = "Moon";
                    seleccionadoP2 = &moon_sprite;
                    posj2[0] = true;
                    posj2[1] = true;
                }
            }
        }

        // Resaltar selección
        if (seleccionadoP1 != nullptr)
            seleccionadoP1->setColor(blue);
        if (seleccionadoP2 != nullptr)
            seleccionadoP2->setColor(red);

        // Botón "Empezar" → iniciar combate si ambos jugadores están seleccionados
        if (button1Bounds.contains(mousePos) || event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
        {
            if(Jugador_uno != "" && Jugador_dos != "")
            {
                juego.SetFondo(new Combate(Jugador_uno, Jugador_dos, juego.GetWindow2()));
            }
        }
        else if (button2Bounds.contains(mousePos) || event.key.code == sf::Keyboard::Escape)
        {
            juego.SetFondo(TipoEscena::MENU); // Botón "Go back" → volver al menú
        }
    }
}

// Método Update (no implementado)
void Selector::Update(Juego &juego)
{
    // ...
}

// Dibuja la pantalla de selección
void Selector::Draw(sf::RenderWindow &window)
{
    window.clear({0, 0, 0});           // Limpia pantalla
    window.draw(m_spr_fondo);     // Dibuja fondo
    window.draw(m_spr_logo);
    window.draw(ryu_sprite);           // Dibuja Ryu
    window.draw(joseph_sprite);        // Dibuja Joseph
    window.draw(moon_sprite);          // Dibuja Moon
    window.draw(camila_sprite);       // Dibuja Camila
    window.draw(boton1);              // Botón "Empezar"
    window.draw(boton2);              // Botón "Go back"
    window.draw(m_t1);                 // Texto "Empezar"
    window.draw(m_t2);                 // Texto "Go back"
    window.display();                  // Muestra todo en pantalla
}
