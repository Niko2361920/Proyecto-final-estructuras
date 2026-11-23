#include "Hospital.h"
#include "Reglas.h"
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>

string leerLinea() {
    string linea;
    getline(cin, linea);
    while (!linea.empty() && (linea.back() == '\r' || linea.back() == ' ')) {
        linea.pop_back();
    }
    return linea;
}

Hospital::Hospital() {
    srand((unsigned int)time(nullptr));
    medicoActual = nullptr;
    turno = 0;
    siguienteCedula = 1001;
    recibidos = 0;
    altas = 0;
    perdidos = 0;
    sumaEspera = 0;
    recibidosJornada = 0;
    altasJornada = 0;
    perdidosJornada = 0;

    nombres = {"Ana", "Luis", "Marta", "Carlos", "Sofia", "Jorge", "Laura", "Pedro", "Valentina", "Andres"};
    apellidos = {"Ruiz", "Mejia", "Velez", "Perez", "Gomez", "Rojas", "Torres", "Diaz", "Castro", "Lopez"};

    for (int i = 0; i < 3; i++) {
        llegaPaciente();
    }
}

Hospital::~Hospital() {
    for (unordered_map<string, Paciente*>::iterator it = registro.begin(); it != registro.end(); it++) {
        delete it->second;
    }
    for (map<string, Medico*>::iterator it = medicos.begin(); it != medicos.end(); it++) {
        delete it->second;
    }
}

string Hospital::elegirAlAzar(list<string>& opciones) {
    list<string>::iterator it = opciones.begin();
    advance(it, rand() % (int)opciones.size());
    return *it;
}

string Hospital::condicionSegunGravedad(int gravedad) {
    switch (gravedad) {
        case 1: return "Dolor en el pecho, posible infarto";
        case 2: return "Fractura expuesta en la pierna";
        case 3: return "Fiebre alta y deshidratacion";
        case 4: return "Esguince de tobillo";
        default: return "Control de presion arterial";
    }
}

void Hospital::llegaPaciente() {
    string cedula = to_string(siguienteCedula);
    siguienteCedula++;
    string nombre = elegirAlAzar(nombres) + " " + elegirAlAzar(apellidos);
    int gravedad = rand() % 5 + 1;

    int turnoLlegada = (turno == 0) ? 1 : turno;
    Paciente* nuevo = new Paciente(cedula, nombre, gravedad, condicionSegunGravedad(gravedad), turnoLlegada);
    insertarEnCola(nuevo);
    registro[cedula] = nuevo;
}

void Hospital::insertarEnCola(Paciente* paciente) {
    auto posicion = cola.begin();
    // Avanzar sobre los empates conserva el orden de llegada.
    while (posicion != cola.end() && (*posicion)->getGravedad() <= paciente->getGravedad()) {
        ++posicion;
    }
    cola.insert(posicion, paciente);
}

void Hospital::avanzarTurno() {
    turno++;

    for (Paciente* paciente : cola) {
        paciente->esperarTurno();
    }

    revisarCriticos();
    sala.avanzarTurno();

    if (rand() % 2 == 0) {
        llegaPaciente();
    }
}

void Hospital::revisarCriticos() {
    while (!cola.empty() && cola.front()->getRiesgo() > Reglas::LIMITE_RIESGO) {
        Paciente* perdido = cola.front();
        cola.pop_front();
        perdido->setEstado("Perdido");
        perdido->setMedico(medicoActual->getNombre(), turno);
        perdidos++;
        perdidosJornada++;
        cout << "!!! Se perdio al paciente critico " << perdido->getNombre()
             << " (cc " << perdido->getCedula() << ") por esperar demasiado !!!" << endl;
    }
}

