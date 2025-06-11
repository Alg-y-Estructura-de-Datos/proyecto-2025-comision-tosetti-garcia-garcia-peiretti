#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "Lista.h"
#include "funciones.h"
#include "HashMap.h"
#include "quickSort.h"

#define NOMBRE_ARCHIVO "ventas_sudamerica.csv"
using namespace std;
using namespace std::chrono;

int main() {
    Lista<Venta> ventas;
    HashMap<int, int> ventasPorID(5000); // tamaño estimado
    int contadorIf = 0;

    auto start = high_resolution_clock::now();

    ifstream archivo(NOMBRE_ARCHIVO);
    string linea;
    char delimitador = ',';

    // Leer encabezado
    getline(archivo, linea);

    // Leer ventas
    while (getline(archivo, linea)) {
        stringstream stream(linea);
        Venta venta;
        string cantidad, precio_unitario, monto_total;

        getline(stream, linea, delimitador); venta.id = stoi(linea);
        getline(stream, venta.fecha, delimitador);
        getline(stream, venta.pais, delimitador);
        getline(stream, venta.ciudad, delimitador);
        getline(stream, venta.cliente, delimitador);
        getline(stream, venta.producto, delimitador);
        getline(stream, venta.categoria, delimitador);
        getline(stream, cantidad, delimitador); venta.cantidad = stoi(cantidad);
        getline(stream, precio_unitario, delimitador); venta.precio_unitario = stof(precio_unitario);
        getline(stream, monto_total, delimitador); venta.monto_total = stof(monto_total);
        getline(stream, venta.medio_envio, delimitador);
        getline(stream, venta.estado_envio, delimitador);

        ventas.insertarUltimo(venta);
        ventasPorID.put(venta.id, ventas.getTamanio() - 1);
        contadorIf++;
    }

    auto end = high_resolution_clock::now();
    double tiempo = duration<double, milli>(end - start).count();

    cout << "--- Carga Inicial desde CSV Completa ---" << endl;
    cout << "Tiempo: " << tiempo << " ms | Condicionales (if ejecutados): " << contadorIf << endl;

    // MENÚ PRINCIPAL DIVIDIDO
    int opcion;
    string entrada;

    do {
        cout << " " << endl;
        cout << "--- Menu Principal ---" << endl;
        cout << "1. Modificacion de Datos" << endl;
        cout << "2. Consultas Automaticas" << endl;
        cout << "3. Consultas Dinamicas" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        getline(cin, entrada);

        try {
            opcion = stoi(entrada);
        } catch (...) {
            cout << "Entrada invalida. Ingrese un numero valido." << endl;;
            continue;
        }

        switch (opcion) {
            case 1: {
    int subop;
    cout << " " << endl;
    cout << "--- Modificacion de Datos ---" << endl;
    cout << "1. Agregar venta" << endl;
    cout << "2. Eliminar venta por ciudad o pais" << endl;
    cout << "3. Modificar venta por ID" << endl;
    cout << "4. Volver al menu principal" << endl;
    cout << "Seleccione una opcion: ";
    getline(cin, entrada);

    try {
        subop = stoi(entrada);
        switch (subop) {
            case 1: agregarVenta(ventas, ventasPorID); break;
            case 2: eliminarVenta(ventas, ventasPorID); break;
            case 3: modificarVenta(ventas, ventasPorID); break;
            case 4: break;
            default: cout << "Opcion invalida.\n"; break;
        }
    } catch (const exception& e) {
        cout << "Error en submenu de modificacion. " << endl;
    }
    break;
}


           case 2: {
    int subop;
    cout << " " << endl;
    cout << "--- Consultas Automaticas ---" << endl;
    cout << "1. Top 5 ciudades por pais" << endl;
    cout << "2. Monto total por producto y pais" << endl;
    cout << "3. Promedio de ventas por categoria y pais" << endl;
    cout << "4. Medio de envio mas usado por pais" << endl;
    cout << "5. Medio de envio mas usado por categoria" << endl;
    cout << "6. Dia con mayor monto total de ventas" << endl;
    cout << "7. Estado de envio mas frecuente por pais" << endl;
    cout << "8. Producto mas vendido por cantidad de unidades" << endl;
    cout << "9. Producto menos vendido por cantidad de unidades" << endl;
    cout << "10. Volver al menu principal" << endl;
    cout << "Seleccione una opcion: ";
    getline(cin, entrada);

    try {
        subop = stoi(entrada);
        switch (subop) {
            case 1: top5CiudadesPorPais(ventas); break;
            case 2: montoTotalPorProductoYPais(ventas); break;
            case 3: promedioVentasPorCategoriaYPais(ventas); break;
            case 4: medioEnvioMasUsadoPorPais(ventas); break;
            case 5: medioEnvioMasUsadoPorCategoria(ventas); break;
            case 6: diaConMayorMontoTotal(ventas); break;
            case 7: estadoEnvioMasFrecuentePorPais(ventas); break;
            case 8: productoMasVendidoPorCantidad(ventas); break;
            case 9: productoMenosVendidoPorCantidad(ventas); break;
            case 10: break;
            default: cout << "Opcion invalida." <<endl ; break;
        }
    } catch (const exception& e) {
        cout << "Error en submenu de consultas automaticas. " << endl;
    }
    break;
}

            case 3: {
                int subop;
                cout << " " << endl;
                cout << "--- Consultas Dinamicas ---" << endl;
                cout << "1. Consultar ventas por ciudad" << endl;
                cout << "2. Consultar ventas por pais y rango de fechas" << endl;
                cout << "3. Comparar dos paises" << endl;
                cout << "4. Comparar dos productos por pais" << endl;
                cout << "5. Filtrar productos por promedio unitario y pais" << endl;
                cout << "6. Filtrar productos por promedio unitario en toda Sudamérica" << endl;
                cout << "7. Volver al menu principal" << endl;
                cout << "Seleccione una opcion: ";
                getline(cin, entrada);

                try {
                    subop = stoi(entrada);
                    switch (subop) {
                        case 1: consultarVentasPorCiudad(ventas); break;
                        case 2: consultarVentasPorFechaYPais(ventas); break;
                        case 3: compararPaises(ventas); break;
                        case 4: compararProductosPorPais(ventas); break;
                        case 5: filtrarProductosPorPromedioUnidad(ventas); break;
                        case 6: filtrarProductosPorPromedioGlobal(ventas); break;
                        case 7: break;
                        default: cout << "Opcion invalida." << endl; break;
                    }
                } catch (const exception& e) {
                    cout << "Error en submenu de consultas dinamicas. " << endl;
                }
                break;
            }

            case 0:
                cout << "Saliendo del programa..." << endl;
                exit(0);
                break;

            default:
                cout << "Opcion invalida. Ingrese un numero entre 0 y 3." << endl;
                break;
        }

    } while (opcion != 0);

    return 0;
}
