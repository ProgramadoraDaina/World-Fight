#include "ParametroSprites.h"

void ParametroSprites::configurar(const std::string& nombre,
                                   sf::Sprite& sprite,
                                   sf::RectangleShape& hurtbox,
                                   sf::RectangleShape& hitbox,
                                   float& ratioHurtboxX,
                                   float& ratioHurtboxY,
                                   float& ratioHitboxX,
                                   float& ratioHitboxY)
{
    // Escala global Y (solo para referencia, NO aplicar aquí)
    // const float escalaGlobalY = 1.1f;

    if (nombre == "Ryu") {
        hurtbox.setSize({180.f, 350.f});
        hitbox.setSize({150.f, 135.f});
        ratioHurtboxX = 0.54f; ratioHurtboxY = 0.57f;
        ratioHitboxX  = 0.33f; ratioHitboxY  = 0.33f;
    }
    else if (nombre == "Camila") {
        hurtbox.setSize({210.f, 330.f});
        hitbox.setSize({175.f, 125.f});
        ratioHurtboxX = 0.63f; ratioHurtboxY = 0.49f;
        ratioHitboxX  = 0.30f; ratioHitboxY  = 0.30f;
    }
    else if (nombre == "Moon") {
        hurtbox.setSize({160.f, 320.f});
        hitbox.setSize({150.f, 150.f});
        ratioHurtboxX = 0.50f; ratioHurtboxY = 0.50f;
        ratioHitboxX  = 0.20f; ratioHitboxY  = 0.35f;
    }
    else if (nombre == "Joseph") {
        hurtbox.setSize({200.f, 390.f});
        hitbox.setSize({175.f, 125.f});
        ratioHurtboxX = 0.62f; ratioHurtboxY = 0.48f;
        ratioHitboxX  = 0.30f; ratioHitboxY  = 0.40f;
    }
    else {
        hurtbox.setSize({180.f, 350.f});
        hitbox.setSize({150.f, 135.f});
        ratioHurtboxX = 0.5f; ratioHurtboxY = 0.5f;
        ratioHitboxX  = 0.5f; ratioHitboxY  = 0.5f;
    }

    // Colores y contornos
    hurtbox.setFillColor(sf::Color(255, 0, 0, 150));
    hurtbox.setOutlineColor(sf::Color::Yellow);
    hurtbox.setOutlineThickness(2.f);

    hitbox.setFillColor(sf::Color(0, 0, 255, 150));
    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setOutlineThickness(2.f);
}

int ParametroSprites::direccionHorizontal(const sf::Sprite& sprite) const {
    const float sx = sprite.getScale().x;
    if (sx > 0.0f)  return -1; // izquierda
    if (sx < 0.0f)  return +1; // derecha
    return +1;
}
void ParametroSprites::aplicarDireccion(const std::string& nombre, sf::Sprite& sprite, bool izquierda) {
    const float escalaGlobalY = 1.4f;
    float escalaX = 1.0f;

    if (nombre == "Camila") {
        escalaX = 0.9f;
    }
    else if (nombre == "Ryu" || nombre == "Moon" || nombre == "Joseph") {
        escalaX = 1.3f;
    }

    // Si izquierda = true → positivo (mira a la izquierda)
    // Si izquierda = false → negativo (mira a la derecha)
    sprite.setScale(izquierda ? escalaX : -escalaX, escalaGlobalY);
}

// ParametroSprites.cpp
bool ParametroSprites::miraIzquierda(const sf::Sprite& sprite) const {
    return sprite.getScale().x > 0.0f;
}
bool ParametroSprites::miraDerecha(const sf::Sprite& sprite) const {
    return sprite.getScale().x < 0.0f;
}

int ParametroSprites::signoKnockback(const sf::Sprite& sprite) const {
    const float sx = sprite.getScale().x;

    // Comportamiento viejo:
    // Si el sprite mira a la IZQUIERDA (sx > 0), empuje hacia la DERECHA (+1).
    // Si el sprite mira a la DERECHA (sx < 0), empuje hacia la IZQUIERDA (-1).
    if (sx > 0.0f) return +1;  // scale.x positivo => empuje a la derecha
    if (sx < 0.0f) return -1;  // scale.x negativo => empuje a la izquierda

    return +1; // valor por defecto si sx == 0
}
