#include "fondo_combate.h"
#include<iostream>
#include <SFML/graphics.hpp>
#include <string>

using namespace std;

void Fondo_combate::setCapa0() {}
void Fondo_combate::setCapa1() {}
void Fondo_combate::setCapa2() {}
void Fondo_combate::setCapa3() {}
void Fondo_combate::setCapaAdelante() {}

void Fondo_combate::setFondoCombate(sf::View& camara)
{
    int prob = rand() % 100;/**Generá un número entero pseudoaleatorio entre 0 y 99 y guardalo en prob*/

    tipo = 0;/**Inicializá la variable tipo con el valor 0 (estado neutro por defecto), antes de decidir si va a ser 1 o 2*/

    if (prob < 50)/**Elijo el tipo de escenario: si el número aleatorio es menor que 50, uso tipo 1; si no, uso tipo 2.
                  (50/50 de probabilidad)*/
    {
        tipo = 1;/**le otorga el valor 1*/
    }
    else
    {
        tipo = 2;
    }
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();/**Leo la resolución actual del escritorio (ancho/alto) para escalar
                                                            y posicionar correctamente texturas y cámara*/

    escenarioAncho = desktop.width + desktop.width / 4;/**Calculá el ancho lógico del escenario (dojo) como el ancho de la
                                                       pantalla más un 25% adicional*/

    camara.setSize(desktop.width, desktop.height);/**Ajustá el tamaño de la vista (sf::View) para que cubra exactamente el ancho
                                                  y el alto de la pantalla actual*/
    /// Capas de fondo para parallax
    // Capa 0, la mas lejana
    if (tipo == 1)/**Si la variante de escenario que elegí al azar es la 1*/
    {
        if (!tex[0].loadFromFile("sprites/dojo/cielo.png"))/**entonces voy a configurar la capa 0 (la más lejana) con la textura
                                                           del cielo*/
        {
            std::cerr << "Error al cargar cielo.png" << std::endl;
        }
    }
    else if (tipo == 2)/**Si la variante de escenario que elegí al azar es la 2*/
    {
        if (!tex[0].loadFromFile("sprites/dojo/1.png"))/**entonces voy a configurar la capa 0 (la más lejana) con la textura
                                                       de bosques de bambu*/
        {
            std::cerr << "Error al cargar 1.png" << std::endl;
        }
    }
    capasAtras[0].setTexture(tex[0]);/**este sprite (capasAtras[0]) va a usar esta imagen (tex[0]) como su textura para
                                      dibujarse en pantalla*/
    scaleXY = {capasAtras[0].getGlobalBounds().width, capasAtras[0].getGlobalBounds().height};/**guarda en scale XY la medida x
                                                                                             e y de el cielo o bosques de bambu*/
    scaleXY = {(desktop.width/scaleXY.x)*1.25f, desktop.height/scaleXY.y};/**calcula cuánto debe escalarse el sprite en ancho y
                                                                          alto para ajustarse al tamaño de la pantalla,
                                                                          aumentando el ancho un 25% extra para que se vea más
                                                                          panorámico*/
    capasAtras[0].setScale(scaleXY);/**aplica al sprite la escala calculada en scaleXY, para que cambie su tamaño según los
                                    factores que acabamos de obtener*/
    capasAtras[0].setPosition(
    (desktop.width - capasAtras[0].getGlobalBounds().width) / 2.0f,/**Resto el ancho del sprite al ancho de la pantalla para
                                                                   saber cuánto espacio sobra, y lo divido por dos para poner la
                                                                   misma cantidad de espacio a la izquierda y a la derecha,
                                                                   logrando que el sprite quede centrado*/
    (desktop.height - capasAtras[0].getGlobalBounds().height) / 2.0f);/**Resto la altura del sprite a la altura de la pantalla
                                                                     para saber cuánto espacio sobra, y divido ese espacio por
                                                                     dos para repartirlo arriba y abajo, logrando que el sprite
                                                                     quede centrado verticalmente*/

    // Capa 1, medio lejana
    if (tipo == 1)/**si el escenario/estado es el tipo 1*/
    {
        if (!tex[1].loadFromFile("sprites/dojo/arboles.png"))/**entonces voy a configurar la capa 1 con la textura de arboles*/
        {
            std::cerr << "Error al cargar arboles.png" << std::endl;
        }
        capasAtras[1].setTexture(tex[1]);/**este sprite (capasAtras[1]) va a usar esta imagen (tex[1]) como su textura para
                                      dibujarse en pantalla*/
        scaleXY = {capasAtras[1].getGlobalBounds().width, capasAtras[1].getGlobalBounds().height};/**mide el ancho y el alto
                                                                                                visibles del sprite capasAtras[1]
                                                                                               (en píxeles, tal como se ve ahora
                                                                                                mismo) y guarda esos dos números
                                                                                                dentro del vector scaleXY*/
        scaleXY = {(desktop.width/scaleXY.x)*0.8f, (desktop.width/scaleXY.x)*0.8f};/**calcula un factor de escala uniforme para
                                                                                  que el sprite se ajuste al ancho de la pantalla
                                                                                  y lo reduce al 80% para que no ocupe todo el
                                                                                  espacio*/
        capasAtras[1].setScale(scaleXY);/**aplica al sprite la escala contenida en scaleXY para cambiar su tamaño en X e Y según
                                        los factores que acabás de calcular (uniforme al 80% del ancho de la pantalla en tu caso)*/

        capasAtras[1].setPosition(0,/**pone el sprite pegado al borde izquierdo (x = 0) y*/
        (desktop.height - capasAtras[1].getGlobalBounds().height) / 2.0f);/**centra verticalmente calculando la posición y como
                                                                         la mitad del espacio sobrante:
                                                                         (alturaPantalla - alturaSprite) / 2*/
    }
    else if (tipo == 2)
    {
        if (!tex[1].loadFromFile("sprites/dojo/2.png"))
        {
            std::cerr << "Error al cargar 2.png" << std::endl;
        }
        capasAtras[1].setTexture(tex[1]);
        scaleXY = {capasAtras[1].getGlobalBounds().width, capasAtras[1].getGlobalBounds().height};
        scaleXY = {(desktop.width/scaleXY.x)*1.25f, desktop.height/scaleXY.y};
        capasAtras[1].setScale(scaleXY);
        capasAtras[1].setPosition((desktop.width - capasAtras[1].getGlobalBounds().width) / 2.0f, (desktop.height - capasAtras[1].getGlobalBounds().height) / 2.0f);
    }


    // Capa 2, cercana
    if (tipo == 1)
    {
        tex[2].loadFromFile("sprites/dojo/dojo.png");
        if (!tex[2].loadFromFile("sprites/dojo/dojo.png"))
        {
            std::cerr << "Error al cargar dojo.png" << std::endl;
        }
        capasAtras[2].setTexture(tex[2]);
        scaleXY = {capasAtras[2].getGlobalBounds().width, capasAtras[2].getGlobalBounds().height};
        scaleXY = {(desktop.width/scaleXY.x)*1.35f, (desktop.width/scaleXY.x)*2.f};
        capasAtras[2].setScale(scaleXY);
        capasAtras[2].setOrigin(0,0);
        capasAtras[2].setPosition((escenarioAncho - capasAtras[2].getGlobalBounds().width) / 2.0f, (desktop.height - capasAtras[2].getGlobalBounds().height) / 2.0f);
    }
    else if (tipo == 2)
    {
        tex[2].loadFromFile("sprites/dojo/3.png");
        if (!tex[2].loadFromFile("sprites/dojo/3.png"))
        {
            std::cerr << "Error al cargar 3.png" << std::endl;
        }
        capasAtras[2].setTexture(tex[2]);
        scaleXY = {capasAtras[2].getGlobalBounds().width, capasAtras[2].getGlobalBounds().height};
        scaleXY = {(desktop.width/scaleXY.x)*1.25f, desktop.height/scaleXY.y};
        capasAtras[2].setScale(scaleXY);
        capasAtras[2].setOrigin(0,0);
        capasAtras[0].setPosition((desktop.width - capasAtras[0].getGlobalBounds().width) / 2.0f, (desktop.height - capasAtras[0].getGlobalBounds().height) / 2.0f);
    }


    // Capa 3, textura del suelo(piso)
    if (tipo == 1)
    {
        tex[3].loadFromFile("sprites/dojo/piso.png");
        if (!tex[3].loadFromFile("sprites/dojo/piso.png"))
        {
            std::cerr << "Error al cargar piso.png" << std::endl;
        }
        capasAtras[3].setTexture(tex[3]);
        capasAtras[3].setScale(1,1.3);
    }
    else if (tipo == 2)
    {
        tex[3].loadFromFile("sprites/dojo/4.png");
        if (!tex[3].loadFromFile("sprites/dojo/4.png"))
        {
            std::cerr << "Error al cargar 4.png" << std::endl;
        }
        capasAtras[3].setTexture(tex[3]);
        capasAtras[3].setScale(1,1.7);
    }


    // Suelo tangible(hit-box?)
    suelo.setSize(sf::Vector2f(escenarioAncho*1.25, capasAtras[3].getGlobalBounds().height - 1));
    suelo.setFillColor(sf::Color(0,0,0,0));
    float bordeInferiorDojo = capasAtras[2].getGlobalBounds().top + capasAtras[2].getGlobalBounds().height;
    float alturaSuelo = capasAtras[3].getGlobalBounds().height;
    suelo.setPosition(-escenarioAncho*0.12, bordeInferiorDojo - alturaSuelo - (alturaSuelo * 0.9f));
    if (tipo == 2)
    {
        suelo.setPosition(-escenarioAncho*0.12, bordeInferiorDojo - alturaSuelo * 0.7);
    }
    /// Capa 0 de las texturas Delanteras(sobre los jugadores) decorativo
    if (tipo == 1)
    {
        tex[4].loadFromFile("sprites/dojo/luz.png");
        if (!tex[4].loadFromFile("sprites/dojo/luz.png"))
        {
            std::cerr << "Error al cargar luz.png" << std::endl;
        }
        capasAdelante[0].setTexture(tex[4]);
        scaleXY = {capasAdelante[0].getGlobalBounds().width, capasAdelante[0].getGlobalBounds().height};
        scaleXY = {(desktop.width/scaleXY.x)*0.6f, (desktop.width/scaleXY.x)*0.74f};
        capasAdelante[0].setScale(scaleXY);
        capasAdelante[0].setOrigin(0,0);
        capasAdelante[0].setPosition((capasAtras[2].getGlobalBounds().width - capasAdelante[0].getGlobalBounds().width), capasAtras[2].getGlobalBounds().top);
    }
    else if (tipo == 2)
    {
        tex[4].loadFromFile("sprites/dojo/5.png");
        if (!tex[4].loadFromFile("sprites/dojo/5.png"))
        {
            std::cerr << "Error al cargar 5.png" << std::endl;
        }
        capasAdelante[0].setTexture(tex[4]);
        scaleXY = {capasAdelante[0].getGlobalBounds().width, capasAdelante[0].getGlobalBounds().height};
        scaleXY = {(desktop.width/scaleXY.x)*0.6f, (desktop.width/scaleXY.x)*0.74f};
        capasAdelante[0].setScale(scaleXY);
        capasAdelante[0].setOrigin(0,0);
        capasAdelante[0].setPosition((capasAtras[2].getGlobalBounds().width - capasAdelante[0].getGlobalBounds().width), capasAtras[2].getGlobalBounds().top);
    }

    margenes_negros.setSize(sf::Vector2f(escenarioAncho*1.5, desktop.height/4));
    margenes_negros.setFillColor(sf::Color(0,0,0));
}