void Hospital::mostrarEstado() {
    cout << endl << "==================================================================================================" << endl;
    cout << "Dr. " << medicoActual->getNombre() << " | Turno: " << turno + 1 << "/" << Reglas::MAX_TURNOS << " | Pacientes en cola: " << cola.size() << endl << endl;
    sala.mostrar();
    cout << endl;
    mostrarCama();
    cout << endl;
    mostrarResumenCola();
    mostrarAlertas();
    cout << endl << "Acciones: [A]Izq [D]Der [R]Recibir [T]Tratar [X]Alta [U]Ultimos [C cedula]Consultar [E]Terminar [O]Cambiar medico" << endl;
    cout << "> ";
}

void Hospital::mostrarCama() {
    Cama& cama = sala.camaActual();
    if (!cama.tienePaciente()) {
        cout << "Cama actual: #" << cama.getNumero() << " (" << cama.getEstado() << ")" << endl;
        return;
    }
    Paciente* p = cama.getPaciente();
    cout << "Cama actual: #" << cama.getNumero() << " - " << p->getNombre()
         << " (gravedad " << p->getGravedad() << ", " << p->nombreGravedad() << ")" << endl;
    int total = cama.getTurnosTotales();
    cout << "Tratamiento: " << total - cama.getTurnosRestantes() << "/" << total << " turnos completados";
    if (cama.tratamientoCompleto()) cout << "  -> listo para dar de alta [X]";
    cout << endl;
}

void Hospital::mostrarResumenCola() {
    if (cola.empty()) {
        cout << "Cola de prioridad: vacia" << endl;
        return;
    }
    cout << "Cola de prioridad (resumen):" << endl;
    auto posicion = cola.begin();
    while (posicion != cola.end()) {
        int gravedad = (*posicion)->getGravedad();
        int cantidad = 0;
        int sumaTurnos = 0;
        while (posicion != cola.end() && (*posicion)->getGravedad() == gravedad) {
            cantidad++;
            sumaTurnos += (*posicion)->getTurnosEsperando();
            ++posicion;
        }
        cout << "  - " << cantidad << " paciente(s) gravedad " << gravedad
             << " (esperando " << sumaTurnos / cantidad << " turnos prom.)";
        if (gravedad == 1) cout << " [ALERTA]";
        cout << endl;
    }
}

void Hospital::mostrarAlertas() {
    for (Paciente* p : cola) {
        if (p->getGravedad() != 1) break;
        cout << "  !! RIESGO: " << p->getNombre() << " (cc " << p->getCedula() << ") lleva "
             << p->getRiesgo() << " turnos sin cama. Se pierde si pasa de " << Reglas::LIMITE_RIESGO << "." << endl;
    }
}

bool Hospital::recibir() {
    Cama& cama = sala.camaActual();
    if (!cama.estaVacia()) {
        cout << "La cama actual no esta vacia." << endl;
        return false;
    }
    if (cola.empty()) {
        cout << "No hay pacientes en la cola." << endl;
        return false;
    }
    Paciente* p = cola.front();
    cola.pop_front();
    cama.asignar(p);
    p->setEstado("En cama #" + to_string(cama.getNumero()));
    p->setMedico(medicoActual->getNombre(), turno + 1);
    recibidos++;
    recibidosJornada++;
    sumaEspera += p->getTiempoEspera();
    cout << "Recibiste a " << p->getNombre() << " en la cama #" << cama.getNumero() << "." << endl;
    return true;
}

bool Hospital::tratar() {
    Cama& cama = sala.camaActual();
    if (!cama.tienePaciente()) {
        cout << "No hay paciente en esta cama." << endl;
        return false;
    }
    if (cama.tratamientoCompleto()) {
        cout << "El tratamiento ya esta completo, usa X para dar de alta." << endl;
        return false;
    }
    cama.tratar();
    cout << "Trataste a " << cama.getPaciente()->getNombre() << "." << endl;
    return true;
}

