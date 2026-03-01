#include <iostream>
#include <string>
using namespace std;

// Variables globales para saber si ya se cargaron los archivos
bool estudiantesCargados = false;
bool cursosCargados      = false;
bool notasCargados       = false;

// ---- Prototipos de funciones (las implementaremos después) ----
void cargarEstudiantes();
void cargarCursos();
void cargarNotas();
void reporteEstadisticasPorCurso();
void reporteRendimientoPorEstudiante();
void reporteTop10();
void reporteCursosConMayorReprobacion();
void reporteAnalisisPorCarrera();
void mostrarMenu();

// Función principal
int main() {
    int opcion;

    do {
        mostrarMenu();

        cout << "Seleccione una opcion: ";
        cin  >> opcion;

        // Limpiar el buffer
        cin.ignore();

        switch (opcion) {
            case 1: cargarEstudiantes();               break;
            case 2: cargarCursos();                    break;
            case 3: cargarNotas();                     break;
            case 4: reporteEstadisticasPorCurso();     break;
            case 5: reporteRendimientoPorEstudiante(); break;
            case 6: reporteTop10();                    break;
            case 7: reporteCursosConMayorReprobacion();break;
            case 8: reporteAnalisisPorCarrera();       break;
            case 9: cout << "\nSaliendo...\n"; break;
            default: cout << "\nOpcion invalida. Intente de nuevo.\n";
        }

        // Pausa para que el usuario pueda leer el resultado
        if (opcion != 9) {
            cout << "\nPresione ENTER para continuar...";
            cin.get();
        }

    } while (opcion != 9);

    return 0;
}

// Muestra el menú
void mostrarMenu() {

    cout << "\n========================================\n";
    cout << "     SISTEMA DE ANALISIS ACADEMICO      \n";
    cout << "========================================\n";

    // Muestra el estado de carga de cada archivo
    cout << " Archivos cargados: ";
    cout << "[Estudiantes:" << (estudiantesCargados ? "SI" : "NO") << "] ";
    cout << "[Cursos:"      << (cursosCargados      ? "SI" : "NO") << "] ";
    cout << "[Notas:"       << (notasCargados        ? "SI" : "NO") << "]\n";
    cout << "----------------------------------------\n";

    cout << " 1. Cargar archivo de estudiantes\n";
    cout << " 2. Cargar archivo de cursos\n";
    cout << " 3. Cargar archivo de notas\n";
    cout << "----------------------------------------\n";
    cout << " 4. Reporte: Estadisticas por Curso\n";
    cout << " 5. Reporte: Rendimiento por Estudiante\n";
    cout << " 6. Reporte: Top 10 Mejores Estudiantes\n";
    cout << " 7. Reporte: Cursos con Mayor Reprobacion\n";
    cout << " 8. Reporte: Analisis por Carrera\n";
    cout << "----------------------------------------\n";
    cout << " 9. Salir\n";
    cout << "========================================\n";
}

// ---- Funciones placeholder (por ahora solo muestran un mensaje) ----
// Las iremos completando en los siguientes pasos

void cargarEstudiantes() {
    cout << "\n[TODO] Cargar archivo estudiantes.lfp\n";
    estudiantesCargados = true; // cambiar a true solo cuando realmente cargue
}

void cargarCursos() {
    cout << "\n[TODO] Cargar archivo cursos.lfp\n";
    cursosCargados = true;
}

void cargarNotas() {
    cout << "\n[TODO] Cargar archivo notas.lfp\n";
    notasCargados = true;
}

void reporteEstadisticasPorCurso() {
    if (!cursosCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar cursos y notas primero.\n";
        return;
    }
    cout << "\n[TODO] Generando reporte de estadisticas por curso...\n";
}

void reporteRendimientoPorEstudiante() {
    if (!estudiantesCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar estudiantes y notas primero.\n";
        return;
    }
    cout << "\n[TODO] Generando reporte de rendimiento por estudiante...\n";
}

void reporteTop10() {
    if (!estudiantesCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar estudiantes y notas primero.\n";
        return;
    }
    cout << "\n[TODO] Generando Top 10 mejores estudiantes...\n";
}

void reporteCursosConMayorReprobacion() {
    if (!cursosCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar cursos y notas primero.\n";
        return;
    }
    cout << "\n[TODO] Generando reporte de cursos con mayor reprobacion...\n";
}

void reporteAnalisisPorCarrera() {
    if (!estudiantesCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar estudiantes y notas primero.\n";
        return;
    }
    cout << "\n[TODO] Generando analisis por carrera...\n";
}