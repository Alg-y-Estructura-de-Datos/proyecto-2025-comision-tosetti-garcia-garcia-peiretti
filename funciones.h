#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Lista.h"
#include "HashMap.h"
#include "quickSort.h"
#include "normalizador_texto.h"
#include <fstream>
#include "HashMapList.h"
using namespace std;

// Estructura principal de una venta, representa una venta individual
struct Venta {
    int id;
    string fecha;
    string pais;
    string ciudad;
    string cliente;
    string producto;
    string categoria;
    int cantidad;
    float precio_unitario;
    float monto_total;
    string medio_envio;
    string estado_envio;
};

// Estructuras auxiliares para estadísticas y ordenamientos
struct CiudadMonto { // Para top ciudades
    std::string ciudad;
    float monto;
    // Orden natural: por monto (para quickSort)
    bool operator<(const CiudadMonto& otro) const {
        return monto < otro.monto;
    }
};

struct ProductoPaisMonto {
    string pais;
    string producto;
    float monto;
};

struct CategoriaPaisEstadistica {
    string pais;
    string categoria;
    float suma;
    int contador;
};
struct MedioEnvioPorPais {
    string pais;
    string medio_envio;
    int contador;
};

struct MedioEnvioPorCategoria {
    string categoria;
    string medio_envio;
    int contador;
};

struct FechaMonto {
    string fecha;
    float monto;
};

struct EstadoEnvioPorPais {
    string pais;
    string estado;
    int contador;
};

struct ProductoCantidad { // Para ordenar por unidades vendidas
    string producto;
    int cantidad;
};

struct ComparacionProductoPais {
    string pais;
    int cantidadA = 0;
    float montoA = 0;
    int cantidadB = 0;
    float montoB = 0;
};

struct ProductoEstadistica {
    string producto;
    int totalCantidad = 0;
    float totalMonto = 0;
};

//Declaracion de funciones
void agregarVenta(Lista<Venta>& ventas, HashMap<int, int>& ventasPorID);
void eliminarVenta(Lista<Venta>& ventas, HashMap<int, int>& ventasPorID);
void modificarVenta(Lista<Venta>& ventas, HashMap<int, int>& ventasPorID);
void guardarVentasCSV(Lista<Venta>& ventas);

// Funciones estadísticas automáticas (invocadas luego de agregarVenta)
void top5CiudadesPorPais(Lista<Venta>& ventas);
void montoTotalPorProductoYPais(Lista<Venta>& ventas);
void promedioVentasPorCategoriaYPais(Lista<Venta>& ventas);
void medioEnvioMasUsadoPorPais(Lista<Venta>& ventas);
void medioEnvioMasUsadoPorCategoria(Lista<Venta>& ventas);
void diaConMayorMontoTotal(Lista<Venta>& ventas);
void estadoEnvioMasFrecuentePorPais(Lista<Venta>& ventas);
void productoMasVendidoPorCantidad(Lista<Venta>& ventas);
void productoMenosVendidoPorCantidad(Lista<Venta>& ventas);

// Otras funciones del sistema
void quickSortCiudadMonto(CiudadMonto* arr, int inicio, int fin);
void quickSortProductoCantidad(ProductoCantidad* arr, int inicio, int fin);
void reprocesarEstadisticas(Lista<Venta>& ventas);
void consultarVentasPorCiudad(const Lista<Venta>& ventas);
void consultarVentasPorFechaYPais(Lista<Venta>& ventas);
void compararPaises(Lista<Venta>& ventas);
void compararProductosPorPais(Lista<Venta>& ventas);
void filtrarProductosPorPromedioUnidad(Lista<Venta>& ventas);
void filtrarProductosPorPromedioGlobal(Lista<Venta>& ventas);

