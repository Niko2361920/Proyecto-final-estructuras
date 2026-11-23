#include "Paciente.h"
#include "Reglas.h"
#include <iostream>

Paciente::Paciente(string cedula, string nombre, int gravedad, string condicion, int turnoLlegada) {
    this->cedula = cedula;
    this->nombre = nombre;
    this->gravedad = gravedad;
    this->condicion = condicion;
    this->turnoLlegada = turnoLlegada;
    tiempoEspera = 0;
    riesgo = 0;
    estado = "En cola";
    medico = "";
    turnoAtencion = 0;
}

string Paciente::getCedula() { return cedula; }
string Paciente::getNombre() { return nombre; }
int Paciente::getGravedad() { return gravedad; }
int Paciente::getTiempoEspera() { return tiempoEspera; }
int Paciente::getTurnosEsperando() { return tiempoEspera / Reglas::MINUTOS_POR_TURNO; }
int Paciente::getRiesgo() { return riesgo; }
string Paciente::getEstado() { return estado; }
string Paciente::getMedico() { return medico; }

void Paciente::esperarTurno() {
    tiempoEspera += Reglas::MINUTOS_POR_TURNO;
    if (gravedad == 1) {
        riesgo++;
    }
}

void Paciente::setEstado(string nuevoEstado) {
    estado = nuevoEstado;
}

void Paciente::setMedico(string nombreMedico, int turno) {
    medico = nombreMedico;
    turnoAtencion = turno;
}

string Paciente::nombreGravedad() {
    switch (gravedad) {
        case 1: return "CRITICO";
        case 2: return "SEVERO";
        case 3: return "MODERADO";
        case 4: return "LEVE";
        default: return "CONSULTA";
    }
}

void Paciente::registrarAlta(string nombreMedico, int turno) {
    estado = "Dado de alta";
    medicoAlta = nombreMedico;
    turnoAlta = turno;
}

void Paciente::mostrar() {
    cout << "  Cedula:      " << cedula << endl;
    cout << "  Nombre:      " << nombre << endl;
    cout << "  Gravedad:    " << gravedad << " (" << nombreGravedad() << ")" << endl;
    cout << "  Condicion:   " << condicion << endl;
    cout << "  Llego en:    turno " << turnoLlegada << endl;
    cout << "  Estado:      " << estado << endl;
    if (medico != "") {
        cout << (estado == "Perdido" ? "  Perdido a cargo de: Dr. " : "  Recibido por: Dr. ") << medico << " (turno " << turnoAtencion << ")" << endl;
    }
    if (!medicoAlta.empty()) {
        cout << "  Alta por: Dr. " << medicoAlta << " (turno " << turnoAlta << ")" << endl;
    }
    cout << "  Espera:      " << tiempoEspera << " min (" << getTurnosEsperando() << " turnos)" << endl;
}
