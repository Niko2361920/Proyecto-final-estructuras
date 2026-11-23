#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <list>
#include <stack>
#include <unordered_map>
#include <map>
#include "Paciente.h"
#include "Cama.h"
#include "Sala.h"
#include "Medico.h"
using namespace std;

class Hospital {
private:
    Sala sala;
    list<Paciente*> cola;
    stack<Paciente*> ultimosAtendidos;
    unordered_map<string, Paciente*> registro;
    map<string, Medico*> medicos;

    list<string> nombres;
    list<string> apellidos;

    Medico* medicoActual;
    int turno;
    int siguienteCedula;

    int recibidos;
    int altas;
    int perdidos;
    int sumaEspera;

    int recibidosJornada;
    int altasJornada;
    int perdidosJornada;

    string elegirAlAzar(list<string>& opciones);
    string condicionSegunGravedad(int gravedad);
    void llegaPaciente();
    void insertarEnCola(Paciente* paciente);
    void avanzarTurno();
    void revisarCriticos();

    void mostrarEstado();
    void mostrarCama();
    void mostrarResumenCola();
    void mostrarAlertas();

    bool recibir();
    bool tratar();
    bool darAlta();
    void verUltimos();
    void consultar(string cedula);

    Medico* obtenerMedico(string nombre);
    string jugarJornada(string nombreMedico);
    void reporteJornada();
    void verJornadasMedico();
    void auditoria();
    void metricasFinales();

public:
    Hospital();
    ~Hospital();
    void ejecutar();
};

#endif