// Solicita datos al usuario y agrega una venta a la lista
inline void agregarVenta(Lista<Venta>& ventas, HashMap<int, int>& ventasPorID) {
    Venta v;
    int contadorIf = 0;
    auto inicio = chrono::high_resolution_clock::now();

    try {
        cout << "Ingrese los datos de la nueva venta: ";

        // Solicita ID de la venta y valida formato
        cout << "ID: ";
        if (!(cin >> v.id)) {
            contadorIf++;
            throw invalid_argument("ID invalido.");
        }
        contadorIf++;
        cin.ignore();

        // Verifica si el ID ya existe en el HashMap
        bool existeID = false;
        try {
            ventasPorID.get(v.id);  // Si lo encuentra, existe
            existeID = true;
        } catch (...) {
            existeID = false;  // Si lanza excepción, no existe
        }

        // Si el ID ya existe, lanza error
        if (existeID) {
            contadorIf++;
            throw invalid_argument("El ID ya existe.");
        }
        contadorIf++;

        // Carga todos los campos restantes de la estructura Venta
        cout << "Fecha (DD/MM/AAAA): ";
        getline(cin, v.fecha);

        cout << "Pais: ";
        getline(cin, v.pais);

        cout << "Ciudad: ";
        getline(cin, v.ciudad);

        cout << "Cliente: ";
        getline(cin, v.cliente);

        cout << "Producto: ";
        getline(cin, v.producto);

        cout << "Categoria: ";
        getline(cin, v.categoria);

        // Valida que la cantidad sea un número positivo
        cout << "Cantidad: ";
        if (!(cin >> v.cantidad) || v.cantidad <= 0) {
            contadorIf++;
            throw invalid_argument("Cantidad invalida.");
        }
        contadorIf++;

        // Valida que el precio sea un número positivo
        cout << "Precio Unitario: ";
        if (!(cin >> v.precio_unitario) || v.precio_unitario <= 0) {
            contadorIf++;
            throw invalid_argument("Precio invalido.");
        }
        contadorIf++;
        cin.ignore();

        // Calcula el monto total de la venta
        v.monto_total = v.cantidad * v.precio_unitario;

        // Resto de datos
        cout << "Medio de Envio: ";
        getline(cin, v.medio_envio);

        cout << "Estado de Envio: ";
        getline(cin, v.estado_envio);

        // Agrega la venta a la lista y actualiza el hash de IDs
        ventas.insertarUltimo(v);
        ventasPorID.put(v.id, ventas.getTamanio() - 1);

        // Guarda cambios en el archivo CSV
        guardarVentasCSV(ventas);

        cout << "Venta agregada correctamente." << endl;

        // Mide el tiempo de ejecución
        auto fin = chrono::high_resolution_clock::now();
        double tiempo = chrono::duration<double, milli>(fin - inicio).count();
        cout << " " << endl;
        cout << "Tiempo: " << tiempo << " ms | Condicionales (if ejecutados): " << contadorIf << endl;

    } catch (const exception& e) {
        // Manejo de errores y limpieza del buffer de entrada
        cout << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Elimina una venta filtrando por país o ciudad e ID seleccionado
inline void eliminarVenta(Lista<Venta>& ventas, HashMap<int, int>& ventasPorID) {
    int contadorIf = 0; // contador if
    auto inicio = chrono::high_resolution_clock::now();

    try {
        string filtro;
        cout << "Ingrese pais o ciudad para buscar: ";
        getline(cin, filtro);

        filtro = normalizar_texto(filtro);

        int coincidencias = 0;
        Lista<int> idsValidos; // Lista auxiliar para guardar los IDs válidos encontrados

        // Buscar coincidencias por país o ciudad
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            Venta v = ventas.getDato(i);
            string ciudad = normalizar_texto(v.ciudad);
            string pais = normalizar_texto(v.pais);

            if (ciudad == filtro || pais == filtro) {
                contadorIf++;
                cout << "ID: " << v.id << " - " << v.producto << " - " << v.monto_total << endl;
                idsValidos.insertarUltimo(v.id); // Guardamos el ID si coincide en lista auxiliar
                coincidencias++;
            }
        }

        // Si no se encontraron coincidencias, lanzar excepción
        if (coincidencias == 0) {
            contadorIf++;
            throw runtime_error("No se encontraron ventas.");
        }
        contadorIf++;

        cout << "Ingrese el ID a eliminar: ";
        int id;
        if (!(cin >> id)) {
            contadorIf++;
            throw invalid_argument("ID invalido.");
        }
        contadorIf++;
        cin.ignore(); // Limpiar buffer

        // Verificar si el ID ingresado estaba entre los mostrados
        bool idPermitido = false;
        for (int i = 0; i < idsValidos.getTamanio(); ++i) {
            if (idsValidos.getDato(i) == id) {
                idPermitido = true;
                break;
            }
        }

        if (!idPermitido) {
            contadorIf++;
            throw runtime_error("El ID ingresado no corresponde a las coincidencias mostradas o no existe.");
        }
        contadorIf++;

        int idx;
        try {
            idx = ventasPorID.get(id);
        } catch (...) {
            throw runtime_error("ID no encontrado en el registro.");
        }

        ventas.remover(idx); // Eliminar de la lista

        // Reindexar el hash, busca la posición de la venta a eliminar usando el hash
        HashMap<int, int> nuevoHash(5000);
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            nuevoHash.put(ventas.getDato(i).id, i);
        }
        ventasPorID = nuevoHash;

        guardarVentasCSV(ventas);  // Guardar cambios en CSV
        cout << "Venta eliminada correctamente." << endl;

        auto fin = chrono::high_resolution_clock::now();
        double tiempo = chrono::duration<double, milli>(fin - inicio).count();
        cout << " " << endl;
        cout << "Tiempo: " << tiempo << " ms | Condicionales (if ejecutados): " << contadorIf << endl;

    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    // Limpiar buffer por seguridad ante errores de entrada
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Busca por ID y permite modificar todos los campos
inline void modificarVenta(Lista<Venta>& ventas, HashMap<int, int>& ventasPorID) {
    int contadorIf = 0; // contador if
    auto inicio = chrono::high_resolution_clock::now();

    try {
        int id;
        cout << "Ingrese ID de venta a modificar: ";
        if (!(cin >> id)) {
            contadorIf++;
            throw invalid_argument("ID invalido."); // Validación del ID (no numérico)
        }
        contadorIf++;
        cin.ignore(); // Limpia el buffer de entrada

        int idx;
        try {
            idx = ventasPorID.get(id); // Busca la posición del ID usando el HashMap
        } catch (...) {
            throw runtime_error("ID no encontrado en el registro.");
        }

        Venta v = ventas.getDato(idx); // Obtiene la venta actual

        // Solicita y actualiza cada campo de la venta
        cout << "Nueva fecha (DD/MM/AAAA): ";
        getline(cin, v.fecha);

        cout << "Nuevo pais: ";
        getline(cin, v.pais);

        cout << "Nueva ciudad: ";
        getline(cin, v.ciudad);

        cout << "Nuevo cliente: ";
        getline(cin, v.cliente);

        cout << "Nuevo producto: ";
        getline(cin, v.producto);

        cout << "Nueva categoria: ";
        getline(cin, v.categoria);

        // Validación: cantidad debe ser un número mayor a 0
        cout << "Nueva cantidad: ";
        if (!(cin >> v.cantidad) || v.cantidad <= 0) {
            contadorIf++;
            throw invalid_argument("Cantidad invalida.");
        }
        contadorIf++;

        // Validación: precio unitario debe ser un número mayor a 0
        cout << "Nuevo precio unitario: ";
        if (!(cin >> v.precio_unitario) || v.precio_unitario <= 0) {
            contadorIf++;
            throw invalid_argument("Precio invalido.");
        }
        contadorIf++;
        cin.ignore(); // Limpia el buffer para evitar errores en los getline siguientes

        // Calcula el nuevo monto total
        v.monto_total = v.cantidad * v.precio_unitario;

        cout << "Nuevo medio de envio: ";
        getline(cin, v.medio_envio);

        cout << "Nuevo estado de envio: ";
        getline(cin, v.estado_envio);

        ventas.reemplazar(idx, v); // Reemplaza la venta modificada en la lista
        guardarVentasCSV(ventas); // Guarda toda la lista actualizada en el archivo CSV

        cout << "Venta modificada correctamente." << endl;

        auto fin = chrono::high_resolution_clock::now();
        double tiempo = chrono::duration<double, milli>(fin - inicio).count();
        cout << " " << endl;
        cout << "Tiempo: " << tiempo << " ms | Condicionales (if ejecutados): " << contadorIf << endl;

    } catch (const exception& e) {
        // Manejo de errores generales
        cout << "Error: " << e.what() << endl;
        cin.clear(); // Limpia estado de error del cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpia todo el buffer de entrada
    }
}

// Ordena ciudades de un país por monto total mayor a menor (quicksort)
void quickSortCiudadMonto(CiudadMonto arr[], int izq, int der) {
    int i = izq, j = der;
    CiudadMonto pivote = arr[(izq + der) / 2];

    while (i <= j) {
        while (arr[i].monto > pivote.monto) i++;
        while (arr[j].monto < pivote.monto) j--;
        if (i <= j) {
            CiudadMonto tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++; j--;
        }
    }

    if (izq < j) quickSortCiudadMonto(arr, izq, j);
    if (i < der) quickSortCiudadMonto(arr, i, der);
}


// Permite ordenar quicksort, compara dos objetos CiudadMonto usando ">"
bool operator>(const CiudadMonto& a, const CiudadMonto& b) {
    return a.monto > b.monto;
}

// Calcula y muestra el top 5 de ciudades con mayor monto total de ventas por cada país
void top5CiudadesPorPais(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now(); // Marca de tiempo para medir rendimiento
    int contadorIf = 0;

    if (ventas.esVacia()) { // Verifica si hay ventas
        contadorIf++;
        cout << "No hay ventas para procesar." << endl;
        return;
    }

    // Hash para agrupar ventas por clave "pais|ciudad" → monto total acumulado,  evita recorrer la lista muchas veces.
    HashMapList<string, float> ciudadPorPais(3000);

    // Después, se separa cada ciudad en su país correspondiente, Hash para reagrupar luego por país → lista de ciudades y sus montos
    HashMapList<string, Lista<CiudadMonto>*> agrupadas(200);

    // Lista auxiliar de países únicos
    Lista<string> clavesPaises;

    // Paso 1: Agrupar montos por combinación única de país + ciudad
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato();
        contadorIf++;
        if (v.pais.empty() || v.ciudad.empty()) continue; // Ignora registros incompletos
        string clave = v.pais + "|" + v.ciudad; // clave compuesta país|ciudad

        try {
            float actual = ciudadPorPais.get(clave);
            ciudadPorPais.remove(clave);
            ciudadPorPais.put(clave, actual + v.monto_total); // Suma monto acumulado
        } catch (...) {
            ciudadPorPais.put(clave, v.monto_total); // Primer inserción de esa ciudad
        }
    }

    // Paso 2: Reorganizar por país → lista de ciudades con sus montos
    for (int i = 0; i < 3000; ++i) {
        Lista<HashEntry<string, float>>* lista = ciudadPorPais.getListaPorIndice(i);
        contadorIf++;
        if (!lista) continue;

        Nodo<HashEntry<string, float>>* nodo = lista->getInicio();
        while (nodo != nullptr) {
            string clave = nodo->getDato().getClave();    // Ej: "Argentina|Buenos Aires"
            float monto = nodo->getDato().getValor();     // Monto total

            size_t pos = clave.find('|'); // Separa país y ciudad
            contadorIf++;
            if (pos == string::npos) {
                nodo = nodo->getSiguiente();
                continue;
            }

            string pais = clave.substr(0, pos);
            string ciudad = clave.substr(pos + 1);
            CiudadMonto cm{ciudad, monto};

            Lista<CiudadMonto>* ptrLista = nullptr;

            try {
                ptrLista = agrupadas.get(pais); // Obtiene lista existente de ese país
            } catch (...) {
                ptrLista = new Lista<CiudadMonto>(); // Crea nueva si no existía
                agrupadas.put(pais, ptrLista);
                clavesPaises.insertarUltimo(pais); // Agrega país a la lista auxiliar
            }

            ptrLista->insertarUltimo(cm); // Agrega ciudad y monto a la lista del país
            nodo = nodo->getSiguiente();
        }
    }

    // Paso 3: Mostrar el top 5 ciudades de cada país
    cout << " " << endl;
    cout << "--- Top 5 Ciudades por Pais ---" << endl;
    for (int i = 0; i < clavesPaises.getTamanio(); ++i) {
        string pais = clavesPaises.getDato(i);
        Lista<CiudadMonto>* ciudades = agrupadas.get(pais);
        int tam = ciudades->getTamanio();

        contadorIf++;
        if (tam == 0) continue;

        vector<CiudadMonto> vec;
        for (int j = 0; j < tam; ++j)
            vec.push_back(ciudades->getDato(j)); // Copia datos a vector para ordenarlos

        // Ordena de mayor a menor por monto
        quickSortCiudadMonto(&vec[0], 0, vec.size() - 1);

        cout << "Pais: " << pais << endl;
        for (int j = 0; j < 5 && j < vec.size(); ++j) {
            contadorIf++;
            cout << "  " << j + 1 << ". " << vec[j].ciudad << " - $" << vec[j].monto << endl;
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Fin de medición
    double tiempo = chrono::duration<double, milli>(end - start).count();
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | Condicionales (if ejecutados): " << contadorIf << endl;
}


// Calcula monto total vendido por producto y país (con try-catch)
void montoTotalPorProductoYPais(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now();
    int contadorIf = 0;

    // Hash para acumular monto total por clave "pais|producto"
    HashMapList<string, float> acumulados(3000);

    // Hash auxiliar para guardar los países únicos registrados
    HashMapList<string, bool> paisesRegistrados(200);

    // Paso 1: agrupar ventas por clave "pais|producto" y registrar países únicos
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato();
        string clave = v.pais + "|" + v.producto;

        // Si ya existía esa combinación país-producto, acumular el monto total
        try {
            float actual = acumulados.get(clave);
            acumulados.remove(clave);
            acumulados.put(clave, actual + v.monto_total);
        } catch (...) {
            acumulados.put(clave, v.monto_total);  // primera aparición
        }

        // Registrar país en hash de países únicos
        try {
            paisesRegistrados.get(v.pais);  // si ya está, no hace nada
        } catch (...) {
            paisesRegistrados.put(v.pais, true);  // lo agrega si es nuevo
        }
    }

    // Paso 2: mostrar acumulado por país
    cout << " " << endl;
    cout << "--- Monto Total por Producto y Pais ---" << endl;

    // Recorre todos los índices del hash de países registrados
    for (int i = 0; i < 200; ++i) {
        Lista<HashEntry<string, bool>>* listaP = paisesRegistrados.getListaPorIndice(i);
        if (listaP != nullptr) contadorIf++;  // if 1
        if (!listaP) continue;  // si no hay nada, salta

        Nodo<HashEntry<string, bool>>* nodoP = listaP->getInicio();
        while (nodoP) {
            string pais = nodoP->getDato().getClave();
            cout << "--------------------------" << endl;
            cout << "Pais: " << pais << endl;

            // Recorre todos los índices del hash de acumulados
            for (int j = 0; j < 3000; ++j) {
                Lista<HashEntry<string, float>>* listaA = acumulados.getListaPorIndice(j);
                if (listaA != nullptr) contadorIf++;  // if 2
                if (!listaA) continue;

                Nodo<HashEntry<string, float>>* nodoA = listaA->getInicio();
                while (nodoA) {
                    string clave = nodoA->getDato().getClave();
                    float monto = nodoA->getDato().getValor();

                    size_t pos = clave.find('|');  // separa país y producto
                    string paisClave = clave.substr(0, pos);
                    string producto = clave.substr(pos + 1);

                    contadorIf++; // if 3
                    if (paisClave == pais) {
                        cout << "- " << producto << ": $" << monto << endl;
                    }

                    nodoA = nodoA->getSiguiente();
                }
            }

            nodoP = nodoP->getSiguiente();
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double tiempo = chrono::duration<double, milli>(end - start).count();
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | Condicionales (if ejecutados): " << contadorIf << endl;
}

// Calcula promedio de ventas por categoría en cada país
void promedioVentasPorCategoriaYPais(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now();
    int contadorIf = 0;

    // HashMapList para acumular montos por "pais|categoria"
    HashMapList<string, float> acumulados(3000);

    // HashMapList para contar cantidad de ventas por "pais|categoria" para sacar promedio
    HashMapList<string, int> cantidades(3000);

    // Hash para registrar los países únicos
    HashMapList<string, bool> paisesRegistrados(200);

    // Paso 1: agrupar montos y contar cantidad de ventas por categoría y país
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato();
        string clave = v.pais + "|" + v.categoria;  // clave combinada

        // Acumulador de monto total por clave
        try {
            float actual = acumulados.get(clave);
            acumulados.remove(clave);                    // se reemplaza el valor
            acumulados.put(clave, actual + v.monto_total);
        } catch (...) {
            acumulados.put(clave, v.monto_total);        // primer aparición
        }

        // Contador de ventas por clave
        try {
            int actual = cantidades.get(clave);
            cantidades.remove(clave);
            cantidades.put(clave, actual + 1);
        } catch (...) {
            cantidades.put(clave, 1);
        }

        // Registrar país como clave única
        try {
            paisesRegistrados.get(v.pais);
        } catch (...) {
            paisesRegistrados.put(v.pais, true);
        }
    }

    // Paso 2: Mostrar los promedios agrupados por país
    cout << " " << endl;
    cout << "--- Promedio de Ventas por Categoria y Pais ---" << endl;

    for (int i = 0; i < 200; ++i) {
        // Recorrer índice por índice del hash de países
        Lista<HashEntry<string, bool>>* listaP = paisesRegistrados.getListaPorIndice(i);
        if (listaP != nullptr) contadorIf++;  // if 1
        if (!listaP) continue;

        Nodo<HashEntry<string, bool>>* nodoP = listaP->getInicio();
        while (nodoP) {
            string pais = nodoP->getDato().getClave();
            cout << "--------------------------" << endl;
            cout << "Pais: " << pais << endl;

            // Buscar las categorías asociadas a ese país
            for (int j = 0; j < 3000; ++j) {
                Lista<HashEntry<string, float>>* listaA = acumulados.getListaPorIndice(j);
                if (listaA != nullptr) contadorIf++;  // if 2
                if (!listaA) continue;

                Nodo<HashEntry<string, float>>* nodoA = listaA->getInicio();
                while (nodoA) {
                    string clave = nodoA->getDato().getClave();  // "pais|categoria"
                    float monto = nodoA->getDato().getValor();

                    size_t pos = clave.find('|');  // separar país de categoría
                    string paisClave = clave.substr(0, pos);
                    string categoria = clave.substr(pos + 1);

                    contadorIf++; // if 3
                    if (paisClave == pais) {
                        int cant = cantidades.get(clave);               // cantidad de ventas
                        float promedio = monto / cant;                  // cálculo de promedio
                        cout << "- " << categoria << ": $" << promedio << endl;
                    }

                    nodoA = nodoA->getSiguiente();
                }
            }

            nodoP = nodoP->getSiguiente();
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double tiempo = chrono::duration<double, milli>(end - start).count();
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | Condicionales (if ejecutados): " << contadorIf << endl;
}

// Cuenta qué medio de envío fue más usado por país
void medioEnvioMasUsadoPorPais(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now();
    int contadorIf = 0;

    // Si la lista está vacía, no se puede procesar
    if (ventas.esVacia()) {
        cout << "No hay ventas cargadas." << endl;
        return;
    }

    // HashMap principal donde cada país apunta a otro hash que cuenta los medios
    HashMapList<string, HashMapList<string, int>*> agrupados(200);

    // Lista para mantener los países únicos registrados
    Lista<string> paisesUnicos;

    // Paso 1: agrupar medios por país
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato();

        // Si falta el país o el medio de envío, se ignora la venta
        if (v.pais.empty() || v.medio_envio.empty()) continue;

        HashMapList<string, int>* medios = nullptr;

        // Intentar obtener el hash de medios para el país actual
        try {
            medios = agrupados.get(v.pais);
        } catch (...) {
            // Si no existía, se crea uno nuevo y se registra el país
            medios = new HashMapList<string, int>(10);
            agrupados.put(v.pais, medios);
            paisesUnicos.insertarUltimo(v.pais);
        }

        // Incrementar el contador del medio de envío en el hash correspondiente
        try {
            int actual = medios->get(v.medio_envio);
            medios->remove(v.medio_envio);
            medios->put(v.medio_envio, actual + 1);
        } catch (...) {
            medios->put(v.medio_envio, 1);  // Primera vez que aparece ese medio
        }
    }

    // Paso 2: mostrar resultados
    cout << " " << endl;
    cout << "--- Medio de Envio mas Usado por Pais ---" << endl;

    for (int i = 0; i < paisesUnicos.getTamanio(); ++i) {
        string pais = paisesUnicos.getDato(i);
        HashMapList<string, int>* medios = agrupados.get(pais);  // obtener hash por país

        string mejorMedio;
        int maxConteo = 0;

        // Recorrer todos los índices del hash del país
        for (int j = 0; j < 10; ++j) {
            Lista<HashEntry<string, int>>* lista = medios->getListaPorIndice(j);
            if (lista != nullptr) contadorIf++;  // if 1
            if (!lista) continue;

            Nodo<HashEntry<string, int>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string medio = nodo->getDato().getClave();   // medio de envío
                int cantidad = nodo->getDato().getValor();   // cantidad de veces usado

                contadorIf++;  // if 2
                if (cantidad > maxConteo) {
                    maxConteo = cantidad;
                    mejorMedio = medio;
                }

                nodo = nodo->getSiguiente();
            }
        }

        contadorIf++;  // if 3
        if (maxConteo > 0) {
            cout << "- " << pais << ": " << mejorMedio << " (" << maxConteo << " ventas)" << endl;
        } else {
            cout << "- " << pais << ": Sin datos disponibles." << endl;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double tiempo = chrono::duration<double, milli>(end - start).count();
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | Condicionales (if ejecutados): " << contadorIf << endl;
}

// Cuenta qué medio fue más usado por categoría
void medioEnvioMasUsadoPorCategoria(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now();  // Tiempo de inicio
    int contadorIf = 0;

    if (ventas.esVacia()) {  // Verifica si hay ventas cargadas
        cout << "No hay ventas cargadas." << endl;
        return;
    }

    // Hash externo: clave = categoría, valor = puntero a hash interno con medios y cantidad
    HashMapList<string, HashMapList<string, int>*> agrupados(200);
    Lista<string> categoriasUnicas;  // Lista para registrar categorías únicas

    // Paso 1: agrupar medios por categoría
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato();
        if (v.categoria.empty() || v.medio_envio.empty()) continue;

        HashMapList<string, int>* medios = nullptr;

        try {
            medios = agrupados.get(v.categoria);  // Obtener hash interno de medios
        } catch (...) {
            medios = new HashMapList<string, int>(10);  // Crear nuevo si no existe
            agrupados.put(v.categoria, medios);         // Asociar la categoría con su hash
            categoriasUnicas.insertarUltimo(v.categoria); // Registrar como nueva categoría
        }

        try {
            int actual = medios->get(v.medio_envio);  // Obtener conteo actual
            medios->remove(v.medio_envio);            // Eliminar para evitar duplicados
            medios->put(v.medio_envio, actual + 1);   // Reinsertar con contador actualizado
        } catch (...) {
            medios->put(v.medio_envio, 1);            // Insertar si no existía
        }
    }

    // Paso 2: mostrar resultados
    cout << " " << endl;
    cout << "--- Medio de Envio mas Usado por Categoria ---" << endl;

    // Recorremos cada categoría única registrada
    for (int i = 0; i < categoriasUnicas.getTamanio(); ++i) {
        string categoria = categoriasUnicas.getDato(i);
        HashMapList<string, int>* medios = agrupados.get(categoria); // Hash de esa categoría

        string mejorMedio;  // Almacena medio más usado
        int maxConteo = 0;  // Almacena la cantidad máxima

        // Recorremos los 10 posibles índices del hash interno
        for (int j = 0; j < 10; ++j) {
            Lista<HashEntry<string, int>>* lista = medios->getListaPorIndice(j);
            if (lista != nullptr) contadorIf++;
            if (!lista) continue;

            Nodo<HashEntry<string, int>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string medio = nodo->getDato().getClave();
                int cantidad = nodo->getDato().getValor();

                contadorIf++;
                if (cantidad > maxConteo) {  // Si es mayor que el actual máximo
                    maxConteo = cantidad;
                    mejorMedio = medio;
                }

                nodo = nodo->getSiguiente();
            }
        }

        contadorIf++;
        if (maxConteo > 0) {
            cout << "- " << categoria << ": " << mejorMedio << " (" << maxConteo << " veces)" << endl;
        } else {
            cout << "- " << categoria << ": Sin datos disponibles." << endl;
        }
    }

    auto end = chrono::high_resolution_clock::now();  // Tiempo de fin
    double tiempo = chrono::duration<double, milli>(end - start).count();
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl;
}


// Calcula el día con mayor monto de ventas
void diaConMayorMontoTotal(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now(); // Marca de tiempo inicial para calcular duración
    int contadorIf = 0; // Contador de sentencias if ejecutadas

    if (ventas.esVacia()) { // Verifica si la lista de ventas está vacía
        cout << "No hay ventas cargadas." << endl;
        return; // Si no hay ventas, se termina la función
    }

    HashMapList<string, float> acumuladoPorDia(500); // Hash map para acumular el monto total por fecha

    // Paso 1: agrupar por fecha
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato(); // Se obtiene la venta del nodo actual
        if (v.fecha.empty()) continue; // Se ignoran ventas sin fecha

        try {
            float actual = acumuladoPorDia.get(v.fecha); // Obtiene el monto actual acumulado para esa fecha
            acumuladoPorDia.remove(v.fecha); // Elimina la entrada actual para actualizarla
            acumuladoPorDia.put(v.fecha, actual + v.monto_total); // Inserta nuevamente con el monto actualizado
        } catch (...) {
            acumuladoPorDia.put(v.fecha, v.monto_total); // Si no existía esa fecha, se agrega con el monto actual
        }
    }

    // Paso 2: buscar la fecha con mayor monto acumulado
    string fechaMaxima; // Variable para guardar la fecha con mayor monto
    float montoMaximo = 0.0f; // Variable para guardar el mayor monto encontrado

    for (int i = 0; i < 500; ++i) { // Se recorre toda la tabla hash (500 buckets)
        Lista<HashEntry<string, float>>* lista = acumuladoPorDia.getListaPorIndice(i); // Obtiene lista en el índice i
        if (lista != nullptr) contadorIf++; // Cuenta if si la lista no es nula
        if (!lista) continue; // Si no hay lista, se continúa al siguiente índice

        Nodo<HashEntry<string, float>>* nodo = lista->getInicio(); // Recorre la lista del bucket actual
        while (nodo != nullptr) {
            string fecha = nodo->getDato().getClave(); // Obtiene la fecha
            float monto = nodo->getDato().getValor(); // Obtiene el monto acumulado

            contadorIf++; // Se ejecuta el if siguiente
            if (monto > montoMaximo) { // Se actualiza si se encuentra un monto mayor
                montoMaximo = monto;
                fechaMaxima = fecha;
            }

            nodo = nodo->getSiguiente(); // Avanza al siguiente nodo
        }
    }

    // Mostrar resultados
    cout << " " << endl;
    cout << "--- Dia con Mayor Monto Total de Ventas ---" << endl;

    if (!fechaMaxima.empty()) { // Si se encontró una fecha válida
        cout << "- " << fechaMaxima << " $" << montoMaximo << endl;
    } else {
        cout << "- No se encontraron fechas validas." << endl;
    }

    auto end = chrono::high_resolution_clock::now(); // Marca de tiempo final
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula duración en milisegundos
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra resultados de tiempo e ifs
}


// Estado de envío más frecuente en cada país
void estadoEnvioMasFrecuentePorPais(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now(); // Marca de tiempo inicial
    int contadorIf = 0; // Contador de sentencias if ejecutadas

    if (ventas.esVacia()) { // Verifica si la lista de ventas está vacía
        cout << "No hay ventas cargadas." << endl;
        return; // Finaliza si no hay datos
    }

    HashMapList<string, HashMapList<string, int>*> agrupados(200); // HashMap que agrupa por país, mapeando a un hash interno de estados
    Lista<string> paisesUnicos; // Lista para mantener los países únicos en orden

    // Paso 1: Agrupar estados por país
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato(); // Obtiene la venta actual
        if (v.pais.empty() || v.estado_envio.empty()) continue; // Ignora ventas sin país o estado de envío

        HashMapList<string, int>* estados = nullptr;

        try {
            estados = agrupados.get(v.pais); // Intenta obtener el hash de estados del país actual
        } catch (...) {
            estados = new HashMapList<string, int>(10); // Si no existe, crea uno nuevo
            agrupados.put(v.pais, estados); // Lo agrega al hash principal
            paisesUnicos.insertarUltimo(v.pais); // Guarda el país como único
        }

        try {
            int actual = estados->get(v.estado_envio); // Obtiene el conteo actual del estado
            estados->remove(v.estado_envio); // Lo remueve para actualizar el valor
            estados->put(v.estado_envio, actual + 1); // Vuelve a ponerlo con el contador incrementado
        } catch (...) {
            estados->put(v.estado_envio, 1); // Si no existía, se inicializa con 1
        }
    }

    // Paso 2: Mostrar resultados
    cout << " " << endl;
    cout << "--- Estado de Envio mas Frecuente por Pais ---" << endl;

    for (int i = 0; i < paisesUnicos.getTamanio(); ++i) { // Recorre la lista de países únicos
        string pais = paisesUnicos.getDato(i); // Obtiene el país actual
        HashMapList<string, int>* estados = agrupados.get(pais); // Obtiene el hash de estados para ese país

        string estadoMasFrecuente;
        int max = 0;

        for (int j = 0; j < 10; ++j) { // Recorre cada bucket del hash de estados
            Lista<HashEntry<string, int>>* lista = estados->getListaPorIndice(j);
            if (lista != nullptr) contadorIf++; // Cuenta el if si la lista no es nula
            if (!lista) continue; // Si la lista es nula, pasa al siguiente bucket

            Nodo<HashEntry<string, int>>* nodo = lista->getInicio();
            while (nodo != nullptr) {
                string estado = nodo->getDato().getClave(); // Clave = estado de envío
                int cantidad = nodo->getDato().getValor(); // Valor = cantidad de ocurrencias

                contadorIf++;
                if (cantidad > max) { // Se actualiza si es el estado más frecuente hasta ahora
                    max = cantidad;
                    estadoMasFrecuente = estado;
                }

                nodo = nodo->getSiguiente(); // Avanza al siguiente nodo de la lista
            }
        }

        contadorIf++;
        if (max > 0) {
            cout << "- " << pais << ": " << estadoMasFrecuente << " (" << max << " veces)" << endl;
        } else {
            cout << "- " << pais << ": Sin datos disponibles." << endl;
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Marca el fin de la ejecución
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula tiempo de ejecución
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra métricas
}


// Ordena productos por cantidad vendida (quicksort descendente)
inline void quickSortProductoCantidad(ProductoCantidad* arr, int inicio, int fin) {
    int i = inicio, j = fin;
    int pivot = arr[(inicio + fin) / 2].cantidad;

    while (i <= j) {
        while (arr[i].cantidad > pivot) i++;
        while (arr[j].cantidad < pivot) j--;
        if (i <= j) {
            ProductoCantidad tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }

    if (inicio < j) quickSortProductoCantidad(arr, inicio, j);
    if (i < fin) quickSortProductoCantidad(arr, i, fin);
}

// Muestra producto con más unidades vendidas
void productoMasVendidoPorCantidad(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now(); // Marca el inicio de ejecución
    int contadorIf = 0; // Contador de sentencias if ejecutadas

    if (ventas.esVacia()) { // Verifica si la lista está vacía
        cout << "No hay ventas cargadas." << endl;
        return; // Termina la función si no hay ventas
    }

    HashMapList<string, int> acumuladoUnidades(1000); // HashMap para acumular unidades vendidas por producto

    // Paso 1: agrupar unidades por producto
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato(); // Obtiene la venta del nodo actual
        if (v.producto.empty()) continue; // Ignora ventas sin nombre de producto

        try {
            int actual = acumuladoUnidades.get(v.producto); // Obtiene unidades ya acumuladas del producto
            acumuladoUnidades.remove(v.producto); // Elimina la entrada actual
            acumuladoUnidades.put(v.producto, actual + v.cantidad); // Inserta nuevamente con la suma de unidades
        } catch (...) {
            acumuladoUnidades.put(v.producto, v.cantidad); // Si no existía, lo agrega con la cantidad actual
        }
    }

    // Paso 2: encontrar el producto con mayor cantidad de unidades
    string productoMax; // Variable para el nombre del producto más vendido
    int maxUnidades = 0; // Máxima cantidad de unidades encontradas

    for (int i = 0; i < 1000; ++i) { // Recorre todos los buckets de la tabla hash
        Lista<HashEntry<string, int>>* lista = acumuladoUnidades.getListaPorIndice(i); // Obtiene la lista del bucket actual
        if (lista != nullptr) contadorIf++; // Cuenta el if si la lista no es nula
        if (!lista) continue; // Si la lista es nula, continúa al siguiente bucket

        Nodo<HashEntry<string, int>>* nodo = lista->getInicio(); // Recorre los nodos de la lista
        while (nodo != nullptr) {
            string producto = nodo->getDato().getClave(); // Obtiene el nombre del producto
            int cantidad = nodo->getDato().getValor(); // Obtiene las unidades vendidas

            contadorIf++; // Cuenta el if de comparación
            if (cantidad > maxUnidades) { // Si es mayor que el máximo actual, lo actualiza
                maxUnidades = cantidad;
                productoMax = producto;
            }

            nodo = nodo->getSiguiente(); // Avanza al siguiente nodo
        }
    }

    // Mostrar resultado
    cout << " " << endl;
    cout << "--- Porducto mas Vendido por Cantidad de Unidades ---" << endl;

    if (!productoMax.empty()) { // Si se encontró un producto válido
        cout << "- " << productoMax << ": " << maxUnidades << " unidades vendidas\n";
    } else {
        cout << "- No se encontraron productos validos." << endl;
    }

    auto end = chrono::high_resolution_clock::now(); // Marca el fin de ejecución
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula el tiempo de ejecución
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra métricas
}


// Muestra producto con menos unidades vendidas
void productoMenosVendidoPorCantidad(Lista<Venta>& ventas) {
    auto start = chrono::high_resolution_clock::now(); // Marca el inicio para medir tiempo de ejecución
    int contadorIf = 0; // Contador de sentencias if ejecutadas

    if (ventas.esVacia()) { // Verifica si la lista está vacía
        cout << "No hay ventas cargadas." << endl;
        return; // Termina la función si no hay datos
    }

    HashMapList<string, int> acumuladoUnidades(1000); // HashMap para acumular unidades por producto

    // Paso 1: agrupar unidades por producto
    for (Nodo<Venta>* nodo = ventas.getInicio(); nodo != nullptr; nodo = nodo->getSiguiente()) {
        Venta v = nodo->getDato(); // Se obtiene la venta actual
        if (v.producto.empty()) continue; // Se ignoran ventas sin nombre de producto

        try {
            int actual = acumuladoUnidades.get(v.producto); // Obtiene la cantidad acumulada
            acumuladoUnidades.remove(v.producto); // Elimina el valor actual
            acumuladoUnidades.put(v.producto, actual + v.cantidad); // Inserta con la nueva cantidad sumada
        } catch (...) {
            acumuladoUnidades.put(v.producto, v.cantidad); // Si no existía, lo agrega con la cantidad actual
        }
    }

    // Paso 2: buscar el producto con menos unidades (pero mayor a 0)
    string productoMin; // Nombre del producto con menos ventas
    int minUnidades = INT_MAX; // Inicializado con el máximo valor posible

    for (int i = 0; i < 1000; ++i) { // Recorre cada bucket del hash
        Lista<HashEntry<string, int>>* lista = acumuladoUnidades.getListaPorIndice(i); // Obtiene la lista en el índice
        if (lista != nullptr) contadorIf++; // Cuenta el if si la lista no es nula
        if (!lista) continue; // Si es nula, pasa al siguiente índice

        Nodo<HashEntry<string, int>>* nodo = lista->getInicio(); // Recorre los nodos de la lista
        while (nodo != nullptr) {
            string producto = nodo->getDato().getClave(); // Clave: nombre del producto
            int cantidad = nodo->getDato().getValor(); // Valor: unidades vendidas

            contadorIf++; // Cuenta el if que compara valores
            if (cantidad > 0 && cantidad < minUnidades) { // Busca el menor valor mayor a 0
                minUnidades = cantidad;
                productoMin = producto;
            }

            nodo = nodo->getSiguiente(); // Avanza al siguiente nodo
        }
    }

    // Mostrar resultado
    cout << " " << endl;
    cout << "--- Producto Menos Vendido por Cantidad de Unidades ---" << endl;

    if (!productoMin.empty()) { // Verifica que se haya encontrado un producto válido
        cout << "- " << productoMin << ": " << minUnidades << " unidades vendidas\n";
    } else {
        cout << "- No se encontraron productos con unidades mayores a 0." << endl;
    }

    auto end = chrono::high_resolution_clock::now(); // Marca el fin de la ejecución
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula duración en milisegundos
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra métricas
}

// Filtra lista de ventas por ciudad exacta
inline void consultarVentasPorCiudad(Lista<Venta>& ventas) {
    int contadorIf = 0; // Contador para medir cuántos if se ejecutan
    auto start = chrono::high_resolution_clock::now(); // Marca de tiempo inicial para medir duración de ejecución

    try {
        string ciudadBuscada;
        cout << "Ingrese el nombre de la ciudad: ";
        getline(cin, ciudadBuscada); // Solicita al usuario la ciudad a consultar

        contadorIf++;
        if (ciudadBuscada.empty()) throw invalid_argument("Ciudad vacia."); // Valida que se haya ingresado una ciudad

        ciudadBuscada = normalizar_texto(ciudadBuscada);  // Normaliza el nombre ingresado (mayúsculas, tildes, espacios, etc.)

        // Crear índice temporal por ciudad (clave: ciudad normalizada, valor: lista de ventas en esa ciudad)
        HashMapList<string, Lista<Venta>*> ventasPorCiudad(500);

        // Recorre todas las ventas para indexarlas por ciudad
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            Venta v = ventas.getDato(i); // Obtiene la venta actual
            string ciudadNormalizada;

            try {
                ciudadNormalizada = normalizar_texto(v.ciudad); // Normaliza la ciudad de la venta
                if (ciudadNormalizada.empty()) continue; // Si la ciudad está vacía después de normalizar, la ignora

                Lista<Venta>* listaCiudad = nullptr;

                try {
                    listaCiudad = ventasPorCiudad.get(ciudadNormalizada); // Intenta obtener la lista ya existente para la ciudad
                } catch (...) {
                    // Si no existía, crea una nueva lista y la inserta
                    listaCiudad = new Lista<Venta>();
                    ventasPorCiudad.put(ciudadNormalizada, listaCiudad);
                }

                listaCiudad->insertarUltimo(v); // Agrega la venta actual a la lista correspondiente a la ciudad
            } catch (...) {
                // Si hay error con la ciudad (corrupta, null, etc.), la ignora
            }
        }

        // Buscar la lista de ventas para la ciudad ingresada
        Lista<Venta>* resultados = nullptr;

        try {
            resultados = ventasPorCiudad.get(ciudadBuscada); // Busca directamente las ventas para la ciudad ingresada
        } catch (...) {
            // Si no existe la ciudad en el índice, lanza error
            throw runtime_error("No se encontraron ventas para la ciudad indicada.");
        }

        cout << " " << endl;
        cout << "--- Ventas realizadas en ciudad: " << ciudadBuscada << " ---" << endl;

        // Imprime todas las ventas encontradas
        for (int i = 0; i < resultados->getTamanio(); ++i) {
            Venta v = resultados->getDato(i);
            cout << "- ID: " << v.id
                 << " | Fecha: " << v.fecha
                 << " | Producto: " << v.producto
                 << " | Cantidad: " << v.cantidad
                 << " | Total: $" << v.monto_total
                 << " | Cliente: " << v.cliente << endl;
        }

        contadorIf++; // Se incrementa por haber mostrado resultados (última validación lógica relevante)
        cout << "Total de ventas encontradas: " << resultados->getTamanio() << endl;

    } catch (const exception& e) {
        // Manejo general de errores (por ciudad vacía, sin resultados, etc.)
        cout << "Error: " << e.what() << endl;
    }

    auto end = chrono::high_resolution_clock::now(); // Marca el final del cronómetro
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula duración total en milisegundos
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra métricas
}

inline int fechaAEntero(const std::string& fecha) {
    if (fecha.length() < 8) return 0;

    int dia = 0, mes = 0, anio = 0;
    size_t p1 = fecha.find('/');
    size_t p2 = fecha.rfind('/');

    if (p1 == std::string::npos || p2 == std::string::npos || p1 == p2)
        return 0;

    try {
        dia = stoi(fecha.substr(0, p1));
        mes = stoi(fecha.substr(p1 + 1, p2 - p1 - 1));
        anio = stoi(fecha.substr(p2 + 1));
    } catch (...) {
        return 0;
    }

    return anio * 10000 + mes * 100 + dia;
}

// Listado de ventas realizadas en un rango de fechas por país
inline void consultarVentasPorFechaYPais(Lista<Venta>& ventas) {
    int contadorIf = 0; // Contador para registrar cuántos if se ejecutan
    auto start = chrono::high_resolution_clock::now(); // Marca el inicio de la ejecución para medir el tiempo

    try {
        string pais;
        cout << "Ingrese el pais: ";
        getline(cin, pais); // Solicita al usuario el país a consultar

        contadorIf++;
        if (pais.empty()) throw invalid_argument("Pais vacio."); // Verifica que el país no esté vacío

        pais = normalizar_texto(pais); // Normaliza el texto ingresado (ej: tildes, mayúsculas, espacios)

        // Construcción de índice por país usando HashMapList
        HashMapList<string, Lista<Venta>*> ventasPorPais(300); // Clave: país normalizado | Valor: lista de ventas

        // Agrupa las ventas por país
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            Venta v = ventas.getDato(i); // Toma la venta actual
            try {
                string paisNormalizado = normalizar_texto(v.pais); // Normaliza el país de la venta

                if (paisNormalizado.empty()) continue; // Si el campo país está vacío, salta la venta

                Lista<Venta>* listaPais = nullptr;

                try {
                    listaPais = ventasPorPais.get(paisNormalizado); // Intenta obtener la lista ya existente
                } catch (...) {
                    listaPais = new Lista<Venta>(); // Si no existe, la crea
                    ventasPorPais.put(paisNormalizado, listaPais); // La asocia al país
                }

                listaPais->insertarUltimo(v); // Agrega la venta a la lista del país correspondiente
            } catch (...) {
                // Ignora ventas con campos de país corruptos
            }
        }

        // Verifica si el país ingresado existe en el índice
        Lista<Venta>* ventasEnPais = nullptr;
        try {
            ventasEnPais = ventasPorPais.get(pais); // Busca la lista asociada al país
        } catch (...) {
            throw runtime_error("Pais inexistente en la base de datos."); // Si no existe, lanza error
        }

        string fechaInicio, fechaFin;
        cout << "Ingrese la fecha de inicio (DD/MM/AAAA): ";
        getline(cin, fechaInicio); // Solicita la fecha inicial
        contadorIf++;
        if (fechaInicio.empty()) throw invalid_argument("Fecha de inicio vacia."); // Verifica que no esté vacía

        cout << "Ingrese la fecha de fin (DD/MM/AAAA): ";
        getline(cin, fechaFin); // Solicita la fecha final
        contadorIf++;
        if (fechaFin.empty()) throw invalid_argument("Fecha de fin vacia."); // Verifica que no esté vacía

        int inicio = fechaAEntero(fechaInicio); // Convierte fecha de inicio a entero (YYYYMMDD)
        int fin = fechaAEntero(fechaFin);       // Convierte fecha de fin a entero (YYYYMMDD)

        cout << "--- Ventas en " << pais << " entre " << fechaInicio << " y " << fechaFin << " ---" << endl;

        int contador = 0; // Contador de coincidencias

        // Recorre solo las ventas asociadas al país ingresado
        for (int i = 0; i < ventasEnPais->getTamanio(); ++i) {
            try {
                Venta v = ventasEnPais->getDato(i); // Toma la venta actual
                int fechaVenta = fechaAEntero(v.fecha); // Convierte la fecha de la venta

                contadorIf++;
                // Verifica si la fecha de la venta está dentro del rango dado
                if (fechaVenta >= inicio && fechaVenta <= fin) {
                    // Muestra la venta si cumple los criterios
                    cout << "- ID: " << v.id
                         << " | Fecha: " << v.fecha
                         << " | Ciudad: " << v.ciudad
                         << " | Producto: " << v.producto
                         << " | Total: $" << v.monto_total << endl;
                    contador++;
                }
            } catch (...) {
                // Ignora ventas con fecha corrupta o inválida
            }
        }

        contadorIf++;
        // Verifica si no se encontró ninguna venta dentro del rango
        if (contador == 0)
            throw runtime_error("No se encontraron ventas en ese pais para el rango de fechas dado.");
        else
            cout << "Total de ventas encontradas: " << contador << endl;

    } catch (const exception& e) {
        // Muestra errores específicos (entrada vacía, país no encontrado, etc.)
        cout << "Error: " << e.what() << endl;
    }

    auto end = chrono::high_resolution_clock::now(); // Marca el final de la ejecución
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula duración en milisegundos
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra estadísticas
}

// Compara dos países en monto, producto más vendido, y envío más usado
inline void compararPaises(Lista<Venta>& ventas) {
    int contadorIf = 0; // Contador de sentencias 'if' ejecutadas
    auto start = chrono::high_resolution_clock::now(); // Marca de inicio para medir tiempo de ejecución

    try {
        string paisA, paisB;

        cout << "Ingrese el primer pais: ";
        getline(cin, paisA); // Solicita el primer país al usuario
        contadorIf++;
        if (paisA.empty()) throw invalid_argument("Primer pais vacio."); // Verifica que no esté vacío

        cout << "Ingrese el segundo pais: ";
        getline(cin, paisB); // Solicita el segundo país
        contadorIf++;
        if (paisB.empty()) throw invalid_argument("Segundo pais vacio."); // Verifica que no esté vacío

        paisA = normalizar_texto(paisA);  // Normaliza el primer país (mayúsculas, tildes, etc.)
        paisB = normalizar_texto(paisB);  // Normaliza el segundo país

        // Verifica si los países existen en la lista de ventas
        bool existeA = false, existeB = false;
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            string pais = normalizar_texto(ventas.getDato(i).pais); // Normaliza país en cada venta
            if (pais == paisA) existeA = true;
            if (pais == paisB) existeB = true;
        }

        contadorIf++;
        if (!existeA || !existeB) {
            throw runtime_error("Uno o ambos paises no existen en la base de datos."); // Error si falta alguno
        }

        contadorIf++;
        if (paisA == paisB) throw invalid_argument("Los paises no pueden ser iguales."); // Error si son iguales

        // Inicializa acumuladores y listas para ambos países
        float montoA = 0, montoB = 0;
        Lista<ProductoCantidad> productosA;
        Lista<ProductoCantidad> productosB;
        Lista<MedioEnvioPorPais> mediosA;
        Lista<MedioEnvioPorPais> mediosB;

        // Recorre todas las ventas
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            Venta v = ventas.getDato(i);
            string paisVenta = normalizar_texto(v.pais); // Normaliza país de la venta

            contadorIf++;
            if (paisVenta == paisA) {
                montoA += v.monto_total; // Suma el monto a país A

                // Actualiza cantidad de productos vendidos
                bool encontrado = false;
                for (int j = 0; j < productosA.getTamanio(); ++j) {
                    contadorIf++;
                    ProductoCantidad pc = productosA.getDato(j);
                    if (pc.producto == v.producto) {
                        pc.cantidad += v.cantidad;
                        productosA.reemplazar(j, pc);
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) productosA.insertarUltimo({v.producto, v.cantidad}); // Si no existe, lo agrega

                // Actualiza uso de medios de envío
                encontrado = false;
                for (int j = 0; j < mediosA.getTamanio(); ++j) {
                    contadorIf++;
                    MedioEnvioPorPais m = mediosA.getDato(j);
                    if (m.medio_envio == v.medio_envio) {
                        m.contador++;
                        mediosA.reemplazar(j, m);
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) mediosA.insertarUltimo({v.pais, v.medio_envio, 1});
            }

            else if (paisVenta == paisB) {
                montoB += v.monto_total; // Suma el monto a país B

                bool encontrado = false;
                for (int j = 0; j < productosB.getTamanio(); ++j) {
                    contadorIf++;
                    ProductoCantidad pc = productosB.getDato(j);
                    if (pc.producto == v.producto) {
                        pc.cantidad += v.cantidad;
                        productosB.reemplazar(j, pc);
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) productosB.insertarUltimo({v.producto, v.cantidad});

                encontrado = false;
                for (int j = 0; j < mediosB.getTamanio(); ++j) {
                    contadorIf++;
                    MedioEnvioPorPais m = mediosB.getDato(j);
                    if (m.medio_envio == v.medio_envio) {
                        m.contador++;
                        mediosB.reemplazar(j, m);
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) mediosB.insertarUltimo({v.pais, v.medio_envio, 1});
            }
        }

        // Muestra resultados de la comparación
        cout << "--- Comparacion entre '" << paisA << "' y '" << paisB << "' ---" << endl;

        // Monto total de ventas por país
        cout << "a) Monto total de ventas:" << endl;
        cout << "- " << paisA << ": $" << montoA << endl;
        cout << "- " << paisB << ": $" << montoB << endl;

        // Lambda para obtener producto más vendido
        auto obtenerTopProducto = [](Lista<ProductoCantidad>& lista) -> ProductoCantidad {
            ProductoCantidad top = {"", 0};
            for (int i = 0; i < lista.getTamanio(); ++i) {
                ProductoCantidad pc = lista.getDato(i);
                if (pc.cantidad > top.cantidad) {
                    top = pc;
                }
            }
            return top;
        };

        ProductoCantidad topA = obtenerTopProducto(productosA);
        ProductoCantidad topB = obtenerTopProducto(productosB);

        // Producto más vendido por país
        cout << "b) Producto mas vendido (por unidades):" << endl;
        cout << "- " << paisA << ": " << topA.producto << " (" << topA.cantidad << " unidades)" << endl;
        cout << "- " << paisB << ": " << topB.producto << " (" << topB.cantidad << " unidades)" << endl;

        // Lambda para obtener medio de envío más usado
        auto obtenerMedioMasUsado = [](Lista<MedioEnvioPorPais>& lista) -> MedioEnvioPorPais {
            MedioEnvioPorPais top = {"", "", 0};
            for (int i = 0; i < lista.getTamanio(); ++i) {
                MedioEnvioPorPais m = lista.getDato(i);
                if (m.contador > top.contador) {
                    top = m;
                }
            }
            return top;
        };

        MedioEnvioPorPais envioA = obtenerMedioMasUsado(mediosA);
        MedioEnvioPorPais envioB = obtenerMedioMasUsado(mediosB);

        // Medio de envío más usado por país
        cout << "c) Medio de envio mas usado:" << endl;
        cout << "- " << paisA << ": " << envioA.medio_envio << " (" << envioA.contador << " veces)" << endl;
        cout << "- " << paisB << ": " << envioB.medio_envio << " (" << envioB.contador << " veces)" << endl;

    } catch (const exception& e) {
        // Muestra errores (entradas inválidas, país no encontrado, etc.)
        cout << "Error: " << e.what() << endl;
    }

    auto end = chrono::high_resolution_clock::now(); // Marca el fin de ejecución
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula duración total
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra métricas
}


// Compara dos productos en todos los países (cantidad y monto)
inline void compararProductosPorPais(Lista<Venta>& ventas) {
    int contadorIf = 0; // Contador de sentencias 'if' ejecutadas
    auto start = chrono::high_resolution_clock::now(); // Marca de tiempo inicial

    try {
        string productoA, productoB;

        cout << "Ingrese el nombre del primer producto: ";
        getline(cin, productoA); // Solicita primer producto al usuario
        contadorIf++;
        if (productoA.empty()) throw invalid_argument("Nombre del primer producto vacio."); // Valida entrada

        cout << "Ingrese el nombre del segundo producto: ";
        getline(cin, productoB); // Solicita segundo producto
        contadorIf++;
        if (productoB.empty()) throw invalid_argument("Nombre del segundo producto vacio."); // Valida entrada

        productoA = normalizar_texto(productoA); // Normaliza nombre del producto A
        productoB = normalizar_texto(productoB); // Normaliza nombre del producto B

        contadorIf++;
        if (productoA == productoB) throw invalid_argument("Los productos no pueden ser iguales."); // Verifica que no sean iguales

        // HashMap principal: clave = país, valor = puntero a struct ComparacionProductoPais
        HashMapList<string, ComparacionProductoPais*> mapa(200);
        Lista<string> paises; // Lista para mantener el orden de países únicos encontrados

        // Recorre todas las ventas
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            Venta v = ventas.getDato(i);
            string clavePais = v.pais;
            string productoVenta = normalizar_texto(v.producto); // Normaliza producto actual

            ComparacionProductoPais* ptr = nullptr;

            // Busca si ya existe el país en el mapa
            try {
                ptr = mapa.get(clavePais);
            } catch (...) {
                // Si no existe, lo crea e inicializa
                ptr = new ComparacionProductoPais();
                ptr->pais = clavePais;
                mapa.put(clavePais, ptr);
                paises.insertarUltimo(clavePais); // Agrega el país a la lista de países únicos
            }

            contadorIf++;
            // Acumula datos para producto A
            if (productoVenta == productoA) {
                ptr->cantidadA += v.cantidad;
                ptr->montoA += v.monto_total;
            }
            // Acumula datos para producto B
            else if (productoVenta == productoB) {
                ptr->cantidadB += v.cantidad;
                ptr->montoB += v.monto_total;
            }
        }

        cout << "--- Comparacion entre productos por pais ---" << endl;

        bool algunDato = false;

        // Muestra los resultados país por país
        for (int i = 0; i < paises.getTamanio(); ++i) {
            string pais = paises.getDato(i);
            ComparacionProductoPais* cmp = mapa.get(pais);
            contadorIf++;

            // Muestra solo si al menos uno de los productos tiene ventas
            if (cmp->cantidadA > 0 || cmp->cantidadB > 0) {
                cout << "Pais: " << pais << endl;
                cout << "  " << productoA << ": " << cmp->cantidadA << " unidades | $" << cmp->montoA << endl;
                cout << "  " << productoB << ": " << cmp->cantidadB << " unidades | $" << cmp->montoB << endl;
                algunDato = true;
            }
        }

        contadorIf++;
        if (!algunDato)
            throw runtime_error("No se encontraron registros de ventas para los productos indicados.");

        // Limpieza de memoria dinámica: elimina punteros creados manualmente
        for (int i = 0; i < 200; ++i) {
            Lista<HashEntry<string, ComparacionProductoPais*>>* lista = mapa.getListaPorIndice(i);
            if (!lista) continue;

            Nodo<HashEntry<string, ComparacionProductoPais*>>* nodo = lista->getInicio();
            while (nodo) {
                delete nodo->getDato().getValor(); // Libera memoria del puntero
                nodo = nodo->getSiguiente();
            }
        }

    } catch (const exception& e) {
        // Captura cualquier excepción y la muestra
        cout << "Error: " << e.what() << endl;
    }

    auto end = chrono::high_resolution_clock::now(); // Marca tiempo final
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula duración en milisegundos
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra estadísticas
}

// Filtra productos en un país según promedio unitario mayor/menor a umbral
inline void filtrarProductosPorPromedioUnidad(Lista<Venta>& ventas) {
    int contadorIf = 0; // Contador de 'if' ejecutados
    auto start = chrono::high_resolution_clock::now(); // Marca de inicio para medir tiempo

    try {
        string pais, tipoFiltro;
        float umbral;

        // Entrada de país
        cout << "Ingrese el pais: ";
        getline(cin, pais);
        contadorIf++;
        if (pais.empty()) throw invalid_argument("Pais vacio.");

        pais = normalizar_texto(pais);  // Normaliza país

        // Verifica si el país existe
        bool existePais = false;
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            string paisVenta = normalizar_texto(ventas.getDato(i).pais);
            contadorIf++;
            if (paisVenta == pais) {
                existePais = true;
                break;
            }
        }

        contadorIf++;
        if (!existePais) throw runtime_error("El pais ingresado no existe en los datos.");

        // Entrada de umbral
        cout << "Ingrese el umbral de promedio por unidad ($): ";
        if (!(cin >> umbral)) throw invalid_argument("Umbral invalido.");
        contadorIf++;
        if (umbral <= 0) throw invalid_argument("El umbral debe ser mayor a 0.");
        cin.ignore(); // Limpia buffer

        // Entrada de tipo de filtro
        cout << "Filtrar por productos con promedio mayor o menor al umbral? (mayor/menor): ";
        getline(cin, tipoFiltro);
        contadorIf++;
        if (tipoFiltro != "mayor" && tipoFiltro != "menor")
            throw invalid_argument("Filtro debe ser 'mayor' o 'menor'.");

        // HashMap para acumular estadísticas por producto
        HashMapList<string, ProductoEstadistica> hash(500);
        Lista<string> claves; // Lista de claves únicas de productos

        // Recorre ventas y acumula datos solo del país indicado
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            Venta v = ventas.getDato(i);
            string paisVenta = normalizar_texto(v.pais);
            contadorIf++;
            if (paisVenta == pais) {
                try {
                    // Si ya existe, acumula
                    ProductoEstadistica pe = hash.get(v.producto);
                    pe.totalCantidad += v.cantidad;
                    pe.totalMonto += v.monto_total;
                    hash.remove(v.producto); // Se actualiza reemplazando
                    hash.put(v.producto, pe);
                } catch (...) {
                    // Si no existe, se agrega nuevo
                    ProductoEstadistica nuevo{v.producto, v.cantidad, v.monto_total};
                    hash.put(v.producto, nuevo);
                    claves.insertarUltimo(v.producto);
                }
            }
        }

        // Encabezado
        cout << "--- Productos en " << pais << " con promedio ";
        cout << (tipoFiltro == "mayor" ? "> " : "< ") << "$" << umbral << " por unidad ---" << endl;

        bool hayResultados = false;

        // Filtro por promedio
        for (int i = 0; i < claves.getTamanio(); ++i) {
            ProductoEstadistica pe = hash.get(claves.getDato(i));
            contadorIf++;
            if (pe.totalCantidad == 0) continue;

            float promedio = pe.totalMonto / pe.totalCantidad;

            contadorIf++;
            if ((tipoFiltro == "mayor" && promedio > umbral) ||
                (tipoFiltro == "menor" && promedio < umbral)) {
                cout << "- " << claves.getDato(i) << ": Promedio $" << promedio << " por unidad" << endl;
                hayResultados = true;
            }
        }

        contadorIf++;
        if (!hayResultados) {
            throw runtime_error("No se encontraron productos que cumplan con ese criterio.");
        }

    } catch (const exception& e) {
        // Manejo de errores
        cout << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Fin y métricas
    auto end = chrono::high_resolution_clock::now();
    double tiempo = chrono::duration<double, milli>(end - start).count();
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl;
}

// Filtrar productos por promedio unitario en toda Sudamérica
inline void filtrarProductosPorPromedioGlobal(Lista<Venta>& ventas) {
    int contadorIf = 0; // Contador de sentencias 'if' ejecutadas
    auto start = chrono::high_resolution_clock::now(); // Marca de tiempo inicial

    float umbral;
    string tipoFiltro;

    try {
        cout << "Ingrese el umbral de promedio por unidad ($): ";
        if (!(cin >> umbral)) throw invalid_argument("Umbral invalido. Debe ser un numero."); // Valida entrada numérica
        contadorIf++;
        if (umbral <= 0) throw invalid_argument("El umbral debe ser mayor a 0."); // Umbral debe ser positivo
        contadorIf++;
        cin.ignore(); // Limpia el buffer

        cout << "Filtrar por productos con promedio mayor o menor al umbral? (mayor/menor): ";
        getline(cin, tipoFiltro); // Solicita tipo de filtro
        contadorIf++;
        if (tipoFiltro.empty()) throw invalid_argument("Tipo de filtro vacio."); // Valida entrada no vacía
        contadorIf++;

        // HashMap para acumular estadísticas por producto
        HashMapList<string, ProductoEstadistica> hash(1000); // Clave: nombre del producto
        Lista<string> claves; // Guarda claves para recorrer después

        // Recorrer todas las ventas
        for (int i = 0; i < ventas.getTamanio(); ++i) {
            Venta v = ventas.getDato(i);

            try {
                // Si ya existe, actualiza total de unidades y monto
                ProductoEstadistica pe = hash.get(v.producto);
                pe.totalCantidad += v.cantidad;
                pe.totalMonto += v.monto_total;
                hash.remove(v.producto); // Elimina y vuelve a insertar para actualizar
                hash.put(v.producto, pe);
            } catch (...) {
                // Si no existe, lo crea y guarda la clave
                ProductoEstadistica nuevo{v.producto, v.cantidad, v.monto_total};
                hash.put(v.producto, nuevo);
                claves.insertarUltimo(v.producto);
            }
        }

        // Muestra encabezado de resultados
        cout << "--- Productos con promedio ";
        cout << (tipoFiltro == "mayor" ? "> " : "< ") << "$" << umbral << " por unidad en toda Sudamerica ---" << endl;

        bool hayResultados = false;

        // Recorre las claves y filtra por el promedio
        for (int i = 0; i < claves.getTamanio(); ++i) {
            ProductoEstadistica pe = hash.get(claves.getDato(i));
            contadorIf++;
            if (pe.totalCantidad == 0) continue; // Evita división por cero

            float promedio = pe.totalMonto / pe.totalCantidad;

            contadorIf++;
            if ((tipoFiltro == "mayor" && promedio > umbral) ||
                (tipoFiltro == "menor" && promedio < umbral)) {
                // Si cumple con el filtro, lo muestra
                cout << "- " << claves.getDato(i) << ": Promedio $" << promedio << " por unidad" << endl;
                hayResultados = true;
            }
        }

        contadorIf++;
        if (!hayResultados) {
            // Si no hubo coincidencias, lo informa
            cout << "No se encontraron productos que cumplan con ese criterio." << endl;
        }

    } catch (const exception& e) {
        // Maneja errores de entrada o ejecución
        cout << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    auto end = chrono::high_resolution_clock::now(); // Marca tiempo final
    double tiempo = chrono::duration<double, milli>(end - start).count(); // Calcula duración
    cout << " " << endl;
    cout << "Tiempo: " << tiempo << " ms | If ejecutados: " << contadorIf << endl; // Muestra métricas
}

//guardarVentasCSV
inline void guardarVentasCSV(Lista<Venta>& ventas) {
    ofstream archivo("ventas_sudamerica.csv");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo para guardar." << endl;
        return;
    }

    archivo << "ID,Fecha,Pais,Ciudad,Cliente,Producto,Categoria,Cantidad,Precio Unitario,Monto Total,Medio Envio,Estado Envio\n";

    for (int i = 0; i < ventas.getTamanio(); ++i) {
        Venta v = ventas.getDato(i);
        archivo << v.id << ","
                << v.fecha << ","
                << v.pais << ","
                << v.ciudad << ","
                << v.cliente << ","
                << v.producto << ","
                << v.categoria << ","
                << v.cantidad << ","
                << v.precio_unitario << ","
                << v.monto_total << ","
                << v.medio_envio << ","
                << v.estado_envio << "\n";
    }

    archivo.close();
}
