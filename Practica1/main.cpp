#include <iostream>
#include <string>
#include <fstream> // para leer archivos
#include <sstream> // para separar por comas
#include <vector> // para guardar listas de datos
#include <filesystem> //
using namespace std;
namespace fs = filesystem;

struct Estudiante {
    int carnet;
    string nombre;
    string apellido;
    string carrera;
    int semestre;
};

struct Curso {
    int codigo;
    string nombre;
    int creditos; // 1-8
    int semestre; // semestre en que se imparte 1-10
    string carrera;
};

// Vectores globales
vector<Estudiante> estudiantes;
vector<Curso> cursos;

//carga
bool estudiantesCargados = false;
bool cursosCargados = false;
bool notasCargados = false;

string seleccionarArchivo(const string& extension);
void cargarEstudiantes();
void cargarCursos();
void cargarNotas();
void reporteEstadisticasPorCurso();
void reporteRendimientoPorEstudiante();
void reporteTop10();
void reporteCursosConMayorReprobacion();
void reporteAnalisisPorCarrera();
void mostrarMenu();

// Seleccionar archivos a cargar
string seleccionarArchivo(const string& extension, const string& sugerido = "") {
    vector<string> encontrados;

    for (const auto& entrada : fs::directory_iterator(".")) {
        if (entrada.is_regular_file()) {
            string nombre = entrada.path().filename().string();
            // El archivo debe tener la extension correcta
            if (nombre.size() < extension.size()) continue;
            if (nombre.substr(nombre.size() - extension.size()) != extension) continue;
            // Si se dio una palabra clave, el nombre debe contenerla
            if (!sugerido.empty() && nombre.find(sugerido) == string::npos) continue;
            encontrados.push_back(nombre);
        }
    }

    // Si no hay coincidencias con el filtro, mostrar todos los .lfp
    if (encontrados.empty()) {
        cout << "\n[Aviso] No se encontro un archivo con \"" << sugerido << "\" en el nombre.\n";
        cout << "Mostrando todos los archivos " << extension << " disponibles:\n";
        for (const auto& entrada : fs::directory_iterator(".")) {
            if (entrada.is_regular_file()) {
                string nombre = entrada.path().filename().string();
                if (nombre.size() >= extension.size() &&
                    nombre.substr(nombre.size() - extension.size()) == extension)
                    encontrados.push_back(nombre);
            }
        }
    }

    if (encontrados.empty()) {
        cout << "\nNo se encontraron archivos *" << extension << " en la carpeta actual.\n";
        return "";
    }

    cout << "\nArchivos disponibles:\n";
    for (int i = 0; i < (int)encontrados.size(); i++)
        cout << "  " << (i + 1) << ". " << encontrados[i] << "\n";
    cout << "  0. Cancelar\n";
    cout << "Seleccione un archivo: ";

    int op;
    cin >> op;
    cin.ignore();

    if (op < 1 || op > (int)encontrados.size()) {
        cout << "Operacion cancelada.\n";
        return "";
    }
    return encontrados[op - 1];
}

// Funcion principal
int main() {
    int opcion;

    do {
        mostrarMenu();

        cout << "Seleccione una opcion: ";
        cin >> opcion;
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

// MENÚ
void mostrarMenu() {
    cout << "\n========================================\n";
    cout << "     SISTEMA DE ANALISIS ACADEMICO      \n";
    cout << "========================================\n";
    cout << " Archivos cargados: ";
    cout << "[Estudiantes:" << (estudiantesCargados ? "SI" : "NO") << "] ";
    cout << "[Cursos:" << (cursosCargados ? "SI" : "NO") << "] ";
    cout << "[Notas:" << (notasCargados ? "SI" : "NO") << "]\n";
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
    string nombreArchivo = seleccionarArchivo(".lfp", "estudiantes");
    if (nombreArchivo.empty()) return;

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo.\n";
        return;
    }

    estudiantes.clear();
    string linea;
    int lineaNum = 0, errores = 0;

    while (getline(archivo, linea)) {
        lineaNum++;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;

        // Separar por comas
        stringstream ss(linea);
        string parte;
        vector<string> campos;
        while (getline(ss, parte, ',')) campos.push_back(parte);

        if (campos.size() != 5) {
            cout << "  [Advertencia] Linea " << lineaNum << " ignorada (campos incorrectos: " << campos.size() << "/5)\n";
            errores++;
            continue;
        }

        try {
            Estudiante e;
            e.carnet = stoi(campos[0]);
            e.nombre = campos[1];
            e.apellido = campos[2];
            e.carrera = campos[3];
            e.semestre = stoi(campos[4]);

            if (e.semestre < 1 || e.semestre > 10) {
                cout << "  [Advertencia] Linea " << lineaNum << ": semestre invalido, se omite.\n";
                errores++;
                continue;
            }

            estudiantes.push_back(e);

        } catch (const invalid_argument&) {
            cout << "  [Error] Linea " << lineaNum << ": valor no numerico -> \"" << linea << "\"\n";
            errores++;
        }

    }

    archivo.close();

    cout << "\n--- Carga completada ---\n";
    cout << "Estudiantes cargados: " << estudiantes.size() << "\n";
    if (errores > 0)
        cout << "Lineas con errores: " << errores << "\n";

    estudiantesCargados = true;
}

void cargarCursos() {
    string nombreArchivo = seleccionarArchivo(".lfp", "cursos");
    if (nombreArchivo.empty()) return;

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo.\n";
        return;
    }

    cursos.clear();
    string linea;
    int lineaNum = 0, errores = 0;

    while (getline(archivo, linea)) {
        lineaNum++;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;

        // Separar por comas
        stringstream ss(linea);
        string parte;
        vector<string> campos;
        while (getline(ss, parte, ',')) campos.push_back(parte);

        // Validar que tenga 5 campos
        if (campos.size() != 5) {
            cout << "  [Advertencia] Linea " << lineaNum << " ignorada (campos incorrectos: " << campos.size() << "/5)\n";
            errores++; continue;
        }

        try {
            Curso c;
            c.codigo = stoi(campos[0]);
            c.nombre = campos[1];
            c.creditos = stoi(campos[2]);
            c.semestre = stoi(campos[3]);
            c.carrera = campos[4];

            // Validar rangos
            if (c.creditos < 1 || c.creditos > 8) {
                cout << "[Advertencia] Linea " << lineaNum << ": Creditos invalidos (" << c.creditos << "), se omite.\n";
                errores++; continue;
            }

            if (c.semestre < 1 || c.semestre > 10) {
                cout << "[Advertencia] Linea " << lineaNum << ": Semestre invalido (" << c.semestre << "), se omite.\n";
                errores++; continue;
            }

            cursos.push_back(c);

        } catch (const invalid_argument&) {
            cout << "  [Error] Linea " << lineaNum << ": valor no numerico -> \"" << linea << "\"\n";
            errores++;
        }

    }
    archivo.close();

    cout << "\n--- Carga completada ---\n";
    cout << "Cursos cargados:  " << cursos.size() << "\n";
    if (errores > 0) cout << "Lineas con errores: " << errores << "\n";

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