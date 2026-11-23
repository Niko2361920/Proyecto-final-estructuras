#include "Medico.h"
#include <iostream>

Medico::Medico(string nombre) {
    this->nombre = nombre;
}

string Medico::getNombre() { return nombre; }

int Medico::cantidadJornadas() {
    return (int)jornadas.size();
}

void Medico::agregarJornada(int turnosTrabajados) {
    jornadas.push_back(turnosTrabajados);
}

void Medico::mostrarJornadas() {
    cout << "Dr. " << nombre << " ha trabajado " << jornadas.size() << " jornada(s)" << endl;
    int numero = 1;
    for (list<int>::iterator it = jornadas.begin(); it != jornadas.end(); it++) {
        cout << "  Jornada " << numero << ": " << *it << " turnos" << endl;
        numero++;
    }
}
