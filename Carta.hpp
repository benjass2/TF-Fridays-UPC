#pragma once
#include "ListaMenu.hpp"
#include "Producto.hpp"
#include "GestorArchivos.hpp"
#include "HashTable.hpp"
#include "Funcionalidades.hpp"
using namespace UtilidadesConsola;

class Carta {
protected:
    ListaMenu<Producto> productos;
    string archivoMenu;
    string tipoServicio;
    string nombreCarta;
    HashTable<int, Producto*> indicePorID;
    HashTable<string, Producto*> indicePorNombre;

public:

    Carta(string tipo = "general", string nombre = "Menu Principal", string archivo = "Data/menu.txt")
        : tipoServicio(tipo), nombreCarta(nombre), archivoMenu(archivo) {
        cargarDesdeArchivo();
        actualizarIndices();
    }

    void actualizarIndices() {

        vector<Producto*> valoresAnteriores = indicePorID.obtenerValores();

        for (Producto* prod : valoresAnteriores) {
            if (prod != nullptr) {
                delete prod;
            }
        }


        indicePorID.limpiar();
        indicePorNombre.limpiar();


        for (int i = 0; i < productos.getTama�o(); i++) {

            Producto p = productos.obtenerEnPosicion(i);


            Producto* nuevaCopiaHeap = new Producto(
                p.getId(), p.getNombre(), p.getPrecio(),
                p.getCategoria(), p.isDisponible()
            );


            indicePorID.insertar(nuevaCopiaHeap->getId(), nuevaCopiaHeap);
            indicePorNombre.insertar(nuevaCopiaHeap->getNombre(), nuevaCopiaHeap);
        }

        cout << ">>> Indices (Hash Tables) actualizados y sincronizados." << endl;


    }

    virtual ~Carta() {
        guardarEnArchivo();
    }

    void agregarAlMenu(const Producto& producto) {
        // Verificar si ya existe
        if (indicePorID.contiene(producto.getId())) {
            cout << ">>> Error: Ya existe un producto con ID " << producto.getId() << endl;
            return;
        }

        productos.insertarFinal(producto);

        // Actualizar �ndices
        Producto* copia = new Producto(producto.getId(), producto.getNombre(),
            producto.getPrecio(), producto.getCategoria(),
            producto.isDisponible());
        indicePorID.insertar(producto.getId(), copia);
        indicePorNombre.insertar(producto.getNombre(), copia);

        cout << ">>> Producto agregado al menu." << endl;
    }


    bool removerDelMenu(int id) {
        if (!indicePorID.contiene(id)) {
            cout << ">>> Producto no encontrado en el menu." << endl;
            return false;
        }

        Producto temp(id, "", 0);
        bool eliminado = productos.eliminar(temp);

        if (eliminado) {
            actualizarIndices();  // Reconstruir �ndices
            cout << ">>> Producto removido del menu." << endl;
            return true;
        }

        return false;
    }

    // Modificar producto existente
    bool modificarProducto(int id, const string& nuevoNombre, double nuevoPrecio,
                          const string& nuevaCategoria, bool nuevaDisponibilidad) {
        if (!indicePorID.contiene(id)) {
            cout << ">>> Producto no encontrado en el menu." << endl;
            return false;
        }

        // Buscar y modificar en la lista
        bool encontrado = false;
        for (int i = 0; i < productos.getTama�o(); i++) {
            Producto& p = productos.obtenerReferencia(i);
            if (p.getId() == id) {
                // Actualizar los datos del producto
                p.setNombre(nuevoNombre);
                p.setPrecio(nuevoPrecio);
                p.setCategoria(nuevaCategoria);
                p.setDisponible(nuevaDisponibilidad);
                encontrado = true;
                break;
            }
        }

        if (encontrado) {
            actualizarIndices();  // Reconstruir �ndices con los nuevos datos
            cout << ">>> Producto modificado exitosamente." << endl;
            return true;
        }

        return false;
    }

    // B�squeda O(1) usando HashTable
    Producto* buscarPlato(int id) {
        Producto** resultado = indicePorID.buscar(id);
        if (resultado != nullptr) {
            Producto* prod = *resultado;
            Producto* copia = new Producto(prod->getId(), prod->getNombre(),
                prod->getPrecio(), prod->getCategoria(),
                prod->isDisponible());
            delete resultado;
            return copia;
        }
        return nullptr;
    }

    // B�squeda por nombre exacto - O(1)
    Producto* buscarPorNombre(const string& nombre) {
        Producto** resultado = indicePorNombre.buscar(nombre);
        if (resultado != nullptr) {
            Producto* prod = *resultado;
            Producto* copia = new Producto(prod->getId(), prod->getNombre(),
                prod->getPrecio(), prod->getCategoria(),
                prod->isDisponible());
            delete resultado;
            return copia;
        }
        return nullptr;
    }

