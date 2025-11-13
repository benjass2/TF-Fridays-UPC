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
        actualizarIndices();  
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

        // 1. Excluir BEBIDAS ALCOHÓLICAS Y CÓCTELES
        if (nombre.find("cerveza") != string::npos ||
            nombre.find("vino") != string::npos ||
            nombre.find("whisky") != string::npos ||
            nombre.find("ron") != string::npos ||
            nombre.find("vodka") != string::npos ||
            nombre.find("mojito") != string::npos ||
            nombre.find("chardonay") != string::npos ||
            nombre.find("tequila") != string::npos ||
            nombre.find("daiquiri") != string::npos ||
            nombre.find("pisco") != string::npos ||     // Excluye Pisco Sour
            nombre.find("margarita") != string::npos || // Excluye Margarita Clasica
            nombre.find("gin") != string::npos          // Excluye Gin Tonic
            ) {
            return false;
        }

        // 2. Excluir LÍQUIDOS CALIENTES / SOPAS (Se enfrían/derraman)
        if (nombre.find("sopa") != string::npos ||         // Excluye Sopa de Tomate, Sopa Criolla
            nombre.find("te caliente") != string::npos || // Excluye Te Caliente
            nombre.find("cafe") != string::npos) {
            return false;
        }

        // 3. Excluir BEBIDAS Y POSTRES FROZEN/DELICADOS (Se derriten/pierden textura)
        if (nombre.find("helado") != string::npos ||      // Excluye Helado de Vainilla
            nombre.find("mousse") != string::npos ||      // Excluye Mousse de Chocolate
            nombre.find("capuchino") != string::npos ||   // Excluye Capuchino Frio
            nombre.find("frappuccino") != string::npos || // Excluye Frappuccino Caramelo
            nombre.find("smoothie") != string::npos ||    // Excluye Smoothie de Mango
            nombre.find("malteada") != string::npos ||    // Excluye Malteada de Vainilla
            nombre.find("batido") != string::npos) {      // Excluye Batido de Platano
            return false;
        }

        // 4. Excluir ALIMENTOS FRITOS/CRUJIENTES (Pierden textura en el viaje)
        if (nombre.find("papas fritas") != string::npos ||     // Excluye Papas Fritas Clasicas
            nombre.find("churros") != string::npos ||          // Excluye Churros con Chocolate
            nombre.find("nachos") != string::npos ||           // Excluye Nachos con Guacamole
            nombre.find("aros de cebolla") != string::npos) { // Excluye Aros de Cebolla
            return false;
        }

        // Si pasa todos los filtros, es apto para delivery.
        return true;
    }


};
