#include "Jugador.h"
#include "RutasSprites.h"
#include <sstream> // Incluye librer�as necesarias para gr�ficos, sonido y l�gica del jugador
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParametroSprites.h"
#include "MaquinaDeEstados.h"
#include "ControlesJugador.h"

using namespace sf; // Permite usar clases y funciones sin prefijo (sf, std)
using namespace std; // Permite usar clases y funciones sin prefijo (sf, std)

Jugador::Jugador(bool Jugador_uno, string name)
    : jugador_uno(Jugador_uno), m_name(name)
{
    rutas_.cargarPNGS(m_name, *this);/**cargo todas las texturas/frames de animación del personaje según su nombre,y las registro
                                     dentro de este Jugador (se usa Rutas.cpp)*/

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();/**obtengo la resolución de pantalla actual para calcular posiciones
                                                            y límites*/
    float escenarioAncho = desktop.width + desktop.width / 4;/**defino el ancho lógico del dojo (un poco mayor que el ancho del
                                                             monitor)*/
    float posicionX;/**creo una variable para calcular la X inicial del sprite*/

    ParametroSprites configurador;/**creo el helper que sabe cómo escalar/orientar el sprite y configurar las cajas
                                    (hurtbox/hitbox) según el personaje (m_name)*/

    // Posición inicial según jugador
    if (Jugador_uno) { /**si es el jugador 1*/
        posicionX = (escenarioAncho / 5) * 1.52f;/**lo ubico más a la izquierda del dojo*/
        m_sprite.setPosition(posicionX, 0);/**coloco el sprite del jugador en (posicionX, 300) y la altura 300, asi aparecera
                                             en el aire y caerá*/
        controles.aplicarPreset(0);
        configurador.aplicarDireccion(m_name, m_sprite, false);/**aplico orientación para que P1 mire a la derecha*/
    }
    else {/**como no es jugador 1 es jugador 2*/
        posicionX = (escenarioAncho / 5) * 3.4f;/**calculo la X inicial del Jugador 2 colocándolo hacia la derecha del dojo*/
        m_sprite.setPosition(posicionX, 0);/**ubico el sprite del Jugador 2 en (posicionX, 300); también empezará en altura y
                                             caerá por la física del juego*/
        controles.aplicarPreset(1);
        configurador.aplicarDireccion(m_name, m_sprite, true);/**aplico orientación para que P2 mire a la izquierda*/
    }

    // Pone el centro del sprite como origen
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, 0.f);/**posiciono el origen del m_sprite en el centro del
                                                                   eje x y arriba de la y*/


    // Variables iniciales
    obj.EstadoInicial();/**arranco con la bandera de “saltando” activa; después se acomodará al tocar suelo
                      (tu lógica de caer/hurtbox con el suelo)*/

    m_SaltoSpeed = 0.0f;/**velocidad vertical inicial del salto (se actualizará por física)*/

    m_sePresionoAtacar = false;/**guardo si la tecla de patada estuvo presionada en el frame actual; sirve para detectar el
                                “flanco” y evitar repeticiones mientras se mantiene pulsada*/

    // Configurar hurtbox y hitbox
    sf::RectangleShape boxHurt;/**creo rectángulos temporales para definir la hurtbox*/
    sf::RectangleShape boxHit;/**creo rectángulos temporales para definir la hitbox*/

    configurador.configurar(m_name, m_sprite, boxHurt, boxHit,
                            hurtboxRatioX, hurtboxRatioY, hitboxRatioX, hitboxRatioY);/**según el personaje (m_name) y el sprite
                                                                                      actual:
                                                                                - seteo tamaño/base de hurtbox/hitbox,
                                                                                - calculo ratios de anclaje (porcentaje dentro
                                                                                del sprite) para ubicarlas correctamente,
                                                                                - actualizo las referencias ratioHurtboxX/Y y
                                                                                ratioHitboxX/Y para usarlas luego al mover/flip*/

    hurtbox.setBox(boxHurt);/**Esta línea asigna la caja de daño (hurtbox) al jugador. Es la zona donde el personaje puede
                            recibir golpes*/
    hitbox.setBox(boxHit);/**Esta línea asigna la caja de ataque (hitbox) al jugador. Es la zona donde el personaje puede
                          dar golpes*/
    VidaPorcentaje = 100;
}
void Jugador::Update(Jugador& opponent)
{
    if (VidaPorcentaje > 0 && opponent.VidaPorcentaje > 0)/**Si alguno está sin vida osea vida no mayor a cero, no hago lógica de combate*/
    {
        ValidateScreenLimits();/**Primero, mantengo al jugador dentro de la pantalla con ValidateScreenLimits()*/
        {
            bool anyKey = controles.anyKey();

            float tiempoActual = m_clock.getElapsedTime().asSeconds();/**uso el tiempoActual en segundos para comparar duraciones
                                                                      de estados*/

            if (obj.golpeado())/**si estoy golpeado y el jugador toca cualquier tecla, cancelo el estado golpeado*/
            {
                if (anyKey)/**Si toca alguna tecla, cancelo el golpeado con false*/
                {
                    obj.cancelarGolpeado();
                    m_knockbackTime = 0.f;
                }
                else if ((tiempoActual - m_tiempoGolpeado) < 1.0f)/**si NO tocó tecla y todavía estoy dentro de 1 segundo de golpeado*/
                {
                    if (m_knockbackTime < m_knockbackDuracion)/**aplico empuje solo mientras el progreso (m_knockbackTime) sea
                                                              menor que la duración total (m_knockbackDuration)*/
                    {
                        m_knockbackTime += 0.1f;/**sumo 0.1 al tiempo acumulado del knockback en este frame*/

                        float factor = (m_knockbackDuracion - m_knockbackTime) / m_knockbackDuracion;/**calculo un factor que empieza en 1 y
                                                                                                     cae hacia 0 a medida que se consume la duración*/

                        float velocidad = m_knockbackImpacto * factor;/**básicamente dice: la velocidad instantánea del
                                                                     empuje = fuerza base * factor (cada vez menor)*/

                        m_sprite.move(m_knockbackDirection * velocidad * 0.1f, 0);/**muevo el sprite en X según la dirección del
                                                                                 empuje y esa velocidad, escalada por 0.1 para
                                                                                 que no sea excesiva por frame*/
                    }

                    ubicarHitbox();/**sincronizo hitbox/hurtbox con la nueva posición tras el empuje*/
                    ubicarHurtbox();
                    return; /** Bloqueamos el resto del Update por este frame*/
                }
                else
                {
                    obj.cancelarGolpeado();
                    m_knockbackTime = 0.f;
                }
            }
        }
        float escenarioAncho = sf::VideoMode::getDesktopMode().width;/**obtengo ancho del escenario*/

        float distanciaMaxima = escenarioAncho * 0.97f;/**y una distancia máxima permitida entre los jugadores*/

        float distanciaActual = std::abs(m_sprite.getPosition().x - opponent.getPosicion().x);/**distancia actual en X entre
                                                                                               mi posición y la del oponente
                                                                                               usando abs para obtener el valor
                                                                                               absoluto*/
        if (!obj.shokeado())/**entramos si no estamos shokeados*/
        {
            // Iniciar salto
            if (controles.arriba() && !obj.saltando())/**si aprieto "arriba" y no estoy saltando, inicio salto
                                                            con velocidad hacia arriba*/
            {

                obj.setSaltando(true);
                obj.setIsDePie(false);
                m_SaltoSpeed = -20.0f;
                setEstado(Estado::SALTO);
            }

            // Actualizar salto
            if (obj.saltando())/**si estoy en salto, me elevo aumentando la velocidad*/
            {
                m_sprite.move(0, m_SaltoSpeed);
                m_SaltoSpeed += 0.7f;/**si esto es cero, el personaje no salta, y tampoco camina, pero si se mueve en x*/
            }

            if (!obj.estaAtacando())/**solo puedo iniciar un ataque si no estoy ya atacando*/
            {
                if (controles.patada() && !m_sePresionoAtacar &&(obj.isDePie() || obj.saltando() || obj.actual() == Estado::CORRIENDO) &&
                        obj.actual() != Estado::ATACAR_PATADA)/**Si todas son verdad se ejecuta la patada.*/
                {
                    obj.setEstaAtacando(true);
                    setEstado(Estado::ATACAR_PATADA);
                    obj.activarLockAtaquePara(Estado::ATACAR_PATADA);
                    Ataque_Patada(opponent);
                }
                m_sePresionoAtacar = controles.patada();/**guardo el estado actual de la tecla del ataque patada para comparar
                                                         en el siguiente frame*/

                if (controles.punio() && !m_wasAtaque_PunioPressed &&
                        (obj.isDePie() || obj.saltando() || obj.actual() == Estado::CORRIENDO) &&
                        obj.actual() != Estado::ATACAR_PUNIO)/**si todas son verdad se ejecuta el ataque_punio*/
                {
                    obj.setEstaAtacando(true);
                    setEstado(Estado::ATACAR_PUNIO);
                    obj.activarLockAtaquePara(Estado::ATACAR_PUNIO);
                    Ataque_Punio(opponent);
                    obj.iniciarCooldownPunio(3.0f);
                }
                m_wasAtaque_PunioPressed = controles.punio();/**guardo el estado actual de la tecla de puño para comparar
                                                                  en el próximo frame*/
            }

            // Movimiento y estado
            float cant_pasos = escenarioAncho * 0.0042;/**defino cuántos píxeles me muevo por paso (en función del ancho del
                                                        escenario)*/
            ParametroSprites configurador;/** creo el configurador que ajusta la dirección (flip del sprite) según la
                                             orientación*/

            if (controles.izquierda())/**ir a la izquierda*/
            {
                if (!obj.saltando() && !obj.estaAtacando())/**si NO estoy saltando y NO estoy atacando,lo pongoen  estado "CORRIENDO"*/
                {
                    Correr();
                }

                configurador.aplicarDireccion(m_name, m_sprite, true); /**hago que el sprite mire hacia la IZQUIERDA (aplicar
                                                                      flip si corresponde), ya que si  es true mira izquierda
                                                                      y si es false mira derecha*/
                if (distanciaActual < distanciaMaxima || m_sprite.getPosition().x > opponent.getPosicion().x)/**me muevo a la
                                                                                                         izquierda SOLO si estoy
                                                                                                         dentro del rango Y estoy
                                                                                                        a la derecha del oponente*/
                {
                    if (!obj.estaAtacando() || (obj.estaAtacando() && obj.saltando()))/**solo me muevo si no estoy atacando o si estoy
                                                                         atacando mientras salto*/
                    {
                        m_sprite.move(-cant_pasos, 0);
                    }
                }
            }
            else if (controles.derecha())/**ir a la izquierda*/
            {
                if (!obj.saltando() && !obj.estaAtacando())/**si NO estoy saltando y NO estoy atacando,lo pongo en  estado "CORRIENDO"*/
                {
                    Correr();
                }

                configurador.aplicarDireccion(m_name, m_sprite, false); /**como es false gira a la derecha si es necesario*/

                if (distanciaActual < distanciaMaxima || m_sprite.getPosition().x < opponent.getPosicion().x)/**me muevo a la
                                                                                                         derecha SOLO si estoy
                                                                                                         dentro del rango Y estoy
                                                                                                        a la izquierda del oponente*/
                {
                    if (!obj.estaAtacando() || (obj.estaAtacando() && obj.saltando()))/**lo mismo que en la izquierda, solo me muevo si
                                                                            no estoy atacando o si ataco mientras salto*/
                    {
                        m_sprite.move(cant_pasos, 0);
                    }
                }
            }
            else if (!obj.saltando())/**si no estoy saltando*/
            {
                bool presionandoAtaque = controles.patada() || controles.punio();
                if (controles.abajo())/**si el jugador está apretando Abajo (m_down), pongo el estado AGACHADO*/
                {
                    Agachar();
                }
                else if (!presionandoAtaque)/**si no estoy agachándome y no estoy apretando ningún ataque (patada/puño), me
                                            pongo de pie (QUIETO)*/
                {
                    Depie();
                }
                obj.puedeUsarPunio();
            }
        }

        // Manejo shokeado
        if (obj.shokeado())/**si esta shokeado*/
        {
            float tiempoActual = m_clock.getElapsedTime().asSeconds();/**leo el tiempo actual del reloj del juego para
                                                                               comparar cuánto llevo shokeado*/
            if (tiempoActual - m_tiempoShokeado >= 2.0f)/**si pasó 2 segundos o más desde que entré a shokeado, salgo del stun
                                                        (deja de estar inmovilizado)*/
            {
                obj.setShokeado(false);
                setEstado(Estado::QUIETO);
                obj.setIsDePie(true);
            }
        }
        m_deltaTime = m_clock.getElapsedTime();/**Actualizo m_deltaTime para tener el tiempo actual disponible en otros
                                                    métodos (por ejemplo, animación, físicas, etc.).*/
    }

    // Knockback continuo en MUERTE / CAYENDO
    if (m_knockbackTime < m_knockbackDuracion && (obj.actual() == Estado::MUERTE || obj.actual() == Estado::CAYENDO))/**Si estoy en los
                                                                                                       estados MUERTE o CAYENDO
                                                                                                      y todavía no terminó el
                                                                                                      tiempo del knockback*/
    {
        m_knockbackTime += 0.1f;/**Avanzo el progreso del knockback en pasos de 0.1 (unidad lógica por frame).*/
        float factor = (m_knockbackDuracion - m_knockbackTime) / m_knockbackDuracion;/**Calculo un factor que va de 1 al 0:
                                                                                   al inicio el empuje es grande y se apaga
                                                                                 linealmente hasta cero cuando se consume la
                                                                                  duración*/
        float velocidad = m_knockbackImpacto * factor;/**La velocidad instantánea del empuje es la fuerza base multiplicada
                                                       por ese factor decreciente*/
        m_sprite.move(m_knockbackDirection * velocidad * 0.2f, 0);/**Desplazo el sprite en X según la dirección
                                                                (m_knockbackDirection = +1 derecha, −1 izquierda),
                                                               escalando por 0.2 para que el desplazamiento por frame
                                                               no sea excesivo*/
        ValidateScreenLimits();
    }
    // Actualizar puntaje
    float currentTime = m_clock.getElapsedTime().asSeconds();/**Cada 1 segundo que pasa desde el último registro (lastScoreTime),
                                                             sumo 100 puntos al score y actualizo lastScoreTime*/
    if (currentTime - lastScoreTime >= 1.0f)
    {
        m_puntaje += 100;
        lastScoreTime = currentTime;
    }
    // Actualizar animación y hitboxes
    actualizarAnimacion(0.02f);/**Avanzo la animación del estado actual usando un paso fijo (0.02)*/

    ubicarHitbox();/**Recoloco las cajas de golpe (hitbox) y cajas de daño (hurtbox) según la nueva
                    posición/escala del sprite para que esten perfectamente alineadas*/
    ubicarHurtbox();
}
sf::Vector2f Jugador::getPosicion() const
{
    return m_sprite.getPosition();/**devuelvo la posición actual del m_sprite del jugador en el mundo*/
}

