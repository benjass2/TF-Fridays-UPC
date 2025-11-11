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
    GestorReserva* gestorReservas;  // ← NUEVO

public:

    Fridays(string nombre = "Fridays Restaurant",
        string dir = "Av. Principal 123",
        string tel = "555-1234")
        : nombreRestaurante(nombre), direccion(dir), telefono(tel) {

        menuPrincipal = new Carta();
        gestorReservas = new GestorReserva();

        cout << ">>> Sistema Fridays inicializado" << endl;
    }

    // Destructor
    ~Fridays() {
        delete menuPrincipal;
        delete gestorReservas;
    }

    // Obtener el menú principal
    Carta* getMenu() {
        return menuPrincipal;
    }

    const string espacio = "                                          ";

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


    // Inicializar sistema con datos predeterminados
    void inicializarSistema() {
        cout << ">>> Inicializando sistema Fridays..." << endl;
        cout << ">>> Sistema listo" << endl;
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



    // ========================================
    // MODO CLIENTE (MODIFICADO)
    // ========================================
    void modoCliente() {
        limpiarPantalla();
        cout << ConsolaColor::ROJO;
        cout << "          TIPO DE SERVICIO" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << ConsolaColor::AMARILLO;
        cout << "\n  ->Como deseas usar Friday's?<-\n" << endl;
        cout << ConsolaColor::BLANCO;
        cout << "  1. HACER UNA RESERVA (Mesa para fecha/hora)" << endl;  // ← NUEVO
        cout << "  2. COMER EN LOCAL (Salon Fridays)" << endl;
        cout << "  3. DELIVERY (Entrega a domicilio)" << endl;
        cout << "  4. Volver" << endl;
        cout << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "  Opcion: ";

        int tipoServicio = leerOpcion();

        // ← NUEVO: Manejar opción de reserva
        if (tipoServicio == 1) {
            hacerReservaCliente();
            return;
        }

        if (tipoServicio == 4) return;  // ← Cambió de 3 a 4

        bool esDelivery = (tipoServicio == 3);  // ← Cambió de 2 a 3
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
            cout << "  5. Mi Carrito (" << pedidoActual->getCantidadProductos() << " productos)" << endl;
            cout << "  6. Confirmar Pedido y Pagar" << endl;
            cout << "  7. Cancelar y Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << AMARILLO;
            cout << "Opcion: ";
            cout << BLANCO;

            opcion = leerOpcion();

            switch (opcion) {
            case 1:
            case 2:
            case 3:
            case 4: {
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
                        pedidoActual->agregarProducto(p);
                        cout << VERDE << "Producto agregado al carrito!" << endl;
                    }
                    else {
                        cout << "Producto no encontrado." << endl;
                    }
                }
                pausar();
                break;
            }

            case 5:
                limpiarPantalla();
                cout << ROJO << "            MI BOLSA" << endl; cout << RESET;
                imprimirSeparadorRojoBlanco(30);
                pedidoActual->mostrarProductos();
                cout << "\nSubtotal: S/" << fixed << setprecision(2) << pedidoActual->calcularTotal() << endl;
                if (esDelivery) {
                    cout << "Tarifa delivery: S/8.90" << endl;
                    cout << "Total: S/" << fixed << setprecision(2) << (pedidoActual->calcularTotal() + 8.90) << endl;
                }
                else {
                    cout << "Total: S/" << fixed << setprecision(2) << pedidoActual->calcularTotal() << endl;
                }
                cout << endl;
                imprimirSeparadorRojoBlanco(30);
                cout << "\nDesea eliminar un producto? (s/n): ";
                {
                    char resp;
                    cin >> resp;
                    cin.ignore(10000, '\n');
                    if (resp == 's' || resp == 'S') {
                        if (pedidoActual->estaVacio()) {
                            cout << "Carrito vacio. No hay productos para eliminar." << endl;
                        }
                        else {
                            int pos;
                            cout << "Ingrese el numero del producto a eliminar: ";
                            cin >> pos;
                            cin.ignore(10000, '\n');
                            if (!pedidoActual->quitarProducto(pos - 1)) {
                                cout << "No se pudo eliminar el producto." << endl;
                            }
                        }
                    }
                }
                pausar();
                break;

            case 6:
                if (pedidoActual->estaVacio()) {
                    cout << "\nCarrito vacio. Agregue productos primero." << endl;
                    pausar();
                }
                else {
                    limpiarPantalla();
                    cout << ROJO << "          CONFIRMAR PEDIDO" << endl;
                    imprimirSeparadorRojoBlanco(20);
                    pedidoActual->mostrarResumen();
                    imprimirSeparadorRojoBlanco(20);

                    cout << AMARILLO << "\nDesea proceder al pago? (s/n): ";
                    char confirmar;
                    cin >> confirmar;
                    cin.ignore(10000, '\n');

                    if (confirmar == 's' || confirmar == 'S') {
                        bool pagoExitoso = procesarPagoPedido(pedidoActual, esDelivery);

                        if (pagoExitoso) {
                            colaPedidos.encolar(pedidoActual);
                            contadorPedidos++;
                            cout << BLANCO;
                            system("cls");
                            cout << "\n Pedido #" << pedidoActual->getNumeroPedido() << " confirmado!" << endl;
                            cout << " Estado: En cola de cocina" << endl;
                            cout << " Tiempo estimado: " << (esDelivery ? "45" : "20") << " minutos" << endl;
                            pausar();

                            delete carta;
                            return;
                        }
                        else {
                            cout << "\nPago no completado. Puede volver a intentar." << endl;
                            pausar();
                        }
                    }
                }
                break;

            case 7:
                delete pedidoActual;
                delete cliente;
                delete carta;
                return;
            }

        } while (opcion != 7);
    }

    //realizar reserva en cliente
    void hacerReservaCliente() {
        limpiarPantalla();
        cout << ROJO << "          HACER RESERVA" << endl;
        imprimirSeparadorRojoBlanco(20);

        // Registrar cliente
        Cliente* cliente = registrarCliente(false);

        // Datos de reserva
        string fecha, hora, observaciones;
        int numeroPersonas, numeroMesa;
        limpiarPantalla();
        cout << BLANCO;
        cout << "\nDATOS DE LA RESERVA" << endl;
        imprimirSeparadorRojoBlanco(20);
        cout << "Fecha (DD/MM/YYYY): ";
        getline(cin, fecha);

        cout << "Hora (HH:MM): ";
        getline(cin, hora);

        cout << "Numero de personas: ";
        cin >> numeroPersonas;
        cin.ignore();

        // Mostrar mesas disponibles
        gestorReservas->mostrarMesasDisponibles(numeroPersonas, fecha, hora);

        cout << "\nSeleccione numero de mesa: ";
        cin >> numeroMesa;
        cin.ignore();

        cout << "Observaciones (opcional): ";
        getline(cin, observaciones);

        // Crear reserva
        gestorReservas->crearReserva(*cliente, numeroMesa, fecha, hora, numeroPersonas, observaciones);

        delete cliente;
        pausar();
    }

    //modo administrador
    void modoAdministrador() {
        int opcion;

        do {
            limpiarPantalla();

            cout << ROJO << "            PANEL ADMINISTRADOR" << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << AMARILLO << "\n   GESTION DE PEDIDOS " << endl;
            cout << BLANCO;
            cout << "  1. Ver Cola de Pedidos" << endl;
            cout << "  2. Procesar pedido" << endl;
            cout << "  3. Ver Historial" << endl;
            cout << AMARILLO << "\n   GESTION DE MENU " << endl;
            cout << BLANCO;
            cout << "  4. Ver menu " << endl;
            cout << "  5. Ordenar producto" << endl;
            cout << "  6. Agregar producto" << endl;
            cout << "  7. Eliminar producto" << endl;
            cout << "  8. Buscar producto" << endl;
            cout << AMARILLO << "\n   GESTION DE RESERVAS " << RESET << endl;  // ← NUEVO
            cout << BLANCO;
            cout << "  9. Ver todas las reservas" << endl;
            cout << "  10. Buscar reserva por codigo" << endl;
            cout << "  11. Reservas del dia" << endl;
            cout << "  12. Cancelar reserva" << endl;
            cout << "  13. Estadisticas de reservas" << endl;
            cout << "  14. Ver todas las mesas" << endl;
            cout << AMARILLO << "\n   SISTEMA " << endl;
            cout << BLANCO;
            cout << "  15. Info del restaurante" << endl;
            cout << "  16. Volver" << endl;
            cout << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << "Opcion: ";

            opcion = leerOpcion();

            switch (opcion) {
            case 1:
                verColaPedidos();
                break;
            case 2:
                procesarSiguientePedido();
                break;
            case 3:
                verHistorialPedidos();
                break;
            case 4:
                limpiarPantalla();
                getMenu()->mostrarMenu();
                pausar();
                break;
            case 5:
            {
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
            break;
            case 6:  // AGREGAR PRODUCTO
            {
                limpiarPantalla();

                cout << ROJO << "AGREGAR PRODUCTO" << endl;
                imprimirSeparadorRojoBlanco(20);

                int id;
                string nombre, categoria;
                double precio;

                cout << "\nIngrese ID del producto: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Verificar si el ID ya existe
                if (getMenu()->buscarPlato(id) != nullptr) {
                    cout << "\n Error: Ya existe un producto con ese ID" << endl;
                    pausar();
                    break;
                }

                cout << "Ingrese nombre del producto: ";
                getline(cin, nombre);

                cout << "Ingrese precio: S/";
                cin >> precio;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Ingrese categoria (plato/bebida/postre): ";
                getline(cin, categoria);

                // Crear y agregar producto
                Producto nuevo(id, nombre, precio, categoria, true);
                getMenu()->agregarAlMenu(nuevo);
                this->guardarDatos();

                cout << "\nProducto agregado exitosamente!" << endl;
                pausar();
            }
            break;
            case 7:  // ELIMINAR PRODUCTO
            {
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
                    break;
                }

                Producto* producto = getMenu()->buscarPlato(id);
                if (producto == nullptr) {
                    cout << "\n Error: Producto no encontrado" << endl;
                    pausar();
                    break;
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

                delete producto;  // Liberar memoria
                pausar();
            }
            break;
            case 8:  // BUSCAR PRODUCTO CON HASHTABLE
            {
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
                case 1:  // Buscar por ID - O(1)
                {
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
                break;

                case 2:  // Buscar por nombre exacto - O(1)
                {
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
                break;

                case 3:  // Buscar por nombre parcial
                {
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
                break;

                case 4:  // Buscar por categoría
                {
                    string categoria;
                    cout << "\nIngrese categoria (plato/bebida/postre): ";
                    getline(cin, categoria);
                    limpiarPantalla();
                    getMenu()->mostrarPorCategoria(categoria);
                }
                break;

                case 5:  // Estadísticas
                {
                    limpiarPantalla();
                    getMenu()->mostrarEstadisticasHashTable();
                }
                break;

                default:
                    cout << ROJO << "\n Opcion invalida" << RESET << endl;
                }

                pausar();
            }

            break;


            //    // ========================================
            //    // NUEVOS CASES: GESTIÓN DE RESERVAS
            //    // ========================================
            case 9: { // Ver todas las reservas
                limpiarPantalla();
                gestorReservas->mostrarTodasReservas();
                pausar();
                break;
            }

            case 10: { // Buscar reserva
                limpiarPantalla();
                cout << ROJO << "BUSCAR RESERVA" << endl;
                imprimirSeparadorRojoBlanco(20);
                string codigo;
                cout << "\nIngrese codigo de reserva: ";
                getline(cin, codigo);
                gestorReservas->buscarReservaPorCodigo(codigo);
                pausar();
                break;
            }

            case 11: { // Reservas del día
                limpiarPantalla();
                cout << ROJO << "RESERVAS DEL DIA" << endl;
                imprimirSeparadorRojoBlanco(20);
                string fecha;
                cout << "\nIngrese fecha (DD/MM/YYYY): ";
                getline(cin, fecha);
                gestorReservas->listarReservasPorFecha(fecha);
                pausar();
                break;
            }

            case 12: { // Cancelar reserva
                limpiarPantalla();
                cout << ROJO << "CANCELAR RESERVA" << endl;
                imprimirSeparadorRojoBlanco(20);
                string codigo;
                cout << "\nIngrese codigo de reserva a cancelar: ";
                getline(cin, codigo);
                gestorReservas->cancelarReserva(codigo);
                pausar();
                break;
            }

            case 13: { // Estadísticas
                limpiarPantalla();
                gestorReservas->mostrarEstadisticas();
                pausar();
                break;
            }

            case 14: { // Ver todas las mesas
                limpiarPantalla();
                gestorReservas->listarTodasMesas();
                pausar();
                break;
            }

            case 15:  // INFO DEL RESTAURANTE
                limpiarPantalla();

                pausar();
                break;
            case 16:  // VOLVER
                break;

            default:
                cout << "\nOpcion invalida. Intente de nuevo." << endl;
                pausar();
                break;
            }

        } while (opcion != 16);
    }

    bool procesarPagoPedido(Pedido* pedido, bool esDelivery) {
        // 1. CÁLCULOS INICIALES
        double totalPedido = pedido->calcularTotal();
        double tarifaDelivery = esDelivery ? 8.90 : 0.0;
        double totalFinal = totalPedido + tarifaDelivery;
        bool pagoExitoso = false;

        // 2. MOSTRAR RESUMEN DEL PEDIDO EN CONSOLA
        limpiarPantalla();

        cout << ROJO << "                 PROCESAR PAGO" << endl;
        imprimirSeparadorRojoBlanco(30);


        pedido->mostrarProductos();



        // Resumen de la cuenta (usando setw para alinear mejor)
        cout << "SUBTOTAL:" << setw(20) << right << "S/" << fixed << setprecision(2) << setw(10) << totalPedido << endl;
        if (esDelivery) {
            cout << "DELIVERY:" << setw(20) << right << "S/" << setw(10) << tarifaDelivery << endl;
        }

        cout << endl;
        cout << VERDE << "TOTAL A PAGAR:" << setw(15) << right << "S/" << setw(10) << totalFinal << endl;
        cout << RESET;
        imprimirSeparadorRojoBlanco(30);

        // 3. SELECCIÓN DEL MÉTODO DE PAGO
        cout << AMARILLO << "\nMETODOS DE PAGO DISPONIBLES"; cout << RESET << endl;
        cout << "1. Efectivo" << endl;
        cout << "2. Tarjeta de Credito" << endl;
        cout << "3. Tarjeta de Debito" << endl;
        cout << "0. Cancelar pago" << endl;
        cout << "---------------------------" << endl;
        cout << AMARILLO << "Seleccione metodo de pago: "; cout << BLANCO;

        int metodoPago = leerOpcion();

        if (metodoPago == 0) {
            cout << "\n Pago cancelado. El pedido no ha sido confirmado." << endl;
            pausar();
            return false;
        }

        // 4. PROCESAMIENTO DEL PAGO
        string fecha = obtenerFechaActual();
        //limpiarPantalla(); // Limpiar antes de empezar el proceso de pago

        switch (metodoPago) {
        case 1: { // Pago en efectivo
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
            _sleep(1000); // Pequeña pausa para simular proceso
            limpiarPantalla();

            string resultado = pago.procesarPago();
            cout << resultado << endl;

            if (montoPagado >= totalFinal) {
                pago.mostrarDetalle();
                pedido->cambiarEstado("pagado");
                pagoExitoso = true;
            }
            else {
                cout << "\n PAGO RECHAZADO - Monto insuficiente" << endl;
                cout << "   Falta: S/" << fixed << setprecision(2) << (totalFinal - montoPagado) << endl;
            }
            break;
        }

        case 2: // Tarjeta de crédito
        case 3: { // Tarjeta de débito
            string numeroTarjeta, banco;
            string tipoTarjeta = (metodoPago == 2) ? "Credito" : "Debito";
            system("cls");
            cout << ROJO << "PAGO CON TARJETA DE " << tipoTarjeta << endl;
            imprimirSeparadorRojoBlanco(30);
            cout << "\nIngrese numero de tarjeta (16 digitos): ";
            getline(cin, numeroTarjeta);

            cout << "Ingrese nombre del banco: ";
            getline(cin, banco);

            // Validar longitud de tarjeta
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
                // Pago exitoso
                limpiarPantalla();
                cout << VERDE << "\n PAGO PROCESADO EXITOSAMENTE " << RESET << endl;
                pago.mostrarDetalle();
                pedido->cambiarEstado("pagado");
                pagoExitoso = true;
            }
            else {
                // Pago rechazado
                cout << ROJO << "\n PAGO RECHAZADO - Por favor, intente con otro metodo." << RESET << endl;
            }
            break;
        }

        default:
            cout << "\n Opción invalida" << endl;
            pausar();
            return false;
        }

        // 5. CONFIRMACIÓN Y COMPROBANTE
        if (pagoExitoso) {

            pausar();

            // Generar y mostrar comprobante
            Comprobante* comprobante = generarComprobante(pedido, totalFinal, fecha);
            pedido->setComprobante(comprobante);

            limpiarPantalla();
            comprobante->mostrarComprobante(); // Mostrar comprobante en una pantalla dedicada

            pausar(); // Pausa para que el usuario pueda ver el comprobante
            limpiarPantalla();


        }

        pausar();
        return pagoExitoso;
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
            // BOLETA - Usar datos del cliente ya registrado
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
            // FACTURA - Solicitar RUC y Razón Social
            string ruc, razonSocial;
            
			limpiarPantalla();
            cout << "\nDATOS PARA FACTURA " << endl;
            imprimirSeparadorRojoBlanco(20);
            cout << "Ingrese RUC (11 digitos): ";
            getline(cin, ruc);

            cout << "Ingrese Razon Social: ";
            getline(cin, razonSocial);

            // Validar RUC
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
        cout << "\nCliente registrado exitosamente!" << endl;
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

                // Mostrar tipo de comprobante
                if (p->getComprobante() != nullptr) {
                    cout << " - " << p->getComprobante()->getTipoComprobante();
                }
                cout << endl;
            }
        }

        pausar();
    }

    string obtenerFechaActual() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        return string(buffer);
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

            // Mostrar comprobante si existe
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

                // Mostrar tipo de comprobante
                if (p->getComprobante() != nullptr) {
                    cout << " - Comprobante: " << p->getComprobante()->getTipoComprobante();
                }
                cout << endl;
            }

            // Opción para ver comprobante de un pedido específico
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

    void guardarDatos() {
        menuPrincipal->guardarEnArchivo();
        cout << VERDE << "Datos guardados exitosamente" << endl;
    }

    // Cargar datos del restaurante
    void cargarDatos() {
        menuPrincipal->cargarDesdeArchivo();
    }

    // Getters
    string getNombreRestaurante() const { return nombreRestaurante; }
    string getDireccion() const { return direccion; }
    string getTelefono() const { return telefono; }

    // Setters
    void setNombreRestaurante(const string& nombre) { nombreRestaurante = nombre; }
    void setDireccion(const string& dir) { direccion = dir; }
    void setTelefono(const string& tel) { telefono = tel; }
};