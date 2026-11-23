#ifndef CAMA_H
#define CAMA_H

#include <string>
#include "Paciente.h"
using namespace std;

class Cama {
private:
    int numero;
    Paciente* paciente;
    string estado;
    int turnosRestantes;
    int limpieza;

public:
    Cama(int numero);

    int getNumero();
    Paciente* getPaciente();
    string getEstado();
    int getTurnosRestantes();
    int getTurnosTotales();

    bool estaVacia();
    bool tienePaciente();
    bool tratamientoCompleto();

    void asignar(Paciente* p);
    void tratar();
    Paciente* liberar();
    void avanzarTurno();
    string etiqueta();
};

#endif
