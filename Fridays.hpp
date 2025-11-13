#pragma once
#include "Funcionalidades.hpp"
#include "Carta.hpp"
#include "CartaLocal.hpp"
#include "CartaDelivery.hpp"
#include "Cliente.hpp"
#include "Pedido.hpp"
#include "Pila.hpp"
#include "Cola.hpp"
#include "GestorPedidos.hpp"
#include "GestorArchivos.hpp"
#include "PagoEfectivo.hpp"
#include "PagoTarjeta.hpp"
#include "Boleta.hpp"
#include "Factura.hpp"
#include "GestorReserva.hpp"
using namespace UtilidadesConsola;

class Fridays {
private:
    Carta* menuPrincipal;
    string nombreRestaurante;
    string direccion;
    string telefono;
    Cola<Pedido*> colaPedidos;
    Pila<Pedido*> historialPedidos;
    int contadorPedidos = 1;
    int contadorClientes = 1;
    GestorReserva* gestorReservas;
    const string espacio = "                                          ";

public:
    Fridays(string nombre = "Fridays Restaurant",
        string dir = "Av. Principal 123",
        string tel = "555-1234")
        : nombreRestaurante(nombre), direccion(dir), telefono(tel) {
        menuPrincipal = new Carta();
        gestorReservas = new GestorReserva();
        cout << ">>> Sistema Fridays inicializado" << endl;
    }

    ~Fridays() {
        delete menuPrincipal;
        delete gestorReservas;
    }

    Carta* getMenu() {
        return menuPrincipal;
    }

    void ejecutar() {
        inicializarSistema();
        GestorPedidos::cargarHistorial(historialPedidos, contadorPedidos);
        int opcion;
        do {
            opcion = menuPrincipalUI();
            switch (opcion) {
            case 1: modoCliente(); break;
            case 2: modoAdministrador(); break;
            case 3:
                limpiarPantalla();
                cout << espacio << AMARILLO << "Gracias por usar Fridays Restaurant System!" << endl;
                GestorPedidos::guardarHistorial(historialPedidos);
                break;
            default:
                cout << espacio << ROJO_BRILLANTE << "Opcion invalida. Intente de nuevo." << endl;
                pausar();
                break;
            }
        } while (opcion != 3);
    }

    void inicializarSistema() {
        cout << ">>> Inicializando sistema Fridays..." << endl;
        cout << ">>> Sistema listo" << endl;
    }

    int menuPrincipalUI() {
        limpiarPantalla();
        mostrarLogoPrincipal();
        cout << "\n\n";
        cout << espacio << ROJO << "!BIENVENIDOS! \n" << endl;
        cout << espacio << AMARILLO << "Seleccione modo de acceso:\n";
        cout << espacio;
        imprimirSeparadorRojoBlanco(20);
        cout << ConsolaColor::BLANCO;
        cout << espacio << "1. CLIENTE \n";
        cout << espacio << "2. ADMINISTRADOR \n";
        cout << espacio << "3. Salir\n";
        cout << espacio;
        imprimirSeparadorRojoBlanco(20);
        cout << espacio << "Opcion: ";
        return leerOpcion();
    }

    void modoCliente() {
        limpiarPantalla();
        cout << ConsolaColor::ROJO;
        cout << "          TIPO DE SERVICIO" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << ConsolaColor::AMARILLO;
        cout << "\n  ->Como deseas usar Friday's?<-\n" << endl;
        cout << ConsolaColor::BLANCO;
        cout << "  1. HACER UNA RESERVA (Mesa para fecha/hora)" << endl;
        cout << "  2. COMER EN LOCAL (Salon Fridays)" << endl;
        cout << "  3. DELIVERY (Entrega a domicilio)" << endl;
        cout << "  4. Volver" << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "  Opcion: ";

        int tipoServicio = leerOpcion();

        if (tipoServicio == 1) {
            hacerReservaCliente();
            return;
        }

        if (tipoServicio == 4) return;

        procesarPedidoCliente(tipoServicio);
    }

    void hacerReservaCliente() {
        limpiarPantalla();
        cout << ROJO << "          HACER RESERVA" << endl;
        imprimirSeparadorRojoBlanco(20);

        Cliente* cliente = registrarCliente(false);

        string fecha, hora, observaciones;
        int numeroPersonas, numeroMesa;
        limpiarPantalla();
        cout << BLANCO;
        cout << ROJO << "DATOS DE LA RESERVA" << RESET << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "Fecha (DD/MM/YYYY): ";
        getline(cin, fecha);

        cout << "Hora (HH:MM): ";
        getline(cin, hora);

        cout << "Numero de personas: ";
        cin >> numeroPersonas;
        cin.ignore();

        gestorReservas->mostrarMesasDisponibles(numeroPersonas, fecha, hora);

        cout << "\nSeleccione numero de mesa: ";
        cin >> numeroMesa;
        cin.ignore();

        cout << "Observaciones (opcional): ";
        getline(cin, observaciones);

        gestorReservas->crearReserva(*cliente, numeroMesa, fecha, hora, numeroPersonas, observaciones);

        delete cliente;
        pausar();
    }