void Jugador::setPosicion(const sf::Vector2f& pos)
{
    m_sprite.setPosition(pos);/**seteo la posición del sprite del jugador a la coordenada recibida, es el setter: mueve al
                              personaje a (pos.x, pos.y)*/
}
void Jugador::ubicarHurtbox()
{
    RectangleShape box = hurtbox.getBox();/**copio la forma (rectángulo) que actualmente representa la hurtbox del jugador,
                                          Trabajo sobre una copia local para ajustar origen/posición y luego la vuelvo a guardar*/

    box.setOrigin(box.getSize().x / 2, box.getSize().y / 2);/**centro el origen de la hurtbox*/

    sf::FloatRect bounds = m_sprite.getGlobalBounds();/**obtengo los límites globales del sprite (posición, ancho, alto después
                                                      de transformaciones)*/

    // Si escala X < 0, reflejamos la proporción
    float ratioX = (m_sprite.getScale().x < 0) ? (1.f - hurtboxRatioX) : hurtboxRatioX;/**si el sprite está volteado en modo
                                                                                      espejo horizontalmente (scale.x < 0),
                                                                                      invierto el ratio X para que la hurtbox
                                                                                      se mantenga correctamente alineada con el
                                                                                      “cuerpo”*/

    float posX = bounds.left + bounds.width * ratioX;
    float posY = bounds.top + bounds.height * hurtboxRatioY;/**Estas líneas calculan la posición de la hurtbox dentro del sprite
                                                            usando porcentajes. Así la caja siempre queda en el lugar correcto
                                                            del cuerpo, aunque el personaje cambie de tamaño o se voltee*/

    box.setPosition(posX, posY);/**pongo la hurtbox (con origen centrado) en la posición calculada*/

    hurtbox.setBox(box);/**guardo la hurtbox actualizada dentro del jugador, para que otras partes del sistema
                        (colisiones, debug, lógica de suelo) usen la versión correcta*/

    ubicarIndicador();/**reposiciono el indicador (P1/P2) por encima de la hurtbox actualizada, para que el icono siga
                      al “cuerpo” del jugador aunque se mueva o se espeje*/
}
void Jugador::ubicarIndicador()
{
    const auto gb = hurtbox.getBox().getGlobalBounds();/**Obtengo los límites globales (posición y tamaño) de la hurtbox actual.
                                                       Esto me da left, top, width y height ya con escala y posición aplicadas*/

    const float centerX = gb.left + gb.width * 0.5f;/**Calculo el centro horizontal de la hurtbox*/

    const float topY    = gb.top - gb.top * 0.1f;/**Guardo la coordenada Y del borde superior de la hurtbox. Esto será mi referencia para
                                  colocar el indicador arriba*/

    // Tamaño real del indicador con transformaciones aplicadas
    const float indW = indicadorSpr.getGlobalBounds().width;/**Obtengo el ancho real del sprite del indicador (ya escalado)
                                                            para poder centrarlo correctamente*/

    const float indH = indicadorSpr.getGlobalBounds().height;/**Obtengo el alto real del sprite del indicador (ya escalado)
                                                             para saber cuánto espacio ocupa verticalmente*/

    const float marginTop = 4.0f;/**Defino un margen fijo (4 píxeles) para que el indicador no quede pegado a la hurtbox, sino
                                un poquito separado*/

    indicadorSpr.setPosition(centerX - indW * 0.5f, topY - indH - marginTop);/**Coloca el sprite del indicador (P1/P2) centrado
                                                                             horizontalmente sobre la hurtbox y justo arriba,
                                                                             dejando un pequeño margen*/
}
void Jugador::ubicarHitbox()
{
    RectangleShape box = hitbox.getBox();/**Tomo la forma actual de la hitbox (un RectangleShape) y trabajo sobre una copia
                                         local llamada box*/

    box.setOrigin(box.getSize().x / 2, box.getSize().y / 2);/**Pongo el origen de la caja en su centro (mitad del ancho, mitad
                                                            del alto) para que, cuando la posicione, el punto que calcule sea el
                                                            centro de la hitbox*/

    sf::FloatRect bounds = m_sprite.getGlobalBounds();/**Obtengo los límites globales del sprite del jugador (posición left/top
                                                      y tamaño width/height), ya con transformaciones (posición, escala, flip)
                                                      aplicadas*/

    float ratioX = (m_sprite.getScale().x < 0) ? (1.f - hitboxRatioX) : hitboxRatioX;/**Si el sprite está espejado
                                                                                    (escala X negativa), reflejo el porcentaje
                                                                                    horizontal (ratioX) usando 1 - hitboxRatioX
                                                                                    (complemento como aprendimos en probabilidad y estadistica),
                                                                                    Si no está espejado, uso hitboxRatioX tal
                                                                                    cual, Esto mantiene la hitbox del lado correcto
                                                                                    del cuerpo cuando el personaje mira a la
                                                                                    derecha/izquierda*/

    float posX = bounds.left + bounds.width * ratioX;
    float posY = bounds.top + bounds.height * hitboxRatioY;/**Estas líneas calculan la posición de la hurtbox dentro del sprite
                                                            usando porcentajes. Así la caja siempre queda en el lugar correcto
                                                            del cuerpo, aunque el personaje cambie de tamaño o se voltee*/

    box.setPosition(posX, posY);/**Coloca la caja (hitbox) en la posición exacta que calculamos dentro del sprite, usando posX y posY*/

    hitbox.setBox(box);/**Ahora que box está bien ajustada (origen centrado y posición correcta), guardo esta versión actualizada
                       dentro del objeto hitbox (tu clase Box)*/
}
const sf::Sprite& Jugador::getIndicador() const
{
    return indicadorSpr;/**Devuelvo el sprite del indicador (P1/P2) por referencia constante, para que quien llame pueda
                       dibujarlo o leer sus bounds sin copiarlo ni modificarlo.*/
}
const sf::RectangleShape& Jugador::getHurtbox() const
{
    return hurtbox.getBox();/**Devuelvo la hurtbox (caja de recibir daño) como un RectangleShape, por referencia constante,
                            para poder dibujarla (modo debug) o consultar sus bounds*/
}
const sf::RectangleShape& Jugador::getHitbox() const
{
    return hitbox.getBox();/**Devuelvo la hitbox (caja de ataque) como RectangleShape, por referencia constante, para poder
                               usar sus bounds en detección de golpes o dibujarla en debug*/
}
void Jugador::setIndicadorScale(float sx, float sy)
{
    indicadorSpr.setScale(sx, sy);/**Escalo el sprite del indicador (P1/P2) en X e Y*/
}
void Jugador::setIndicadorTexture(const sf::Texture& tex)
{
    indicadorSpr.setTexture(tex);/**Asigno la textura del indicador*/
}

