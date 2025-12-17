#include <fstream>        // Para manejo de archivos (lectura/escritura)
#include <vector>         // Para usar contenedores dinámicos (vector)
#include "Archivo.h"      // Incluye la definición de la clase Archivo
#include "Combate.h"      // Incluye la clase Combate (para lógica relacionada)
#include "menu.h"         // Incluye la clase Menu (para volver al menú)
#include <cstring>        // Para funciones de manejo de cadenas (strcpy)
#include <algorithm>      // Para funciones como sort y reverse
#include <sstream>        // Para crear cadenas dinámicas (stringstream)
#include <iostream>       // Para salida en consola (depuración)
#include "JugadorData.h"
using namespace sf;       // Permite usar clases de SFML sin prefijo sf::
using namespace std;      // Permite usar clases estándar sin prefijo std::

/* Constructor de Archivo: inicializa la pantalla del leaderboard */

Archivo::Archivo(std::string fnombre, int GanadorPuntos, bool puedeGuardar)
    : m_filename(fnombre), m_GanadorPuntos(GanadorPuntos), puedeGuardar(puedeGuardar)
{

    // Fuente
    m_f1.loadFromFile("Src/fonts/arcade.ttf");

    // Fondo
    m_tex_fondodegradado.loadFromFile("src/Fondo degradado.png");
    m_spr_fondodegradado.setTexture(m_tex_fondodegradado);

    m_tex_logo.loadFromFile("Src/world fight titulo del juego.png");
    m_spr_logo.setTexture(m_tex_logo);

    // Resolución actual
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    float screenWidth  = static_cast<float>(desktop.width);
    float screenHeight = static_cast<float>(desktop.height);

    // Escala del fondo
    sf::Vector2u bgSize = m_tex_fondodegradado.getSize();
    float scaleX = screenWidth / bgSize.x;
    float scaleY = screenHeight / bgSize.y;
    float finalScale = std::max(scaleX, scaleY);
    m_spr_fondodegradado.setScale(finalScale, finalScale);

    // Base para uiScale (ancho del fondo)
    float baseWidth = static_cast<float>(bgSize.x);
    float uiScale = screenWidth / baseWidth;

    // Logo adaptado
    m_spr_logo.setScale(0.1859f * uiScale, 0.1859f * uiScale);
    m_spr_logo.setPosition(20.f * uiScale, 10.f * uiScale);

    // Botón "Volver"
    boton1.setFillColor(sf::Color(212, 43, 43));
    boton1.setSize(sf::Vector2f(230.f * uiScale, 30.f * uiScale));
    boton1.setPosition(30.f * uiScale, screenHeight - (80.f * uiScale));

    // Texto dentro del botón
    m_t6.setFont(m_f1);
    m_t6.setString("Volver al menu");
    m_t6.setCharacterSize(static_cast<unsigned>(14 * uiScale));
    m_t6.setFillColor(sf::Color::White);
    m_t6.setPosition(
        boton1.getPosition().x + (boton1.getSize().x - m_t6.getLocalBounds().width) * 0.5f,
        boton1.getPosition().y + (boton1.getSize().y - m_t6.getLocalBounds().height) * 0.5f
    );

    // Títulos principales
    m_t1.setFont(m_f1);
    m_t1.setString("Tabla de");
    m_t1.setCharacterSize(static_cast<unsigned>(18 * uiScale));
    m_t1.setFillColor(sf::Color::White);
    m_t1.setPosition(screenWidth * 0.55f, 20.f * uiScale);

    m_t7.setFont(m_f1);
    m_t7.setString("Clasificacion");
    m_t7.setCharacterSize(static_cast<unsigned>(18 * uiScale));
    m_t7.setFillColor(sf::Color::White);
    m_t7.setPosition(screenWidth * 0.48f, 55.f * uiScale);

    // Texto dinámico (nombre ganador)
    m_t2.setFont(m_f1);
    m_t2.setCharacterSize(static_cast<unsigned>(22 * uiScale));
    m_t2.setFillColor(sf::Color::White);
    m_t2.setPosition(40.f * uiScale, 192.f * uiScale);

    if (puedeGuardar)
    {
        m_t3.setFont(m_f1);
        m_t3.setString("Escribe el nombre del ganador");
        m_t3.setCharacterSize(static_cast<unsigned>(8 * uiScale));
        m_t3.setFillColor(sf::Color::White);
        m_t3.setPosition(30.f * uiScale, 170.f * uiScale);

        m_t4.setFont(m_f1);
        m_t4.setString("Presiona ENTER para guardar");
        m_t4.setCharacterSize(static_cast<unsigned>(8 * uiScale));
        m_t4.setFillColor(sf::Color(255, 255, 255, 0));
        m_t4.setPosition(30.f * uiScale, 220.f * uiScale);

        m_t5.setFont(m_f1);
        m_t5.setString(std::to_string(GanadorPuntos) + " puntos");
        m_t5.setCharacterSize(static_cast<unsigned>(5 * uiScale));
        m_t5.setFillColor(sf::Color::White);
        m_t5.setPosition(50.f * uiScale, 485.f * uiScale);
    }

    // Encabezados columnas
    m_nombre.setFont(m_f1);
    m_nombre.setString("NOMBRE");
    m_nombre.setCharacterSize(static_cast<unsigned>(10 * uiScale));
    m_nombre.setFillColor(sf::Color::White);
    m_nombre.setPosition(screenWidth * 0.45f, 80.f * uiScale);

    m_puntos.setFont(m_f1);
    m_puntos.setString("PUNTOS");
    m_puntos.setCharacterSize(static_cast<unsigned>(10 * uiScale));
    m_puntos.setFillColor(sf::Color::White);
    m_puntos.setPosition(screenWidth * 0.61f, 80.f * uiScale);

    m_totalVictorias.setFont(m_f1);
    m_totalVictorias.setString("VICTORIAS");
    m_totalVictorias.setCharacterSize(static_cast<unsigned>(10 * uiScale));
    m_totalVictorias.setFillColor(sf::Color::White);
    m_totalVictorias.setPosition(screenWidth * 0.75f, 80.f * uiScale);

    // Historial de rachas
    m_rachaLista.setFont(m_f1);
    m_rachaLista.setCharacterSize(static_cast<unsigned>(8 * uiScale));
    m_rachaLista.setFillColor(sf::Color::White);
    m_rachaLista.setPosition(screenWidth * 0.45f, screenHeight * 0.55f);

    // Cargar datos previos
    LoadDataFromFile(m_filename);
    m_PuntajeSaved = false;
}