    void procesarPedidoCliente(int tipoServicio) {
        bool esDelivery = (tipoServicio == 3);
        string servicioTexto = esDelivery ? "delivery" : "local";

        Cliente* cliente = registrarCliente(esDelivery);

        Carta* carta = nullptr;
        if (esDelivery) {
            CartaDelivery* cartaDelivery = new CartaDelivery();
            cartaDelivery->cargarProductos(getMenu()->getProductos());
            carta = cartaDelivery;
        }
        else {
            CartaLocal* cartaLocal = new CartaLocal();
            cartaLocal->cargarProductos(getMenu()->getProductos());
            carta = cartaLocal;
        }

        Pedido* pedidoActual = new Pedido(contadorPedidos, cliente, servicioTexto);

        int opcion;
        do {
            opcion = mostrarMenuPedido(pedidoActual, carta, esDelivery);
            procesarOpcionPedido(opcion, pedidoActual, carta, esDelivery);
        } while (opcion != 7);

        delete carta;
    }

    int mostrarMenuPedido(Pedido* pedido, Carta* carta, bool esDelivery) {
        limpiarPantalla();
        cout << ROJO << "        MENU FRIDAYS - " << (esDelivery ? "DELIVERY" : "LOCAL") << RESET << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << endl;
        cout << "  1. Ver solo PLATOS PRINCIPALES" << endl;
        cout << "  2. Ver solo BEBIDAS" << endl;
        cout << "  3. Ver solo POSTRES" << endl;
        cout << endl;
        cout << "  4. Ver MENU COMPLETO" << endl;
        cout << "  5. Mi Carrito (" << pedido->getCantidadProductos() << " productos)" << endl;
        cout << "  6. Confirmar Pedido y Pagar" << endl;
        cout << "  7. Cancelar y Volver" << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << AMARILLO;
        cout << "Opcion: ";
        cout << BLANCO;
        return leerOpcion();
    }

    void procesarOpcionPedido(int opcion, Pedido* pedido, Carta* carta, bool esDelivery) {
        switch (opcion) {
        case 1:
        case 2:
        case 3:
        case 4:
            agregarProductoAlPedido(opcion, pedido, carta);
            break;
        case 5:
            verCarrito(pedido, esDelivery);
            break;
        case 6:
            confirmarYPagarPedido(pedido, esDelivery);
            break;
        case 7:
            delete pedido;
            break;
        }
    }

    void agregarProductoAlPedido(int opcion, Pedido* pedido, Carta* carta) {
        string categoria;
        switch (opcion) {
        case 1: categoria = "plato"; break;
        case 2: categoria = "bebida"; break;
        case 3: categoria = "postre"; break;
        case 4: categoria = "completo"; break;
        }

        mostrarMenuPorCategoria(carta, categoria);
        imprimirSeparadorRojoBlanco(40);
        cout << "\nDesea agregar un producto a su bolsa? (s/n): ";
        char resp;
        cin >> resp;
        cin.ignore(10000, '\n');

        if (resp == 's' || resp == 'S') {
            Producto* p = seleccionarProductoPorID(carta);
            if (p != nullptr) {
                pedido->agregarProducto(p);
                cout << VERDE << "Producto agregado al carrito!" << endl;
            }
            else {
                cout << "Producto no encontrado." << endl;
            }
        }
        pausar();
    }

    void verCarrito(Pedido* pedido, bool esDelivery) {
        limpiarPantalla();
        cout << ROJO << "            MI BOLSA" << endl;
        cout << RESET;
        imprimirSeparadorRojoBlanco(30);
        pedido->mostrarProductos();
        cout << "\nSubtotal: S/" << fixed << setprecision(2) << pedido->calcularTotal() << endl;
        if (esDelivery) {
            cout << "Tarifa delivery: S/8.90" << endl;
            cout << "Total: S/" << fixed << setprecision(2) << (pedido->calcularTotal() + 8.90) << endl;
        }
        else {
            cout << "Total: S/" << fixed << setprecision(2) << pedido->calcularTotal() << endl;
        }
        cout << endl;
        imprimirSeparadorRojoBlanco(30);
        cout << "\nDesea eliminar un producto? (s/n): ";
        char resp;
        cin >> resp;
        cin.ignore(10000, '\n');
        if (resp == 's' || resp == 'S') {
            if (pedido->estaVacio()) {
                cout << "Carrito vacio. No hay productos para eliminar." << endl;
            }
            else {
                int pos;
                cout << "Ingrese el numero del producto a eliminar: ";
                cin >> pos;
                cin.ignore(10000, '\n');
                if (!pedido->quitarProducto(pos - 1)) {
                    cout << "No se pudo eliminar el producto." << endl;
                }
            }
        }
        pausar();
    }

