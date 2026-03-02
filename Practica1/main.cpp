#include <iostream>
#include <string>
#include <fstream> // para leer archivos
#include <sstream> // para separar por comas
#include <vector> // para guardar listas de datos
#include <filesystem>
#include <cmath> // para sqrt
#include <iomanip> // para fixed y setprecision (decimales)
# include <algorithm> // para sort
using namespace std;
namespace fs = filesystem;

// Se crea la carpeta para guardar los html
string carpetaSalida = "";

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

struct Nota {
    int carnet;
    int codigoCurso;
    float nota; // 0.0 - 100.0
    string ciclo; // 1S o 2S
    int anio;
};

// Vectores globales
vector<Estudiante> estudiantes;
vector<Curso> cursos;
vector<Nota> notas;

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
    // Crear carpeta reportes
    carpetaSalida = (fs::current_path() / "reportes").string();
    fs::create_directories(carpetaSalida); // la crea si no existe, no falla si ya existe
    cout << "Reportes HTML se guardaran en: " << carpetaSalida << "";
    int opcion;

    do {
        mostrarMenu();

        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: cargarEstudiantes(); break;
            case 2: cargarCursos(); break;
            case 3: cargarNotas();break;
            case 4: reporteEstadisticasPorCurso(); break;
            case 5: reporteRendimientoPorEstudiante(); break;
            case 6: reporteTop10(); break;
            case 7: reporteCursosConMayorReprobacion(); break;
            case 8: reporteAnalisisPorCarrera(); break;
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

// Carga de archivos
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
        while (getline(ss, parte, ',')) // esto es el split()
            campos.push_back(parte);

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
    cout << "Cursos cargados: " << cursos.size() << "\n";
    if (errores > 0) cout << "Lineas con errores: " << errores << "\n";

    cursosCargados = true;
}

void cargarNotas() {
    string nombreArchivo = seleccionarArchivo(".lfp", "notas");
    if (nombreArchivo.empty()) return;

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo.\n";
        return;
    }

    notas.clear();
    string linea;
    int lineaNum = 0, errores = 0;

    while (getline(archivo, linea)) {
        lineaNum++;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;

        stringstream ss(linea);
        string parte;
        vector<string> campos;
        while (getline(ss, parte, ',')) campos.push_back(parte);

        // 5 campos
        if (campos.size() != 5) {
            cout << "[Advertencia] Linea" << lineaNum << "ignorada (campos incorrectos: " << campos.size() << "/5)\n)";
            errores++; continue;
        }

        try {
            Nota n;
            n.carnet = stoi(campos[0]);
            n.codigoCurso = stoi(campos[1]);
            n.nota = stof(campos[2]); // string to float
            n.ciclo = campos[3];
            n.anio = stoi(campos[4]);

            // validar nota entre 0 y 100
            if (n.nota < 0.0f || n.nota > 100.0f) {
                cout << "  [Advertencia] Linea " << lineaNum << ": ciclo invalido (\"" << n.ciclo << "\"), se omite.\n";
                errores++;
            }
            notas.push_back(n);
        } catch (const invalid_argument&) {
            cout << "[Error] Linea " << lineaNum << ": valor no numerico -> \"" << linea << "\"\n";
            errores++;
        }
    }
    archivo.close();

    cout << "\n--- Carga completada ---\n";
    cout << "Notas cargadas: " << notas.size() << "\n";
    if (errores > 0) cout << "Lineas con errores: " << errores << "\n";

    notasCargados = true;
}

