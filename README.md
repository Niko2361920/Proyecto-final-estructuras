# Simulador de Urgencias

Proyecto de Estructuras de Datos en C++. El simulador funciona en la terminal. Tomas el rol del médico de turno y decides a quién recibir, cuándo tratar a cada paciente y cuándo darlo de alta. La sala tiene diez camas y cada jornada admite hasta 50 turnos.

> **Estado del repositorio.** La primera entrega contiene las clases `Cama`, `Sala` y `Medico`, además de la configuración de compilación. Para ejecutar el simulador deben integrarse `Hospital.cpp`, `Hospital.h`, `Paciente.cpp`, `Paciente.h`, `main.cpp` y `Reglas.h` en la carpeta `Urgencias/`.

## Compilar y ejecutar en Windows

Para obtener el proyecto:

```powershell
git clone https://github.com/Niko2361920/Proyecto-final-estructuras.git
cd Proyecto-final-estructuras
```

Una vez integrados todos los archivos fuente, puedes elegir cualquiera de estas opciones.

| Entorno | Requisitos y pasos |
|---|---|
| **VS Code** | Instala la extensión **C/C++** de Microsoft y las herramientas de compilación de Visual Studio con la carga **Desarrollo para el escritorio con C++**. Abre la carpeta raíz y pulsa **F5**. Si aparece un selector, elige **Ejecutar Urgencias**. La tarea prepara MSVC, compila los archivos `.cpp` y abre el simulador en la terminal integrada. |
| **Visual Studio** | Instala la carga **Desarrollo para el escritorio con C++**, el conjunto de herramientas **MSVC v143** y un SDK de Windows compatible. Abre `Urgencias.sln`, selecciona **Debug / x64** y pulsa **F5**. |
| **PowerShell** | Con las herramientas de C++ instaladas, ejecuta los comandos de abajo desde la raíz. |

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .vscode/build.ps1
.\build\Urgencias.exe
```

## Cómo se juega

Elige **1** en el menú principal y escribe el nombre del médico. La **M** indica su posición. Cada acción se escribe en la terminal y se confirma con **Enter**.

| Entrada | Acción |
|---|---|
| `A` / `D` | Ir a la cama anterior o siguiente. |
| `R` | Recibir al primero de la cola en la cama actual, si está disponible. |
| `T` | Aplicar un tratamiento al paciente de la cama actual. |
| `X` | Dar de alta al paciente cuando completa el tratamiento. La cama pasa a limpieza. |
| `U` | Ver las altas de la jornada, empezando por la más reciente. |
| `C 1001` | Buscar en el historial el paciente con cédula `1001`. |
| `E` | Cerrar la jornada y mostrar su reporte. |
| `O` | Cambiar de médico e iniciar otra jornada. |

El menú principal también permite consultar las jornadas de un médico (**2**), auditar pacientes perdidos (**3**), buscar por cédula (**4**) y salir con las métricas acumuladas (**5**).

La gravedad **1** es la más urgente y la **5** la menos urgente. Si dos pacientes tienen la misma gravedad, pasa primero quien llegó antes. Cada turno que un paciente sigue en cola añade cinco minutos de espera. Un paciente crítico pasa a perdido si supera seis turnos en cola. El tratamiento requiere **6 − gravedad** acciones `T`.

## Cómo está organizado el código

| Archivo | Responsabilidad |
|---|---|
| `main.cpp` | Crea el hospital y llama a `ejecutar()`. |
| `Hospital.h` y `Hospital.cpp` | Coordinan el menú, el triage, el avance de turnos, los registros y las métricas. |
| `Sala.h` y `Sala.cpp` | Guardan las camas en una lista doblemente enlazada. Un iterador marca la cama actual del médico. |
| `Cama.h` y `Cama.cpp` | Administran el estado de una cama, su paciente, el tratamiento pendiente y la limpieza. |
| `Paciente.h` y `Paciente.cpp` | Guardan la cédula, la gravedad, la espera, el riesgo y los datos de atención. |
| `Medico.h` y `Medico.cpp` | Guardan las jornadas de cada médico. |
| `Reglas.h` | Reúne las constantes del simulador. |

### Estructuras de datos

- **Lista doblemente enlazada** (`std::list<Cama>`). Permite mover el iterador de la sala hacia la cama anterior o siguiente.
- **Cola de prioridad** (`std::list<Paciente*>` ordenada). La inserción recorre la lista y coloca cada paciente detrás de quienes ya esperan con su misma gravedad.
- **Pila** (`std::stack<Paciente*>`). La última alta aparece primero al consultar con `U`.
- **Tabla hash** (`std::unordered_map`). Busca pacientes por cédula y conserva sus fichas entre jornadas mientras el programa está abierto.
- **Mapa ordenado** (`std::map`). Permite buscar médicos por nombre y consultar sus jornadas.

## Caso de prueba · Alta y consulta del historial

1. Inicia una jornada con **1** y un nombre de médico. Comprueba que el médico comienza en la cama **1**.
2. Pulsa **R**. Observa el nombre y la gravedad del paciente que ocupa la cama. Los pacientes se generan al azar, así que los datos pueden variar.
3. Intenta dar el alta con **X**. La cama debe seguir ocupada porque aún falta tratamiento.
4. Pulsa **T** hasta completar el tratamiento. La etiqueta de la cama muestra **+** cuando el paciente está listo para el alta.
5. Pulsa **X**. La cama entra en limpieza. Usa **U** para ver el alta y anotar la cédula. Luego usa **C** con esa cédula para comprobar que la ficha sigue en el historial.
6. Usa **E** y después **5**. Comprueba que el total de recibidos y de altas es **1** en esta ejecución.

En las etiquetas de la sala, **V** indica cama vacía, **L** indica limpieza, **P1** a **P5** muestran la gravedad del paciente, **\*** señala un paciente crítico y **+** indica tratamiento completo.
