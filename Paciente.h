#ifndef PACIENTE_H
#define PACIENTE_H

#include <string>
using namespace std;

class Paciente {
private:
    string cedula;
    string nombre;
    int gravedad;
    string condicion;
    int turnoLlegada;
    int tiempoEspera;
    int riesgo;
    string estado;
    string medicoAlta;
    int turnoAlta = 0;
    string medico;
    int turnoAtencion;

public:
    Paciente(string cedula, string nombre, int gravedad, string condicion, int turnoLlegada);

    string getCedula();
    string getNombre();
    int getGravedad();
    int getTiempoEspera();
    int getTurnosEsperando();
    int getRiesgo();
    string getEstado();
    string getMedico();

    void esperarTurno();
    void setEstado(string nuevoEstado);
    void setMedico(string nombreMedico, int turno);
    string nombreGravedad();
    void registrarAlta(string nombreMedico, int turno);
    void mostrar();
};

#endif
