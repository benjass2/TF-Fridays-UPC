#pragma once
#include <string>
#include <iostream>
namespace ConsolaColor {

    // ===================================
    // Códigos de CONTROL
    // ===================================

    // Restablece todos los atributos (color, negrita, etc.) a la configuración predeterminada.
    const std::string RESET = "\033[0m";

    // ===================================
    // COLORES DE TEXTO (Foreground Standard)
    // ===================================

    const std::string NEGRO = "\033[30m";
    const std::string ROJO = "\033[31m";
    const std::string VERDE = "\033[32m";
    const std::string AMARILLO = "\033[33m";
    const std::string AZUL = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CIAN = "\033[36m";
    const std::string BLANCO = "\033[37m";

    // ===================================
    // COLORES DE TEXTO (Foreground Brillante/Bright)
    // ===================================

    const std::string NEGRO_BRILLANTE = "\033[90m";
    const std::string ROJO_BRILLANTE = "\033[91m";
    const std::string VERDE_BRILLANTE = "\033[92m";
    const std::string AMARILLO_BRILLANTE = "\033[93m";
    const std::string AZUL_BRILLANTE = "\033[94m";
    const std::string MAGENTA_BRILLANTE = "\033[95m";
    const std::string CIAN_BRILLANTE = "\033[96m";
    const std::string BLANCO_BRILLANTE = "\033[97m";

    // ===================================
    // ESTILOS DE FORMATO (Modifiers)
    // ===================================

    const std::string NEGRITA = "\033[1m";      // También usado para colores brillantes en algunas terminales.
    const std::string SUBRAYADO = "\033[4m";
    const std::string INVERTIR_COLOR = "\033[7m"; // Invierte el color de fondo y primer plano.


    // ===================================
    // COLORES DE FONDO (Background Standard)
    // ===================================

    const std::string FONDO_NEGRO = "\033[40m";
    const std::string FONDO_ROJO = "\033[41m";
    const std::string FONDO_VERDE = "\033[42m";
    const std::string FONDO_AMARILLO = "\033[43m";
    const std::string FONDO_AZUL = "\033[44m";
    const std::string FONDO_MAGENTA = "\033[45m";
    const std::string FONDO_CIAN = "\033[46m";
    const std::string FONDO_BLANCO = "\033[47m";

    // ===================================
    // COLORES DE FONDO (Background Brillante/Bright)
    // ===================================

    const std::string FONDO_NEGRO_BRILLANTE = "\033[100m";
    const std::string FONDO_ROJO_BRILLANTE = "\033[101m";
    const std::string FONDO_VERDE_BRILLANTE = "\033[102m";
    const std::string FONDO_AMARILLO_BRILLANTE = "\033[103m";
    const std::string FONDO_AZUL_BRILLANTE = "\033[104m";
    const std::string FONDO_MAGENTA_BRILLANTE = "\033[105m";
    const std::string FONDO_CIAN_BRILLANTE = "\033[106m";
    const std::string FONDO_BLANCO_BRILLANTE = "\033[107m";

    const std::string BORRAR_PANTALLA = "\033[2J\033[H";



    // ===================================
    // FUNCIONES AUXILIARES (Opcional, pero útil)
    // ===================================


    inline std::string aplicarColor(const std::string& texto, const std::string& color) {
        return color + texto + RESET;
    }

}