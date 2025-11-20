/**
 * @file Buscador.cpp
 * @author Juan Bohorquez (jbohorquezsa@unal.edu.co)
 * @author Julian Quintero (julquinteroca@unal.edu.co)
 * @author Santiago Herrera (sanherrerapa@unal.edu.co)
 * 
 * @brief Sistema de Gestión y Búsqueda de Correos
 * -------------------------------------------------------------------------
 * Este programa permite:
 *  - Cargar correos desde un archivo de texto.
 *  - Crear correos nuevos con indexación automática.
 *  - Buscar correos por remitente.
 *  - Buscar correos por palabra clave usando matriz dispersa.
 *  - Ordenar correos por fecha mediante un árbol binario.
 * 
 * Estructuras empleadas:
 *  - Mapas hash (unordered_map)
 *  - Árbol binario de búsqueda
 *  - Matriz dispersa (representada como mapas anidados)
 * 
 * Interfaz:
 *  - Basada en terminal con códigos ANSI de color.
 * -------------------------------------------------------------------------
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

// ============================================================================
// COLORES ANSI
// ============================================================================
const string RED   = "\033[31m";
const string GREEN = "\033[32m";
const string WHITE = "\033[37m";
const string BOLD  = "\033[1m";
const string RESET = "\033[0m";

/**
 * @brief Limpia la pantalla (solo Windows).
 */
void limpiarPantalla() {
    system("cls");
}

// ============================================================================
// ESTRUCTURAS
// ============================================================================

/**
 * @struct Correo
 * @brief Representa un correo con metadatos e ID autogenerado.
 */
struct Correo {
    int id;
    string remitente;
    string asunto;
    string cuerpo;
    string fecha;
};

/**
 * @struct NodoCorreo
 * @brief Nodo del árbol binario que almacena un correo.
 */
struct NodoCorreo {
    Correo data;
    NodoCorreo* izq;
    NodoCorreo* der;
    NodoCorreo(Correo c) : data(c), izq(nullptr), der(nullptr) {}
};

/**
 * @class ArbolCorreos
 * @brief Árbol binario ordenado por fecha del correo.
 */
class ArbolCorreos {
private:
    NodoCorreo* raiz;

    /**
     * @brief Inserta un nodo basado en comparación de fecha.
     */
    NodoCorreo* insertar(NodoCorreo* nodo, Correo c) {
        if (!nodo) return new NodoCorreo(c);
        if (c.fecha < nodo->data.fecha)
            nodo->izq = insertar(nodo->izq, c);
        else
            nodo->der = insertar(nodo->der, c);
        return nodo;
    }

    /**
     * @brief Recorrido inOrden para obtener correos ordenados.
     */
    void inOrden(NodoCorreo* nodo, vector<Correo>& lista) {
        if (!nodo) return;
        inOrden(nodo->izq, lista);
        lista.push_back(nodo->data);
        inOrden(nodo->der, lista);
    }

public:
    ArbolCorreos() : raiz(nullptr) {}

    /**
     * @brief Inserta un correo en el árbol.
     */
    void insertar(Correo c) { raiz = insertar(raiz, c); }

    /**
     * @brief Obtiene los correos ordenados por fecha.
     */
    vector<Correo> obtenerOrdenados() {
        vector<Correo> lista;
        inOrden(raiz, lista);
        return lista;
    }
};

// ============================================================================
// ESTRUCTURAS GLOBALES
// ============================================================================
unordered_map<string, vector<Correo>> correosPorRemitente;
unordered_map<int, unordered_map<string, int>> matrizDispersa;
unordered_map<int, Correo> correosPorID;

// ============================================================================
// CREAR CORREO (INDEXACIÓN + MAPAS + MATRIZ DISPERSA)
// ============================================================================
/**
 * @brief Crea un correo nuevo, lo indexa y actualiza estructuras globales.
 */
Correo crearCorreo(string rem, string asu, string cue, string fecha) {
    static int nextID = 1;
    Correo c = {nextID++, rem, asu, cue, fecha};

    // Indexación por remitente y por ID
    correosPorRemitente[c.remitente].push_back(c);
    correosPorID[c.id] = c;

    // Conversión a minúsculas
    auto aMinusculas = [](string s) {
        for (char &ch : s) ch = tolower(ch);
        return s;
    };

    string texto = aMinusculas(asu + " " + cue);
    string palabra;

    // Construcción de la matriz dispersa
    for (char ch : texto) {
        if (isalnum((unsigned char)ch))
            palabra += ch;
        else {
            if (!palabra.empty()) {
                matrizDispersa[c.id][palabra] = 1;
                palabra.clear();
            }
        }
    }
    if (!palabra.empty())
        matrizDispersa[c.id][palabra] = 1;

    return c;
}

// ============================================================================
// LEER ARCHIVO TXT
// ============================================================================
/**
 * @brief Carga correos desde un archivo con formato:
 *        remitente;asunto;cuerpo;fecha
 */
void cargarCorreosDesdeArchivo(string nombreArchivo, ArbolCorreos &arbol) {
    ifstream file(nombreArchivo);
    if (!file.is_open()) {
        cout << RED << "No se pudo abrir el archivo.\n" << RESET;
        return;
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string rem, asu, cue, fec;

        getline(ss, rem, ';');
        getline(ss, asu, ';');
        getline(ss, cue, ';');
        getline(ss, fec, ';');

        if (rem.empty()) continue;

        Correo c = crearCorreo(rem, asu, cue, fec);
        arbol.insertar(c);
    }

    file.close();
    cout << GREEN << "Correos cargados correctamente.\n" << RESET;
}