bool Jugador::CheckCollision(const Jugador& other) const   // Verifica colisi�n entre este jugador y otro
{
    return Objeto::CheckCollision(other);/**Reutilizo la lógica de colisión de Objeto para saber si este jugador está
                                         colisionando con el otro jugador*/
}
void Jugador::Correr()
{
    if (obj.actual() != Estado::CORRIENDO)/**si no esta corriendo*/
    {
        setEstado(Estado::CORRIENDO);/**Cambio el estado actual del jugador a CORRIENDO y activo la animacion correspondiente*/
        obj.setIsDePie(false);/**Marco que el jugador ya no está quieto/de pie*/
        obj.setAgachado(false);/**Aseguro que el jugador no está agachado mientras corre*/
    }
}
void Jugador::Ataque_Punio(Jugador& opponent)
{
    const float danio = 10.0f;/**Defino cuánto daño hace el puño: 10 puntos de vida*/

    const int puntos = 300;/**Defino los puntos que gana el atacante si el golpe conecta: 300 puntos*/

    m_PunioDeployed = m_clock.getElapsedTime().asSeconds();/**Anoto el momento (en segundos) en que se lanzó el puño, para
                                                           manejar los cooldowns*/
    if (obj.actual() != Estado::ATACAR_PUNIO)/**si aun no estoy en estado atacar_punio*/
        setEstado(Estado::ATACAR_PUNIO);/**cambio el estado y activo la animación de puño*/

    ubicarHitbox();/**Actualizo la posición de la hitbox (caja de golpe) para este frame, colocándola proporcionalmente dentro
                   del sprite y corrigiendo el flip si el personaje mira al otro lado*/

    opponent.ubicarHurtbox();/**Le pido al oponente que actualice su hurtbox (caja de recibir daño) para este frame, con su
                             posición actual y flip*/

    if (hitbox.checkColision(opponent.hurtbox.getBox()))/**Pregunto si mi hitbox colisiona con la hurtbox del oponente*/
    {
        if (!opponent.obj.agachado())/**si el oponente no esta agachado, ya que el agachado es la unica manera de esquivar*/
        {
            ParametroSprites configurador;/**Ajusto la dirección (flip) del sprite del oponente para que mire hacia quien lo
                                          golpeó (o en la dirección correcta), usando la utilidad ParametroSprites*/
            configurador.aplicarDireccion(opponent.m_name, opponent.m_sprite,opponent.getPosicion().x > getPosicion().x);

            /**Configuro el empuje del oponente tras recibir el golpe de puño*/
            opponent.m_knockbackDirection = (opponent.getPosicion().x > getPosicion().x) ? 1 : -1;/**Dirección*/
            opponent.m_knockbackImpacto = 110.f;/**la fuerza del empuje del puño*/
            opponent.m_knockbackDuracion = 0.5f;/**Duración*/
            opponent.m_knockbackTime = 0.f;/**Tiempo usado: arranco en 0.f (apenas empieza el empuje)*/

            // Aplicar daño y estado
            opponent.SetVida(opponent.GetVida() - danio);/**Le resto al oponente la cantidad de vida que corresponde al golpe
                                                         (danio)*/
            opponent.Golpeado();/**Marco al oponente en estado golpeado: actualizo su animación, activo la bandera de golpe,
                                y registro el tiempo para manejar el stun y la salida del estado.*/

            m_puntaje += puntos;/**Sumo a mi score los puntos del golpe exitoso*/
        }
    }
}
void Jugador::Ataque_Patada(Jugador& opponent)
{
    const float danio = 5.0f;/**La patada hace 5 puntos de daño (menos que el puño)*/

    const int puntos = 100;/**Si la patada entra, sumo 100 puntos (recompensa menor que el puño)*/

    m_PatadaDeployed = m_clock.getElapsedTime().asSeconds();/**Registro el momento exacto en que lancé la patada (para manejar
                                                            tiempos/cooldowns)*/

    if (obj.actual() != Estado::ATACAR_PATADA)/**Si todavía no estoy en la animación de patada, cambio el estado a ATACAR_PATADA*/
        setEstado(Estado::ATACAR_PATADA);

    ubicarHitbox();/**Actualizo la hitbox (caja de golpe) del atacante en este frame, proporcional al sprite y corrigiendo
                   el flip si corresponde*/
    opponent.ubicarHurtbox();/**Le pido al oponente que actualice su hurtbox (caja de recibir daño) en este frame*/

    if (hitbox.checkColision(opponent.hurtbox.getBox()))/**pregunto si mi hitbox colisiona con la hurtbox del oponente*/
    {
        if (!opponent.obj.agachado())/**y el oponente no esta agachado*/
        {
            ParametroSprites configurador;/**Ajusto la orientación del sprite del oponente para que mire hacia el atacante o
                                          en la dirección adecuada, usando una utilidad centralizada (ParametroSprites)*/
            configurador.aplicarDireccion(opponent.m_name, opponent.m_sprite,
                                          opponent.getPosicion().x > getPosicion().x);

            // Knockback
            opponent.m_knockbackDirection = (opponent.getPosicion().x > getPosicion().x) ? 1 : -1;/**Dirección*/
            opponent.m_knockbackImpacto = 160.f;/**la fuerza del empuje del patada*/
            opponent.m_knockbackDuracion = 0.5f;/**Duración*/
            opponent.m_knockbackTime = 0.f;/**Tiempo usado: arranco en 0.f (apenas empieza el empuje)*/

            // Aplicar daño y estado
            opponent.SetVida(opponent.GetVida() - danio);/**Le resto al oponente la cantidad de vida que corresponde al golpe
                                                         (danio)*/
            opponent.Golpeado();/**Marco al oponente en estado golpeado: actualizo su animación, activo la bandera de golpe,
                                y registro el tiempo para manejar el stun y la salida del estado.*/
            m_puntaje += puntos;/**Sumo a mi score los puntos del golpe exitoso*/
        }
    }
}
void Jugador::SetDeltaTime(sf::Time deltaTime)/**Actualiza el tiempo delta medido para usar en cálculos (animación, físicas u
                                              otros)*/
{
    m_deltaTime = deltaTime;
}

