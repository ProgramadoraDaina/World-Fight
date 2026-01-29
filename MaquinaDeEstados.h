#ifndef MAQUINADEESTADOS_H_INCLUDED
#define MAQUINADEESTADOS_H_INCLUDED
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;
enum class Estado
{
    QUIETO,
    CORRIENDO,
    SALTO,
    AGACHADO,
    ATACAR_PATADA,
    ATACAR_PUNIO,
    GOLPEADO,
    SHOKEADO,
    MUERTE,
    CAYENDO,
    CELEBRANDO
};
// Máquina de estados + animaciones/fotogramas
class MaquinaDeEstados
{
public:

    MaquinaDeEstados();
    void actualizar(float dt);
    void aplicar(sf::Sprite& sprite) const;
    Estado actual() const;
    bool terminoCiclo() const;
    bool estaEnUltimoFrame() const;
    void entrarGolpeadoAleatorio();
    void iniciarCooldownPunio(float duracionSegundos);
    void cancelarGolpeado();
    void activarAtaque(Estado ataque);
    void entrarShokeado(float duracionSegundos);
    bool puedeUsarPunio() const;
    void setSaltando(bool v);
    void setGolpeado(bool v);
    void cambiar(Estado e);
    void iniciarShokeado(float segundos);
    void setShokeado(bool v);
    void setCanUsePunio(bool v);
    void setCaidoMuerto(bool v);
    bool canUsePunio() const;
    bool texWasChangedOnPatada() const;
    bool texWasChangeAtaque_Punio() const;
    bool isDePie() const;
    void actualizarTimers(float dt);
    bool estaAtacando() const;
    bool caidoMuerto() const;
    bool saltando() const;
    bool golpeado() const;
    void setEstaAtacando(bool v);
    void setAgachado(bool v);
    void setTexWasChangeAtaque_Punio(bool v);
    bool agachado() const;
    void setIsDePie(bool v);
    bool shokeado() const;
    void setTexWasChangedOnPatada(bool v);
    void activarLockAtaquePara(Estado e);
    void desactivarLockAtaque();
    bool lockAtaqueActivo() const;
    Estado estadoBloqueado() const;
    void DePie();
    void agacharse();
private:
    Estado estado_{Estado::QUIETO};
    float multiplicadorVelocidad(Estado estado) const;
    bool esEstadoCongelado(Estado estado) const;
    std::map<Estado, std::vector<sf::Texture>> animaciones_;
    Estado estado_actual_{Estado::QUIETO};
    std::size_t frame_actual_{0};
    float tiempo_acumulado_{0.f};
    float tiempo_por_frame_{0.1f};
    bool termino_ciclo_{false};
    bool ultimo_frame_{false};
    bool esta_atacando_{false};
    bool  shokeado_activo_{false};
    float punio_cooldown_restante_{0.f};
    float shokeado_restante_{0.f};

    bool m_canUsePunio = false;
    bool m_golpeado = false;
    bool m_shokeado = false;
    bool m_caido_muerto = false;
    bool m_Saltando;
    bool m_Agachado;
    bool m_isDePie;
    bool m_texWasChangedOnPatada = false;
    bool m_texWasChangeAtaque_Punio = false;
    bool m_estaAtacando = false;
    bool m_attackLockActive = false;
    float m_punioCooldownRestante    = 0.f;
    float m_shokeadoRestante         = 0.f;
    bool   m_lockAtaque = false;
    Estado m_estadoBloqueado = Estado::QUIETO;
};



#endif // MAQUINADEESTADOS_H_INCLUDED
