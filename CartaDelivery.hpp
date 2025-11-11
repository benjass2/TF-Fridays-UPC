#pragma once
#include "Carta.hpp"
#include <iostream>  

using namespace std;

class CartaDelivery : public Carta {
private:
    double tarifaDelivery;
    int tiempoEstimadoMin;
    double montoMinimoDelivery;

public:
    CartaDelivery() : Carta("delivery", "Menu para Delivery", "Data/menu.txt") {
        tarifaDelivery = 8.90;
        tiempoEstimadoMin = 35;
        montoMinimoDelivery = 25.00;
    }

    ~CartaDelivery() {}

    void cargarProductos(const ListaMenu<Producto>& productosExternos) override {
        productos = ListaMenu<Producto>();
        for (int i = 0; i < productosExternos.getTamaño(); i++) {
            Producto p = productosExternos.obtenerEnPosicion(i);
            if (esAptoParaDelivery(p)) {
                productos.insertarFinal(p);
            }
        }
        actualizarIndices();  // ? AGREGAR ESTA LÍNEA
        cout << ">>> Carta Delivery: " << productos.getTamaño() << " productos cargados (filtrados)" << endl;
    }

    double getTarifaDelivery() const { return tarifaDelivery; }
    int getTiempoEstimado() const { return tiempoEstimadoMin; }
    double getMontoMinimo() const { return montoMinimoDelivery; }

    void setTarifaDelivery(double tarifa) {
        if (tarifa >= 0) tarifaDelivery = tarifa;
    }

    void setTiempoEstimado(int minutos) {
        if (minutos > 0) tiempoEstimadoMin = minutos;
    }

private:
    string convertirMinusculas(string texto) const {
        for (size_t i = 0; i < texto.length(); i++) {
            if (texto[i] >= 'A' && texto[i] <= 'Z') {
                texto[i] = texto[i] + ('a' - 'A');
            }
        }
        return texto;
    }

    bool esAptoParaDelivery(const Producto& p) const {
        string nombre = convertirMinusculas(p.getNombre());

        // Excluir bebidas alcohólicas
        if (nombre.find("cerveza") != string::npos ||
            nombre.find("vino") != string::npos ||
            nombre.find("whisky") != string::npos ||
            nombre.find("ron") != string::npos ||
            nombre.find("vodka") != string::npos ||
            nombre.find("mojito") != string::npos) {
            return false;
        }

        // Excluir productos delicados
        if (nombre.find("helado") != string::npos) {
            return false;
        }

        // Excluir sopas (se enfrían)
        if (nombre.find("sopa") != string::npos) {
            return false;
        }

        // Excluir bebidas calientes
        if (nombre.find("te caliente") != string::npos ||
            nombre.find("cafe") != string::npos) {
            return false;
        }

        // Excluir frozen/batidos (se derriten)
        if (nombre.find("frozen") != string::npos ||
            nombre.find("batido") != string::npos) {
            return false;
        }

        // Excluir alimentos que pierden textura
        if (nombre.find("papas fritas") != string::npos ||
            nombre.find("churros") != string::npos ||
            nombre.find("nachos") != string::npos) {
            return false;
        }

        // Excluir postres muy delicados
        if (nombre.find("mousse") != string::npos) {
            return false;
        }

        return true;
    }
};
