#include "Combate.h"
#include "menu.h"
#include "Archivo.h"
#include <sstream> // Incluye cabecera necesaria para el programa
#include <SFML/Window/Keyboard.hpp> // Incluye cabecera necesaria para el programa
#include "Jugador.h"
#include <iostream>

using namespace std;

Combate::Combate(string Jugador_uno, string Jugador_dos, RenderWindow &window)
    : window_referencia(window), bv_p1(true), bv_p2(false) // Inicialización
{

    p1 = Jugador(true, Jugador_uno);/**creo el jugador 1 indicando que es el del lado izquierdo (true) y le asigno el
                                    nombre/clase de personaje elegido (Jugador_uno)*/
    p2 = Jugador(false, Jugador_dos);/**creo el jugador 2 indicando que es el del lado derecho (false) y le asigno el
                                    nombre/clase de personaje elegido (Jugador_dos)*/


    // Cargar fuentes y textos
    m_f1.loadFromFile("Src/fonts/arcade.ttf");/**cargo la fuente arcade.ttf para dibujar textos HUD(informacion que aparece en
                                              pantalla para saber el estado de tu personaje, como en este juego es la barra de
                                              vida, y en otros juegos, el nombre, la foto de el), rótulos, etc.*/

    m_t1.setFont(m_f1);/**configuro el texto “Fight!” (fuente, color rojo, tamaño 20) y centro su origen horizontal para poder
                       ubicarlo centrado con facilida*/
    m_t1.setFillColor(Color(204, 0, 0));
    m_t1.setString("Fight!");
    m_t1.setCharacterSize(20);
    m_t1.setOrigin(m_t1.getLocalBounds().width / 2, 0);

    m_t2.setFont(m_f1);/**preparo el texto del botón “Regresar al Menú” (blanco, tamaño 18), centro su origen y lo ubico cerca
                       de la parte inferior de la pantalla*/
    m_t2.setFillColor(Color(255, 255, 255));
    m_t2.setString("Regresar al Menu");
    m_t2.setCharacterSize(18);
    m_t2.setOrigin(m_t2.getLocalBounds().width / 2, m_t2.getLocalBounds().height / 2);
    m_t2.setPosition(40, 690);

    gameEnded  = false;/**inicializo banderas/estado: el combate no terminó, no está en pausa y aun no hay ganador*/
    menu_pausa = false;
    Ganador    = 0;

    m_puntosP1.setFont(m_f1);/**configuro el texto que muestra los puntos del Jugador 1 (tamaño grande, gris, posición superior)*/
    m_puntosP1.setCharacterSize(40);
    m_puntosP1.setFillColor(Color(120, 120, 120));
    m_puntosP1.setPosition(400, 70);

    m_puntosP2.setFont(m_f1);/**configuro el texto que muestra los puntos del Jugador 2 (tamaño grande, gris, posición superior)*/
    m_puntosP2.setCharacterSize(40);
    m_puntosP2.setFillColor(Color(120, 120, 120));
    m_puntosP2.setPosition(720, 70);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();/**obtengo la resolución actual del escritorio para dimensionar
                                                            overlays y elementos que deben cubrir la pantalla*/

    blackoutRect.setSize(Vector2f(desktop.width, desktop.height));
    blackoutRect.setFillColor(Color(0, 0, 0, 0));/**creo un rectángulo invisible (overlay negro con alpha 0) del tamaño de toda
                                                 la pantalla; lo usamos para oscurecer la escena en pausas o al finalizar*/

    leaderboardRect.setSize(Vector2f(430, 80));
    leaderboardRect.setFillColor(Color(0, 0, 0, 0));/**creo el rectángulo del botón la “tabla de clasificacion”; de inicio lo
                                                    dejo transparente (alpha 0) y luego lo pinto/posiciono cuando corresponde*/

    menuRect.setSize(Vector2f(330, 80));
    menuRect.setFillColor(Color(212, 43, 43));
    menuRect.setOrigin(menuRect.getLocalBounds().width / 2, menuRect.getLocalBounds().height / 2);
    menuRect.setPosition(0, 670);/**creo el botón de “Regresar al Menú” (rectángulo rojo), centro su origen y lo ubico en y=670;
                                 su x la ajustaré respecto de la cámara cuando esté en pausa*/

    reloj.Start();/**inicio el cronómetro de la ronda (reinicia el conteo de tiempo)*/

    original = window.getDefaultView();/**guardo la view original de la ventana para poder restaurarla al cambiar de escena
                                      (salir del combate)*/

    fondo.setFondoCombate(camara);/**preparo el fondo del combate (capas parallax, suelo físico, márgenes) y lo vinculo con la
                                  view de combate (camara)*/

    if (!indicadorTexP1.loadFromFile("src/Fondo/P1.png"))
        throw std::runtime_error("Error cargando src/Fondo/P1.png");
    if (!indicadorTexP2.loadFromFile("src/Fondo/P2.png"))
        throw std::runtime_error("Error cargando src/Fondo/P2.png");/**cargo las texturas de los indicadores “P1” y “P2”; si
                                                                   fallan, lanzo un error para que el juego no continúe sin HUD
                                                                   clave*/
    p1.setIndicadorTexture(indicadorTexP1);
    p2.setIndicadorTexture(indicadorTexP2);/**asigno la textura “P1” al indicador del Jugador 1 y la textura “P2” al indicador
                                           del Jugador 2*/
    p1.setIndicadorScale(0.3f, 0.3f);
    p2.setIndicadorScale(0.3f, 0.3f);/**escalo los indicadores para que se vean del tamaño deseado (30% del tamaño original)*/


    p1.ubicarIndicador();
    p2.ubicarIndicador();/**posiciono los indicadores por encima de la hurtbox de cada jugador, centrados y con un margen*/

}
Combate::~Combate() {} // Destructor de la clase Combate

