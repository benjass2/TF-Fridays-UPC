#pragma once
#include "Producto.hpp"
#include "Cliente.hpp"
#include "Cola.hpp"
#include "Comprobante.hpp"
#include <string>
#include <iostream>
using namespace std;

class Pedido {
private:
    int numeroPedido;
    Cliente* cliente;
    Cola<Producto*> productos;
    double total;
    string estado;
    string tipoServicio;
    Comprobante* comprobante;

public:
    Pedido(int numero = 0, Cliente* cli = nullptr, string tipo = "local")
        : numeroPedido(numero), cliente(cli), total(0.0),
        estado("pendiente"), tipoServicio(tipo), comprobante(nullptr) {
    }


    ~Pedido() {}


    void agregarProducto(Producto* producto) {
        productos.encolar(producto);
        total += producto->getPrecio();
        cout << ">>> Producto agregado: " << producto->getNombre() << endl;
    }


    bool quitarProducto(int posicion) {
        if (posicion < 0 || posicion >= productos.getTamaño()) {
            cout << ">>> Posicion invalida" << endl;
            return false;
        }

        Producto* productoEliminado = productos.obtenerEnPosicion(posicion);
        total -= productoEliminado->getPrecio();

        if (productos.eliminarEnPosicion(posicion)) {
            cout << ">>> Producto eliminado: " << productoEliminado->getNombre() << endl;
            return true;
        }

        return false;
    }




    Producto* procesarSiguienteProducto() {
        if (productos.estaVacia()) {
            cout << ">>> No hay mas productos en el pedido" << endl;
            return nullptr;
        }

        Producto* producto = productos.desencolar();
        cout << ">>> Procesando: " << producto->getNombre() << endl;
        return producto;
    }

    Producto* verSiguienteProducto() const {
        if (productos.estaVacia()) {
            return nullptr;
        }
        return productos.verFrente();
    }


    double calcularTotal() const {
        return total;
    }


    void mostrarResumen() const {

        cout << "PEDIDO #" << numeroPedido << endl;


        if (cliente != nullptr) {
            cout << "Cliente: " << cliente->getNombreCompleto() << endl;
            cout << "DNI: " << cliente->getDni() << endl;
            cout << "Telefono: " << cliente->getTelefono() << endl;
            if (tipoServicio == "delivery") {
                cout << "Direccion: " << cliente->getDireccion() << endl;
            }
        }

        cout << "Tipo: " << tipoServicio << endl;
        cout << "Estado: " << estado << endl;
        cout << "Productos en cola: " << productos.getTamaño() << endl;
        cout << "Total: S/" << fixed << setprecision(2) << total << endl;


        //Mostrar tipo de comprobante si existe
        if (comprobante != nullptr)
        {
            cout << "Comprobante: " << comprobante->getTipoComprobante() << " N" << comprobante->getNumeroComprobante() << endl;
        }


    }


    void mostrarProductos() const {
        cout << ">>> Productos del pedido #" << numeroPedido << " del cliente: " << getCliente()->getNombreCompleto() << endl;

        if (productos.estaVacia()) {
            cout << ">>> Pedido vacio" << endl;
            return;
        }

        for (int i = 0; i < productos.getTamaño(); i++) {
            Producto* p = productos.obtenerEnPosicion(i);
            cout << (i + 1) << ". " << p->getNombre()
                << " - S/" << fixed << setprecision(2) << p->getPrecio() << endl;
        }
    }

    void cambiarEstado(const string& nuevoEstado) {
        estado = nuevoEstado;
        /* cout << ">>> Pedido #" << numeroPedido << " - Estado: " << estado << endl;*/
    }

    bool estaVacio() const {
        return productos.estaVacia();
    }

    bool operator==(const Pedido& otro) const {
        return numeroPedido == otro.numeroPedido;
    }

    // Getters
    int getNumeroPedido() const { return numeroPedido; }
    Cliente* getCliente() const { return cliente; }
    string getNombreCliente() const {
        return cliente ? cliente->getNombreCompleto() : "Sin cliente";
    }
    string getEstado() const { return estado; }
    string getTipoServicio() const { return tipoServicio; }
    int getCantidadProductos() const { return productos.getTamaño(); }
    Cola<Producto*>* getProductos() { return &productos; }
    Comprobante* getComprobante() const { return comprobante; }

    // Setters
    void setCliente(Cliente* cli) { cliente = cli; }
    void setTipoServicio(const string& tipo) { tipoServicio = tipo; }
    void setComprobante(Comprobante* comp) { comprobante = comp; }
};