void Jugador::SetVida(float Porce)   // Establece la vida del jugador
{
    VidaPorcentaje = Porce;/**Setea el porcentaje de vida actual del jugador*/
}

float Jugador::GetVida()   // Devuelve la vida actual del jugador
{
    return VidaPorcentaje;/**Devuelve el porcentaje de vida actual del jugador*/
}

void Jugador::ValidateScreenLimits() /**Evita que el jugador se salga de los límites horizontales del escenario*/
{
    auto pos = m_sprite.getPosition();/**Obtiene la posición actual del sprite (X, Y) en la pantalla*/

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();/**Obtiene la resolución actual del escritorio para calcular el ancho
                                                            del escenario*/

    float escenarioAncho = desktop.width + desktop.width / 4;/**Define el ancho lógico del dojo (un 25% más ancho que la
                                                             pantalla) para permitir margen horizontal*/

    if (pos.x > escenarioAncho)/**si la posición X del jugador supera el límite derecho*/
        m_sprite.setPosition(escenarioAncho, pos.y); /** lo coloca justo en el borde (escenarioAncho) manteniendo la misma Y*/

    if (pos.x < 0)/**Si el jugador intenta moverse más allá del borde izquierdo*/
        m_sprite.setPosition(0, pos.y);/**lo coloco exactamente en el borde izquierdo (X=0), sin cambiar su altura*/
}
string Jugador::GetScoreStr()
{
    stringstream scoreStr;/**Crea un objeto stringstream para construir una cadena de texto dinámicamente*/
    scoreStr<<m_puntaje;/** Inserta el valor actual del puntaje del jugador (m_puntaje) dentro del stringstream*/
    return scoreStr.str();/**Convierte el contenido del stringstream en un string y lo devuelve*/
}
bool Jugador::getCaidoMuerto()
{
    return obj.caidoMuerto();
}
void Jugador::restart()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();/**Obtiene la resolución actual del escritorio para calcular el
                                                            ancho del escenario*/

    float escenarioAncho = desktop.width + desktop.width / 4;/**Define el ancho lógico del dojo (un 25% más ancho que la pantalla)*/

    float posicionX;/**Variable temporal para calcular la posición X inicial del jugador*/

