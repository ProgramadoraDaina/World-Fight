#include <SFML/Graphics.hpp>           // Incluye la librería SFML para gráficos
#include "Juego.h"                     // Incluye la definición de la clase Juego

// Constructor de la clase Juego
Juego::Juego(Fondo *s)
    : m_window(sf::VideoMode::getDesktopMode(), "World Fight", sf::Style::Fullscreen),/**Creo la ventana en pantalla completa,
                                                                                     tomando el modo de video del escritorio*/
      m_fondo(s)/** y guardo el puntero a la escena/fondo actual(m_fondo)*/
{
    m_window.setFramerateLimit(60);/**Fijo el tope de FPS a 60 para que el juego corra estable*/
}

void Juego::Run() { /**Este es el método principal que corre el bucle del juego*/

    while (m_window.isOpen()) {        /**Mientras la ventana esté abierta*/

        ProcessEvents();/**Primero, leo y distribuyo todos los eventos de este frame (teclado, mouse, cierre de ventana). Si
                         el evento es cerrar, cierro; si no, se lo paso a la escena actual para que actúe*/

        Update();  /**Después actualizo la lógica del juego: muevo personajes, reviso colisiones, sumo tiempo/racha,
                   avanzo animaciones, todo lo que corresponde a este frame*/

        Draw(); /**Luego dibujo la escena actual en la ventana: fondo, jugadores, HUD, textos, todo*/

        if (m_siguiente_fondo) {/**Chequeo si alguna escena pidió cambiar (por ejemplo, del Menú al Selector, del Combate
                                al Archivo). Ese pedido se guarda en m_siguiente_fondo*/

            delete m_fondo;/**Primero borro la escena actual para liberar memoria (no dejar fugas)*/

            m_fondo = m_siguiente_fondo;/**Ahora pongo la nueva escena como escena actual (la que se va a actualizar y dibujar
                                        a partir de este momento)*/
            m_siguiente_fondo = nullptr;/**Limpio el puntero de la escena ‘pendiente’ porque ya se activó. Así evito
                                          recambiarla accidentalmente*/
        }
    }
}
// Procesa los eventos de la ventana
void Juego::ProcessEvents() {
    Event e;/**Creo una variable para guardar temporalmente el evento que vaya saliendo de la cola*/

    while (m_window.pollEvent(e)) {    /**Mientras haya eventos pendientes en la ventana, los voy sacando uno por uno para
                                        procesarlos*/
        if (e.type == Event::Closed)   /**Si el evento indica que el usuario cerró la ventana(por ejemplo esc)*/

            m_window.close();          /**entonces cierro la ventana del juego ahora mismo*/
        else
            m_fondo->ProcessEvents(*this, e);/**Delego el evento en la escena activa (m_fondo). Cada escena decide qué hacer
                                               con ese evento*/
    }
}
void Juego::Update() {/**Este método se usa para actualizar la lógica del juego en este frame*/

    m_fondo->Update(*this);/**Le pido a la escena actual (m_fondo) que actualice su lógica, pasándole el Juego con
                            *this (para que la escena pueda acceder a la ventana, cambiar de escena, manejar rachas, etc.)*/
}
void Juego::Draw() {/**Este método se usa para dibujar todo lo que la escena quiera mostrar en pantalla*/

    m_fondo->Draw(m_window);/**Le pido a la escena actual que se dibuje usando la ventana del juego*/
}
void Juego::SetFondo(Fondo *siguiente_escena) {/**Este método sirve para solicitar un cambio de escena*/
    delete m_fondo;
    m_fondo = siguiente_escena;   /**Guardo el puntero de la nueva escena para que el cambio real se haga al
                                            final del frame dentro de Run() (es más seguro y evita errores mientras se
                                             está dibujando/actualizando)*/
}
void Juego::Exit() {/**Método para salir del juego*/

    m_window.close();/**Cierro la ventana del juego ahora*/
}
const RenderWindow &Juego::GetWindow() {/**Devuelvo la ventana del juego como referencia constante (para consultarla
                                        sin modificarla)*/
    return m_window;                   /**Retorna la ventana actual*/
}
RenderWindow &Juego::GetWindow2() {/**Devuelvo la misma ventana pero como referencia no-const (por si se necesita modificar algo)*/
   return m_window;                   /** Retorna la ventana actual*/
}
int Juego::GetRachaJugador1() const
{
    return rachaJugador1;/**retorna la racha actual de victorias del Jugador 1*/
}
int Juego::GetRachaJugador2() const
{
    return rachaJugador2;/**retorna la racha actual de victorias del Jugador 2*/
}
void Juego::IncrementarRachaJugador1()
{
    rachaJugador1++; rachaJugador2 = 0;/**Cuando gana P1, incremento su racha y reseteo la de P2 a 0*/
}
void Juego::IncrementarRachaJugador2()
{
    rachaJugador2++; rachaJugador1 = 0;/**Cuando gana P2, incremento su racha y reseteo la de P1 a 0*/
}
void Juego::ResetRachas()
{
    rachaJugador1 = 0; rachaJugador2 = 0;/**Pongo ambas rachas en 0 (por ejemplo, al iniciar una nueva partida)*/
}