    void confirmarYPagarPedido(Pedido* pedido, bool esDelivery) {
        if (pedido->estaVacio()) {
            cout << "\nCarrito vacio. Agregue productos primero." << endl;
            pausar();
            return;
        }

        limpiarPantalla();
        cout << ROJO << "          CONFIRMAR PEDIDO" << endl;
        imprimirSeparadorRojoBlanco(20);
        pedido->mostrarResumen();
        imprimirSeparadorRojoBlanco(20);

        cout << AMARILLO << "\nDesea proceder al pago? (s/n): ";
        char confirmar;
        cin >> confirmar;
        cin.ignore(10000, '\n');

        if (confirmar == 's' || confirmar == 'S') {
            bool pagoExitoso = procesarPagoPedido(pedido, esDelivery);

            if (pagoExitoso) {
                colaPedidos.encolar(pedido);
                contadorPedidos++;
                cout << BLANCO;
                system("cls");
                cout << "\n Pedido #" << pedido->getNumeroPedido() << " confirmado!" << endl;
                cout << " Estado: En cola de cocina" << endl;
                cout << " Tiempo estimado: " << (esDelivery ? "45" : "20") << " minutos" << endl;
                pausar();
            }
            else {
                cout << "\nPago no completado. Puede volver a intentar." << endl;
                pausar();
            }
        }
    }

    Cliente* registrarCliente(bool esDelivery) {
        limpiarPantalla();
        cout << ROJO << "REGISTRO DE CLIENTE" << endl;
        imprimirSeparadorRojoBlanco(20);

        string nombre, apellido, telefono, dni, direccion = "";

        cout << "Ingrese nombre: ";
        getline(cin, nombre);

        cout << "Ingrese apellido: ";
        getline(cin, apellido);

        cout << "Ingrese DNI: ";
        getline(cin, dni);

        cout << "Ingrese telefono: ";
        getline(cin, telefono);

        if (esDelivery) {
            cout << "Ingrese direccion de entrega: ";
            getline(cin, direccion);
        }

        Cliente* cliente = new Cliente(contadorClientes++, nombre, apellido, telefono, dni, direccion);
        cout << endl;
        cout << VERDE << "Cliente registrado exitosamente!" << RESET << endl;
        pausar();

        return cliente;
    }

    void mostrarMenuPorCategoria(Carta* carta, const string& categoria) {
        limpiarPantalla();
        if (categoria == "completo") {
            carta->mostrarMenu();
        }
        else {
            carta->mostrarPorCategoria(categoria);
        }
    }

    Producto* seleccionarProductoPorID(Carta* carta) {
        int id;
        cout << "\nIngrese ID del producto: ";
        cin >> id;
        cin.ignore(10000, '\n');
        return carta->buscarPlato(id);
    }

    void modoAdministrador() {
        int opcion;
        do {
            opcion = mostrarMenuAdministrador();
            procesarOpcionAdministrador(opcion);
        } while (opcion != 5);
    }

    int mostrarMenuAdministrador() {
        limpiarPantalla();
        cout << ROJO << "            PANEL ADMINISTRADOR" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << BLANCO;
        cout << "  1. GESTION DE PEDIDOS" << endl;
        cout << "  2. GESTION DE MENU" << endl;
        cout << "  3. GESTION DE RESERVAS" << endl;
        cout << "  4. INFO DEL RESTAURANTE" << endl;
        cout << "  5. Volver" << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "Opcion: ";
        return leerOpcion();
    }

    void procesarOpcionAdministrador(int opcion) {
        switch (opcion) {
        case 1: menuGestionPedidos(); break;
        case 2: menuGestionMenu(); break;
        case 3: menuGestionReservas(); break;
        case 4:
            limpiarPantalla();
            pausar();
            break;
        case 5: break;
        default:
            cout << "\nOpcion invalida. Intente de nuevo." << endl;
            pausar();
            break;
        }
    }