/* Destructor: guarda datos al salir */
Archivo::~Archivo()
{
    SaveDataToFile(m_filename); /**Cuando el objeto se destruye, guarda el ranking en el archivo.*/
}

void Archivo::ProcessEvents(Juego &juego, sf::Event &event)
{
    if ((event.type == sf::Event::TextEntered) && puedeGuardar) /**Basicamente dice: “Si el tipo de evento que
                                                                ocurrio es que el usuario escribió algo en el teclado
                                                                y además esta permitido guardar el nombre (puedeGuardar
                                                                es verdadero), entonces entra a este bloque y procesa
                                                                la entrada de texto.”*/
    {
        if (event.text.unicode < 128 && event.text.unicode != 27) /**Si el código del caracter que se escribio es menor a 128
                                                                  (es decir, es un carácter ASCII válido) y no es la tecla ESC
                                                                   (cuyo código es 27), entonces lo aceptamos como valido*/
        {
            if (event.text.unicode == 8 && !currentInput.empty()) /**Si el usuario presiono la tecla de borrar texto (codigo 8)
                                                                   y el texto actual no esta vacio, entonces borra el ultimo
                                                                   caracter del texto que se está escribiendo*/
            {
                currentInput.pop_back(); /**la funcion de pop_back es  Eliminar el ultimo elemento de la cadena o vector*/
            }
            else if (event.text.unicode != 8 && currentInput.length() < 8)
            {
                if (event.text.unicode != 13) // Si no es ENTER
                {
                    currentInput += static_cast<char>(event.text.unicode);
                }
            }

            m_t2.setString(currentInput); // Actualiza texto en pantalla
            string input = currentInput;

            // Si se presiona ENTER y hay texto y aún no guardaste el puntaje
            if (event.text.unicode == 13 && !currentInput.empty() && !m_PuntajeSaved)
            {

// Buscar a mano el jugador con nombre == input
                auto it = m_leaders.end(); // por defecto "no encontrado"
                for (auto iter = m_leaders.begin(); iter != m_leaders.end(); ++iter)
                {
                    if (iter->GetNombre() == input)
                    {
                        it = iter;
                        break;
                    }
                }
                if (it == m_leaders.end())
                {
                    // Nuevo jugador
                    JugadorData newJugador;
                    newJugador.SetNombre(currentInput);
                    newJugador.SetPuntaje(m_GanadorPuntos);
                    newJugador.IncrementarVictorias();
                    m_leaders.push_back(newJugador);
                    m_PuntajeSaved = true;
                }
                else
                {
                    // Jugador existente: actualiza datos
                    it->AddPuntaje(m_GanadorPuntos);
                    it->IncrementarVictorias();
                    m_PuntajeSaved = true;
                }

                // Actualiza rachas
                if (ganadorID == 1)
                {
                    juego.IncrementarRachaJugador1();
                    GuardarRacha(nombreGanador, juego.GetRachaJugador1(), 1);
                    GuardarRacha(nombreJugador2, juego.GetRachaJugador2(), 2);
                }
                else if (ganadorID == 2)
                {
                    juego.IncrementarRachaJugador2();
                    GuardarRacha(nombreGanador, juego.GetRachaJugador2(), 2);
                    GuardarRacha(nombreJugador1, juego.GetRachaJugador1(), 1);
                }

// Ordenar descendente por puntaje (burbuja)
                for (size_t i = 0; i < m_leaders.size(); ++i) /**Recorre la lista de jugadores varias veces*/
                {
                    for (size_t j = 0; j + 1 < m_leaders.size(); ++j) /**En cada pasada, compara dos jugadores consecutivos*/
                    {
                        if (m_leaders[j].GetPuntaje() < m_leaders[j + 1].GetPuntaje()) /**si el puntaje del jugador actual es
                                                                                       menor que el del siguiente*/
                        {
                            swap(m_leaders[j], m_leaders[j + 1]); /**Funcion de swap intercambia los valores de dos objetos
                                                                    asi los jugadores con mas puntos van quedando al principio*/
                        }
                    }
                }
            }
        }

        // Mostrar texto "Press ENTER" si hay algo escrito
        if (!currentInput.empty())
            m_t4.setFillColor(sf::Color(255, 255, 255, 100));
        else
            m_t4.setFillColor(sf::Color(255, 255, 255, 0));
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos(sf::Mouse::getPosition(juego.GetWindow()));

        if (boton1.getGlobalBounds().contains(mousePos))
        {
            SaveDataToFile(m_filename);
            juego.SetFondo(TipoEscena::MENU);
        }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            SaveDataToFile(m_filename);
            juego.SetFondo(TipoEscena::MENU);
        }
    }
}

