# Simulador de Urgencias

Proyecto de Estructuras de Datos en C++. El simulador funciona en la terminal. Tomas el rol del médico de turno y decides a quién recibir, cuándo tratar a cada paciente y cuándo darlo de alta. La sala tiene diez camas y cada jornada admite hasta 50 turnos.

## Compilar y ejecutar en Windows

Abre la carpeta que contiene la solución `Urgencias.sln` y el directorio `Urgencias/`. Puedes iniciar el simulador desde cualquiera de estos entornos.

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

## Del inicio de la jornada al alta

1. `main()` crea un `Hospital`. Su constructor genera tres pacientes en espera. Cada uno recibe una cédula consecutiva desde 1001, una combinación aleatoria de nombre y apellido y un nivel de gravedad. En este punto las diez camas están vacías.
2. `ejecutar()` presenta el menú. Al iniciar una jornada, `jugarJornada()` recupera o crea el médico por nombre, lo ubica en la primera cama y pone a cero sus contadores de esa jornada.
3. `recibir()` toma al primero de la cola y lo asigna a la cama actual. La tabla por cédula conserva un puntero al mismo paciente. Por eso una consulta refleja los cambios posteriores de estado.
4. Cada tratamiento reduce en uno el contador de la cama. Al completar el último, el paciente queda listo para el alta. La acción `X` registra al médico, agrega al paciente a la pila y pone la cama en mantenimiento.
5. Al cambiar de médico se reinician la posición, los turnos, los contadores de jornada y la pila de altas. Las camas, la cola, los pacientes registrados y las estadísticas globales continúan en memoria hasta cerrar el programa.

### Cuándo avanza el reloj

Los movimientos, las recepciones, los tratamientos y las altas consumen un turno cuando se realizan con éxito. Un intento de moverse a la izquierda desde la primera cama o de dar un alta prematura deja el reloj en el mismo turno. Consultar la pila con `U` consume un turno incluso si está vacía. Consultar con `C` consume un turno al proporcionar una cédula, aunque la búsqueda no encuentre coincidencias. Las opciones del menú principal se ejecutan fuera de la jornada.

Al avanzar el reloj, el hospital incrementa el turno, actualiza la espera de quienes siguen en cola, revisa el riesgo de los críticos y actualiza la limpieza de las camas. Al final puede llegar otro paciente. Una cama liberada inicia un contador de dos avances de limpieza; el avance correspondiente al alta ya cuenta como el primero.

| Gravedad | Clasificación del simulador | Acciones de tratamiento |
|---|---|---|
| 1 | Crítico | 5 |
| 2 | Severo | 4 |
| 3 | Moderado | 3 |
| 4 | Leve | 2 |
| 5 | Consulta | 1 |

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

## Cómo interpretar los reportes

| Vista | Información que muestra |
|---|---|
| Reporte de jornada | Recepciones, altas, pérdidas y turnos usados por el médico en esa jornada. |
| Jornadas por médico | Cantidad de jornadas y turnos trabajados en cada una. La búsqueda usa el nombre con el que se registró. |
| Auditoría | Recorre el registro histórico, identifica los pacientes en estado perdido y agrupa el total por médico a cargo. |
| Métricas finales | Total de recepciones, altas y pérdidas, promedio de espera de los recibidos, camas desocupadas y cantidad de médicos registrados. |

El promedio se calcula dividiendo los minutos acumulados hasta la recepción entre el número de pacientes recibidos. Si todavía no hubo recepciones, se muestra cero. El indicador de camas desocupadas cuenta las camas sin paciente, incluidas las que siguen en limpieza. Para recibir a alguien con `R`, la cama debe haber vuelto al estado vacío.

## Comprobaciones adicionales

- **Límite de movimiento.** Desde la cama 1, ingresar `A`. Se debe conservar la posición y el turno.
- **Empate en triage.** Si dos pacientes tienen la misma gravedad, revisar `Hospital::insertarEnCola()`. Su comparación `<=` coloca al recién llegado detrás de quienes ya esperaban con esa gravedad.
- **Cambio de médico.** Tras dar un alta, anotar la cédula con `U` y cambiar de médico con `O`. La nueva jornada debe comenzar en la cama 1 con la pila vacía. La ficha del paciente anterior debe seguir disponible con `C`.
- **Riesgo crítico.** Si hay un paciente de gravedad 1 en cola, dejarlo esperar siete avances desde su llegada. Debe quedar registrado como perdido. Al finalizar la jornada, la opción 3 permite verificar el médico a cargo.

## Archivos para compilar

`Urgencias.sln` y `Urgencias/Urgencias.vcxproj` describen la solución y el proyecto para Visual Studio. En VS Code, `.vscode/tasks.json` llama al script de compilación y `.vscode/launch.json` inicia el ejecutable. Los archivos generados, como `.obj`, `.pdb` y `.exe`, se vuelven a crear al compilar.