bool Hospital::darAlta() {
    Cama& cama = sala.camaActual();
    if (!cama.tratamientoCompleto()) {
        cout << "No hay un paciente con tratamiento completo en esta cama." << endl;
        return false;
    }
    Paciente* p = cama.liberar();
    p->registrarAlta(medicoActual->getNombre(), turno + 1);
    ultimosAtendidos.push(p);
    altas++;
    altasJornada++;
    cout << p->getNombre() << " fue dado de alta. La cama queda en limpieza." << endl;
    return true;
}

void Hospital::verUltimos() {
    if (ultimosAtendidos.empty()) {
        cout << "Aun no has dado de alta a ningun paciente." << endl;
        return;
    }
    cout << "Dr. " << medicoActual->getNombre() << ", ultimos pacientes dados de alta:" << endl;
    stack<Paciente*> copia = ultimosAtendidos;
    int posicion = 1;
    while (!copia.empty()) {
        Paciente* p = copia.top();
        cout << "  > [" << posicion << "] " << p->getNombre() << " (cc " << p->getCedula()
             << ", gravedad " << p->getGravedad() << ")" << endl;
        copia.pop();
        posicion++;
    }
}

void Hospital::consultar(string cedula) {
    unordered_map<string, Paciente*>::iterator it = registro.find(cedula);
    if (it == registro.end()) {
        cout << "No existe un paciente con cedula " << cedula << "." << endl;
        return;
    }
    cout << "Resultado de busqueda en Tabla Hash (casillero " << registro.bucket(cedula) << "):" << endl;
    it->second->mostrar();
}

Medico* Hospital::obtenerMedico(string nombre) {
    map<string, Medico*>::iterator it = medicos.find(nombre);
    if (it != medicos.end()) {
        return it->second;
    }
    Medico* nuevo = new Medico(nombre);
    medicos[nombre] = nuevo;
    return nuevo;
}

string Hospital::jugarJornada(string nombreMedico) {
    medicoActual = obtenerMedico(nombreMedico);
    turno = 0;
    recibidosJornada = 0;
    altasJornada = 0;
    perdidosJornada = 0;
    ultimosAtendidos = stack<Paciente*>();
    sala.irAlInicio();

    string siguienteMedico = "";
    bool terminar = false;

    while (turno < Reglas::MAX_TURNOS && !terminar) {
        mostrarEstado();
        string linea = leerLinea();
        if (cin.eof() && linea.empty()) break;
        if (linea.empty()) continue;

        char accion = (char)toupper(linea[0]);
        bool usoTurno = false;

        switch (accion) {
            case 'A':
                usoTurno = sala.moverIzquierda();
                if (!usoTurno) cout << "Ya estas en la primera cama." << endl;
                break;
            case 'D':
                usoTurno = sala.moverDerecha();
                if (!usoTurno) cout << "Ya estas en la ultima cama." << endl;
                break;
            case 'R':
                usoTurno = recibir();
                break;
            case 'T':
                usoTurno = tratar();
                break;
            case 'X':
                usoTurno = darAlta();
                break;
            case 'U':
                verUltimos();
                usoTurno = true;
                break;
            case 'C': {
                string cedula = linea.substr(1);
                while (!cedula.empty() && cedula[0] == ' ') cedula.erase(0, 1);
                if (cedula.empty()) {
                    cout << "Cedula: ";
                    cedula = leerLinea();
                }
                if (!cedula.empty()) {
                    consultar(cedula);
                    usoTurno = true;
                }
                break;
            }
            case 'E':
                terminar = true;
                break;
            case 'O':
                cout << "Nombre del nuevo medico: ";
                siguienteMedico = leerLinea();
                if (siguienteMedico.empty() && !cin.eof()) siguienteMedico = "Sin nombre";
                terminar = true;
                break;
            default:
                cout << "Accion no valida." << endl;
        }

        if (usoTurno) {
            avanzarTurno();
        }
    }

    reporteJornada();
    return siguienteMedico;
}

