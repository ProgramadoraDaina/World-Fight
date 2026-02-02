
#include "MaquinaDeEstados.h"

// ====== ctor ======
MaquinaDeEstados::MaquinaDeEstados() = default;

// ====== estado actual ======
void MaquinaDeEstados::cambiar(Estado e)
{
    // --- BLOQUEO: si estoy en ATACAR_* y el lock está activo, NO aceptar cambios de estado ---
    if (m_lockAtaque && estado_ == m_estadoBloqueado) {
        // Acepto sólo permanecer en el mismo estado (avanza animación) o re-entrar al mismo
        if (e != m_estadoBloqueado) {
            return; // rechazo cualquier intento de salir de ATACAR_PUNIO/PATADA
        }
    }

    estado_ = e;
}

void MaquinaDeEstados::activarLockAtaquePara(Estado e) {
        if (e == Estado::ATACAR_PUNIO || e == Estado::ATACAR_PATADA) {
            m_lockAtaque = true;
            m_estadoBloqueado = e;
        }
    }
    void MaquinaDeEstados::desactivarLockAtaque() {
        m_lockAtaque = false;
        m_estadoBloqueado = Estado::QUIETO;
    }

bool MaquinaDeEstados::lockAtaqueActivo() const { return m_lockAtaque; }
    Estado MaquinaDeEstados::estadoBloqueado() const { return m_estadoBloqueado; }

Estado MaquinaDeEstados::actual() const
{
    return estado_;
}

// ====== setters de booleanos ======
void MaquinaDeEstados::setCanUsePunio(bool v)              { m_canUsePunio = v; }
void MaquinaDeEstados::setGolpeado(bool v)                 { m_golpeado = v; }
void MaquinaDeEstados::setShokeado(bool v)                 { m_shokeado = v; }
void MaquinaDeEstados::setCaidoMuerto(bool v)              { m_caido_muerto = v; }
void MaquinaDeEstados::setSaltando(bool v)                 { m_Saltando = v; }
void MaquinaDeEstados::setAgachado(bool v)                 { m_Agachado = v; }
void MaquinaDeEstados::setIsDePie(bool v)                  { m_isDePie = v; }
void MaquinaDeEstados::setTexWasChangedOnPatada(bool v)    { m_texWasChangedOnPatada = v; }
void MaquinaDeEstados::setTexWasChangeAtaque_Punio(bool v) { m_texWasChangeAtaque_Punio = v; }
void MaquinaDeEstados::setEstaAtacando(bool v)             { m_estaAtacando = v; }

// ====== getters de booleanos ======
bool MaquinaDeEstados::canUsePunio() const                 { return m_canUsePunio; }
bool MaquinaDeEstados::golpeado() const                    { return m_golpeado; }
bool MaquinaDeEstados::shokeado() const                    { return m_shokeado; }
bool MaquinaDeEstados::caidoMuerto() const                 { return m_caido_muerto; }
bool MaquinaDeEstados::saltando() const                    { return m_Saltando; }
bool MaquinaDeEstados::agachado() const                    { return m_Agachado; }
bool MaquinaDeEstados::isDePie() const
{
    return m_isDePie;
}
bool MaquinaDeEstados::texWasChangedOnPatada() const
{
    return m_texWasChangedOnPatada;
}
bool MaquinaDeEstados::texWasChangeAtaque_Punio() const
{
    return m_texWasChangeAtaque_Punio;
}
bool MaquinaDeEstados::estaAtacando() const
{
    return m_estaAtacando;
}

// ====== timers ======
void MaquinaDeEstados::iniciarCooldownPunio(float segundos)
{
    m_punioCooldownRestante = segundos;
    m_canUsePunio = false;
}

bool MaquinaDeEstados::puedeUsarPunio() const
{
    return m_canUsePunio;
}

void MaquinaDeEstados::iniciarShokeado(float segundos)
{
    m_shokeado = true;
    m_shokeadoRestante = segundos;
}

void MaquinaDeEstados::actualizarTimers(float dt)
{
    // cooldown del puño
    if (m_punioCooldownRestante > 0.f) {
        m_punioCooldownRestante -= dt;
        if (m_punioCooldownRestante <= 0.f) {
            m_punioCooldownRestante = 0.f;
            m_canUsePunio = true;
        }
    }

    // shokeado (stun temporal)
    if (m_shokeado) {
        m_shokeadoRestante -= dt;
        if (m_shokeadoRestante <= 0.f) {
            m_shokeado = false;
            cambiar(Estado::QUIETO);
            m_isDePie = true;
        }
    }
}

// ====== reglas de negocio ======
void MaquinaDeEstados::cancelarGolpeado()
{
    if (estado_ == Estado::GOLPEADO) {
        m_golpeado = false;
        cambiar(Estado::QUIETO);
        m_isDePie = true;
    }
}
void MaquinaDeEstados::EstadoInicial(){
cambiar(Estado::SALTO);
m_Saltando = true;
m_Agachado = false;
m_isDePie = false;
m_canUsePunio = false;
}