/* Actualiza los textos con los datos del leaderboard */
void Archivo::Update(Juego &juego)
{
    stringstream nombres, puntos, victorias; /**Creo tres objetos especiales para ir armando texto dinamico: uno para
                                             nombres, otro para puntos y otro para victorias, utilizamos stringstream
                                             para evitar concatenar manualmente cada dato con el operador + y agregar
                                             saltos de linea siendo mas eficiente*/
    nombres<<"NOMBRES"<<endl;
    puntos<<"PUNTOS"<<endl;
    victorias<<"VICTORIAS"<<endl;


    for (const auto& p : m_leaders) /**recorre cada jugador que esta guardado en el vector m_leaders, auto es lo que hace
                                    que deduzca automaticamente el tipo de dato y el & es para qu trabajemos con el original
                                     y no una copia, y p es el iterador*/
    {
        nombres   << p.GetNombre()         <<endl;
        puntos    << p.GetPuntaje()        <<endl;
        victorias << p.GetTotalVictorias() <<endl;

        m_nombre.setString(nombres.str()); /**Con el metodo str() convierto toda la lista de nombres de tipo stringstream en
                                           un solo texto y lo muestro en pantalla usando el objeto tipo Text de la libreria
                                           SFML m_nombre (este es el nombre de la lista nombres)*/
        m_puntos.setString(puntos.str());
        m_totalVictorias.setString(victorias.str());

        FILE* archivo = fopen("rachas.dat", "rb"); /**Abrimos el archivo llamado rachas.dat en modo lectura binaria*/

        if (archivo)   /**Si archivo no es NULL, significa que el archivo se abrió bien*/
        {

            fseek(archivo, 0, SEEK_END); /**mueve el puntero al final del archivo.*/
            long tam = ftell(archivo); /**nos dice en qué posición está el puntero, eso equivale al tamaño total en bytes*/
            fseek(archivo, 0, SEEK_SET); /**vuelve el puntero al inicio para poder leer desde el principio*/

            if (tam > 0)  /**entra si el archivo no esta vacio, osea contenido con tamaño mayor a 0*/
            {

                string contenido = "Historial de rachas:\n"; /**encabezado para mostrar el historial*/
                string buffer;           /**creamos un string llamado buffer*/

                buffer.resize(static_cast<size_t>(tam)); /**y lo redimensionamos con el resize para que tenga espacio suficiente para
                                                 todos los bytes del archivo**/

                size_t leidos = fread(&buffer[0], 1, static_cast<size_t>(tam), archivo);/**creamos una variable size_t donde se guardara
                                                                               el resultado de fread(osea la cantidad de bytes
                                                                                leidos)osea lee hasta tam bytes del archivo
                                                                                archivo, guárdalos en buffer (empezando en el
                                                                                primer caracter[0]), y dime cuántos bytes se
                                                                                leyeron realmente*/
                buffer.resize(leidos); /** reduce o ajusta el tamaño del string buffer para que tenga exactamente la cantidad
                               de bytes que se leyeron del archivo */
                fclose(archivo);/**Cierra el archivo*/

                contenido += buffer;  /**El operador += en strings concatena/une el texto de la derecha al de la izquierda.*/
                m_rachaLista.setString(contenido); /**Toma el texto que está en contenido y muéstralo en el objeto gráfico m_rachaLista*/
            }
            else
            {
                fclose(archivo); /**cierra el archivo*/
                m_rachaLista.setString("Historial de rachas:\n(archivo vacío)");/**si el archivo esta vacio escribe que esta vacio*/
            }
        }
        else
        {
            m_rachaLista.setString("No se pudo leer el historial de rachas.");
        }
    }
}
/* Dibuja la pantalla del leaderboard */
void Archivo::Draw(RenderWindow &window)
{
    window.clear({0,0,0});
    window.draw(m_spr_fondodegradado);
    window.draw(m_spr_logo);
    window.draw(boton1);
    window.draw(m_t1);
    window.draw(m_t7);
    window.draw(m_t2);
    window.draw(m_t3);
    window.draw(m_t4);
    window.draw(m_t5);
    window.draw(m_t6);
    window.draw(m_nombre);
    window.draw(m_puntos);
    window.draw(m_totalVictorias);

    window.draw(m_rachaLista);

    window.display();
}

