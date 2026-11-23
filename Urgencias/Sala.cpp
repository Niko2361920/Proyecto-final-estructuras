#include "Sala.h"
#include "Reglas.h"
#include <iostream>

Sala::Sala() {
    for (int i = 1; i <= Reglas::CAMAS; i++) {
        camas.push_back(Cama(i));
    }
    actual = camas.begin();
}

bool Sala::moverIzquierda() {
    if (actual == camas.begin()) {
        return false;
    }
    actual--;
    return true;
}

bool Sala::moverDerecha() {
    list<Cama>::iterator siguiente = actual;
    siguiente++;
    if (siguiente == camas.end()) {
        return false;
    }
    actual = siguiente;
    return true;
}

void Sala::irAlInicio() {
    actual = camas.begin();
}

Cama& Sala::camaActual() {
    return *actual;
}

void Sala::avanzarTurno() {
    for (list<Cama>::iterator it = camas.begin(); it != camas.end(); it++) {
        it->avanzarTurno();
    }
}

int Sala::camasVacias() {
    int total = 0;
    for (list<Cama>::iterator it = camas.begin(); it != camas.end(); it++) {
        if (!it->tienePaciente()) total++;
    }
    return total;
}

void Sala::mostrar() {
    string lineaCamas = "";
    string lineaMedico = "";
    for (list<Cama>::iterator it = camas.begin(); it != camas.end(); it++) {
        string texto = it->etiqueta();
        while (texto.size() < 8) texto += " ";
        if (it != camas.begin()) {
            lineaCamas += "=";
            lineaMedico += " ";
        }
        lineaCamas += "[" + texto + "]";
        if (it == actual) {
            lineaMedico += "    M     ";
        } else {
            lineaMedico += "          ";
        }
    }
    cout << lineaCamas << endl;
    cout << lineaMedico << endl;
    cout << "V=Vacia  L=Limpieza  P#=Paciente(gravedad)  *=Critico  +=Listo para alta" << endl;
}
