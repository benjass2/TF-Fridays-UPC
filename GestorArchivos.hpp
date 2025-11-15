#pragma once
#include "Lista.hpp"
#include "Producto.hpp"
#include "ArbolBST.hpp"
#include "Reserva.hpp"
#include "Mesa.hpp"
#include "Cliente.hpp"
#include "Funcionalidades.hpp"


class GestorArchivos {
    static int convertirSeguro(const string& texto, int valorPorDefecto = 0) {
        try {
            return stoi(texto);
        }
        catch (...) {
            return valorPorDefecto;
        }
    }


public:


    static void guardarProductos(const Lista<Producto>& productos, const string& archivo) {
        ofstream file(archivo);

        if (!file.is_open()) {
            cout << ">>> Error: No se pudo abrir el archivo " << archivo << endl;
            return;
        }

        file << "# Menu Fridays Restaurant\n";
        file << "# Formato: ID,Nombre,Precio,Categoria,Disponible\n";

        for (int i = 0; i < productos.getTamaño(); i++) {
            Producto p = productos.obtenerEnPosicion(i);
            file << p.getId() << ","
                << p.getNombre() << ","
                << p.getPrecio() << ","
                << p.getCategoria() << ","
                << (p.isDisponible() ? "1" : "0") << "\n";
        }

        file.close();
    }

    static Lista<Producto> cargarProductos(const string& archivo) {
        Lista<Producto> productos;
        ifstream file(archivo);

        if (!file.is_open()) {
            cout << ">>> Archivo " << archivo << " no encontrado. Usando menu por defecto." << endl;
            return productos;
        }

        string linea;
        while (getline(file, linea)) {
            // Saltar líneas vacías y comentarios
            if (linea.empty() || linea[0] == '#') continue;

            stringstream ss(linea);
            int id, disponible;
            string nombre, categoria;
            double precio;

            ss >> id;
            ss.ignore();
            getline(ss, nombre, ',');
            ss >> precio;
            ss.ignore();
            getline(ss, categoria, ',');
            ss >> disponible;

            Producto producto(id, nombre, precio, categoria, disponible == 1);
            productos.insertarFinal(producto);
        }

        file.close();
        return productos;
    }

    // ========================================
    // MÉTODOS PARA RESERVAS (NUEVOS)
    // ========================================

    static void guardarReservas(const ArbolBST<Reserva>& arbol, const string& archivo = "Data/reservas.txt") {
        ofstream file(archivo);

        if (!file.is_open()) {
            cout << ">>> Error: No se pudo abrir " << archivo << endl;
            return;
        }

        file << "# Reservas - Fridays Restaurant\n";
        file << "# Formato: CodigoReserva,ClienteID,ClienteNombre,ClienteApellido,ClienteDNI,ClienteTelefono,";
        file << "NumeroMesa,CapacidadMesa,UbicacionMesa,TipoMesa,Fecha,Hora,NumPersonas,Estado,Observaciones\n";

        // Recorrer árbol in-order con lambda
        arbol.inOrder([&file](Reserva r) {
            Cliente c = r.getCliente();
            Mesa m = r.getMesa();

            file << r.getCodigoReserva() << ","
                << c.getId() << ","
                << c.getNombre() << ","
                << c.getApellido() << ","
                << c.getDni() << ","
                << c.getTelefono() << ","
                << m.getNumeroMesa() << ","
                << m.getCapacidad() << ","
                << m.getUbicacion() << ","
                << m.getTipoMesa() << ","
                << r.getFecha() << ","
                << r.getHora() << ","
                << r.getNumeroPersonas() << ","
                << r.getEstado() << ","
                << r.getObservaciones() << "\n";
            });

        file.close();
    }

