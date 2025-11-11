#pragma once
#include "Pedido.hpp"
#include "Pila.hpp"
#include "Cliente.hpp"
#include "Producto.hpp"
#include "Comprobante.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class GestorPedidos {
public:
    static bool guardarHistorial(const Pila<Pedido*>& historial, const string& nombreArchivo = "Data/historialPedidos.txt") {
        ofstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            cout << ">>> Error: No se pudo abrir el archivo " << nombreArchivo << " para escribir." << endl;
            return false;
        }
        archivo << "# Historial de Pedidos - Fridays Restaurant" << endl;
        archivo << "# Formato: PedidoID,ClienteNombre,ClienteDNI,ClienteTelefono,ClienteDireccion,Total,Fecha,Estado,TipoServicio,TipoComprobante,NumComprobante" << endl;

        // Escribir cada pedido del historial
        for (int i = 0; i < historial.getTamaño(); i++) {
            Pedido* p = historial.obtenerEnPosicion(i);
            Cliente* c = p->getCliente();
            Comprobante* comp = p->getComprobante();

            archivo << p->getNumeroPedido() << ","
                << (c ? c->getNombreCompleto() : "Sin cliente") << ","
                << (c ? c->getDni() : "") << ","
                << (c ? c->getTelefono() : "") << ","
                << (c ? c->getDireccion() : "") << ","
                << p->calcularTotal() << ","
                << (comp ? comp->getFecha() : "Sin fecha") << ","
                << p->getEstado() << ","
                << p->getTipoServicio() << ","
                << (comp ? comp->getTipoComprobante() : "Sin comprobante") << ","
                << (comp ? comp->getNumeroComprobante() : 0) << endl;
        }

        archivo.close();
        cout << ">>> Historial guardado: " << historial.getTamaño() << " pedidos" << endl;
        return true;
    }

    static void cargarHistorial(Pila<Pedido*>& historial, int& contadorPedidos, const string& nombreArchivo = "Data/historial_pedidos.txt") {
        ifstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            cout << ">>> Archivo " << nombreArchivo << " no encontrado. Se creara uno nuevo." << endl;
            return;
        }

        string linea;
        int pedidosLeidos = 0;
        int maxNumeroPedido = 0;

        while (getline(archivo, linea)) {
            // Saltar líneas vacías o comentarios
            if (linea.empty() || linea[0] == '#') {
                continue;
            }

            // Parsear línea
            stringstream ss(linea);
            string numPedidoStr, nombreCliente, dni, telefono, direccion;
            string totalStr, fecha, estado, tipoServicio, tipoComprobante, numComprobanteStr;

            if (getline(ss, numPedidoStr, ',') &&
                getline(ss, nombreCliente, ',') &&
                getline(ss, dni, ',') &&
                getline(ss, telefono, ',') &&
                getline(ss, direccion, ',') &&
                getline(ss, totalStr, ',') &&
                getline(ss, fecha, ',') &&
                getline(ss, estado, ',') &&
                getline(ss, tipoServicio, ',') &&
                getline(ss, tipoComprobante, ',') &&
                getline(ss, numComprobanteStr)) {

                try {
                    int numPedido = stoi(numPedidoStr);

                    // Actualizar contador de pedidos
                    if (numPedido > maxNumeroPedido) {
                        maxNumeroPedido = numPedido;
                    }

                    // Crear cliente temporal (solo para mostrar datos)
                    string nombre = nombreCliente.substr(0, nombreCliente.find(' '));
                    string apellido = nombreCliente.substr(nombreCliente.find(' ') + 1);
                    Cliente* cliente = new Cliente(0, nombre, apellido, telefono, dni, direccion);

                    // Crear pedido básico (sin productos, solo para historial)
                    Pedido* pedido = new Pedido(numPedido, cliente, tipoServicio);
                    pedido->cambiarEstado(estado);

                    // Agregar a historial
                    historial.apilar(pedido);
                    pedidosLeidos++;
                }
                catch (const exception& e) {
                    cout << ">>> Error al procesar linea: " << linea << endl;
                }
            }
        }

        archivo.close();

        // Actualizar contador de pedidos para nuevos pedidos
        if (maxNumeroPedido > 0) {
            contadorPedidos = maxNumeroPedido + 1;
        }

        if (pedidosLeidos > 0) {
            cout << ">>> " << pedidosLeidos << " pedidos cargados desde " << nombreArchivo << endl;
            cout << ">>> Proximo numero de pedido: " << contadorPedidos << endl;
        }
    }


    static void generarReporteVentas(const Pila<Pedido*>& historial, const string& nombreReporte = "Data/reporte_ventas.txt") {
        ofstream reporte(nombreReporte);

        if (!reporte.is_open()) {
            cout << ">>> Error al crear el reporte." << endl;
            return;
        }

        reporte << "================================================================" << endl;
        reporte << "           REPORTE DE VENTAS - FRIDAYS RESTAURANT               " << endl;
        reporte << "================================================================" << endl;

        double totalVentas = 0.0;
        int totalPedidos = historial.getTamaño();
        int pedidosLocal = 0, pedidosDelivery = 0;
        int boletas = 0, facturas = 0;

        reporte << "\n>>> HISTORIAL DE PEDIDOS:" << endl;
        reporte << string(60, '-') << endl;

        for (int i = 0; i < totalPedidos; i++) {
            Pedido* p = historial.obtenerEnPosicion(i);
            double total = p->calcularTotal();
            totalVentas += total;

            // Contadores
            if (p->getTipoServicio() == "local") pedidosLocal++;
            else pedidosDelivery++;

            Comprobante* comp = p->getComprobante();
            if (comp) {
                string tipo = comp->getTipoComprobante();
                if (tipo.find("BOLETA") != string::npos) boletas++;
                else if (tipo.find("FACTURA") != string::npos) facturas++;
            }

            reporte << "Pedido #" << p->getNumeroPedido()
                << " | Cliente: " << p->getNombreCliente()
                << " | Total: S/" << fixed << setprecision(2) << total
                << " | Tipo: " << p->getTipoServicio()
                << " | Estado: " << p->getEstado() << endl;
        }

        reporte << "\n>>> ESTADISTICAS:" << endl;
        reporte << string(40, '-') << endl;
        reporte << "Total de pedidos: " << totalPedidos << endl;
        reporte << "Pedidos en local: " << pedidosLocal << endl;
        reporte << "Pedidos delivery: " << pedidosDelivery << endl;
        reporte << "Boletas emitidas: " << boletas << endl;
        reporte << "Facturas emitidas: " << facturas << endl;
        reporte << "\nVENTAS TOTALES: S/" << fixed << setprecision(2) << totalVentas << endl;

        if (totalPedidos > 0) {
            reporte << "Ticket promedio: S/" << fixed << setprecision(2) << (totalVentas / totalPedidos) << endl;
        }

        reporte.close();
        cout << ">>> Reporte de ventas generado: " << nombreReporte << endl;
    }

    // Buscar pedidos por cliente (usando lambda)
    static Pila<Pedido*> buscarPorCliente(const Pila<Pedido*>& historial, const string& dni) {
        Pila<Pedido*> resultado;

        for (int i = 0; i < historial.getTamaño(); i++) {
            Pedido* p = historial.obtenerEnPosicion(i);
            Cliente* c = p->getCliente();
            if (c && c->getDni() == dni) {
                resultado.apilar(p);
            }
        }

        return resultado;
    }

    // Filtrar pedidos usando lambda
    static Pila<Pedido*> filtrarPedidos(const Pila<Pedido*>& historial,
        function<bool(Pedido*)> condicion) {
        Pila<Pedido*> resultado;

        for (int i = 0; i < historial.getTamaño(); i++) {
            Pedido* p = historial.obtenerEnPosicion(i);
            if (condicion(p)) {
                resultado.apilar(p);
            }
        }

        return resultado;
    }

    // Calcular total de ventas
    static double calcularTotalVentas(const Pila<Pedido*>& historial) {
        double total = 0.0;
        for (int i = 0; i < historial.getTamaño(); i++) {
            total += historial.obtenerEnPosicion(i)->calcularTotal();
        }
        return total;
    }
};