/**Reset de estados*/
    obj.EstadoInicial();
    m_knockbackDirection = 0;
    m_knockbackImpacto = 0.f;
    m_knockbackDuracion = 0.f;
    m_knockbackTime = 0.f;

    Depie();/**Pone al jugador en estado QUIETO (de pie)*/

    obj.setSaltando(true);/**Marca que está en salto para que la lógica lo haga caer al suelo*/

    // Aplicar dirección correcta según jugador
    ParametroSprites configurador;/**Crea el helper que ajusta la orientación (flip) y escala según el personaje*/
    if (jugador_uno)
    {
        configurador.aplicarDireccion(m_name, m_sprite, false);/**Aplica flip para que P1 mire a la derecha*/
        posicionX = (escenarioAncho / 5.f) * 1.52f;/**lo coloca hacia la izquierda*/
    }
    else
    {
        configurador.aplicarDireccion(m_name, m_sprite, true);/**Aplica flip para que P2 mire a la izquierda*/
        posicionX = (escenarioAncho / 5.f) * 3.4f;/**lo coloca hacia la derecha*/
    }
    m_sprite.setPosition(posicionX, 300.f);/**Ubica el sprite en la posición calculada (X) y altura 300 (aparece en el aire
                                           para caer)*/
}
void Jugador::Golpeado()
{
    // Si NO queda shokeado, mostramos golpeado
    int prob = rand() % 100;
    if (GetVida() <= 40)/**Si la vida del jugador es 40% o menor, existe chance de quedar "shokeado" (aturdido)*/
    {
        if (prob < 30)   // 30% chance
        {
            setEstado(Estado::SHOKEADO);

            obj.setShokeado(true);

            m_tiempoShokeado = m_clock.getElapsedTime().asSeconds();/**Registra el instante (en segundos) en que empezó el
                                                                    shokeado para poder medir cuánto dura y cuándo salir del
                                                                    stun*/
            return;/**sale del metodo porque quedo en estado shokeado, por lo tanto la logica de golpeado la saltea por completo*/
        }
    }
    if (animaciones.find(Estado::GOLPEADO) != animaciones.end())/**Si existe una colección de frames para el estado "GOLPEADO",
                                                          la usamos*/
    {
        int totalFrames = animaciones[Estado::GOLPEADO].size();/**Cuenta cuántos frames hay disponibles para "GOLPEADO"*/

        int randomIndex = rand() % totalFrames;/**Elige al azar uno de los frames para variar la reacción visual*/

        m_sprite.setTexture(animaciones[Estado::GOLPEADO][randomIndex]);/**cambia la textura visible del jugador al frame seleccionado
                                                                 de "GOLPEADO"*/

        obj.setGolpeado(true);

        m_tiempoGolpeado = m_clock.getElapsedTime().asSeconds();/**Guarda el instante del golpe para medir por cuánto tiempo se
                                                                mantiene la reacción*/
    }
}
void Jugador::Agachar()
{
    if (obj.actual() != Estado::AGACHADO)/**si no esta agachado*/
    {
        setEstado(Estado::AGACHADO);/**setea el estado agachado*/
    }
    obj.setAgachado(true);/**activa el agachado*/
    obj.setIsDePie(false);/**y vuelve false el depie/quieto*/
}
void Jugador::Depie()
{
    if (!obj.isDePie())/**si no esta depie/quieto*/
    {
        obj.setIsDePie(true);/**activa el depie/quieto*/
        obj.setAgachado(false);/**vuelve false el agachado, ya que no peude estar de pie y a su vez estar agachado*/
        setEstado(Estado::QUIETO);/**setea el estado quieto*/
    }
}
void Jugador::Morir()
{
    if (obj.actual() != Estado::MUERTE)/**entra en muerte si no esta ya muerto*/
    {
        setEstado(Estado::MUERTE);/**setea el estado en muerte*/
        obj.setIsDePie(false);/**vuelve false los demas estados*/
        obj.setAgachado(false);
        obj.setSaltando(false);

        ParametroSprites configurador;
        m_knockbackDirection = configurador.signoKnockback(m_sprite);

        m_knockbackImpacto = 80.f;/**Define la intensidad base del empuje horizontal durante la muerte*/
        m_knockbackDuracion = 5.5f;/**Duración en cantidad de frames del knockback*/
        m_knockbackTime = 0.f;/**Reinicia el progreso del knockback para empezar desde cero*/
    }
}

