#pragma once
#include "ArbolBST.hpp"
#include "Reserva.hpp"
#include "Mesa.hpp"
#include "Cliente.hpp"
#include "Lista.hpp"
#include "GestorArchivos.hpp"
#include "Funcionalidades.hpp"
using namespace ConsolaColor;

class GestorReserva {
private:
    ArbolBST<Reserva> arbolReservas;
    Lista<Mesa> mesas;

public:


    GestorReserva() {
        // Intentar cargar mesas desde archivo
        mesas = GestorArchivos::cargarMesas();

        // Si no hay mesas cargadas, inicializar por defecto
        if (mesas.estaVacia()) {
            inicializarMesas();
            // Guardar las mesas inicializadas
            GestorArchivos::guardarMesas(mesas);
        }

        // Cargar reservas existentes
        GestorArchivos::cargarReservas(arbolReservas);
    }

    // Destructor
    ~GestorReserva() {

        guardarDatos();
    }



    void inicializarMesas() {
        // Mesas regulares - Interior (10 mesas)
        for (int i = 1; i <= 10; i++) {
            mesas.insertarFinal(Mesa(i, 4, "Interior", "Regular"));
        }

        // Mesas familiares - Interior (5 mesas)
        for (int i = 11; i <= 15; i++) {
            mesas.insertarFinal(Mesa(i, 6, "Interior", "Familiar"));
        }

        // Mesas terraza (5 mesas)
        for (int i = 16; i <= 20; i++) {
            mesas.insertarFinal(Mesa(i, 4, "Terraza", "Regular"));
        }

        // Mesas VIP (5 mesas)
        for (int i = 21; i <= 25; i++) {
            mesas.insertarFinal(Mesa(i, 4, "VIP", "VIP"));
        }

        cout << ">>> " << mesas.getTamaño() << " mesas inicializadas." << endl;
    }



    void guardarDatos() {
        GestorArchivos::guardarReservas(arbolReservas);
        GestorArchivos::guardarMesas(mesas);
    }


    //Crear reserva
    bool crearReserva(const Cliente& cliente, int numeroMesa, string fecha, string hora, int numeroPersonas, string observaciones = "") {

        // Buscar la mesa
        Mesa* mesaSeleccionada = buscarMesa(numeroMesa);

        if (mesaSeleccionada == nullptr) {
            cout << ROJO << ">>> Error: Mesa no encontrada." << RESET << endl;
            return false;
        }

        // Verificar capacidad
        if (!mesaSeleccionada->tieneCapacidadPara(numeroPersonas)) {
            cout << ROJO << ">>> Error: La mesa solo tiene capacidad para " << mesaSeleccionada->getCapacidad() << " personas." << RESET << endl;
            delete mesaSeleccionada;  // Liberar memoria
            return false;
        }

        // Crear la reserva
        Reserva nuevaReserva(cliente, *mesaSeleccionada, fecha, hora, numeroPersonas, observaciones);

        // Insertar en el árbol BST
        arbolReservas.insertar(nuevaReserva);

        cout << VERDE << "\n>>> Reserva creada exitosamente!" << RESET << endl;
        cout << "Codigo de reserva: " << nuevaReserva.getCodigoReserva() << endl;

        delete mesaSeleccionada;  // Liberar memoria

        // Guardar cambios
        guardarDatos();

        return true;
    }


    //Mostrar todas las reservas 
    void mostrarTodasReservas() {
        if (arbolReservas.estaVacio()) {
            cout << "\n>>> No hay reservas registradas." << endl;
            return;
        }


        cout << ROJO << "                    RESERVAS (Orden Cronologico)                        " << endl;
        imprimirSeparadorRojoBlanco(40);


        cout << left << setw(10) << "CODIGO"
            << setw(25) << "CLIENTE"
            << setw(12) << "FECHA"
            << setw(8) << "HORA"
            << setw(8) << "MESA"
            << setw(6) << "PERS."
            << setw(12) << "ESTADO" << endl;

        imprimirSeparadorRojoBlanco(40);

        // Recorrido in-order con lambda ⭐
        arbolReservas.inOrder([](Reserva r) {
            r.mostrarResumen();
            });


        cout << "Total de reservas: " << arbolReservas.getTamaño() << endl;
    }


