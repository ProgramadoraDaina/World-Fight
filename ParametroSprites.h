
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
class ParametroSprites
{
public:
    ParametroSprites() = default;

    void configurar(const string& nombre, sf::Sprite& sprite, sf::RectangleShape& hurtbox, sf::RectangleShape& hitbox,
                    float& ratioHurtboxX,float& ratioHurtboxY, float& ratioHitboxX, float& ratioHitboxY);
    void aplicarDireccion(const std::string& nombre, sf::Sprite& sprite, bool izquierda);
    bool miraIzquierda(const sf::Sprite& sprite) const;
    bool miraDerecha(const sf::Sprite& sprite) const;
    int signoKnockback(const sf::Sprite& sprite) const;
    int direccionHorizontal(const sf::Sprite& sprite) const;

};