    void menuGestionPedidos() {
        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        GESTION DE PEDIDOS" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << BLANCO;
            cout << "  1. Ver Cola de Pedidos" << endl;
            cout << "  2. Procesar pedido" << endl;
            cout << "  3. Ver Historial" << endl;
            cout << "  4. Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << "Opcion: ";
            opcion = leerOpcion();

            switch (opcion) {
            case 1: verColaPedidos(); break;
            case 2: procesarSiguientePedido(); break;
            case 3: verHistorialPedidos(); break;
            case 4: break;
            default:
                cout << "\nOpcion invalida." << endl;
                pausar();
                break;
            }
        } while (opcion != 4);
    }

    void menuGestionMenu() {
        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        GESTION DE MENU" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << BLANCO;
            cout << "  1. Ver menu" << endl;
            cout << "  2. Ordenar producto" << endl;
            cout << "  3. Agregar producto" << endl;
            cout << "  4. Eliminar producto" << endl;
            cout << "  5. Buscar producto" << endl;
            cout << "  6. Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << "Opcion: ";
            opcion = leerOpcion();

            switch (opcion) {
            case 1:
                limpiarPantalla();
                getMenu()->mostrarMenu();
                pausar();
                break;
            case 2: ordenarProductos(); break;
            case 3: agregarProducto(); break;
            case 4: eliminarProducto(); break;
            case 5: buscarProducto(); break;
            case 6: break;
            default:
                cout << "\nOpcion invalida." << endl;
                pausar();
                break;
            }
        } while (opcion != 6);
    }

    void menuGestionReservas() {
        int opcion;
        do {
            limpiarPantalla();
            cout << ROJO << "        GESTION DE RESERVAS" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << BLANCO;
            cout << "  1. Ver todas las reservas" << endl;
            cout << "  2. Buscar reserva por codigo" << endl;
            cout << "  3. Reservas del dia" << endl;
            cout << "  4. Cancelar reserva" << endl;
            cout << "  5. Estadisticas de reservas" << endl;
            cout << "  6. Ver todas las mesas" << endl;
            cout << "  7. Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << "Opcion: ";
            opcion = leerOpcion();

            switch (opcion) {
            case 1:
                limpiarPantalla();
                gestorReservas->mostrarTodasReservas();
                pausar();
                break;
            case 2: buscarReserva(); break;
            case 3: reservasDelDia(); break;
            case 4: cancelarReserva(); break;
            case 5:
                limpiarPantalla();
                gestorReservas->mostrarEstadisticas();
                pausar();
                break;
            case 6:
                limpiarPantalla();
                gestorReservas->listarTodasMesas();
                pausar();
                break;
            case 7: break;
            default:
                cout << "\nOpcion invalida." << endl;
                pausar();
                break;
            }
        } while (opcion != 7);
    }

    void verColaPedidos() {
        limpiarPantalla();
        cout << "COLA DE PEDIDOS PENDIENTES " << endl;
        imprimirSeparadorRojoBlanco(20);

        if (colaPedidos.estaVacia()) {
            cout << "\nNo hay pedidos pendientes." << endl;
        }
        else {
            cout << "\nPedidos en espera: " << colaPedidos.getTamaño() << "\n" << endl;
            for (int i = 0; i < colaPedidos.getTamaño(); i++) {
                Pedido* p = colaPedidos.obtenerEnPosicion(i);
                cout << "[" << (i + 1) << "] Pedido #" << p->getNumeroPedido()
                    << " - Cliente: " << p->getNombreCliente()
                    << " - Total: S/" << fixed << setprecision(2) << p->calcularTotal()
                    << " - Estado: " << p->getEstado()
                    << " - Tipo: " << p->getTipoServicio();

                if (p->getComprobante() != nullptr) {
                    cout << " - " << p->getComprobante()->getTipoComprobante();
                }
                cout << endl;
            }
        }
        pausar();
    }

    void procesarSiguientePedido() {
        limpiarPantalla();
        cout << "         PROCESAR SIGUIENTE PEDIDO" << endl;
        imprimirSeparadorRojoBlanco(20);

        if (colaPedidos.estaVacia()) {
            cout << "\nNo hay pedidos para procesar." << endl;
        }
        else {
            Pedido* pedido = colaPedidos.desencolar();
            cout << "\nProcesando pedido #" << pedido->getNumeroPedido() << "..." << endl;
            pedido->mostrarResumen();

            if (pedido->getComprobante() != nullptr) {
                cout << "\nCOMPROBANTE EMITIDO" << endl;
                pedido->getComprobante()->mostrarComprobante();
            }

            pedido->cambiarEstado("completado");
            historialPedidos.apilar(pedido);
            GestorPedidos::guardarHistorial(historialPedidos);

            cout << "\nPedido completado y movido al historial!" << endl;
        }
        pausar();
    }

    void verHistorialPedidos() {
        limpiarPantalla();
        cout << "       HISTORIAL DE PEDIDOS " << endl;
        imprimirSeparadorRojoBlanco(20);

        if (historialPedidos.estaVacia()) {
            cout << "\nNo hay pedidos en el historial." << endl;
        }
        else {
            cout << "\nPedidos completados: " << historialPedidos.getTamaño() << "\n" << endl;
            for (int i = 0; i < historialPedidos.getTamaño(); i++) {
                Pedido* p = historialPedidos.obtenerEnPosicion(i);
                cout << "[" << (i + 1) << "] Pedido #" << p->getNumeroPedido()
                    << " - Cliente: " << p->getNombreCliente()
                    << " - Total: S/" << fixed << setprecision(2) << p->calcularTotal()
                    << " - Estado: " << p->getEstado();

                if (p->getComprobante() != nullptr) {
                    cout << " - Comprobante: " << p->getComprobante()->getTipoComprobante();
                }
                cout << endl;
            }

            cout << "\nDesea ver el comprobante de algun pedido? (s/n): ";
            char ver;
            cin >> ver;
            cin.ignore(10000, '\n');

            if (ver == 's' || ver == 'S') {
                int numPedido;
                cout << "Ingrese numero de pedido [1-" << historialPedidos.getTamaño() << "]: ";
                cin >> numPedido;
                cin.ignore(10000, '\n');

                if (numPedido > 0 && numPedido <= historialPedidos.getTamaño()) {
                    Pedido* p = historialPedidos.obtenerEnPosicion(numPedido - 1);
                    if (p->getComprobante() != nullptr) {
                        limpiarPantalla();
                        p->getComprobante()->mostrarComprobante();
                    }
                    else {
                        cout << "Este pedido no tiene comprobante." << endl;
                    }
                }
            }
        }
        pausar();
    }

    void ordenarProductos() {
        limpiarPantalla();
        cout << "Ordenar productos por precio:\n";
        imprimirSeparadorRojoBlanco(20);
        cout << "1. Menor a mayor\n";
        cout << "2. Mayor a menor\n";
        cout << AMARILLO << "Opcion: ";
        int orden = leerOpcion();
        getMenu()->ordenarPorPrecio(orden == 1);
        cout << VERDE << "\nProductos ordenados exitosamente!" << RESET << endl;
        pausar();
    }

    void agregarProducto() {
        limpiarPantalla();
        cout << ROJO << "AGREGAR PRODUCTO" << endl;
        imprimirSeparadorRojoBlanco(20);

        int id;
        string nombre, categoria;
        double precio;

        cout << "\nIngrese ID del producto: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (getMenu()->buscarPlato(id) != nullptr) {
            cout << "\n Error: Ya existe un producto con ese ID" << endl;
            pausar();
            return;
        }

        cout << "Ingrese nombre del producto: ";
        getline(cin, nombre);

        cout << "Ingrese precio: S/";
        cin >> precio;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Ingrese categoria (plato/bebida/postre): ";
        getline(cin, categoria);

        Producto nuevo(id, nombre, precio, categoria, true);
        getMenu()->agregarAlMenu(nuevo);
        this->guardarDatos();

        cout << "\nProducto agregado exitosamente!" << endl;
        pausar();
    }

    void eliminarProducto() {
        limpiarPantalla();
        cout << ROJO << "ELIMINAR PRODUCTO          " << endl;
        imprimirSeparadorRojoBlanco(40);

        getMenu()->mostrarMenu();

        int id;
        cout << "\nIngrese ID del producto a eliminar (0 para cancelar): ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (id == 0) {
            cout << "Operacion cancelada." << endl;
            pausar();
            return;
        }

        Producto* producto = getMenu()->buscarPlato(id);
        if (producto == nullptr) {
            cout << "\n Error: Producto no encontrado" << endl;
            pausar();
            return;
        }

        cout << "\nProducto encontrado: " << producto->getNombre()
            << " - S/" << producto->getPrecio() << endl;
        cout << "¿Confirmar eliminacion? (s/n): ";
        char confirmar;
        cin >> confirmar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (confirmar == 's' || confirmar == 'S') {
            getMenu()->removerDelMenu(id);
            this->guardarDatos();
            cout << "\n Producto eliminado exitosamente!" << endl;
        }
        else {
            cout << "\nOperacion cancelada." << endl;
        }

        delete producto;
        pausar();
    }

    void buscarProducto() {
        limpiarPantalla();
        cout << ROJO << "BUSCAR PRODUCTO" << endl;
        imprimirSeparadorRojoBlanco(20);

        cout << AMARILLO << "\nBuscar por:" << endl;
        cout << BLANCO;
        cout << "1. ID " << endl;
        cout << "2. Nombre exacto " << endl;
        cout << "3. Nombre parcial " << endl;
        cout << "4. Categoria" << endl;
        cout << "5. Ver estadisticas de busqueda" << endl;
        cout << "Opcion: ";
        int opcionBusqueda = leerOpcion();

        switch (opcionBusqueda) {
        case 1:
            buscarProductoPorID();
            break;
        case 2:
            buscarProductoPorNombreExacto();
            break;
        case 3:
            buscarProductoPorNombreParcial();
            break;
        case 4:
            buscarProductoPorCategoria();
            break;
        case 5:
            limpiarPantalla();
            getMenu()->mostrarEstadisticasHashTable();
            break;
        default:
            cout << ROJO << "\n Opcion invalida" << RESET << endl;
        }
        pausar();
    }

    void buscarProductoPorID() {
        int id;
        cout << "\nIngrese ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Producto* p = getMenu()->buscarPlato(id);

        if (p != nullptr) {
            cout << VERDE << "\n Producto encontrado!" << RESET << endl;
            cout << "\nID: " << p->getId() << endl;
            cout << "Nombre: " << p->getNombre() << endl;
            cout << "Precio: S/" << fixed << setprecision(2) << p->getPrecio() << endl;
            cout << "Categoria: " << p->getCategoria() << endl;
            cout << "Disponible: " << (p->isDisponible() ? "Si" : "No") << endl;
            delete p;
        }
        else {
            cout << ROJO << "\n Producto no encontrado" << RESET << endl;
        }
    }

    void buscarProductoPorNombreExacto() {
        string nombre;
        cout << "\nIngrese nombre exacto del producto: ";
        getline(cin, nombre);

        Producto* p = getMenu()->buscarPorNombre(nombre);

        if (p != nullptr) {
            cout << VERDE << "\n Producto encontrado!" << RESET << endl;
            cout << "\nID: " << p->getId() << endl;
            cout << "Nombre: " << p->getNombre() << endl;
            cout << "Precio: S/" << fixed << setprecision(2) << p->getPrecio() << endl;
            cout << "Categoria: " << p->getCategoria() << endl;
            cout << "Disponible: " << (p->isDisponible() ? "Si" : "No") << endl;
            delete p;
        }
        else {
            cout << ROJO << "\n Producto no encontrado" << RESET << endl;
            cout << AMARILLO << "Consejo: Use busqueda parcial (opcion 3)" << RESET << endl;
        }
    }

    void buscarProductoPorNombreParcial() {
        string texto;
        cout << "\nIngrese parte del nombre: ";
        getline(cin, texto);

        ListaMenu<Producto> resultados = getMenu()->buscarPorNombreParcial(texto);

        if (!resultados.estaVacia()) {
            cout << VERDE << "\n>>> Resultados (" << resultados.getTamaño() << "):" << RESET << endl;
            imprimirSeparadorRojoBlanco(30);

            for (int i = 0; i < resultados.getTamaño(); i++) {
                Producto p = resultados.obtenerEnPosicion(i);
                cout << "ID: " << p.getId() << " - " << p.getNombre()
                    << " - S/" << fixed << setprecision(2) << p.getPrecio()
                    << " (" << p.getCategoria() << ")" << endl;
            }
            imprimirSeparadorRojoBlanco(30);
        }
        else {
            cout << ROJO << " No se encontraron productos" << RESET << endl;
        }
    }

    void buscarProductoPorCategoria() {
        string categoria;
        cout << "\nIngrese categoria (plato/bebida/postre): ";
        getline(cin, categoria);
        limpiarPantalla();
        getMenu()->mostrarPorCategoria(categoria);
    }

    void buscarReserva() {
        limpiarPantalla();
        cout << ROJO << "BUSCAR RESERVA" << endl;
        imprimirSeparadorRojoBlanco(20);
        string codigo;
        cout << "\nIngrese codigo de reserva: ";
        getline(cin, codigo);
        limpiarPantalla();
        gestorReservas->buscarReservaPorCodigo(codigo);
        pausar();
    }

    void reservasDelDia() {
        limpiarPantalla();
        cout << ROJO << "RESERVAS DEL DIA" << endl;
        imprimirSeparadorRojoBlanco(20);
        string fecha;
        cout << "\nIngrese fecha (DD/MM/YYYY): ";
        getline(cin, fecha);
        gestorReservas->listarReservasPorFecha(fecha);
        pausar();
    }

    void cancelarReserva() {
        limpiarPantalla();
        cout << ROJO << "CANCELAR RESERVA" << endl;
        imprimirSeparadorRojoBlanco(20);
        string codigo;
        cout << "\nIngrese codigo de reserva a cancelar: ";
        getline(cin, codigo);
        gestorReservas->cancelarReserva(codigo);
        pausar();
    }

    bool procesarPagoPedido(Pedido* pedido, bool esDelivery) {
        double totalPedido = pedido->calcularTotal();
        double tarifaDelivery = esDelivery ? 8.90 : 0.0;
        double totalFinal = totalPedido + tarifaDelivery;
        bool pagoExitoso = false;

        limpiarPantalla();
        cout << ROJO << "                 PROCESAR PAGO" << endl;
        imprimirSeparadorRojoBlanco(30);

        pedido->mostrarProductos();

        cout << "SUBTOTAL:" << setw(20) << right << "S/" << fixed << setprecision(2) << setw(10) << totalPedido << endl;
        if (esDelivery) {
            cout << "DELIVERY:" << setw(20) << right << "S/" << setw(10) << tarifaDelivery << endl;
        }

        cout << endl;
        cout << VERDE << "TOTAL A PAGAR:" << setw(15) << right << "S/" << setw(10) << totalFinal << endl;
        cout << RESET;
        imprimirSeparadorRojoBlanco(30);

        cout << AMARILLO << "\nMETODOS DE PAGO DISPONIBLES";
        cout << RESET << endl;
        cout << "1. Efectivo" << endl;
        cout << "2. Tarjeta de Credito" << endl;
        cout << "3. Tarjeta de Debito" << endl;
        cout << "0. Cancelar pago" << endl;
        cout << "---------------------------" << endl;
        cout << AMARILLO << "Seleccione metodo de pago: ";
        cout << BLANCO;

        int metodoPago = leerOpcion();

        if (metodoPago == 0) {
            cout << "\n Pago cancelado. El pedido no ha sido confirmado." << endl;
            pausar();
            return false;
        }

        string fecha = obtenerFechaActual();

        switch (metodoPago) {
        case 1:
            pagoExitoso = procesarPagoEfectivo(totalFinal, fecha);
            break;
        case 2:
        case 3:
            pagoExitoso = procesarPagoTarjeta(totalFinal, fecha, metodoPago == 2 ? "Credito" : "Debito");
            break;
        default:
            cout << "\n Opción invalida" << endl;
            pausar();
            return false;
        }

        if (pagoExitoso) {
            pedido->cambiarEstado("pagado");
            pausar();

            Comprobante* comprobante = generarComprobante(pedido, totalFinal, fecha);
            pedido->setComprobante(comprobante);

            limpiarPantalla();
            comprobante->mostrarComprobante();
            pausar();
            limpiarPantalla();
        }

        pausar();
        return pagoExitoso;
    }

    bool procesarPagoEfectivo(double totalFinal, string fecha) {
        limpiarPantalla();
        cout << ROJO << "PAGO EN EFECTIVO" << endl;
        imprimirSeparadorRojoBlanco(20);
        double montoPagado;
        cout << "Total a Pagar: S/" << fixed << setprecision(2) << totalFinal << endl;
        cout << "\nIngrese monto con el que paga: S/";
        cin >> montoPagado;
        cin.ignore(10000, '\n');

        PagoEfectivo pago(totalFinal, fecha, montoPagado);
        cout << "\n>>> Procesando pago en efectivo..." << endl;
        _sleep(1000);
        limpiarPantalla();

        string resultado = pago.procesarPago();
        cout << resultado << endl;

        if (montoPagado >= totalFinal) {
            pago.mostrarDetalle();
            return true;
        }
        else {
            cout << "\n PAGO RECHAZADO - Monto insuficiente" << endl;
            cout << "   Falta: S/" << fixed << setprecision(2) << (totalFinal - montoPagado) << endl;
            return false;
        }
    }

    bool procesarPagoTarjeta(double totalFinal, string fecha, string tipoTarjeta) {
        string numeroTarjeta, banco;
        system("cls");
        cout << ROJO << "PAGO CON TARJETA DE " << tipoTarjeta << endl;
        imprimirSeparadorRojoBlanco(30);
        cout << "\nIngrese numero de tarjeta (16 digitos): ";
        getline(cin, numeroTarjeta);

        cout << "Ingrese nombre del banco: ";
        getline(cin, banco);

        if (numeroTarjeta.length() < 16) {
            cout << "\n ERROR: Numero de tarjeta invalido (minimo 16 digitos)" << endl;
            pausar();
            return false;
        }

        PagoTarjeta pago(totalFinal, fecha, numeroTarjeta, tipoTarjeta, banco);
        cout << endl;

        cout << ">>>Procesando pago con tarjeta de " << tipoTarjeta << "..." << endl;
        _sleep(1500);
        cout << ">>>Conectando con el banco: " << banco << "..." << endl;
        _sleep(1500);
        cout << ">>>Verificando datos de la tarjeta..." << endl;
        _sleep(2000);

        string resultado = pago.procesarPago();
        cout << "\n>>> " << resultado << endl;
        _sleep(1000);

        if (resultado.find("ERROR") == string::npos && numeroTarjeta.length() >= 16) {
            limpiarPantalla();
            cout << VERDE << "\n PAGO PROCESADO EXITOSAMENTE " << RESET << endl;
            pago.mostrarDetalle();
            return true;
        }
        else {
            cout << ROJO << "\n PAGO RECHAZADO - Por favor, intente con otro metodo." << RESET << endl;
            return false;
        }
    }

    Comprobante* generarComprobante(Pedido* pedido, double totalFinal, string fecha) {
        limpiarPantalla();
        cout << BLANCO;
        cout << ROJO << "              COMPROBANTE" << RESET << endl;
        imprimirSeparadorRojoBlanco(20);

        cout << "1.BOLETA" << endl;
        cout << "2.FACTURA" << endl;

        imprimirSeparadorRojoBlanco(20);

        cout << AMARILLO << "\nSeleccione su tipo de comprobante: " << RESET;

        int tipoComprobante = leerOpcion();
        Comprobante* comprobante = nullptr;

        if (tipoComprobante == 1) {
            Cliente* cliente = pedido->getCliente();
            comprobante = new Boleta(
                fecha,
                totalFinal,
                cliente->getNombreCompleto(),
                cliente->getDni(),
                pedido->getProductos()
            );
        }
        else if (tipoComprobante == 2) {
            string ruc, razonSocial;

            limpiarPantalla();
            cout << "\nDATOS PARA FACTURA " << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << "Ingrese RUC (11 digitos): ";
            getline(cin, ruc);

            cout << "Ingrese Razon Social: ";
            getline(cin, razonSocial);

            if (ruc.length() != 11) {
                cout << "\n ERROR: El RUC debe tener 11 digitos" << endl;
                cout << " Se generara una Boleta en su lugar" << endl;
                pausar();

                Cliente* cliente = pedido->getCliente();
                comprobante = new Boleta(
                    fecha,
                    totalFinal,
                    cliente->getNombreCompleto(),
                    cliente->getDni(),
                    pedido->getProductos()
                );
            }
            else {
                comprobante = new Factura(
                    fecha,
                    totalFinal,
                    razonSocial,
                    ruc,
                    pedido->getProductos()
                );

                cout << "\n Factura generada para: " << razonSocial << endl;
                cout << " RUC: " << ruc << endl;
            }
        }
        else {
            cout << "\n Opcion invalida. Se generara Boleta por defecto." << endl;
            Cliente* cliente = pedido->getCliente();
            comprobante = new Boleta(
                fecha,
                totalFinal,
                cliente->getNombreCompleto(),
                cliente->getDni(),
                pedido->getProductos()
            );
        }

        pausar();
        return comprobante;
    }

    void guardarDatos() {
        menuPrincipal->guardarEnArchivo();
        cout << VERDE << "Datos guardados exitosamente" << endl;
    }

    void cargarDatos() {
        menuPrincipal->cargarDesdeArchivo();
    }

    string obtenerFechaActual() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        return string(buffer);
    }

    void limpiarPantalla() {
        system("cls");
    }

    void pausar() {
        cout << ROJO << "Presione Enter para continuar...";
        cin.ignore(10000, '\n');
    }

    int leerOpcion() {
        int opcion;
        while (true) {
            string entrada;
            getline(cin, entrada);
            try {
                opcion = stoi(entrada);
                break;
            }
            catch (const exception& e) {
                cout << "Entrada invalida. Ingrese un numero: ";
            }
        }
        return opcion;
    }

    string getNombreRestaurante() const { return nombreRestaurante; }
    string getDireccion() const { return direccion; }
    string getTelefono() const { return telefono; }
    void setNombreRestaurante(const string& nombre) { nombreRestaurante = nombre; }
    void setDireccion(const string& dir) { direccion = dir; }
    void setTelefono(const string& tel) { telefono = tel; }
};