void Fondo_combate::setSuelo() {}
void Fondo_combate::updateFondo(float& camaraX, sf::VideoMode& desktop)
{
    capasAtras[0].setPosition(camaraX * 1.2f - desktop.width/1.3, capasAtras[0].getPosition().y);
    if (tipo == 1)
    {
        capasAtras[1].setPosition(camaraX * 0.7 - desktop.width/4.25, desktop.height * 0.4f);
    }
    else
    {
        capasAtras[1].setPosition(camaraX * 0.7 - desktop.width/2.25, capasAtras[1].getPosition().y);
    }
}

const sf::Sprite& Fondo_combate::getCapasAtras(int i)
{
    return capasAtras[i];
}

const sf::Sprite& Fondo_combate::getCapasAdelante(int i)
{
    return capasAdelante[i];
}

sf::FloatRect Fondo_combate::getSuelo() const
{
    return suelo.getGlobalBounds();
}

const sf::RectangleShape& Fondo_combate::getSueloRect() const
{
    return suelo;
}

sf::FloatRect Fondo_combate::getMLeft()
{
    return margen_left.getGlobalBounds();
}

sf::FloatRect Fondo_combate::getMRight()
{
    return margen_right.getGlobalBounds();
}

void Fondo_combate::drawCapasAtras(sf::RenderWindow& window)
{
    for (int i=0; i<3; i++)
    {
        window.draw(capasAtras[i]);
    }
    int tam = (int)(escenarioAncho / capasAtras[3].getGlobalBounds().width);
    for (int i=0; i<=tam+3; i++)
    {
        float ySuelo = capasAtras[2].getGlobalBounds().top + capasAtras[2].getGlobalBounds().height - capasAtras[3].getGlobalBounds().height;
        capasAtras[3].setPosition(i * capasAtras[3].getGlobalBounds().width * 0.9 - 100, ySuelo);
        window.draw(capasAtras[3]);
    }
    //margenes negros superior e inferior
    margenes_negros.setOrigin(0, margenes_negros.getGlobalBounds().height);
    margenes_negros.setPosition(escenarioAncho*-0.2, capasAtras[2].getGlobalBounds().top);
    window.draw(margenes_negros);
    margenes_negros.setOrigin(0, 0);
    margenes_negros.setPosition(escenarioAncho*-0.2, capasAtras[2].getGlobalBounds().top + capasAtras[2].getGlobalBounds().height);
    if (tipo == 2)
    {
        margenes_negros.setPosition(escenarioAncho*-0.2, capasAtras[2].getGlobalBounds().top + capasAtras[2].getGlobalBounds().height - capasAtras[3].getGlobalBounds().height + capasAtras[3].getGlobalBounds().height * 0.9);
    }
    window.draw(margenes_negros);
}

void Fondo_combate::drawCapasAdelante(sf::RenderWindow& window)
{
    for (int i=0; i<1; i++)
    {
        if (tipo == 1)
        {
            window.draw(capasAdelante[i]);
        }
        else
        {
            capasAdelante[0].setPosition((capasAtras[2].getGlobalBounds().width - capasAdelante[0].getGlobalBounds().width), capasAtras[2].getGlobalBounds().top);
            window.draw(capasAdelante[i]);
            capasAdelante[0].setPosition((capasAtras[2].getGlobalBounds().width / 1.7 - capasAdelante[0].getGlobalBounds().width), capasAtras[2].getGlobalBounds().top);
            window.draw(capasAdelante[i]);
        }
    }
}

void setSuelo();
void updateFondo();