void Combate::ProcessEvents(Juego &juego, Event &event)   // Habilitamos el cierre del juego con el boton ESCAPE // M todo que procesa eventos del juego (teclado, mouse)
{
    if(event.type == Event::KeyPressed && event.key.code == Keyboard::Return && gameEnded)/**Si el evento es una tecla presionada,
                                                                                         la tecla es ENTER (Return), y además el
                                                                                         combate ya terminó (gameEnded == true),
                                                                                         entonces*/
    {
        Archivo* archivo = new Archivo("leaderboard.dat", PuntosDelGanador, true);/**Creo la escena de leaderboard (Archivo) para
                                                                                 guardar el puntaje; le paso el archivo
                                                                                 leaderboard.dat, los puntos del ganador y true
                                                                                 para habilitar guardar nombre*/

        // Pasar nombres reales de los jugadores
        archivo->SetJugadores(p1.GetNombre(), p2.GetNombre());/**Le digo al leaderboard cuáles fueron los nombres reales de P1
                                                             y P2 (para mostrar y gestionar rachas)*/

        // Pasar el nombre del ganador
        if(Ganador == 1)
        {
            archivo->SetGanador(p1.GetNombre(), 1);/**Le informo al leaderboard quién ganó (nombre e ID 1/2), para que actualice
                                                    su registro; si fue empate, no se setea ganador*/
        }
        else if(Ganador == 2)
        {
            archivo->SetGanador(p2.GetNombre(), 2);
        }

        juego.SetFondo(archivo);/**Le pido al Juego que cambie de escena al leaderboard al final del frame*/
    }
    else if(event.type == sf::Event::MouseButtonPressed)/**Si el evento fue un click de mouse*/
    {
        sf::Vector2f mousePos = window_referencia.mapPixelToCoords(
                                    sf::Mouse::getPosition(window_referencia),/**Convierto la posición de píxeles del mouse a
                                                                          coordenadas del mundo*/
                                    camara);/**usando la vista del combate (camara)*/
        sf::FloatRect menuRectBounds = menuRect.getGlobalBounds();/**Crea una caja invisible (FloatRect) que representa el tamaño
                                                              y posición actual del botón menuRect en la pantalla. Esta caja
                                                              incluye su ancho, alto y coordenadas globales, para que después
                                                              pueda preguntar si el mouse está dentro de esa área*/

        sf::FloatRect leaderboardRectBounds = leaderboardRect.getGlobalBounds();/**Crea una caja invisible (FloatRect) que
                                                                            representa el area completa del boton rojo
                                                                            (leaderboardRect) en la pantalla, tomando en
                                                                            cuenta su posicion, tamanio y origen. Esta caja
                                                                            sirve para saber visualmente donde esta el boton
                                                                            y cuanto ocupa*/

        if (menuRectBounds.contains(mousePos))/**Si la posicion del mouse (convertida a coordenadas del mundo) esta dentro del
                                         Area del boton de menu*/
        {
            window_referencia.setView(original);/**restauro la vista original para salir del modo combate y volver a la vista
                                            base*/
            juego.SetFondo(new Menu);/**creo un objeto de la clase Menu (la pantalla principal del juego) se lo paso al objeto
                                  juego para que lo active como la proxima escena*/
        }
        else if(leaderboardRectBounds.contains(mousePos))/**si la posición del mouse (ya convertida a coordenadas del mundo) cae
                                                     dentro del area del boton de Leaderboard*/
        {

            window_referencia.setView(original);/** restauro la vista original de la ventana (salgo de la vista de combate) para
                                            que la próxima escena se dibuje con la camara default*/

            juego.SetFondo(new Archivo("leaderboard.dat", PuntosDelGanador, true));/**creo un objeto de la clase Archivo, le paso
                                                                               el nombre del archivo de ranking
                                                                               leaderboard.dat, el puntaje del ganador y el
                                                                               flag true para indicar que se permite guardar;
                                                                              luego se lo pido al objeto juego como la proxima
                                                                              escena*/
        }
    }
    else if(event.type == sf::Event::KeyPressed)
    {
        if(event.key.code == sf::Keyboard::Escape)/**si el evento es una tecla presionada
                                              y la tecla es Escape, activo/desactivo
                                              el modo pausa*/
        {
            menu_pausa = !menu_pausa;/**invierto el estado del menú de pausa (toggle). Si estaba false pasa a true, y si estaba true
                                 pasa a false*/
        }
        if (event.key.code == sf::Keyboard::Enter && menu_pausa)/**si el evento es una tecla presionada
                                              y la tecla es Enter, salgo del combate*/
        {
            window_referencia.setView(original);/**restauro la vista original para salir del modo combate y volver a la vista
                                            base*/
            juego.SetFondo(new Menu);/**creo un objeto de la clase Menu (la pantalla principal del juego) se lo paso al objeto
                                  juego para que lo active como la proxima escena*/
        }
    }

}