// Utilidades HTML
void htmlAbre(ofstream& f, const string& titulo) {
    if (!f.is_open()) {
        cout << "  [ERROR HTML] No se pudo crear el archivo HTML.\n";
        cout << "  Carpeta actual: " << fs::current_path().string() << "\n";
        return;
    }
    f << "<!DOCTYPE html>\n<html lang=\"es\">\n<head>\n";
    f << "<meta charset=\"UTF-8\">\n";
    f << "<title>" << titulo << "</title>\n";
    f << "<style>\n";
    f << "  body { font-family: Arial, sans-serif; margin: 30px; background: #f4f4f4; }\n";
    f << "  h1   { background: #2c3e50; color: white; padding: 15px; border-radius: 6px; }\n";
    f << "  h2   { color: #2c3e50; border-bottom: 2px solid #2c3e50; padding-bottom: 5px; }\n";
    f << "  table{ width: 100%; border-collapse: collapse; margin-bottom: 30px; background: white; }\n";
    f << "  th   { background: #2c3e50; color: white; padding: 10px; text-align: left; }\n";
    f << "  td   { padding: 8px 10px; border-bottom: 1px solid #ddd; }\n";
    f << "  tr:hover { background: #eaf0fb; }\n";
    f << "  .aprobado  { color: #27ae60; font-weight: bold; }\n";
    f << "  .reprobado { color: #e74c3c; font-weight: bold; }\n";
    f << "  .badge { background:#2c3e50; color:white; border-radius:12px; padding:2px 10px; font-size:0.85em; }\n";
    f << "</style>\n</head>\n<body>\n";
    f << "<h1>" << titulo << "</h1>\n";
}

void htmlCierra(ofstream& f) {
    f << "</body>\n</html>\n";
}

// Reportes
void reporteEstadisticasPorCurso() {
    if (!cursosCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar cursos y notas primero.\n"; return;
    }

    cout << " REPORTE 1: ESTADISTICAS POR CURSO \n";

    //Abrir html
    ofstream html(carpetaSalida + "/reporte1_estadisticas.html");
    htmlAbre(html, "Reporte 1: Estadisticas por Curso");
    html << fixed << setprecision(2);  // 2 decimales en el html

    for (const Curso& c : cursos) {
        vector<float> notasCurso;
        for (const Nota& n : notas)
            if (n.codigoCurso == c.codigo)
                notasCurso.push_back(n.nota);

        html << "<h2>" << c.nombre << " <span class=\"badge\">Codigo: " << c.codigo << "</span></h2>\n";

        if (notasCurso.empty()) {
            cout << "  Sin calificaciones registradas.\n";
            html << "<p>Sin calificaciones registradas.</p>\n";
            continue;
        }

        float suma = 0, maximo = notasCurso[0], minimo = notasCurso[0];
        for (float nota : notasCurso) {
            suma += nota;
            if (nota > maximo) maximo = nota;
            if (nota < minimo) minimo = nota;
        }
        float promedio = suma / notasCurso.size();

        float sumaDif = 0;
        for (float nota : notasCurso)
            sumaDif += (nota - promedio) * (nota - promedio);
        float desviacion = sqrt(sumaDif / notasCurso.size());

        vector<float> ord = notasCurso;
        sort(ord.begin(), ord.end());
        float mediana;
        int tam = ord.size();
        mediana = (tam % 2 == 0) ? (ord[tam/2-1] + ord[tam/2]) / 2.0f : ord[tam/2];

        // HTML
        html << "<table>\n";
        html << "<tr><th>Estadistica</th><th>Valor</th></tr>\n";
        html << "<tr><td>Estudiantes</td><td>" << notasCurso.size() << "</td></tr>\n";
        html << "<tr><td>Promedio</td><td>" << promedio << "</td></tr>\n";
        html << "<tr><td>Nota maxima</td><td>" << maximo << "</td></tr>\n";
        html << "<tr><td>Nota minima</td><td>" << minimo << "</td></tr>\n";
        html << "<tr><td>Desv. estandar</td><td>" << desviacion << "</td></tr>\n";
        html << "<tr><td>Mediana</td><td>" << mediana << "</td></tr>\n";
        html << "</table>\n";
    }

    htmlCierra(html);
    html.close();

    cout << "\n========================================\n";
    cout << "Archivo generado: " << carpetaSalida << "/reporte1_estadisticas.html\n";
}