// ============================================================================
// INTERFAZ ANSI
// ============================================================================
/**
 * @brief Muestra un correo completo en pantalla.
 */
void verCorreo(const Correo &c) {
    limpiarPantalla();
    cout << BOLD << WHITE << "[ LEYENDO MENSAJE ]" << RESET << "\n\n";

    cout << GREEN << "ID: " << RESET << c.id << "\n";
    cout << GREEN << "Remitente: " << RESET << WHITE << c.remitente << RESET << "\n";
    cout << GREEN << "Asunto: " << RESET << RED << c.asunto << RESET << "\n";
    cout << GREEN << "Fecha: " << RESET << WHITE << c.fecha << "\n\n";

    cout << WHITE << c.cuerpo << RESET << "\n\n";
    cout << "Presione ENTER para volver...";
    cin.ignore();
    cin.get();
}

/**
 * @brief Despliega la lista de correos ordenados por fecha.
 */
void verOrdenados(ArbolCorreos& arbol) {
    vector<Correo> lista = arbol.obtenerOrdenados();
    limpiarPantalla();

    cout << BOLD << WHITE << "[ CORREOS ORDENADOS POR FECHA ]" << RESET << "\n\n";

    for (auto &c : lista) {
        cout << GREEN << c.id << RESET << "  "
             << WHITE << c.remitente << RESET << "  "
             << RED << c.asunto << RESET << "  "
             << WHITE << c.fecha << RESET << "\n";
    }

    cout << "\nIngrese ID de correo para abrirlo o 0 para volver: ";
    int id; cin >> id;

    if (id != 0 && correosPorID.count(id)) {
        cin.ignore();
        verCorreo(correosPorID[id]);
    }
}

/**
 * @brief Búsqueda de correos según remitente.
 */
void buscarRemitenteANSI() {
    limpiarPantalla();
    cout << BOLD << WHITE << "[ BUSCAR POR REMITENTE ]" << RESET << "\n\n";

    cout << "Ingrese remitente: ";
    string rem;
    cin.ignore();
    getline(cin, rem);

    if (!correosPorRemitente.count(rem)) {
        cout << RED << "No se encontraron correos de ese remitente." << RESET;
        cin.get();
        return;
    }

    auto& lista = correosPorRemitente[rem];

    limpiarPantalla();
    cout << BOLD << WHITE << "[ RESULTADOS ]" << RESET << "\n\n";

    for (auto &c : lista) {
        cout << GREEN << c.id << RESET << "  "
             << WHITE << c.asunto << RESET << "  "
             << WHITE << c.fecha << RESET << "\n";
    }

    cout << "\nIngrese ID para abrir correo o 0 para volver: ";
    int id; cin >> id;

    if (id != 0 && correosPorID.count(id)) {
        cin.ignore();
        verCorreo(correosPorID[id]);
    }
}

/**
 * @brief Búsqueda basada en la matriz dispersa por palabra clave.
 */
void buscarPalabraANSI() {
    limpiarPantalla();
    cout << BOLD << WHITE << "[ BUSCAR PALABRA CLAVE ]" << RESET << "\n\n";

    cout << "Ingrese palabra: ";
    string palabra;
    cin.ignore();
    getline(cin, palabra);

    for (char &c : palabra) c = tolower(c);

    vector<int> resultados;

    for (auto &fila : matrizDispersa) {
        if (fila.second.count(palabra))
            resultados.push_back(fila.first);
    }

    limpiarPantalla();

    if (resultados.empty()) {
        cout << RED << "No se encontraron coincidencias." << RESET;
        cin.get();
        return;
    }

    cout << BOLD << WHITE << "[ RESULTADOS ]" << RESET << "\n\n";

    for (int id : resultados) {
        Correo &c = correosPorID[id];
        cout << GREEN << c.id << RESET << "  "
             << RED << c.asunto << RESET << "  "
             << WHITE << c.remitente << RESET
             << "  " << WHITE << c.fecha << RESET << "\n";
    }

    cout << "\nIngrese ID para abrir correo o 0 para volver: ";
    int id; cin >> id;

    if (id != 0 && correosPorID.count(id)) {
        cin.ignore();
        verCorreo(correosPorID[id]);
    }
}

// ============================================================================
// PROGRAMA PRINCIPAL
// ============================================================================
int main() {
    ArbolCorreos arbol;

    // Correos predefinidos
    arbol.insertar(crearCorreo("juan@correo.com", "Reunion de equipo", "Reunion urgente mañana", "2025-11-10"));
    arbol.insertar(crearCorreo("ana@correo.com", "Entrega de tarea", "La tarea esta lista", "2025-11-11"));
    arbol.insertar(crearCorreo("luis@correo.com", "Proyecto nuevo", "Debemos entregar el reporte", "2025-11-09"));

    // Carga de archivo externo
    cargarCorreosDesdeArchivo("correos.txt", arbol);

    // Menú principal
    while (true) {
        limpiarPantalla();
        cout << BOLD << WHITE << "[ MENU PRINCIPAL ]" << RESET << "\n\n";
        cout << GREEN << "1" << RESET << ". Ver correos ordenados\n";
        cout << GREEN << "2" << RESET << ". Buscar por remitente\n";
        cout << GREEN << "3" << RESET << ". Buscar por palabra clave\n";
        cout << GREEN << "0" << RESET << ". Salir\n\n";

        cout << "Seleccione una opcion: ";
        int op;
        cin >> op;

        if (op == 0) break;
        if (op == 1) verOrdenados(arbol);
        else if (op == 2) buscarRemitenteANSI();
        else if (op == 3) buscarPalabraANSI();
    }

    return 0;
}
