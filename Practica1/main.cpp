#include <iostream>
#include <string>
#include <fstream>   // para leer archivos
#include <sstream>   // para separar por comas
#include <vector>    // para guardar listas de datos
using namespace std;

struct Estudiante {
    int carnet;
    string nombre;
    string apellido;
    string carrera;
    int semestre;
};

//Vector global de estudiantes
vector<Estudiante> estudiantes;

// Variables globales para saber si ya se cargaron los archivos
bool estudiantesCargados = false;
bool cursosCargados = false;
bool notasCargados = false;

void cargarEstudiantes();
void cargarCursos();
void cargarNotas();
void reporteEstadisticasPorCurso();
void reporteRendimientoPorEstudiante();
void reporteTop10();
void reporteCursosConMayorReprobacion();
void reporteAnalisisPorCarrera();
void mostrarMenu();

//Función principal
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
            case 9: cout << "\nSaliendo del sistema. Hasta luego!\n"; break;
            default: cout << "\nOpcion invalida. Intente de nuevo.\n";
        }

        if (opcion != 9) {
            cout << "\nPresione ENTER para continuar...";
            cin.get();
        }

    } while (opcion != 9);

    return 0;
}

//menú
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

void cargarEstudiantes() {
    string nombreArchivo;
    cout << "\nIngrese el nombre del archivo (ej: estudiantes.lfp): ";
    getline(cin, nombreArchivo);

    // Intentar abrir el archivo
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo '" << nombreArchivo << "'.\n";
        return;
    }

    // Limpiar el vector
    estudiantes.clear();

    string linea;
    int lineaNum = 0;
    int errores  = 0;

    // Leer el archivo
    while (getline(archivo, linea)) {
        lineaNum++;

        if (!linea.empty() && linea.back() == '\r')
            linea.pop_back();

        // Ignorar líneas vacías
        if (linea.empty()) continue;

        // Separar la línea por comas
        stringstream ss(linea);
        string parte;
        vector<string> campos;

        while (getline(ss, parte, ',')) {   // leer hasta cada coma
            campos.push_back(parte);
        }

        // Validar que la línea tenga 5 campos
        if (campos.size() != 5) {
            cout << "  [Advertencia] Linea " << lineaNum << " ignorada (formato incorrecto): " << linea << "\n";
            errores++;
            continue;
        }

        // Crear un estudiante
        Estudiante e;
        e.carnet    = stoi(campos[0]);   // stoi = string to int
        e.nombre    = campos[1];
        e.apellido  = campos[2];
        e.carrera   = campos[3];
        e.semestre  = stoi(campos[4]);

        // Validar semestre
        if (e.semestre < 1 || e.semestre > 10) {
            cout << "  [Advertencia] Linea " << lineaNum << ": semestre invalido (" << e.semestre << "), se omite.\n";
            errores++;
            continue;
        }

        // Agregar el estudiante al vector
        estudiantes.push_back(e);
    }

    archivo.close();

    // Mostrar resumen de la carga
    cout << "\n--- Carga completada ---\n";
    cout << "Estudiantes cargados: " << estudiantes.size() << "\n";
    if (errores > 0)
        cout << "Lineas con errores:   " << errores << "\n";

    estudiantesCargados = true;
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