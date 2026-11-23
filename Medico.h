#ifndef MEDICO_H
#define MEDICO_H

#include <string>
#include <list>
using namespace std;

class Medico {
private:
    string nombre;
    list<int> jornadas;

public:
    Medico(string nombre);

    string getNombre();
    int cantidadJornadas();
    void agregarJornada(int turnosTrabajados);
    void mostrarJornadas();
};

#endif
