#pragma once
#include "RutasSprites.h"
#include "Objeto.h"
#include "box.h"
#include "fondo_combate.h"
#include <SFML/Window/Keyboard.hpp>
#include<vector>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include "MaquinaDeEstados.h"
using namespace sf;
using namespace std;

class BarraDeVida;  // Declaración anticipada

class Jugador : public Objeto
{
private:
    MaquinaDeEstados obj;
    bool jugador_uno; // Indica si es el jugador 1 o el 2
    float lastScoreTime = 0.0f;
    string nombre;
    RutasSprites rutas_;
    string m_name;
    Keyboard::Key m_up, m_right, m_down, m_left, m_ataquePatada, m_ataquePunio;// Teclas

    // texturas del jugador
    Texture m_normalTex, m_jumpTex, m_basicAtkTex, m_AgacharTex, m_AtacarTex;
    sf::Sprite indicadorSpr;

    // Estados del jugador
    //bool m_Saltando;
    //bool m_Agachado;
    //bool m_isDePie; // Nuevo estado para indicar si está parado
    //bool m_golpeado = false;
    //bool m_shokeado = false;
    //bool m_caido_muerto = false;
    bool m_sePresionoAtacar;
    int m_PatadaDeployed;
    Box hurtbox;
    Box hitbox;

    float m_knockbackImpacto = 0.f;
    float m_knockbackTime = 0.f;
    float m_knockbackDuracion = 0.f;
    int m_knockbackDirection = 1; // +1 derecha, -1 izquierda

    bool m_wasAtaque_PunioPressed;
    //bool m_canUsePunio = false;
    int m_recoveryTime;
    int m_PunioDeployed = 0;
    float m_tiempoGolpeado = 0.f;
    float m_tiempoShokeado = 0.f;
    float m_SaltoSpeed;
    float VidaPorcentaje = 100.0;
    int m_puntaje = 0;
    int m_rondasGanadas = 0;
    Clock m_clock;
    Time m_deltaTime;
    //bool m_texWasChangedOnPatada = false;
    //bool m_texWasChangeAtaque_Punio = false;
    float tiempoPorFrame = 0.1f;
    float tiempoAcumulado = 0.f;
    size_t frameActual = 0;
    //bool m_estaAtacando = false;
    float hurtboxRatioX;  // 40% del ancho
    float hurtboxRatioY;  // 50% del alto
    float hitboxRatioX;    // 70% del ancho
    float hitboxRatioY;    // 20% del alto

std::map<Estado, std::vector<sf::Texture>> animaciones;

public:
    Jugador(bool jugador_uno, string name);
    void Update(Jugador& opponent);
    void SetVida(float perc);
    float GetVida();
    void restart();

    // Ataques
    void Ataque_Patada(Jugador& opponent);
    void Ataque_Punio(Jugador& opponent);

    bool CheckCollision(const Jugador& other) const;
    void SetDeltaTime(sf::Time deltaTime);
    void ValidateScreenLimits();
    void Golpeado();
    //void Agachar();
    void Morir();
    void Correr();
    void Celebrar();
    void Caer();
    void WonARound();
    int GetRondasGanadas() const;
    string GetScoreStr();
    int GetPuntaje();
    bool CanUseAtaquePunio() const;
    const string&GetNombre() const;
    void actualizarAnimacion(float deltaTime);

    bool getCaidoMuerto();
    sf::Vector2f getPosicion() const;
    void setPosicion(const sf::Vector2f& pos);
    void ubicarHurtbox();
    void ubicarHitbox();
    const sf::RectangleShape& getHurtbox() const;
    const sf::RectangleShape& getHitbox() const;
    void caerSuelo(const Fondo_combate& fondo);
    void setIndicadorTexture(const sf::Texture& tex);
    // Posicionar el indicador exactamente sobre la hurtbox
    void ubicarIndicador();
    const sf::Sprite& getIndicador() const;
    void setIndicadorScale(float sx, float sy);
void setEstado(Estado estado);
void agregarAnimacion(Estado estado, const std::vector<std::string>& rutas);

};