void Combate::Update(Juego &juego) // Método que actualiza la lógica del combate
{
    // setea los contadores de puntos
    m_puntosP1.setString(p1.GetScoreStr());/**Actualizo en pantalla los puntos de cada jugador con lo que recibe la clase Jugador*/
    m_puntosP2.setString(p2.GetScoreStr());

    // Termina el juego cuando no quedan más rounds
    if (m_actualRound > m_totalRounds)/**si el round actual es mayor al total de rounds*/
    {
        gameEnded = true;/**el juego termina*/
    }

    // Si muere algún jugador o se termina el tiempo del cronómetro, y todavía quedan rounds por pelear
    if (menu_pausa)/**Si el menú de pausa está activo*/
    {
        menuRect.setPosition(camara.getCenter());/**pongo el botón y el texto en el centro de la cámara y muestro un overlay
                                                 semitransparente*/
        m_t2.setPosition(camara.getCenter());
        blackoutRect.setFillColor(Color(0, 0, 0, 128));
        blackoutRect.setPosition(camara.getCenter().x - camara.getSize().x / 2, 0);
    }
    else
    {
        menuRect.setPosition(-500, -500);/**Si no está pausado, los oculto y apago el overlay*/
        m_t2.setPosition(-500, -500);
        blackoutRect.setFillColor(Color(0, 0, 0, 0));
    }

    if (p1.GetVida() <= 0 || p2.GetVida() <= 0 || reloj.SegundosRestantes() <= 0)/**Si alguno murió o se acabó el tiempo,
                                                                                  entonces arranco el conteo del cartel
                                                                                  (reinicio m_clock)*/
    {
        m_clock.restart();/**reinicio el reloj*/
        int GanadorDeRonda;/**y preparo una variable para guardar quien gano la ronda*/

        if (p1.GetVida() <= 0 || p2.GetVida() <= 0)/**Si hubo alguna muerte*/
        {
            // Ronda final: muerte y celebración
            if (p1.GetVida() <= 0)/**si murio jugador 1*/
            {
                if (p2.GetRondasGanadas() < 2)/**Si el oponente todavía no ganó 2 rondas, el que perdió cae (CAYENDO)*/
                {
                    p1.Caer(); // o p1.setEstado("CAYENDO");
                }
                else
                {
                    /**Si el oponente ya alcanzó 2 rondas*/

                    p1.Morir();  /** el que perdió muere (MUERTE)*/

                    if (p1.getCaidoMuerto()) /**y cuando queda “caído muerto”*/
                    {
                        p2.Celebrar();/**el otro celebra*/
                    }
                }
            }
            if (p2.GetVida() <= 0)/**si murio jugador 2*/
            {
                if (p1.GetRondasGanadas() < 2)/**Si el oponente todavía no ganó 2 rondas, el que perdió cae (CAYENDO)*/
                {
                    p2.Caer(); // o p2.setEstado("CAYENDO");
                }
                else   /**Si el oponente ya alcanzó 2 rondas*/
                {
                    p2.Morir(); /** el que perdió muere (MUERTE)*/
                    if (p2.getCaidoMuerto())/**y cuando queda “caído muerto”*/
                    {
                        p1.Celebrar();/**el otro celebra*/
                    }
                }
            }
        }

        // guarda al ganador del round
        if (p1.GetVida() > p2.GetVida())/**Si el Jugador 1 tiene mas vida que el Jugador 2*/
        {
            if (!finDeRonda)/**Si todavia no marqué que la ronda terminó*/
            {
                p1.WonARound();/**Le sumo una ronda ganada al Jugador 1*/
            }
            GanadorDeRonda = 1;/**Guardo que el ganador de esta ronda es el Jugador 1*/
        }
        else if (p2.GetVida() > p1.GetVida())/**En caso contrario, si el Jugador 2 tiene mas vida que el Jugador 1*/
        {
            if (!finDeRonda)/**Si todavía no marque fin de ronda*/
            {
                p2.WonARound();/**Le sumo una ronda ganada al Jugador 2*/
            }
            GanadorDeRonda = 2;/**Guardo que el ganador de esta ronda es el Jugador 2*/
        }
        else
        {
            GanadorDeRonda = 0;/**Si ninguno tiene más vida (están iguales) por lo tanto empate*/
        }
        finDeRonda = true;/**Marco que la ronda terminó para no volver a sumar ni evaluar nuevamente en este mismo cierre*/

        if (p1.getCaidoMuerto() || p2.getCaidoMuerto() || reloj.SegundosRestantes() <= 0)/**si la ronda finalizo porque murio el
                                                                                         jugador 1 o 2, o el tiempo termino
                                                                                        entonces entramos en el bloque de cierre
                                                                                        de ronda*/
        {
            if (reloj.SegundosRestantes() <= 0 && m_actualRound > 3)/**Si el tiempo llego a cero y ya estamos en el round 3,
                                                                    marcamos gameEnded = true y el combate entero termina*/
            {
                gameEnded = true;
            }
            if (m_clock.getElapsedTime().asSeconds() < 6)/**muestra un cartel con el ganador del round durante 6 segundos*/
            {
                blackoutRect.setFillColor(Color(0, 0, 0, 128));/**Activa un fondo semitransparente (negro al 50%) que cubre la
                                                               pantalla del combate*/
                blackoutRect.setPosition(camara.getCenter().x - camara.getSize().x / 2, 0);/**posicionado desde el borde izquierdo
                                                                                          de la view (la cámara) para que el overlay
                                                                                          se ancle visualmente a la vista actual*/
                stringstream GanadordeRoundMsg;
                m_GanadorRondaText.setCharacterSize(47);
                m_GanadorRondaText.setFont(m_f1);
                m_GanadorRondaText.setFillColor(Color(204, 0, 0));
                if (GanadorDeRonda != 0)/**Si hay ganador (1 o 2), escribe “Jugador X ganó la ronda #N!*/
                {
                    /**el objeto GanadordeRoundMsg sirve para armar texto dinámicamente usando el operador <<*/
                    GanadordeRoundMsg << "Jugador " << GanadorDeRonda << " Gano la ronda #" << m_actualRound << "!";
                }
                else
                {
                    GanadordeRoundMsg << "Es un empate!";/**si no, pone “Es un empate!*/
                }
                m_GanadorRondaText.setString(GanadordeRoundMsg.str());/**Pasa el texto entero con str(sirve para obtener todo
                                                                      el contenido acumulado como un string)al objeto gráfico
                                                                      (m_GanadorRondaText) para que se dibuje en pantalla*/
                if (Keyboard::isKeyPressed(Keyboard::Return))/**Si el jugador está presionando la tecla ENTER(Return)*/
                {
                    if (p1.GetRondasGanadas() == 2 || p2.GetRondasGanadas() == 2 || gameEnded)/**Verificá si el combate ya
                                                                                            termino revisando la cantidad de
                                                                                            rondas ganadas para volver game
                                                                                            ended = true si gameended es ya es
                                                                                            true*/
                    {
                        gameEnded = true;
                    }
                    else /**Si ninguno llegó a 2 rondas ganadas y no estaba gameEnded, entonces arrancá el siguiente round*/
                    {
                        EmpezarSiguienteRound();
                    }
                    m_GanadorRondaText.setString("");/**Vacia el texto del cartel de ganador de la ronda (lo pone en "") para
                                                     que deje de mostrarse*/
                }
                /**aqui se hace que el centro del texto sea su punto de referencia*/
                m_GanadorRondaText.setOrigin(m_GanadorRondaText.getLocalBounds().width / 2, m_GanadorRondaText.getLocalBounds().height / 2);

                m_GanadorRondaText.setPosition(camara.getCenter());/**Poné el texto exactamente en el centro de la cámara
                                                                  Como el origen del texto ya está centrado, el cartel queda
                                                                  perfectamente centrado en pantalla aunque la cámara se mueva*/
            }
            else
            {
                if (p1.GetRondasGanadas() == 2 || p2.GetRondasGanadas() == 2 || gameEnded)/**Verificá si el combate ya
                                                                                            termino revisando la cantidad de
                                                                                            rondas ganadas para volver game
                                                                                            ended = true si gameended es ya es
                                                                                            true*/
                {
                    gameEnded = true;
                }
                else/**Si ninguno llegó a 2 rondas ganadas y no estaba gameEnded, entonces arrancá el siguiente round*/
                {
                    EmpezarSiguienteRound();
                }
                ClockYaFueReiniciado = false;/**Reiniciamos la bandera que se usa para saber si el reloj del round ya fue
                                              reiniciado para evitar doble reinicio*/
                m_GanadorRondaText.setString("");/**Borra el mensaje del cartel de ganador de la ronda (lo deja vacío)**/

                if (!menu_pausa)/**Si no estamos en modo pausa, apaga el overlay negro (lo vuelve transparente)*/
                {
                    blackoutRect.setFillColor(Color(0, 0, 0, 0));
                }
                blackoutRect.setPosition(camara.getCenter().x - camara.getSize().x / 2, 0);/**Reposiciona el overlay negro para
                                                                                           que cubra la pantalla desde el borde
                                                                                           izquierdo de la view (cámara) actual*/
            }

        }
    }

    if (gameEnded)/**Si el combate ya termino*/
    {
        window_referencia.setView(original);/**Restaura la vista original de la ventana a la por defecto*/
        return;/**corta la función Update en este punto (no se ejecuta nada más abajo)*/
    }

    if (reloj.SegundosRestantes() > 0 && !menu_pausa)/**si el tiempo no ha terminado */
    {
        reloj.Update();/**avanza el cronómetro y refresca el texto “mm:ss”*/

        p1.Update(p2);/**actualiza la logica de ambos jugadores,entrada de teclado, movimiento, estados, animacion,
                      deteccion de golpes (hitbox vs hurtbox), daño, knockback, etc*/
        p2.Update(p1);
        bv_p1.SetVida(p1.GetVida());/**sincroniza el HUD de vida para que las barras reflejen el porcentaje de vida
                                         actual de P1 y P2*/
        bv_p2.SetVida(p2.GetVida());
    }

    sf::FloatRect bv1 = p1.getSprite().getGlobalBounds();/**Obtiene los rectángulos globales (posición y tamaño en pantalla) de
                                                         los sprites de ambos jugadores y los guarda en bv1*/
    sf::FloatRect bv2 = p2.getSprite().getGlobalBounds();

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();/**desktop captura la resolución actual del monitor (ancho/alto)*/

    float camaraX = (bv1.left + bv1.width / 2.f + bv2.left + bv2.width / 2.f) / 2.f;/**camaraX calcula la X del punto medio
                                                                                    entre los centros de los sprites de P1 y
                                                                                    P2 ya que left es el borde izquierdo y el
                                                                                    width te dara el borde derecho y
                                                                                    dividiendolo a la mitad obtendremos el
                                                                                    centro*/
    float dojoInicioX = fondo.getCapasAtras(2).getPosition().x;/**Lee la posición X (borde izquierdo porque el origen esta en el
                                                               lado izquierdo) de la capa 2 del fondo, esta X es el inicio
                                                               horizontal del escenario dentro de la vista global*/

    float dojoFinX = dojoInicioX + fondo.getCapasAtras(2).getGlobalBounds().width;/**Calcula el fin del dojo en X: inicio + ancho
                                                                                  visible de la capa 2 (considera
                                                                                  escala/transformaciones),con esto ya tenemos el
                                                                                  rango que ocupa el escenario en pantalla*/

    /**Define los límites dentro de los cuales puede moverse el centro de la camara*/
    float limiteIzq = dojoInicioX + camara.getSize().x / 2.f;/**inicio del dojo + mitad del ancho de la camara, evita que se vea
                                                             “fuera” del dojo por la izquierda*/

    float limiteDer = dojoFinX - camara.getSize().x / 2.f;/**fin del dojo − mitad del ancho de la camara,evita salirse por la
                                                          derecha asi la camara evita mostrar fuera del escenario*/

    camaraX = std::clamp(camaraX, limiteIzq, limiteDer);/**Recorta (clamp) la X deseada de la cámara para que quede siempre
                                                         dentro de [limiteIzq, limiteDer]*/

    camara.setCenter(camaraX, desktop.height / 2.f);/**Posiciona el centro de la view (sf::View) en (camaraX, altura/2) y como
                                                    esta en el update, hace que se actualice a cada frame dandole profundidad*/

    fondo.updateFondo(camaraX, desktop);/**Actualiza el parallax del fondo_combate: mueve las capas de los arboles y cielo en
                                        funcion de la X de la camara para dar sensacion de profundidad*/

    m_t1.setPosition(camara.getCenter().x, 40);/**Coloca el texto “Fight!” (m_t1) anclado a la camara*/

    m_puntosP1.setOrigin(m_puntosP1.getLocalBounds().width, 0);/**posiciona el origen en la esquina superior derecha(el cero es la altura)*/

    m_puntosP2.setOrigin(0, 0);/**la posiciona en la esquina superior izquierda (por defecto)*/

    m_puntosP1.setPosition(camara.getCenter().x - (camara.getSize().x * 0.1f), 70);/**posiciona los numeros de los puntajes en el
                                                                                   centro para luego restarle 0.1f asi quedar un
                                                                                   poco mas a la izquierda(el 70 es la coordenada
                                                                                   vertical)*/
    m_puntosP2.setPosition(camara.getCenter().x + (camara.getSize().x * 0.1f), 70);/**posiciona los numeros de los puntajes en el
                                                                                   centro para luego sumarle 0.1f asi quedar un
                                                                                   poco mas a la derecha(el 70 es la coordenada
                                                                                   vertical)*/
    bv_p1.setCenter(camara);/**posiciona la barra de vida del p1 para que siga la camara*/
    bv_p2.setCenter(camara);/**posiciona la barra de vida del p2 para que siga la camara*/
    reloj.setCenter(camara);/**posiciona el reloj del tiempo de cada ronda para que siga la camara*/

    p1.caerSuelo(fondo);/**Ajusta la posicion vertical de cada jugador para que su hurtbox (la caja que representa el cuerpo
                        para recibir daño) para que caiga y toque el suelo fisico del dojo (el rectangulo que definimos en
                        Fondo_combate)*/
    p2.caerSuelo(fondo);

}
void Combate::Draw(RenderWindow &window)   // Muestra en la nueva escena el fondo, textos y los jugadores // M todo que dibuja todos los elementos en pantalla
{

    window.setView(camara);/**Pongo la vista de combate (camara) para que todo lo que dibuje se ancle al combate*/
    window.clear({0,0,0});/**limpio la pantalla en negro antes de dibujar la nueva frame*/
    fondo.drawCapasAtras(window);/**Dibujo primero las capas traseras del escenario (parallax al fondo)*/
    window.draw(m_t1);/**dibujo el texto ‘Fight!’ en esta vista*/

    // Dibujar las hitbox y hurtbox

    //window.draw(p1.getHurtbox()); /**Tengo el debug de cajas de daño (hurtbox) y golpe (hitbox) listo para pintar, pero lo
    //                              dejo comentado para no mostrarlas siempre*/
    //window.draw(p1.getHitbox());
    //window.draw(p2.getHurtbox());
    //window.draw(p2.getHitbox());

    p1.Draw(window);/**Dibujo los dos jugadores */
    p2.Draw(window);

    window.draw(p1.getIndicador());/**dibujo los indicadores de los jugadores*/
    window.draw(p2.getIndicador());

    fondo.drawCapasAdelante(window);/**dibujo las capas delanteras del escenario*/
    reloj.Draw(window);/**dibujo el reloj de la ronda*/
    window.draw(m_puntosP1);/**dibujo puntaje de p1*/
    window.draw(m_puntosP2);/**dibujo puntaje de p2*/
    bv_p1.Draw(window);/**dibujo barra de vida del p1*/
    bv_p2.Draw(window);/**dibujo barra de vida del p2*/

    window.draw(blackoutRect);/**Dibujo el overlay negro (blackout) para oscurecer la escena*/
    if (gameEnded)  /**Si el combate terminó*/
    {
        blackoutRect.setFillColor(Color(0, 0, 0, 128));/**activo el overlay semitransparente*/
        blackoutRect.setPosition(camara.getCenter().x - camara.getSize().x/2, 0);/**y lo anclo a la cámara: arranco desde el
                                                                                 borde izquierdo de la vista para cubrir toda
                                                                                 la pantalla de combate*/
        Text winnerText;/**Creo el texto temporal del ganador*/
        winnerText.setFont(m_f1);/**tipo de fuente*/
        winnerText.setFillColor(Color(255, 255, 255));/**fuente blanca*/
        winnerText.setCharacterSize(47);/**tamanio grande*/

        // LOGICA PARA GUARDAR EL GANADOR.
        if (Ganador != 0) /**Comprueba si ya hay un ganador (1 o 2)*/
        {
            winnerText.setString("Jugador " + to_string(Ganador) + " gano la batalla!");/**imprime el texto anunciando al ganador*/
        }
        else/**sino comparo cuantas rondas ganadas tiene cada jugador*/
        {
            if (p1.GetRondasGanadas() > p2.GetRondasGanadas())/**Si P1 tiene más rondas que P2*/
            {
                Ganador = 1;/**el ganador es 1*/
                PuntosDelGanador = p1.GetPuntaje();/**guardamos el puntaje en una variable para que quede en el top de
                                                   clasificaciones*/
            }
            else if (p2.GetRondasGanadas() > p1.GetRondasGanadas())/**Si P2 tiene más rondas que P1*/
            {
                Ganador = 2;/**el ganador es 2*/
                PuntosDelGanador = p2.GetPuntaje();/**guardamos el puntaje en una variable para que quede en el top de
                                                   clasificaciones*/
            }

            if (Ganador != 0)   /**si ahora que verificamos las rondas ganadas hay ganador, lo imprimimos*/
            {
                winnerText.setString("Jugador " + to_string(Ganador) + " gano la batalla!");
            }
            else/**si aun asi habiendolo verificado no hay ganador*/
            {
                winnerText.setString("Es un empate!");/**imprimimos que es un empate y por lo tanto no hay ganador*/
            }
        }
        winnerText.setOrigin(winnerText.getLocalBounds().width/2, winnerText.getLocalBounds().height/2);/**pongo el origen de el
                                                                                                       texto del ganador en el
                                                                                                       centro del texto*/
        winnerText.setPosition(camara.getCenter());/**posiciono el texto del ganador en el centro de la camara*/


        sf::Text botonText;/**armo un sf::Text blanco, tamaño 21, con ese mensaje*/
        botonText.setFont(m_f1);
        botonText.setFillColor(Color(255, 255, 255));
        botonText.setCharacterSize(21);
        botonText.setString("Presiona ENTER para guardar puntos");
        botonText.setOrigin(botonText.getLocalBounds().width/2, botonText.getLocalBounds().height/2);/**centramos el origen en el
                                                                                                     centro del texto*/

        leaderboardRect.setFillColor(Color(212,43,43));/**Pinta el leaderboardRect el rectangulo que usamos como boton, de color
                                                       rojo*/
        /**ajustamos el tamaño del boton*/
        leaderboardRect.setSize(sf::Vector2f(botonText.getLocalBounds().width + 40,/**ancho del texto (botonText) + 40 píxeles
                                                                                   de margen (20 a cada lado)*/
                                             leaderboardRect.getSize().y));/**deja el mismo alto que ya tenia el leaderboardRect*/
        /**centra el origen en el centro del boton*/
        leaderboardRect.setOrigin(leaderboardRect.getLocalBounds().width / 2, leaderboardRect.getLocalBounds().height / 2);

        leaderboardRect.setPosition(camara.getCenter().x, camara.getCenter().y / 1.2);/**posiciona el boton con la x del boton en el centro y en 490 la
                                                               altura, osea y*/

        botonText.setPosition(leaderboardRect.getPosition());/**posicionamos el botontext en la misma posicion que el
                                                             leaderboardrect*/

        window.draw(winnerText);/**Dibuja el texto del ganador*/
        window.draw(leaderboardRect);/**Dibuja el botón rojo*/
        window.draw(botonText);/**Dibuja el texto encima del botón rojo*/
    }
    else
    {
        window.draw(m_GanadorRondaText);/**dibuja el texto que dice el ganador*/
    }
    window.draw(menuRect);/**dibuja el rectangulo rojo del boton regresar al menu*/
    window.draw(m_t2);/**dibuja el texto regresar al menu*/
    window.setView(original);/**Vuelve la ventana a la vista original (normal) después de haber usado la vista de combate
                            (camara)*/

    window.display();/**Muestra en pantalla todo lo que dibujamos en este frame*/
}

void Combate::EmpezarSiguienteRound()   // Al comenzar nuevo round // M todo que reinicia variables para el siguiente round
{
    finDeRonda = false;/**La ronda ya no está terminada por lo tanto volvemos al modo de juego normal*/
    m_actualRound++; /**Subir el número de ronda (pasamos a la siguiente)*/
    reloj.Start(); /**Reinicia el cronometro de la ronda (vuelve a empezar desde cero)*/
    p1.SetVida(100.0f); /**Poné la vida de ambos jugadores en 100%*/
    p2.SetVida(100.0f);
    p1.restart(); /**Pone a cada jugador en su posición inicial y con estado inicial(Quieto)*/
    p2.restart();
    m_GanadorRondaText.setString("");/**Borra el cartel de ‘Ganador de la ronda’ que se mostraba al cierre anterior*/
    blackoutRect.setFillColor(Color(0, 0, 0, 0));/**volvemos invisible el color negro traslucido que pusimos antes para
                                                 anunciar al ganador*/
}
