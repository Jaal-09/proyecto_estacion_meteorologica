#include "PantallaManager.h"

PantallaManager::PantallaManager() {
    // Arranca en la pantalla principal
    vistaActual = 1; 
}

void PantallaManager::iniciar() {
    tft.init();

    // Modo horizontal
    tft.setRotation(1);       
    limpiarPantalla(TFT_NAVY); 
}

void PantallaManager::limpiarPantalla(uint16_t color) {
    tft.fillScreen(color);
}

void PantallaManager::cambiarVista() {
    vistaActual = (vistaActual == 1) ? 2 : 1;

    // Limpieza rápida al cambiar de menú
    limpiarPantalla(TFT_NAVY); 
}

void PantallaManager::mostrarTexto(String texto, int x, int y, int tamano, uint16_t color, uint16_t fondo) {
    tft.setTextColor(color, fondo);
    tft.setTextSize(tamano);
    tft.drawString(texto, x, y); 
}

void PantallaManager::actualizarInterfaz(String hora, String fecha, float temp, float hum, float lux) {
    // Banner Superior Común para ambas vistas
    tft.fillRect(0, 0, 320, 38, TFT_BLUE);
    mostrarTexto("ESTACION METEOROLOGICA", 30, 10, 2, TFT_WHITE, TFT_BLUE);

    if (vistaActual == 1) {
        // ================= VISTA 1: TIEMPO Y TEMPERATURA =================
        // Recuadro de Hora y Fecha
        tft.drawRect(10, 50, 300, 75, TFT_GREEN);
        mostrarTexto("FECHA: " + fecha, 20, 60, 2, TFT_GREEN, TFT_NAVY);
        mostrarTexto("HORA:  " + hora, 20, 90, 3, TFT_WHITE, TFT_NAVY);

        // Recuadro de Temperatura justo debajo
        tft.drawRect(10, 135, 300, 55, TFT_RED);
        mostrarTexto("TEMP: " + String(temp, 1) + " C", 20, 150, 3, TFT_RED, TFT_NAVY);
        
        mostrarTexto("-> Toca para ver mas", 80, 200, 1, TFT_SILVER, TFT_NAVY);
    } 
    else {
        // ================= VISTA 2: DETALLES (HUMEDAD Y LUZ) =================
        // Recuadro Humedad
        tft.drawRect(10, 50, 300, 60, TFT_CYAN);
        mostrarTexto("HUMEDAD: " + String(hum, 1) + " %", 20, 68, 3, TFT_CYAN, TFT_NAVY);

        // Recuadro Iluminación
        tft.drawRect(10, 120, 300, 60, TFT_YELLOW);
        mostrarTexto("LUZ: " + String(lux, 0) + " lx", 20, 138, 3, TFT_YELLOW, TFT_NAVY);
        
        mostrarTexto("-> Toca para regresar", 80, 200, 1, TFT_SILVER, TFT_NAVY);
    }

    // Pie de página institucional
    tft.drawLine(0, 218, 320, 218, TFT_SILVER);
    mostrarTexto("U. DE PAMPLONA - MECATRÓNICA", 45, 224, 1, TFT_SILVER, TFT_NAVY);
}
