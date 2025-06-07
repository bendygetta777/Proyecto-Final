#include <iostream>
#include <cstdlib>
#include <ctime>
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
void mostrarMano(const Jugador &jugador, const char* nombre)
{
    cout << nombre << " mano: ";
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
    for(int i = 0; i < 52; ++i) {
        cout << mazo[i].rango << " de " << mazo[i].palo << "    ";
        if ((i+1) % 4 == 0) cout << endl;
    }
    cout << endl;
}
int main()
{
    srand(time(0));
    Carta mazo[52];
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
    mostrarMano(banca, "Banca");
    return 0;
}