    static void cargarReservas(ArbolBST<Reserva>& arbol, const string& archivo = "Data/reservas.txt") {
        ifstream file(archivo);

        if (!file.is_open()) {
            // No mostrar error si es primera vez
            return;
        }

        string linea;
        int contador = 0;

        // Resetear contador de Reserva antes de cargar
        Reserva::resetearContador();

        while (getline(file, linea)) {
            // Saltar comentarios
            if (linea.empty() || linea[0] == '#') continue;

            try {
                stringstream ss(linea);
                string codigoReserva, clienteNombre, clienteApellido, clienteDNI, clienteTelefono;
                string ubicacionMesa, tipoMesa, fecha, hora, estado, observaciones;
                int clienteID, numeroMesa, capacidadMesa, numPersonas;

                // Parsear línea
                getline(ss, codigoReserva, ',');
                ss >> clienteID; ss.ignore();
                getline(ss, clienteNombre, ',');
                getline(ss, clienteApellido, ',');
                getline(ss, clienteDNI, ',');
                getline(ss, clienteTelefono, ',');
                ss >> numeroMesa; ss.ignore();
                ss >> capacidadMesa; ss.ignore();
                getline(ss, ubicacionMesa, ',');
                getline(ss, tipoMesa, ',');
                getline(ss, fecha, ',');
                getline(ss, hora, ',');
                ss >> numPersonas; ss.ignore();
                getline(ss, estado, ',');
                getline(ss, observaciones);

                // Crear objetos
                Cliente cliente(clienteID, clienteNombre, clienteApellido, clienteTelefono, clienteDNI);
                Mesa mesa(numeroMesa, capacidadMesa, ubicacionMesa, tipoMesa);

                // Crear reserva con código específico
                Reserva reserva(codigoReserva, cliente, mesa, fecha, hora, numPersonas, estado, observaciones);

                arbol.insertar(reserva);
                contador++;
            }
            catch (...) {
                // Ignorar líneas con errores
                continue;
            }
        }

        file.close();

        if (contador > 0) {
            cout << ">>> " << contador << " reservas cargadas." << endl;
        }
    }

    // ========================================
    // MÉTODOS PARA MESAS (NUEVOS)
    // ========================================

    static void guardarMesas(const Lista<Mesa>& mesas, const string& archivo = "Data/mesas.txt") {
        ofstream file(archivo);

        if (!file.is_open()) {
            cout << ">>> Error: No se pudo abrir " << archivo << endl;
            return;
        }

        file << "# Mesas - Fridays Restaurant\n";
        file << "# Formato: NumeroMesa,Capacidad,Ubicacion,TipoMesa,Disponible\n";

        for (int i = 0; i < mesas.getTamaño(); i++) {
            Mesa m = mesas.obtenerEnPosicion(i);
            file << m.getNumeroMesa() << ","
                << m.getCapacidad() << ","
                << m.getUbicacion() << ","
                << m.getTipoMesa() << ","
                << (m.isDisponible() ? "1" : "0") << "\n";
        }

        file.close();
    }

    static Lista<Mesa> cargarMesas(const string& archivo = "Data/mesas.txt") {
        Lista<Mesa> mesas;
        ifstream file(archivo);

        if (!file.is_open()) {
            // No mostrar error si es primera vez
            return mesas;
        }

        string linea;
        int contador = 0;

        while (getline(file, linea)) {
            if (linea.empty() || linea[0] == '#') continue;

            try {
                stringstream ss(linea);
                int numeroMesa, capacidad, disponible;
                string ubicacion, tipoMesa;

                ss >> numeroMesa; ss.ignore();
                ss >> capacidad; ss.ignore();
                getline(ss, ubicacion, ',');
                getline(ss, tipoMesa, ',');
                ss >> disponible;

                Mesa mesa(numeroMesa, capacidad, ubicacion, tipoMesa);
                if (disponible == 0) mesa.ocupar();

                mesas.insertarFinal(mesa);
                contador++;
            }
            catch (...) {
                continue;
            }
        }

        file.close();

        if (contador > 0) {
            cout << ">>> " << contador << " mesas cargadas." << endl;
        }

        return mesas;
    }
};