/* Guarda datos en archivo binario */

void Archivo::SaveDataToFile(const string& filename)
{
    ofstream file(filename); /**Creamos un objeto file de tipo ofstream (flujo de salida a archivo) y lo abrimos con el
                             nombre que nos pasaron por lo tanto, ofstream abre el archivo en modo texto y lo sobrescribe
                             si ya existe*/
    if (!file.is_open()) /**Verificamos si el archivo se abrió correctamente*/
    {
        cerr << "Error al abrir " << filename <<endl;/**mensaje de error*/
        return;
    }

    for (const auto& p : m_leaders) /**Recorremos cada elemento p dentro de la coleccion m_leaders, auto& significa que p
                                    es una referencia al objeto jugador y const indica que no lo vamos a modificar*/
    {
        file << p.GetNombre() << ';' /**Escribimos en el archivo los datos del jugador*/
             << p.GetPuntaje() << ';' /**el ';' se usa como separador de datos*/
             << p.GetTotalVictorias() <<endl;
    }
}

void Archivo::LoadDataFromFile(const string& filename) /** método que carga datos desde un archivo*/
{
    ifstream file(filename); /**Abrimos el archivo en modo lectura usando ifstream*/
    if (!file) return; /**Si el archivo no se pudo abrir, salimos de la función sin hacer nada*/

    string nombre;/**Creamos variables para guardar temporalmente el nombre del jugador, sus puntos y sus victorias*/
    int puntos, victorias;

    while (file.good()) /**Mientras el archivo esté en buen estado (no error ni fin de archivo), seguimos leyendo datos*/
    {
        if (!getline(file, nombre, ';')) break; /**Leemos desde el archivo hasta encontrar un ";" y guardamos eso en nombre
                                                  Si falla, salimos del bucle*/
        string temp; /**Creamos una variable temporal para leer los números como texto*/

        if (!getline(file, temp, ';')) break; /**Leemos el puntaje como texto hasta ; y lo convertimos a número con stoi*/
        puntos = stoi(temp);

        if (!getline(file, temp)) break; // hasta fin de línea
        victorias = stoi(temp);

        JugadorData jd; /** Crea un objeto JugadorData*/
        jd.SetNombre(nombre);/**Asigna el nombre*/
        jd.SetPuntaje(puntos);/**Asigna el puntaje*/
        // En vez de bucle, asignamos directamente:
        for (int i = 0; i < victorias; ++i) jd.IncrementarVictorias(); /** Incrementa victorias*/

        m_leaders.push_back(jd); /**Esta línea mete el jugador que acabamos de leer del archivo dentro del vector m_leaders
                                 que maneja el ranking. Sin esto, el jugador no apareceria en la tabla*/
    }
}

