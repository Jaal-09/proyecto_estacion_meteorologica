/**
 * @file PantallaManager.cpp
 * @brief Implementación de las funciones gráficas e interfaz de usuario de PantallaManager.
 * @details Desarrolla la lógica para dibujar banners, recuadros dinámicos con bordes 
 * de colores, fuentes tipográficas escaladas y la alternancia de menús informativos.
 * @date Junio 2026
 */

#include "PantallaManager.h"

PantallaManager::PantallaManager() {
    // Arranca mostrando por defecto la pantalla principal con la hora
    vistaActual = 1; 
}

void PantallaManager::iniciar() {
    tft.init();

    // Configuración en modo horizontal (Formato paisaje para pantallas de 320x240)
    tft.setRotation(1);       
    limpiarPantalla(TFT_NAVY); 
}

void PantallaManager::limpiarPantalla(uint16_t color) {
    tft.fillScreen(color);
}

void PantallaManager::cambiarVista() {
    vistaActual = (vistaActual == 1) ? 2 : 1;

    // Limpieza rápida del búfer al cambiar de menú para evitar superposiciones de texto
    limpiarPantalla(TFT_NAVY); 
}

void PantallaManager::mostrarTexto(String texto, int x, int y, int tamano, uint16_t color, uint16_t fondo) {
    tft.setTextColor(color, fondo);
    tft.setTextSize(tamano);
    tft.drawString(texto, x, y); 
}

void PantallaManager::actualizarInterfaz(String hora, String fecha, float temp, float hum, float lux, float presion, String estadoLuz) {
    // Banner Superior Común para ambas vistas
    tft.fillRect(0, 0, 320, 38, TFT_BLUE);
    mostrarTexto("ESTACION METEOROLOGICA", 30, 10, 2, TFT_WHITE, TFT_BLUE);

    if (vistaActual == 1) {
        // ================= VISTA 1: TIEMPO Y TEMPERATURA =================
        // Recuadro de Hora y Fecha en color verde
        tft.drawRect(10, 50, 300, 75, TFT_GREEN);
        mostrarTexto("FECHA: " + fecha, 20, 60, 2, TFT_GREEN, TFT_NAVY);
        mostrarTexto("HORA:  " + hora, 20, 90, 3, TFT_WHITE, TFT_NAVY);

        // Recuadro de Temperatura en color azul cielo
        tft.drawRect(10, 135, 300, 55, TFT_SKYBLUE);
        mostrarTexto("TEMP: " + String(temp, 1) + " C", 20, 150, 3, TFT_SKYBLUE, TFT_NAVY);
        
        // Texto de navegación actualizado para el uso del botón físico
        mostrarTexto("-> Pulsar boton para ver mas", 50, 200, 1, TFT_SILVER, TFT_NAVY);
    } 
    else {

        // ================= VISTA 2: DETALLES (HUMEDAD, LUZ Y PRESIÓN) =================
    // Recuadro de Humedad
    tft.drawRect(10, 45, 300, 45, TFT_CYAN);
    mostrarTexto("HUM: " + String(hum, 1) + " %", 20, 58, 2, TFT_CYAN, TFT_NAVY);

    // =========================================================================
    // 🛠️ RECUADRO DE LUZ CORREGIDO (Evita el efecto fantasma 'lxlx')
    // =========================================================================
    tft.drawRect(10, 98, 300, 45, TFT_YELLOW);

    // 1. Borramos el fondo del recuadro de texto antes de pintar para desaparecer residuos viejos
    

    // 2. Pintamos los Luxes asegurando que el color de fondo (TFT_NAVY) obligue a borrar los píxeles anteriores
    

    // 3. Pintamos el estado al lado sin emojis y bien alineado
    mostrarTexto(estadoLuz, 170, 111, 2, TFT_WHITE, TFT_NAVY);
    // =========================================================================

    // Recuadro de Presión Atmosférica
    tft.drawRect(10, 151, 300, 45, TFT_RED);
    mostrarTexto("PRES: " + String(presion, 1) + " hPa", 20, 164, 2, TFT_RED, TFT_NAVY);

        // ================= VISTA 2: DETALLES (HUMEDAD, LUZ Y PRESIÓN) =================
        // Recuadro de Humedad en color cian
        tft.drawRect(10, 45, 300, 45, TFT_CYAN);
        mostrarTexto("HUMEDAD: " + String(hum, 1) + " %", 20, 58, 2, TFT_CYAN, TFT_NAVY);

        // Recuadro de Iluminación en color amarillo (Muestra Lux y el Estado textual)
        tft.drawRect(10, 98, 300, 45, TFT_YELLOW);

        //para evitar errores al pintar
        tft.fillRect(11, 99, 298, 43, TFT_NAVY); 

        
        mostrarTexto("LUZ: " + String(lux, 0) + " lx", 20, 111, 2, TFT_YELLOW, TFT_NAVY);
        // Pintamos el diagnóstico natural corrido a la derecha (X=160) para que no se pisen
        mostrarTexto(estadoLuz, 160, 111, 2, TFT_WHITE, TFT_NAVY);
        
        // Recuadro de Presión Atmosférica en color rojo
        tft.drawRect(10, 151, 300, 45, TFT_RED);
        mostrarTexto("PRESION: " + String(presion, 1) + " hPa", 20, 164, 2, TFT_RED, TFT_NAVY);
        
        // Texto de navegación actualizado para el uso del botón físico
        mostrarTexto("-> Pulsar boton para regresar", 50, 204, 1, TFT_SILVER, TFT_NAVY);
    }

    // Pie de página institucional estático
    tft.drawLine(0, 218, 320, 218, TFT_SILVER);
    mostrarTexto("U. DE PAMPLONA - MECATRONICA", 45, 224, 1, TFT_SILVER, TFT_NAVY);
}