#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdlib>

using namespace std;

const char MURO = '|';
const char CAMINO = '.';
const char ENTRADA = 'E';
const char SALIDA = 'S';
const char VISITADO = 'x';
const char RECORRIDO_FINAL = '*';

struct Punto {
    int x, y;
    Punto(int _x, int _y) : x(_x), y(_y) {}
};

// Direcciones de movimiento: derecha, abajo, izquierda, arriba
int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};

// Función para verificar si un punto está dentro del laberinto y es un camino
bool esValido(int x, int y, int ancho, int alto, vector<vector<char>>& laberinto) {
    return (x > 0 && x < ancho - 1 && y > 0 && y < alto - 1 && laberinto[y][x] == MURO);
}

// Función para generar el laberinto usando DFS
void generarLaberinto(int ancho, int alto, vector<vector<char>>& laberinto) {
    srand(time(0));
    stack<Punto> pila;
    pila.push(Punto(1, 1));
    laberinto[1][1] = CAMINO;

    random_device rd;
    mt19937 g(rd());

    while (!pila.empty()) {
        Punto actual = pila.top();
        pila.pop();
        vector<int> direcciones = {0, 1, 2, 3};
        shuffle(direcciones.begin(), direcciones.end(), g);

        for (int i = 0; i < 4; ++i) {
            int nx = actual.x + dx[direcciones[i]] * 2;
            int ny = actual.y + dy[direcciones[i]] * 2;

            if (esValido(nx, ny, ancho, alto, laberinto)) {
                laberinto[actual.y + dy[direcciones[i]]][actual.x + dx[direcciones[i]]] = CAMINO;
                laberinto[ny][nx] = CAMINO;
                pila.push(Punto(nx, ny));
            }
        }
    }

    laberinto[0][1] = ENTRADA;
    laberinto[alto-1][ancho-2] = SALIDA;
}

// Función para resolver el laberinto usando backtracking
bool resolverLaberinto(int x, int y, int ancho, int alto, vector<vector<char>>& laberinto) {
    if (x == ancho - 2 && y == alto - 1) {
        laberinto[y][x] = RECORRIDO_FINAL;
        return true;
    }

    if (laberinto[y][x] == CAMINO || laberinto[y][x] == ENTRADA) {
        laberinto[y][x] = VISITADO;

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (resolverLaberinto(nx, ny, ancho, alto, laberinto)) {
                laberinto[y][x] = RECORRIDO_FINAL;
                return true;
            }
        }

        laberinto[y][x] = CAMINO;
    }

    return false;
}

// Función para visualizar el laberinto
void visualizarLaberinto(const vector<vector<char>>& laberinto) {
    for (const auto& fila : laberinto) {
        for (const auto& celda : fila) {
            cout << celda;
        }
        cout << endl;
    }
}

int main() {
    int ancho, alto;
    cout << "Ingrese el ancho del laberinto (número impar mayor que 1): ";
    cin >> ancho;
    cout << "Ingrese el alto del laberinto (número impar mayor que 1): ";
    cin >> alto;

    // Asegurarse de que el ancho y alto sean números impares
    if (ancho % 2 == 0) ancho++;
    if (alto % 2 == 0) alto++;

    vector<vector<char>> laberinto(alto, vector<char>(ancho, MURO));
    generarLaberinto(ancho, alto, laberinto);
    cout << "Laberinto generado:\n";
    visualizarLaberinto(laberinto);

    if (resolverLaberinto(1, 1, ancho, alto, laberinto)) {
        cout << "Laberinto resuelto:\n";
        visualizarLaberinto(laberinto);
    } else {
        cout << "No se encontró una solución para el laberinto.\n";
    }

    return 0;
}