void Hospital::reporteJornada() {
    medicoActual->agregarJornada(turno);
    cout << endl << "Dr. " << medicoActual->getNombre() << " - Reporte de la jornada" << endl;
    cout << "================================" << endl;
    cout << "Pacientes recibidos:      " << recibidosJornada << endl;
    cout << "Pacientes dados de alta:  " << altasJornada << endl;
    cout << "Pacientes perdidos:       " << perdidosJornada << endl;
    cout << "Turnos usados:            " << turno << "/" << Reglas::MAX_TURNOS << endl;
}

void Hospital::verJornadasMedico() {
    if (medicos.empty()) {
        cout << "Aun no hay medicos registrados." << endl;
        return;
    }
    cout << "Medicos registrados:" << endl;
    for (map<string, Medico*>::iterator it = medicos.begin(); it != medicos.end(); it++) {
        cout << "  - " << it->first << endl;
    }
    cout << "Nombre del medico: ";
    string nombre;
    nombre = leerLinea();
    map<string, Medico*>::iterator it = medicos.find(nombre);
    if (it == medicos.end()) {
        cout << "No existe ese medico." << endl;
        return;
    }
    it->second->mostrarJornadas();
}

void Hospital::auditoria() {
    cout << "Auditoria del registro historico (Tabla Hash)" << endl;
    cout << "=============================================" << endl;
    map<string, int> perdidosPorMedico;
    int total = 0;
    for (unordered_map<string, Paciente*>::iterator it = registro.begin(); it != registro.end(); it++) {
        Paciente* p = it->second;
        if (p->getEstado() == "Perdido") {
            total++;
            perdidosPorMedico[p->getMedico()]++;
            cout << "  - " << p->getNombre() << " (cc " << p->getCedula() << ") a cargo de Dr. " << p->getMedico() << endl;
        }
    }
    cout << "Total de pacientes perdidos: " << total << endl;
    for (map<string, int>::iterator it = perdidosPorMedico.begin(); it != perdidosPorMedico.end(); it++) {
        cout << "  Dr. " << it->first << ": " << it->second << endl;
    }
}

void Hospital::metricasFinales() {
    cout << endl << "METRICAS DEL PROGRAMA" << endl;
    cout << "================================" << endl;
    cout << "Pacientes recibidos:        " << recibidos << endl;
    cout << "Pacientes dados de alta:    " << altas << endl;
    cout << "Pacientes perdidos:         " << perdidos << endl;
    if (recibidos > 0) {
        cout << "Tiempo promedio de espera:  " << (double)sumaEspera / recibidos << " min" << endl;
    } else {
        cout << "Tiempo promedio de espera:  0 min" << endl;
    }
    cout << "Camas desocupadas:          " << sala.camasVacias() << "/" << Reglas::CAMAS << endl;
    cout << "Cantidad de medicos:        " << medicos.size() << endl;
}

void Hospital::ejecutar() {
    string opcion = "";
    while (opcion != "5") {
        cout << endl << "====== SIMULADOR DE URGENCIAS ======" << endl;
        cout << "1. Iniciar jornada de un medico" << endl;
        cout << "2. Ver jornadas de un medico" << endl;
        cout << "3. Auditoria de pacientes perdidos" << endl;
        cout << "4. Buscar paciente por cedula" << endl;
        cout << "5. Salir y ver metricas" << endl;
        cout << "Opcion: ";
        if (cin.eof()) break;
        opcion = leerLinea();

        if (opcion == "1") {
            cout << "Nombre del medico: ";
            string nombre;
            nombre = leerLinea();
            if (nombre.empty()) nombre = "Sin nombre";
            while (nombre != "") {
                nombre = jugarJornada(nombre);
            }
        } else if (opcion == "2") {
            verJornadasMedico();
        } else if (opcion == "3") {
            auditoria();
        } else if (opcion == "4") {
            cout << "Cedula: ";
            string cedula;
            cedula = leerLinea();
            consultar(cedula);
        }
    }
    metricasFinales();
}