void reporteRendimientoPorEstudiante() {
if (!estudiantesCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar estudiantes y notas primero.\n"; return;
    }

    cout << " REPORTE 2: RENDIMIENTO POR ESTUDIANTE  \n";

    ofstream html(carpetaSalida + "/reporte2_rendimiento.html");
    htmlAbre(html, "Reporte 2: Rendimiento por Estudiante");
    html << fixed << setprecision(2);
    html << "<table>\n<tr><th>Nombre</th><th>Carnet</th><th>Carrera</th><th>Semestre</th>"
         << "<th>Promedio</th><th>Aprobados</th><th>Reprobados</th><th>Creditos</th></tr>\n";

    for (const Estudiante& e : estudiantes) {
        vector<float> misNotas;
        int aprobados = 0, reprobados = 0, creditos = 0;

        for (const Nota& n : notas) {
            if (n.carnet == e.carnet) {
                misNotas.push_back(n.nota);
                if (n.nota >= 61) {
                    aprobados++;
                    for (const Curso& c : cursos)
                        if (c.codigo == n.codigoCurso) { creditos += c.creditos; break; }
                } else reprobados++;
            }
        }

        cout << fixed << setprecision(2);

        if (misNotas.empty()) {
            html << "<tr><td>" << e.nombre << " " << e.apellido << "</td><td>" << e.carnet
                 << "</td><td>" << e.carrera << "</td><td>" << e.semestre
                 << "</td><td colspan=\"4\">Sin calificaciones</td></tr>\n";
            continue;
        }

        float suma = 0;
        for (float nota : misNotas) suma += nota;
        float promedio = suma / misNotas.size();

        string clasePromedio = (promedio >= 61) ? "aprobado" : "reprobado";
        html << "<tr><td>" << e.nombre << " " << e.apellido << "</td>"
             << "<td>" << e.carnet << "</td>"
             << "<td>" << e.carrera << "</td>"
             << "<td>" << e.semestre << "</td>"
             << "<td class=\"" << clasePromedio << "\">" << promedio << "</td>"
             << "<td class=\"aprobado\">"  << aprobados  << "</td>"
             << "<td class=\"reprobado\">" << reprobados << "</td>"
             << "<td>" << creditos << "</td></tr>\n";
    }

    html << "</table>\n";
    htmlCierra(html);
    html.close();
    cout << "\n========================================\n";
    cout << "Archivo generado: " << carpetaSalida << "/reporte2_rendimiento.html\n";
}

void reporteTop10() {
    if (!estudiantesCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar estudiantes y notas primero.\n"; return;
    }

    vector<pair<float, int>> promedios;
    for (int i = 0; i < (int)estudiantes.size(); i++) {
        float suma = 0; int cnt = 0;
        for (const Nota& n : notas)
            if (n.carnet == estudiantes[i].carnet) { suma += n.nota; cnt++; }
        if (cnt > 0) promedios.push_back({suma / cnt, i});
    }

    sort(promedios.begin(), promedios.end(), [](const pair<float,int>& a, const pair<float,int>& b) {
        return a.first > b.first;
    });

    cout << " REPORTE 3: TOP 10 MEJORES ESTUDIANTES\n";

    ofstream html(carpetaSalida + "/reporte3_top10.html");
    htmlAbre(html, "Reporte 3: Top 10 Mejores Estudiantes");
    html << fixed << setprecision(2);
    html << "<table>\n<tr><th>#</th><th>Carnet</th><th>Nombre</th><th>Carrera</th><th>Semestre</th><th>Promedio</th></tr>\n";

    int limite = min((int)promedios.size(), 10);
    for (int i = 0; i < limite; i++) {
        const Estudiante& e = estudiantes[promedios[i].second];
        float prom = promedios[i].first;
        html << "<tr><td>" << (i+1) << "</td><td>" << e.carnet
             << "</td><td>" << e.nombre << " " << e.apellido
             << "</td><td>" << e.carrera << "</td><td>" << e.semestre
             << "</td><td class=\"aprobado\">" << prom << "</td></tr>\n";
    }

    html << "</table>\n";
    htmlCierra(html);
    html.close();
    cout << "\n========================================\n";
    cout << "Archivo generado: " << carpetaSalida << "/reporte3_top10.html\n";
}

