#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;
struct Carta
{
    char rango[3];
    char palo[10];
    int valor;
};
struct Jugador
{
    Carta mano[11];
    int tamanio_mano;
    int puntaje;
};
void inicializarMazo(Carta mazo[52])
{
    const char* rangos[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    const char* palos[] = {"Picas", "Corazones", "Diamantes", "Treboles"};
    int pos = 0;
    for(int s = 0; s < 4; ++s)
    {
        for(int i = 0; i < 13; ++i)
        {
            Carta c;
            int j = 0;
            while (rangos[i][j] != '\0')
            {
                c.rango[j] = rangos[i][j];
                j++;
            }
            c.rango[j] = '\0';
            j = 0;
            while (palos[s][j] != '\0')
            {
                c.palo[j] = palos[s][j];
                j++;
            }
            c.palo[j] = '\0';
            if (c.rango[0] == 'A' && c.rango[1] == '\0') c.valor = 11;
            else if (c.rango[0] == 'J' || c.rango[0] == 'Q' || c.rango[0] == 'K') c.valor = 10;
            else c.valor = atoi(c.rango);
            mazo[pos++] = c;
        }
    }
}
void mezclarMazo(Carta mazo[52])
{
    for(int i = 51; i > 0; --i)
    {
        int j = rand() % (i + 1);
        Carta temp = mazo[i];
        mazo[i] = mazo[j];
        mazo[j] = temp;
    }
}
bool cartaExiste(const Jugador &jugador, const Carta &carta)
{
    for(int i = 0; i < jugador.tamanio_mano; ++i)
    {
        bool mismoRango = true, mismoPalo = true;
        int j = 0;
        while(jugador.mano[i].rango[j] != '\0' && carta.rango[j] != '\0')
        {
            if (jugador.mano[i].rango[j] != carta.rango[j])
            {
                mismoRango = false;
                break;
            }
            j++;
        }
        if (jugador.mano[i].rango[j] != '\0' || carta.rango[j] != '\0')
            mismoRango = false;
        j = 0;
        while(jugador.mano[i].palo[j] != '\0' && carta.palo[j] != '\0')
        {
            if (jugador.mano[i].palo[j] != carta.palo[j])
            {
                mismoPalo = false;
                break;
            }
            j++;
        }
        if (jugador.mano[i].palo[j] != '\0' || carta.palo[j] != '\0')
            mismoPalo = false;
        if (mismoRango && mismoPalo)
            return true;
    }
    return false;
}
void repartirCartas(Carta mazo[52], int &indiceMazo, Jugador &jugador)
{
    if (indiceMazo >= 52 || jugador.tamanio_mano >= 11)
        return;
    Carta cartaActual = mazo[indiceMazo];
    while (cartaExiste(jugador, cartaActual))
    {
        indiceMazo++;
        if (indiceMazo >= 52)
            return;
        cartaActual = mazo[indiceMazo];
    }
    jugador.mano[jugador.tamanio_mano++] = cartaActual;
    indiceMazo++;
}
void mostrarMano(const Jugador &jugador, const char* nombre, bool mostrarSoloUnaCarta = false, bool revelarPuntaje = false)
{
    cout << nombre << " mano: ";
    if (mostrarSoloUnaCarta && jugador.tamanio_mano > 0)
    {
        cout << "[Carta oculta]    ";
        cout << jugador.mano[1].rango << " de " << jugador.mano[1].palo << "    ";
        if (revelarPuntaje)
            cout << "(Puntaje: " << jugador.puntaje << ")";
        else
            cout << "(Puntaje: ?)";
        cout << endl;
        return;
    }
    for(int i = 0; i < jugador.tamanio_mano; ++i)
    {
        cout << jugador.mano[i].rango << " de " << jugador.mano[i].palo << "    ";
    }
    cout << "(Puntaje: " << jugador.puntaje << ")" << endl;
}
int calcularPuntaje(Jugador &jugador)
{
    int total = 0;
    int ases = 0;
    for(int i = 0; i < jugador.tamanio_mano; ++i)
    {
        total += jugador.mano[i].valor;
        if (jugador.mano[i].rango[0] == 'A' && jugador.mano[i].rango[1] == '\0')
            ases++;
    }
    while (total > 21 && ases > 0)
    {
        total -= 10;
        ases--;
    }
    return total;
}
void separarCartas(const Carta mazo[52])
{
    cout << "Mazo completo de 52 cartas:" << endl;
    for(int i = 0; i < 52; ++i)
    {
        cout << mazo[i].rango << " de " << mazo[i].palo << "    ";
        if ((i+1) % 4 == 0) cout << endl;
    }
    cout << endl;
}
void turnoBanca(Carta mazo[52], int &indiceMazo, Jugador &banca)
{
    cout << "\nTurno de la banca:" << endl;
    mostrarMano(banca, "Banca");
    while (banca.puntaje < 17)
    {
        repartirCartas(mazo, indiceMazo, banca);
        banca.puntaje = calcularPuntaje(banca);
        mostrarMano(banca, "Banca");
    }
}
void determinarGanador(const Jugador &jugador, const Jugador &banca)
{
    cout << "\n=== RESULTADO FINAL ===" << endl;
    mostrarMano(jugador, "Jugador");
    mostrarMano(banca, "Banca");
    if (jugador.puntaje > 21)
    {
        cout << "La banca gana. El jugador se paso de 21." << endl;
    } 
    else if (banca.puntaje > 21)
    {
        cout << "El jugador gana. La banca se paso de 21." << endl;
    } 
    else if (jugador.puntaje > banca.puntaje)
    {
        cout << "El jugador gana con " << jugador.puntaje 
             << " puntos contra " << banca.puntaje << " de la banca." << endl;
    } 
    else if (jugador.puntaje < banca.puntaje) {
        cout << "La banca gana con " << banca.puntaje 
             << " puntos contra " << jugador.puntaje << " del jugador." << endl;
    } 
    else {
        cout << "Empate. Ambos tienen " << jugador.puntaje << " puntos." << endl;
    }
}
void guardarPartida(const string& resultado)
{
    ofstream archivo("historial.txt", ios::app);
    if (archivo.is_open())
    {
        time_t ahora = time(nullptr);
        archivo << ctime(&ahora) << resultado << "\n";
        archivo.close();
    }
}
void mostrarHistorial()
{
    ifstream archivo("historial.txt");
    cout << "\n=== HISTORIAL DE PARTIDAS ===\n";
    if (archivo.is_open())
    {
        string linea;
        while (getline(archivo, linea))
        {
            cout << linea << "\n";
        }
        archivo.close();
    } else
    {
        cout << "No hay historial aun.\n";
    }
    cout << "\nPresiona Enter para continuar...";
    cin.ignore();
    cin.get();
}
int mostrarMenu()
{
    int opcion;
    cout << "\n=== MENU BLACKJACK ===\n"
         << "1. Jugar nueva partida\n"
         << "2. Ver historial\n"
         << "3. Salir\n"
         << "Seleccione: ";
    cin >> opcion;
    while (opcion < 1 || opcion > 3)
    {
        cout << "Opcion no valida. Elija 1-3: ";
        cin >> opcion;
    }  
    return opcion;
}
string determinarResultado(Jugador &jugador, Jugador &banca)
{
    if (jugador.puntaje > 21)
    {
        guardarPartida("Jugador se paso de 21. Gana la banca");
        return "La banca gana. El jugador se paso de 21.";
    } 
    else if (banca.puntaje > 21)
    {
        guardarPartida("Banca se paso de 21. Gana el jugador");
        return "El jugador gana. La banca se paso de 21.";
    }
    else if (jugador.puntaje == 21 && jugador.tamanio_mano == 2)
    {
        guardarPartida("Jugador gano con BLACKJACK");
        return "El jugador gana con BLACKJACK.";
    } 
    else if (jugador.puntaje > banca.puntaje)
    {
        guardarPartida("Jugador gana " + to_string(jugador.puntaje) + " vs " + to_string(banca.puntaje));
        return "El jugador gana con " + to_string(jugador.puntaje) + " contra " + to_string(banca.puntaje);
    } 
    else if (jugador.puntaje < banca.puntaje)
    {
        guardarPartida("Banca gana " + to_string(banca.puntaje) + " vs " + to_string(jugador.puntaje));
        return "La banca gana con " + to_string(banca.puntaje) + " contra " + to_string(jugador.puntaje);
    } 
    else
    {
        guardarPartida("Empate con " + to_string(jugador.puntaje) + " puntos");
        return "Empate. Ambos tienen " + to_string(jugador.puntaje) + " puntos.";
    }
}
int main()
{
    srand(time(0));
    Carta mazo[52];
    int opcion = mostrarMenu();
    while (opcion != 3)
    {
        if (opcion == 1)
        {
            bool seguirJugando = true; 
            while (seguirJugando)
            {
                inicializarMazo(mazo);
                mezclarMazo(mazo);
                separarCartas(mazo);
                int indiceMazo = 0;
                Jugador jugador = { {}, 0, 0 };
                Jugador banca = { {}, 0, 0 };
                repartirCartas(mazo, indiceMazo, jugador);
                repartirCartas(mazo, indiceMazo, banca);
                repartirCartas(mazo, indiceMazo, jugador);
                repartirCartas(mazo, indiceMazo, banca);
                jugador.puntaje = calcularPuntaje(jugador);
                banca.puntaje = calcularPuntaje(banca);
                mostrarMano(jugador, "Jugador");
                mostrarMano(banca, "Banca", true, false);
                char opcion;
                bool turnoJugador = true;
                bool jugadorPasado = false;
                if (jugador.puntaje == 21 && jugador.tamanio_mano == 2)
                    {
                        cout << "BLACKJACK Has ganado con 21" << endl;
                    }
                while (turnoJugador && jugador.puntaje < 21 && jugadorPasado == false)
                {
                    cout << "Quieres pedir carta (H) o plantarte (S)? ";
                    cin >> opcion;
                    if (opcion == 'H' || opcion == 'h')
                    {
                        repartirCartas(mazo, indiceMazo, jugador);
                        jugador.puntaje = calcularPuntaje(jugador);
                        mostrarMano(jugador, "Jugador");
                        if (jugador.puntaje > 21)
                        {
                            cout << "Te has pasado de 21. Perdiste." << endl;
                            turnoJugador = false;
                            jugadorPasado = true;
                        } else if (jugador.puntaje == 21)
                        {
                            cout << "Has alcanzado 21! Te plantas automaticamente." << endl;
                            turnoJugador = false;
                        }
                    }
                    else if (opcion == 'S' || opcion == 's')
                    {
                        cout << "Te plantas con puntaje: " << jugador.puntaje << endl;
                        turnoJugador = false;
                        mostrarMano(banca, "Banca", false, true);
                    }
                    else
                    {
                        cout << "Opcion invalida. Por favor ingresa H o S." << endl;
                    }
                }
                if (!jugadorPasado)
                {
                    if (!(jugador.puntaje == 21 && jugador.tamanio_mano == 2))
                    {
                        turnoBanca(mazo, indiceMazo, banca);
                    }
                }
                determinarGanador(jugador, banca);
                string resultado = determinarResultado(jugador, banca);
                cout << "\nQuieres jugar otra partida? (s/n): ";
                char respuesta;
                cin >> respuesta;
                seguirJugando = (respuesta == 's' || respuesta == 'S');
            }
        }
        else if (opcion == 2)
            {
                mostrarHistorial();
            }   
            opcion = mostrarMenu();
        }
    cout << "\nGracias por jugar!\n";
    inicializarMazo(mazo);
    mezclarMazo(mazo);
    separarCartas(mazo);
    int indiceMazo = 0;
    Jugador jugador = { {}, 0, 0 };
    Jugador banca = { {}, 0, 0 };
    repartirCartas(mazo, indiceMazo, jugador);
    repartirCartas(mazo, indiceMazo, banca);
    repartirCartas(mazo, indiceMazo, jugador);
    repartirCartas(mazo, indiceMazo, banca);
    jugador.puntaje = calcularPuntaje(jugador);
    banca.puntaje = calcularPuntaje(banca);
    mostrarMano(jugador, "Jugador");
    mostrarMano(banca, "Banca", true, false);
    char opcion;
    bool turnoJugador = true;
    bool jugadorPasado = false;
    while (turnoJugador && jugador.puntaje <= 21)
    {
        cout << "Quieres pedir carta (H) o plantarte (S)? ";
        cin >> opcion;
        if (opcion == 'H' || opcion == 'h')
        {
            repartirCartas(mazo, indiceMazo, jugador);
            jugador.puntaje = calcularPuntaje(jugador);
            mostrarMano(jugador, "Jugador");
            if (jugador.puntaje > 21)
            {
                cout << "Te has pasado de 21. Perdiste." << endl;
                turnoJugador = false;
                jugadorPasado = true;
            } else if (jugador.puntaje == 21)
            {
                cout << "Has alcanzado 21! Te plantas automaticamente." << endl;
                turnoJugador = false;
            }
        }
        else if (opcion == 'S' || opcion == 's')
        {
            cout << "Te plantas con puntaje: " << jugador.puntaje << endl;
            turnoJugador = false;
            mostrarMano(banca, "Banca", false, true);
        }
        else
        {
            cout << "Opcion invalida. Por favor ingresa H o S." << endl;
        }
    }
    if (!jugadorPasado)
    {
       turnoBanca(mazo, indiceMazo, banca);
    }
    determinarGanador(jugador, banca);
    return 0;
}