void Jugador::Caer()
{
    if (obj.actual() != Estado::CAYENDO)/**se pone en estado cayendo si no estaba ya en estado cayendo*/
    {
        setEstado(Estado::CAYENDO);/**lo setes en estado cayendo*/
        obj.setIsDePie(false);/**vuelve falsos los demas estados*/
        obj.setAgachado(false);
        obj.setSaltando(false);

        ParametroSprites configurador;
        m_knockbackDirection = configurador.signoKnockback(m_sprite);

        m_knockbackImpacto = 80.f;/**Define la intensidad base del empuje horizontal durante la caida*/
        m_knockbackDuracion = 5.5f;/**Duración en cantidad de frames del knockback*/
        m_knockbackTime = 0.f;/**Reinicia el progreso del knockback para empezar desde cero*/
    }
}
void Jugador::Celebrar()
{
// Entra a CELEBRANDO solo si no está ya en ese estado
    if (obj.actual() != Estado::CELEBRANDO)
    {
        setEstado(Estado::CELEBRANDO);

        // Flags coherentes en la FSM
        obj.setIsDePie(true);     // celebrando se considera “de pie”
        obj.setAgachado(false);
        obj.setSaltando(false);   // ← reemplaza m_Saltando = false;
    }
}
bool Jugador::CanUseAtaquePunio() const
{
    // Delegar en la Máquina de Estados (FSM)
    return obj.puedeUsarPunio();
}

