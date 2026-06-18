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

    // Configuración en modo horizontal (320x240)
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

void PantallaManager::actualizarInterfaz(String hora, String fecha, float temp, float hum, float lux, float presion, String estadoLuz, String estadoPresion) {
    // ================= Banner Superior Común para ambas vistas =================
    tft.fillRect(0, 0, 320, 38, TFT_BLUE);
    
    // Dibujamos la nube al lado derecho
    int cloudX = 40;
    int cloudY = 10;
    tft.fillCircle(cloudX, cloudY + 10, 8, TFT_WHITE);       // Esfera izquierda
    tft.fillCircle(cloudX + 8, cloudY + 4, 10, TFT_WHITE);   // Esfera central superior
    tft.fillCircle(cloudX + 18, cloudY + 8, 9, TFT_WHITE);   // Esfera derecha
    tft.fillRect(cloudX, cloudY + 12, 18, 7, TFT_WHITE);     // Base plana para unir los círculos

    // Nombre en el banner
    mostrarTexto("METEOROS-32", 95, 10, 2, TFT_WHITE, TFT_BLUE);

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
        
        // 1. Recuadro de Humedad en color cian
        tft.drawRect(10, 45, 300, 45, TFT_CYAN);
        mostrarTexto("HUMEDAD: " + String(hum, 1) + " %", 20, 58, 2, TFT_CYAN, TFT_NAVY);

        // 2. Recuadro de Iluminación en color amarillo (Muestra Lux y el Estado textual)
        tft.drawRect(10, 98, 300, 45, TFT_YELLOW);
        
        // Borramos el fondo interno del recuadro antes de pintar para desaparecer residuos de texto viejo
        tft.fillRect(11, 99, 298, 43, TFT_NAVY); 
        
        mostrarTexto("LUZ: " + String(lux, 0) + " lx", 20, 111, 2, TFT_YELLOW, TFT_NAVY);
        // Pintamos el diagnóstico natural corrido a la derecha 
        mostrarTexto(estadoLuz, 160, 111, 2, TFT_WHITE, TFT_NAVY);
        
        // 3. Recuadro de Presión Atmosférica en color Naranja
        tft.drawRect(10, 151, 300, 45, TFT_ORANGE);
        
        //Borrar para no dejar residuos
        tft.fillRect(11, 152, 298, 43, TFT_NAVY); 
        
        mostrarTexto("PRES: " + String(presion, 1) + " hPa", 10, 164, 2, TFT_ORANGE, TFT_NAVY);
        
        //Pintar estado segun la presión
        mostrarTexto(estadoPresion, 200, 164, 2, TFT_WHITE, TFT_NAVY);
        
        // Texto de navegación para el uso del botón físico
        mostrarTexto("-> Pulsar boton para regresar", 50, 204, 1, TFT_SILVER, TFT_NAVY);
    }

    // Pie de página institucional estático
    tft.drawLine(0, 218, 320, 218, TFT_SILVER);
    mostrarTexto("U. DE PAMPLONA - MECATRONICA", 45, 224, 1, TFT_SILVER, TFT_NAVY);
}