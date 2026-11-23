#include "Cama.h"

Cama::Cama(int numero) {
    this->numero = numero;
    paciente = nullptr;
    estado = "Vacia";
    turnosRestantes = 0;
    limpieza = 0;
}

int Cama::getNumero() { return numero; }
Paciente* Cama::getPaciente() { return paciente; }
string Cama::getEstado() { return estado; }
int Cama::getTurnosRestantes() { return turnosRestantes; }

int Cama::getTurnosTotales() {
    if (paciente == nullptr) return 0;
    return 6 - paciente->getGravedad();
}

bool Cama::estaVacia() {
    return estado == "Vacia";
}

bool Cama::tienePaciente() {
    return paciente != nullptr;
}

bool Cama::tratamientoCompleto() {
    return paciente != nullptr && turnosRestantes == 0;
}

void Cama::asignar(Paciente* p) {
    paciente = p;
    estado = "Ocupada";
    turnosRestantes = getTurnosTotales();
}

void Cama::tratar() {
    if (turnosRestantes > 0) {
        turnosRestantes--;
    }
}

Paciente* Cama::liberar() {
    Paciente* p = paciente;
    paciente = nullptr;
    estado = "Mantenimiento";
    turnosRestantes = 0;
    limpieza = 2;
    return p;
}

void Cama::avanzarTurno() {
    if (estado == "Mantenimiento") {
        limpieza--;
        if (limpieza <= 0) {
            estado = "Vacia";
        }
    }
}

string Cama::etiqueta() {
    string texto = "#" + to_string(numero) + ":";
    if (estado == "Vacia") {
        texto += "V";
    } else if (estado == "Mantenimiento") {
        texto += "L";
    } else {
        texto += "P" + to_string(paciente->getGravedad());
        if (paciente->getGravedad() == 1) texto += "*";
        if (tratamientoCompleto()) texto += "+";
    }
    return texto;
}