    // B�squeda parcial (cuando no sabes el nombre completo)
    ListaMenu<Producto> buscarPorNombreParcial(const string& texto) {
        ListaMenu<Producto> resultados;

        for (int i = 0; i < productos.getTama�o(); i++) {
            Producto p = productos.obtenerEnPosicion(i);
            string nombre = p.getNombre();

            // Convertir a min�sculas para b�squeda case-insensitive
            for (size_t j = 0; j < nombre.length(); j++) {
                if (nombre[j] >= 'A' && nombre[j] <= 'Z') {
                    nombre[j] = nombre[j] + ('a' - 'A');
                }
            }

            string busqueda = texto;
            for (size_t j = 0; j < busqueda.length(); j++) {
                if (busqueda[j] >= 'A' && busqueda[j] <= 'Z') {
                    busqueda[j] = busqueda[j] + ('a' - 'A');
                }
            }

            if (nombre.find(busqueda) != string::npos) {
                resultados.insertarFinal(p);
            }
        }

        return resultados;
    }

    // Mostrar estad�sticas de rendimiento
    void mostrarEstadisticasHashTable() const {
        cout << "\n=== ESTADISTICAS DE BUSQUEDA ===" << endl;
        cout << "Indice por ID:" << endl;
        indicePorID.mostrarEstadisticas();
        cout << "\nIndice por Nombre:" << endl;
        indicePorNombre.mostrarEstadisticas();
    }
    virtual void mostrarMenu() const {
        cout << "\n";

        cout << "                          " << nombreCarta << "                        " << endl;
        cout << "                          Servicio: " << tipoServicio << "                        " << endl;
        imprimirSeparadorRojoBlanco(40);

        if (productos.estaVacia()) {
            cout << ">>> El menu esta vacio." << endl;
            return;
        }

        cout << left << setw(4) << "ID"
            << setw(35) << "PRODUCTO"
            << setw(10) << "PRECIO"
            << setw(12) << "CATEGORIA"
            << "ESTADO" << endl;
        imprimirSeparadorRojoBlanco(40);

        for (int i = 0; i < productos.getTama�o(); i++) {
            cout << productos.obtenerEnPosicion(i) << endl;
        }
        cout << endl;
        imprimirSeparadorRojoBlanco(40);
        cout << " >>> Total de productos : " << productos.getTama�o() << endl;

    }


    void mostrarPorCategoria(const string& categoria) const {
        auto productosFiltrados = productos.filtrar([categoria](const Producto& p) {

            return p.getCategoria() == categoria && p.isDisponible();
            });

        cout << "\n>>> Productos en categoria: " << categoria << endl;

        imprimirSeparadorRojoBlanco(40);


        if (productosFiltrados.estaVacia()) {
            cout << ">>> No hay productos en esta categoria o estan agotados." << endl;
            return;
        }


        cout << left << setw(4) << "ID"
            << setw(35) << "PRODUCTO"
            << setw(10) << "PRECIO"
            << "ESTADO" << endl;
        imprimirSeparadorRojoBlanco(40);



        for (int i = 0; i < productosFiltrados.getTama�o(); i++) {
            const Producto& p = productosFiltrados.obtenerEnPosicion(i);


            cout << left << setw(4) << p.getId()
                << setw(35) << p.getNombre()
                << "S/" << right << setw(6) << fixed << setprecision(2) << p.getPrecio()
                << "    " << (p.isDisponible() ? "V" : "X")
                << endl;
        }
        imprimirSeparadorRojoBlanco(40);
        cout << ">>> Total de productos en la categoria: " << productosFiltrados.getTama�o() << endl;
    }


    void ordenarPorPrecio(bool ascendente = true) {
        productos.ordenar([ascendente](const Producto& a, const Producto& b) {
            return ascendente ? a.getPrecio() < b.getPrecio() : a.getPrecio() > b.getPrecio();
            });
        actualizarIndices();
        cout << ">>> Productos ordenados por precio "
            << (ascendente ? "(menor a mayor)" : "(mayor a menor)") << endl;
    }

    // Filtrar productos con lambda
    ListaMenu<Producto> filtrarProductos(function<bool(const Producto&)> condicion) const {
        return productos.filtrar(condicion);
    }

    // Guardar men� en archivo
    void guardarEnArchivo() const {
        GestorArchivos::guardarProductos(productos, archivoMenu);
    }

    // Cargar men� desde archivo
    void cargarDesdeArchivo() {
        productos = GestorArchivos::cargarProductos(archivoMenu);
    }




    int getTotalProductos() const { return productos.getTama�o(); }
    bool estaVacio() const { return productos.estaVacia(); }
    string getTipoServicio() const { return tipoServicio; }
    string getNombreCarta() const { return nombreCarta; }

    // Obtener lista completa de productos 
    const ListaMenu<Producto>& getProductos() const { return productos; }

    // Cargar productos desde otra lista (para CartaLocal/Delivery)
    virtual void cargarProductos(const ListaMenu<Producto>& productosExternos) {
        productos = productosExternos;
    }


    void setTipoServicio(const string& tipo) { tipoServicio = tipo; }
    void setNombreCarta(const string& nombre) { nombreCarta = nombre; }
};