int Jugador::GetPuntaje()
{
    return m_puntaje;/**Devuelve el puntaje acumulado del jugador (entero)*/
}
int Jugador::GetRondasGanadas() const
{
    return m_rondasGanadas;/**Devuelve cuántas rondas ganó el jugador hasta ahora*/
}
void Jugador::WonARound()
{
    m_rondasGanadas++;/**Suma 1 al contador de rondas ganadas por este jugador*/
}
const string& Jugador::GetNombre() const
{
    return m_name;/**Devuelve (por referencia constante) el nombre del personaje/jugador*/
}

void Jugador::agregarAnimacion(const Estado estado, const std::vector<std::string>& rutas)
{
    // 1) Cargar frames
    std::vector<sf::Texture> frames;
    frames.reserve(rutas.size()); // evita realocaciones

    for (const auto& ruta : rutas)
    {
        sf::Texture tex;
        if (!tex.loadFromFile(ruta))
        {
            throw std::runtime_error("Error cargando textura: " + ruta);
        }
        tex.setSmooth(true);                  // se ve mejor al escalar/voltear
        frames.push_back(std::move(tex));     // mover, no copiar
    }

    // 2) Registrar en el mapa por Estado
    animaciones[estado] = std::move(frames);

    // 3) Si es la PRIMERA animación registrada, inicializar FSM y visual
    if (animaciones.size() == 1)
    {
        // Estado inicial usando la FSM (en lugar de asignar al getter)
        obj.cambiar(estado);                  // ← MaquinaDeEstados::cambiar(...)
        frameActual      = 0;
        tiempoAcumulado  = 0.f;

        // Mostrar el primer frame del estado recién fijado
        m_sprite.setTexture(animaciones[estado][frameActual]);
    }
}
void Jugador::caerSuelo(const Fondo_combate& fondo)
{
    if (hurtbox.checkColision(fondo.getSueloRect()))/**Si la hurtbox (la caja que representa el cuerpo del jugador) está
                                                    colisionando con el rectángulo del suelo del escenario, entonces corregí
                                                    la posición para que el jugador quede apoyado en el piso*/
    {
        // Obtener la posición de la parte inferior de la hurtbox
        float hurtboxBottom = hurtbox.getBox().getPosition().y + hurtbox.getBox().getSize().y / 2.f;/**Básicamente, esa línea
                                                                                                    calcula la Y del borde
                                                                                                    inferior de la hurtbox
                                                                                                    sumando la Y del centro y la
                                                                                                    mitad de su altura. */

        // Ajustar la posición del sprite para que la parte inferior de la hurtbox esté en el suelo
        float sueloY = fondo.getSuelo().top;/**Básicamente, sueloY = fondo.getSuelo().top me da la altura de la cara superior
                                            del suelo (en píxeles de pantalla).*/

        float deltaY = sueloY - hurtboxBottom;/**Básicamente, deltaY es la corrección vertical necesaria: si es positivo, bajo
                                              al jugador; si es negativo, lo subo; si es cero, ya está perfectamente apoyado*/
        // Mover sprite verticalmente
        m_sprite.move(0.f, deltaY);/**Básicamente, m_sprite.move(0.f, deltaY) corrige la posición vertical del personaje: lo
                                   baja o sube exactamente deltaY píxeles para dejar la hurtbox apoyada en el suelo, sin
                                   alterar su X*/

        // Actualizar hurtbox para reflejar la nueva posición del sprite
        ubicarHurtbox();
        ubicarHitbox();

        obj.setSaltando(false);/**apaga la bandera de salto justo después de aterrizar; así el jugador vuelve a la lógica de piso
                           (caminar, agacharse, atacar), y el sistema de estados y animaciones no lo trata como si siguiera en
                           el aire*/
    }
}
void Jugador::setEstado(Estado estado)
{
    if (animaciones.find(estado) != animaciones.end())/**Antes de cambiar el estado, verifico que exista en el mapa animaciones.
                                                      Si animaciones.find(estado) no devuelve end(), significa que hay una
                                                      animación cargada para ese estado*/
    {
        obj.cambiar(estado);/**Actualizo la variable estadoActual para que el jugador sepa en qué estado está*/
        frameActual = 0;/**Reinicio el índice del frame a 0, porque cuando cambio de estado quiero empezar la animación desde
                        el primer frame*/
        tiempoAcumulado = 0.f;/**Reinicio el tiempo acumulado para la animación, así el temporizador arranca desde cero y no
                                salta frames*/
        m_sprite.setTexture(animaciones[estado][frameActual]);/**Cambio la textura visible del sprite al primer frame ([0]) de
                                                              la animación correspondiente al nuevo estado*/
    }
}
void Jugador::actualizarAnimacion(float deltaTime)
{
    if (animaciones.find(obj.actual()) == animaciones.end()) return;/**Si el estado actual no tiene frames cargados, no hay
                                                                    nada que animar, salir del método*/

    float speedMultiplier = 1.0f;/**Factor de “velocidad” de la animación. Por defecto 1.0 (velocidad normal)*/

    if (obj.actual() == Estado::MUERTE || obj.actual() == Estado::CAYENDO)/**Para muerte/caída*/
    {
        speedMultiplier = 4.f;/**los frames avanzan 4 veces mas lento*/
    }
    tiempoAcumulado += deltaTime;/**Suma el tiempo transcurrido desde el último frame, usando el delta recibido por parámetro*/

    if (tiempoAcumulado >= tiempoPorFrame * speedMultiplier && !animaciones[obj.actual()].empty())/**Si el tiempo acumulado desde
                                                                                                  el último cambio de frame es
                                                                                                 suficiente (considerando el
                                                                                                multiplicador de velocidad del
                                                                                                estado actual) y además el
                                                                                                estado tiene frames cargados*/
    {
        tiempoAcumulado = 0.f;/**reiniciamos el tiempo acumulado, asi volver a repetirlo en el siguiente frame*/
        frameActual++;/** entonces ya es momento de avanzar al siguiente frame*/

        if (frameActual >= animaciones[obj.actual()].size())/**si el frame actual es mayor o igual al frame maximo del estado
                                                             en cuestion*/
        {
            obj.setEstaAtacando(false);/**el atacando se vuelve false, por si estaba en el estado de ataque*/

            if (obj.actual() == Estado::MUERTE || obj.actual() == Estado::CAYENDO || obj.actual() == Estado::GOLPEADO)/**Si el estado es MUERTE,
                                                                                                     CAYENDO o GOLPEADO*/
            {

                frameActual = animaciones[obj.actual()].size() - 1;/**no se reinicia la animación: se queda “congelado” en el
                                                                   último frame*/
                if (obj.actual() == Estado::MUERTE || obj.actual() == Estado::CAYENDO)
                {
                    obj.setCaidoMuerto(true);/**Activa la bandera que indica que el jugador quedó caído/muerto*/
                }
            }
            else
            {
                // ✅ Comportamiento normal para otros estados
                frameActual = 0;
                if (obj.actual() == Estado::ATACAR_PATADA || obj.actual() == Estado::ATACAR_PUNIO)
                {
                    obj.desactivarLockAtaque();
                    setEstado(Estado::QUIETO);/**Vuelve a QUIETO al terminar animación,asi que una vez que termina la animación de
                                        ataque, regresa al estado QUIETO*/
                    m_knockbackTime = 0;/**Resetea el tiempo de knockback por si quedó con algún empuje residual*/
                }
            }
        }

        m_sprite.setTexture(animaciones[obj.actual()][frameActual]);/**toma la textura del frame 'frameActual' del estado
                                                                    'estadoActual' animaciones[estadoActual] es el vector de
                                                                    frames) y se la asigna al sprite del jugador
                                                                    el sprite mostrará ese fotograma en pantalla en el próximo
                                                                    draw*/
    }
}
