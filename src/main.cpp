/**
 * @file main.cpp
 * @brief Punto de entrada principal para la estación meteorológica METEOROS-32.
 * @details Inicializa el objeto central de la estación.
 * @date Junio 2026
 */

#include <Arduino.h>
#include "EstacionMeteorologica.h"

// --- Configuración de Periféricos y Hardware ---
const int PIN_BOTON_EXTERNO = 5;     // Pulsador para cambio de pantallas (GPIO 5)

// --- Credenciales de Red Local (Sincronización NTP) ---
const char* WIFI_SSID = "mg52"; 
const char* WIFI_PASS = "wifi1234"; 

// Instancia global del sistema central de la estación
EstacionMeteorologica estacion(PIN_BOTON_EXTERNO, WIFI_SSID, WIFI_PASS);

/**
 * @brief Configuración inicial del sistema.
 */
void setup() {
    // Delega la inicialización de buses, pantalla, RTC y sensores al objeto central
    estacion.iniciar();
}

/**
 * @brief Bucle de ejecución continua.
 */
void loop() {
    // Procesa de forma asíncrona las lecturas, refresco de TFT y rebote del botón
    estacion.procesar();
}