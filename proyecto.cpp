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
void inicializarMazo(Carta mazo[52])
{
    const char* rangos[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    const char* palos[] = {"Picas", "Corazones", "Diamantes", "Treboles"};
    int pos = 0;
    for(int s = 0; s < 4; ++s) {
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
void separarCartas(const Carta mazo[52])
{
    cout << "Mazo completo de 52 cartas:" << endl;
    for(int i = 0; i < 52; ++i) {
        cout << mazo[i].rango << " de " << mazo[i].palo << "    ";
        if ((i+1) % 4 == 0) cout << endl;
    }
}
int main()
{
    srand(time(0));
    Carta mazo[52];
    inicializarMazo(mazo);
    separarCartas(mazo);
    return 0;
}