#ifndef SALA_H
#define SALA_H

#include <list>
#include "Cama.h"
using namespace std;

class Sala {
private:
    list<Cama> camas;
    list<Cama>::iterator actual;

public:
    Sala();

    bool moverIzquierda();
    bool moverDerecha();
    void irAlInicio();
    Cama& camaActual();
    void avanzarTurno();
    int camasVacias();
    void mostrar();
};

#endif