void reporteCursosConMayorReprobacion() {
    if (!cursosCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar cursos y notas primero.\n"; return;
    }

    struct EstadoCurso {
        int codigo;
        string nombre;
        int total, aprobados, reprobados;
        float porcentaje;
    };

    vector<EstadoCurso> estados;
    for (const Curso& c : cursos) {
        EstadoCurso ec;
        ec.codigo = c.codigo; ec.nombre = c.nombre;
        ec.total = ec.aprobados = ec.reprobados = 0;
        for (const Nota& n : notas) {
            if (n.codigoCurso == c.codigo) {
                ec.total++;
                if (n.nota >= 61) ec.aprobados++;
                else ec.reprobados++;
            }
        }
        if (ec.total > 0) {
            ec.porcentaje = (ec.reprobados * 100.0f) / ec.total;
            estados.push_back(ec);
        }
    }

    sort(estados.begin(), estados.end(), [](const EstadoCurso& a, const EstadoCurso& b) {
        return a.porcentaje > b.porcentaje;
    });
    cout << " REPORTE 4: CURSOS CON MAYOR REPROBACION\n";

    ofstream html(carpetaSalida + "/reporte4_reprobacion.html");
    htmlAbre(html, "Reporte 4: Cursos con Mayor Reprobacion");
    html << fixed << setprecision(1);
    html << "<table>\n<tr><th>Codigo</th><th>Nombre</th><th>Total</th>"
         << "<th>Aprobados</th><th>Reprobados</th><th>% Reprobacion</th></tr>\n";

    for (const EstadoCurso& ec : estados) {

        string clase = (ec.porcentaje > 50) ? "reprobado" : "aprobado";
        html << "<tr><td>" << ec.codigo << "</td><td>" << ec.nombre
             << "</td><td>" << ec.total << "</td>"
             << "<td class=\"aprobado\">" << ec.aprobados << "</td>"
             << "<td class=\"reprobado\">" << ec.reprobados << "</td>"
             << "<td class=\"" << clase << "\">" << ec.porcentaje << "%</td></tr>\n";
    }

    html << "</table>\n";
    htmlCierra(html);
    html.close();
    cout << "\n========================================\n";
    cout << "Archivo generado: " << carpetaSalida << "/reporte4_reprobacion.html\n";
}

void reporteAnalisisPorCarrera() {
    if (!estudiantesCargados || !notasCargados) {
        cout << "\nERROR: Debe cargar estudiantes y notas primero.\n"; return;
    }

    vector<string> carreras;
    for (const Estudiante& e : estudiantes) {
        bool existe = false;
        for (const string& car : carreras)
            if (car == e.carrera) { existe = true; break; }
        if (!existe) carreras.push_back(e.carrera);
    }

    cout << "REPORTE 5: ANALISIS POR CARRERA\n";

    ofstream html(carpetaSalida + "/reporte5_carreras.html");
    htmlAbre(html, "Reporte 5: Analisis por Carrera");
    html << fixed << setprecision(2);

    for (const string& carrera : carreras) {

        int totalEst = 0;
        float sumaNotas = 0; int cntNotas = 0;

        for (const Estudiante& e : estudiantes) {
            if (e.carrera == carrera) {
                totalEst++;
                for (const Nota& n : notas)
                    if (n.carnet == e.carnet) { sumaNotas += n.nota; cntNotas++; }
            }
        }

        int totalCursos = 0;
        for (const Curso& c : cursos)
            if (c.carrera == carrera) totalCursos++;

        float prom = (cntNotas > 0) ? sumaNotas / cntNotas : 0.0f;

        html << "<h2>Carrera: " << carrera << "</h2>\n";
        html << "<table>\n<tr><th>Dato</th><th>Valor</th></tr>\n";
        html << "<tr><td>Total estudiantes</td><td>" << totalEst << "</td></tr>\n";
        html << "<tr><td>Promedio general</td><td>" << prom << "</td></tr>\n";
        html << "<tr><td>Cursos disponibles</td><td>" << totalCursos << "</td></tr>\n";
        html << "</table>\n";

        html << "<h3>Distribucion por semestre</h3>\n";
        html << "<table>\n<tr><th>Semestre</th><th>Estudiantes</th></tr>\n";

        for (int sem = 1; sem <= 10; sem++) {
            int cnt = 0;
            for (const Estudiante& e : estudiantes)
                if (e.carrera == carrera && e.semestre == sem) cnt++;
            if (cnt > 0) {
                html << "<tr><td>Semestre " << sem << "</td><td>" << cnt << "</td></tr>\n";
            }
        }
        html << "</table>\n";
    }

    htmlCierra(html);
    html.close();
    cout << "\n========================================\n";
    cout << "Archivo generado: " << carpetaSalida << "/reporte5_carreras.html\n";
}