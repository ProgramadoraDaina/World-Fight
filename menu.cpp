#include "menu.h"
#include "Combate.h"
#include "Archivo.h"
#include "Instrucciones.h"
#include "seleccion.h"


Menu::Menu()
{
    // Fuente y textos
    m_f1.loadFromFile("Src/fonts/arcade.ttf");

    // Fondo
    m_tex_fondo.loadFromFile("src/Fondo degradado.png");
    m_spr_fondo.setTexture(m_tex_fondo);

    m_tex_logo.loadFromFile("src/world fight titulo del juego.png");
    m_spr_logo.setTexture(m_tex_logo);

    // Obtener resolución actual
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    float screenAncho  = static_cast<float>(desktop.width);
    float screenAlto = static_cast<float>(desktop.height);

    // Escala del fondo para cubrir pantalla
    sf::Vector2u bgSize = m_tex_fondo.getSize();
    float scaleX = screenAncho / bgSize.x;
    float scaleY = screenAlto / bgSize.y;
    float finalScale = std::max(scaleX, scaleY);
    m_spr_fondo.setScale(finalScale, finalScale);

    // Base para uiScale (ancho del fondo, sin hardcode)
    float baseWidth = static_cast<float>(bgSize.x);
    float uiScale = screenAncho / baseWidth;

    // Logo adaptado
    m_spr_logo.setScale(0.1859f * uiScale, 0.1859f * uiScale);
    m_spr_logo.setPosition(20.f * uiScale, 10.f * uiScale);

    // Tamaño base para botones
    float marginX = 30.f * uiScale;
    float marginY = 15.f * uiScale;

    // Textos
    m_t1.setFont(m_f1);
    m_t1.setString("Empezar a Jugar");
    m_t1.setCharacterSize(static_cast<unsigned>(15 * uiScale));
    m_t1.setFillColor(sf::Color::White);

    m_t2.setFont(m_f1);
    m_t2.setString("Tabla de clasificacion");
    m_t2.setCharacterSize(static_cast<unsigned>(15 * uiScale));
    m_t2.setFillColor(sf::Color::White);

    m_t3.setFont(m_f1);
    m_t3.setString("Como jugar / Instrucciones");
    m_t3.setCharacterSize(static_cast<unsigned>(15 * uiScale));
    m_t3.setFillColor(sf::Color::White);

    // Botones adaptados al texto (sin hardcode)
    boton1.setSize(sf::Vector2f(m_t1.getLocalBounds().width + marginX,
                                m_t1.getLocalBounds().height + marginY));
    boton1.setFillColor(sf::Color(212, 43, 43));
    boton1.setPosition(screenAncho * 0.07, screenAlto * 0.5);

    boton2.setSize(sf::Vector2f(m_t2.getLocalBounds().width + marginX,
                                m_t2.getLocalBounds().height + marginY));
    boton2.setFillColor(sf::Color(212, 43, 43));
    boton2.setPosition(screenAncho * 0.07, screenAlto * 0.6);

    boton3.setSize(sf::Vector2f(m_t3.getLocalBounds().width + marginX,
                                m_t3.getLocalBounds().height + marginY));
    boton3.setFillColor(sf::Color(212, 43, 43));
    boton3.setPosition(screenAncho * 0.07, screenAlto * 0.7);

    // Centrar textos dentro de los botones
    m_t1.setPosition(
        boton1.getPosition().x + (boton1.getSize().x - m_t1.getLocalBounds().width) * 0.5f,
        boton1.getPosition().y + (boton1.getSize().y - m_t1.getLocalBounds().height) * 0.5f
    );

    m_t2.setPosition(
        boton2.getPosition().x + (boton2.getSize().x - m_t2.getLocalBounds().width) * 0.5f,
        boton2.getPosition().y + (boton2.getSize().y - m_t2.getLocalBounds().height) * 0.5f
    );

    m_t3.setPosition(
        boton3.getPosition().x + (boton3.getSize().x - m_t3.getLocalBounds().width) * 0.5f,
        boton3.getPosition().y + (boton3.getSize().y - m_t3.getLocalBounds().height) * 0.5f
    );
}

Menu::~Menu() {} // Destructor vacío

void Menu::ProcessEvents(Juego &juego, Event &event)   // Maneja eventos del menú
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
    {
        juego.Exit(); // Si se presiona ESC, cierra el juego
    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(juego.GetWindow())); // Obtiene posición del mouse
        sf::FloatRect boton1Bounds = boton1.getGlobalBounds(); // Área del botón 1
        sf::FloatRect boton2Bounds = boton2.getGlobalBounds(); // Área del botón 2
        sf::FloatRect boton3Bounds = boton3.getGlobalBounds(); // Área del botón 3

        if (boton1Bounds.contains(mousePos))
        {
            juego.SetFondo(new Selector()); // Cambia a la escena de selección de personajes
        }
        else if(boton2Bounds.contains(mousePos))
        {
            juego.SetFondo(new Archivo("leaderboard.dat", 6000, false)); // Muestra leaderboard
        }
        else if(boton3Bounds.contains(mousePos))
        {
            juego.SetFondo(new Instrucciones()); // Muestra instrucciones
        }
    }
    else if(event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
        {
       switch (i)
            {
            case 0:
                juego.SetFondo(new Selector()); // Cambia a la escena de selección de personajes
                break;
            case 1:
                juego.SetFondo(new Archivo("leaderboard.dat", 6000, false)); // Muestra leaderboard
                break;
            case 2:
                juego.SetFondo(new Instrucciones()); // Muestra instrucciones
                break;
            default:
                break;
            }
        }
        else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Left ||event.key.code == sf::Keyboard::Up)
        {
            i--;
            if (i<0)
            {
                i = 0;
            }
        }
        else if (event.key.code == sf::Keyboard::D ||event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Right ||event.key.code == sf::Keyboard::Down)
        {
            i++;
            if (i > 2)
            {
                i = 2;
            }
        }

        boton1.setFillColor(sf::Color(212, 43, 43));
        boton2.setFillColor(sf::Color(212, 43, 43));
        boton3.setFillColor(sf::Color(212, 43, 43));
        switch (i)
        {
        case 0:
            boton1.setFillColor(sf::Color(255, 100, 100));
            break;
        case 1:
            boton2.setFillColor(sf::Color(255, 100, 100));
            break;
        case 2:
            boton3.setFillColor(sf::Color(255, 100, 100));
            break;
        default:
            break;
        }
    }
}

void Menu::Update(Juego &juego) {} // No hace nada en cada frame

void Menu::Draw(RenderWindow &window)  // Dibuja el menu
{
    window.clear({0,0,0});             // Limpia la pantalla con color negro
    window.draw(m_spr_fondo);
    window.draw(m_spr_logo);// Dibuja el fondo
    window.draw(boton1);              // Dibuja botón 1
    window.draw(boton2);              // Dibuja botón 2
    window.draw(boton3);              // Dibuja botón 3
    window.draw(m_t1);                 // Dibuja texto del botón 1
    window.draw(m_t2);                 // Dibuja texto del botón 2
    window.draw(m_t3);                 // Dibuja texto del botón 3
    window.display();                  // Muestra todo en pantalla
}