    //Buscar reserva por codigo
    void buscarReservaPorCodigo(string codigo) {
        bool encontrada = false;

        // Recorrer árbol buscando el código con lambda ⭐
        arbolReservas.inOrder([&](Reserva r) {
            if (r.getCodigoReserva() == codigo) {
                r.mostrarInfo();
                encontrada = true;
            }
            });

        if (!encontrada) {
            cout << "\n>>> Reserva no encontrada." << endl;
        }
    }


    //Listar reserva por fecha
    void listarReservasPorFecha(string fecha) {
        int contador = 0;

        cout << ROJO << "\n>>> Reservas para el dia: " << fecha << RESET << endl;
        imprimirSeparadorRojoBlanco(20);

        // Filtrar por fecha con lambda ⭐
        arbolReservas.inOrder([&](Reserva r) {
            if (r.getFecha() == fecha) {
                r.mostrarResumen();
                contador++;
            }
            });

        if (contador == 0) {
            cout << ">>> No hay reservas para esta fecha." << endl;
        }
        else {
            imprimirSeparadorRojoBlanco(20);
            cout << "Total: " << contador << " reservas" << endl;
        }
    }


    //Mostrrar mesas sisponibles
    void mostrarMesasDisponibles(int capacidadMinima, string fecha, string hora) {
        cout << ROJO << "\n>>> Mesas disponibles (capacidad >= " << capacidadMinima << "):" << RESET << endl;
        imprimirSeparadorRojoBlanco(30);

        cout << left << setw(6) << "MESA"
            << setw(12) << "CAPACIDAD"
            << setw(12) << "UBICACION"
            << setw(10) << "TIPO"
            << setw(10) << "ESTADO" << endl;
        imprimirSeparadorRojoBlanco(30);

        for (int i = 0; i < mesas.getTamaño(); i++) {
            Mesa mesa = mesas.obtenerEnPosicion(i);
            if (mesa.getCapacidad() >= capacidadMinima) {
                mesa.mostrarResumen();
            }
        }


    }


    //Cancelar reserva
    void cancelarReserva(string codigo) {
        bool encontrada = false;
        Reserva reservaEliminar;

        // Buscar la reserva con lambda ⭐
        arbolReservas.inOrder([&](Reserva r) {
            if (r.getCodigoReserva() == codigo) {
                reservaEliminar = r;
                encontrada = true;
            }
            });

        if (encontrada) {
            arbolReservas.eliminar(reservaEliminar);
            cout << VERDE << ">>> Reserva " << codigo << " cancelada exitosamente." << RESET << endl;

            // Guardar cambios
            guardarDatos();
        }
        else {
            cout << ROJO << ">>> Reserva no encontrada." << endl;
        }
    }


    //Buscar mesa por numero
    Mesa* buscarMesa(int numeroMesa) {
        for (int i = 0; i < mesas.getTamaño(); i++) {
            Mesa mesa = mesas.obtenerEnPosicion(i);
            if (mesa.getNumeroMesa() == numeroMesa) {
                // Retornar puntero a una copia de la mesa encontrada
                return new Mesa(mesa);
            }
        }
        return nullptr;
    }


    //Mostrar estadistica 
    void mostrarEstadisticas() {

        cout << ROJO << "    ESTADÍSTICAS DEL SISTEMA           " << RESET << endl;
        imprimirSeparadorRojoBlanco(30);

        cout << " Total de mesas: " << mesas.getTamaño() << endl;
        cout << " Total de reservas: " << arbolReservas.getTamaño() << endl;
        cout << " Altura del árbol BST: " << arbolReservas.getAltura() << endl;

    }

    //Listar todas las mesas
    void listarTodasMesas() {

        cout << ROJO << "           MESAS DEL RESTAURANTE FRIDAY'S              " << RESET << endl;
        imprimirSeparadorRojoBlanco(40);


        cout << left << setw(6) << "MESA"
            << setw(12) << "CAPACIDAD"
            << setw(12) << "UBICACION"
            << setw(10) << "TIPO"
            << setw(10) << "ESTADO" << endl;
        imprimirSeparadorRojoBlanco(40);

        for (int i = 0; i < mesas.getTamaño(); i++) {
            Mesa mesa = mesas.obtenerEnPosicion(i);
            mesa.mostrarResumen();
        }


    }



    int getTotalReservas() const {
        return arbolReservas.getTamaño();
    }

    int getTotalMesas() const {
        return mesas.getTamaño();
    }
};