void Archivo::GuardarRacha(const string& nombre, int racha, int id)
{
    ofstream file("rachas.dat", ios::app);/**Abro (o creo si no existe) un archivo llamado rachas.dat en modo agregar (append),
                                           para escribir al final sin borrar lo anterior.*/
    if (file.is_open()) /**Si el archivo se abrió correctamente, entro a escribir.*/
    {
        file <<id<<" - "<< nombre << ": " << racha << " victorias seguidas\n"; /**Escribo una línea con el formato
                                                                            id(1 o 2) - nombre: racha victorias seguidas y un
                                                                            salto de línea al final*/
        file.close();/**Cierro el archivo para que se guarde bien lo escrito y liberar el recurso*/
    }
    else
    {
        cerr << "No se pudo abrir rachas.dat para guardar la racha." <<endl;/**Si no pude abrir el archivo, muestro un mensaje
                                                                            de error*/
    }
}

void Archivo::CargarRachasDesdeArchivo()
{
    ifstream file("rachas.dat");/**Creo un objeto llamado file que intenta abrir rachas.dat para leer.*/
    if (!file.is_open()) /**Pregunto: ¿el archivo se abrió bien? Si no, muestro un error y me voy del método*/
    {
        cerr << "No se pudo abrir rachas.dat para leer las rachas." << std::endl;
        return;
    }
    stringstream ss;/**Creo un objeto llamado ss que sirve como acumulador de texto dinámico*/
    ss << "Historial de rachas:"<<endl;/**Escribo el título ‘Historial de rachas:’ dentro del acumulador ss, seguido de
                                       un salto de línea*/
    string linea;/**Declaro una variable de texto llamada linea para guardar, temporalmente, cada línea que leo del archivo*/

    while (getline(file, linea))/**Mientras pueda leer una línea desde el archivo (usando getline con file),
                                la agrego al acumulador de texto ss*/
    {
        ss << linea <<endl;/**Agregá la línea que acabo de leer al acumulador ss y después meté un salto de línea
                            para que quede ordenado*/
    }
    file.close();/**Cierra el archivo*/
    m_rachaLista.setString(ss.str());/**Convertí todo lo acumulado en ss (con ss.str()) a un solo texto y mostralo en la
                                     interfaz, usando el objeto gráfico m_rachaLista*/
}
void Archivo::SetJugadores(const string& j1, const string& j2)
{
    nombreJugador1 = j1;
    nombreJugador2 = j2;
}

void Archivo::SetGanador(const string& nombre, int id)
{
    nombreGanador = nombre;
    ganadorID = id;
}
