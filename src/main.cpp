/**
 * @file main.cpp
 * @brief Ciclo principal y coordinación de la Estación Meteorológica IoT.
 * @details Este programa coordina de manera eficiente la lectura síncrona de múltiples 
 * sensores ambientales (BME280 y BH1750), gestiona el tiempo a través de un chip local RTC 
 * calibrado por internet (NTP) y maneja la navegación de pantallas mediante un pulsador físico.
 * @date Junio 2026
 */

#include <Arduino.h>
#include "PantallaManager.h"
#include "RelojManager.h"
#include "Bme280Manager.h"
#include "Bh1750Manager.h" 

/** @brief Pin digital del ESP32 asignado para el botón físico de navegación (GPIO 5). */
const int PIN_BOTON_EXTERNO = 5; 

// Instancias de control para el hardware del proyecto
PantallaManager miPantalla;    /**< Controlador de la interfaz gráfica en la pantalla TFT. */
RelojManager miReloj;          /**< Gestiona el tiempo local y la sincronización Wi-Fi. */
Bme280Manager miBme;          /**< Mánager para el sensor de temperatura, humedad y presión. */
Bh1750Manager miLuz;          /**< Mánager para el sensor de iluminancia digital. */      

// Variables para el control de tiempos y refresco de pantalla
unsigned long tiempoUltimaActualizacion = 0;   /**< Almacena el milisegundo exacto del último refresco de los sensores. */

const unsigned long INTERVALO_TFT = 1000;       /**< Frecuencia con la que se actualizan los datos en pantalla (1 segundo). */       

bool primeraActualizacion = true;               /**< Bandera encargada de limpiar la pantalla una sola vez al arrancar. */              

// Gestión de estados para el pulsador mecánico
bool estadoBotonAnterior = HIGH; /**< Guarda el estado lógico previo del botón para detectar flancos de bajada. */

// Configuración de la red wifi
const char* WIFI_SSID = "mg52"; 
const char* WIFI_PASS = "wifi1234"; 

/**
 * @brief Configuración inicial del sistema (Setup hardware).
 * @details Inicializa los periféricos, define el comportamiento del GPIO del botón con Pull-Up,
 * levanta los buses de comunicación y realiza la sincronización horaria inicial.
 */


void setup() {
    Serial.begin(115200);
    delay(500);
    
    // Configurar el pin del botón como entrada con la resistencia Pull-Up interna activa
    pinMode(PIN_BOTON_EXTERNO, INPUT_PULLUP);
    
    Serial.println("[SISTEMA] Iniciando TFT y Reloj...");
    miPantalla.iniciar();
    miReloj.iniciar();
    miBme.iniciar();
    
    // Inicia el bus Wire nativo en los pines 21 y 22 para el luxómetro
    miLuz.iniciar();       
    
    miPantalla.mostrarTexto("Iniciando Sistema NTP...", 15, 100, 2, TFT_WHITE, TFT_NAVY);
    
    // Llamamos a la sincronización automática por Wi-Fi para poner a punto el RTC
    miReloj.sincronizarHoraPorWiFi(WIFI_SSID, WIFI_PASS);
    
    delay(1000);
}

/**
 * @brief Bucle de ejecución infinita del firmware.
 * @details Se encarga de dos procesos en paralelo:\n
 * 1. Monitoreo temporal (cada 1s) para leer sensores y repintar la interfaz gráfica.\n
 * 2. Monitoreo por flancos para detectar pulsaciones instantáneas en el botón externo.
 */
void loop() {

    // ================= 1. REFRESH DE DATOS (Temporizado cada 1 segundo) =================
    if (millis() - tiempoUltimaActualizacion >= INTERVALO_TFT) {
        tiempoUltimaActualizacion = millis();
        
        if (primeraActualizacion) {
            miPantalla.limpiarPantalla(TFT_NAVY);
            miPantalla.iniciar(); 
            primeraActualizacion = false; 
        }
        
        // Extracción del tiempo actual desde los registros del mánager del reloj
        String horaActual  = miReloj.obtenerHora();
        String fechaActual = miReloj.obtenerFecha();
        
        // Captura de variables ambientales desde el bus I2C secundario (Wire1)
        float t_real = miBme.obtenerTemperatura();
        float h_real = miBme.obtenerHumedad();
        float p_real = miBme.obtenerPresion();     
        
        // Captura de luminosidad y procesamiento de su diagnóstico textual (Bus Wire nativo)
        float l_real = miLuz.obtenerLux(); 
        String estadoLuz = miLuz.obtenerEstadoTexto();

        // Reporte rápido por el Monitor Serie para verificar el correcto flujo del programa
        Serial.printf("[SISTEMA] Temp: %.1f C | Hum: %.1f %% | Pres: %.1f hPa | Luz: %.1f lx | %s\n", 
                      t_real, h_real, p_real, l_real, estadoLuz.c_str());
        
        // Renderizado dinámico en la pantalla pasando los 7 parámetros requeridos
        miPantalla.actualizarInterfaz(horaActual, fechaActual, t_real, h_real, l_real, p_real, estadoLuz);
    }

    // ================= 2. DETECCIÓN DEL BOTÓN FÍSICO =================
    bool estadoBotonActual = digitalRead(PIN_BOTON_EXTERNO);
    
    // Si el botón cambia su estado lógico de reposo (HIGH) a presionado (LOW)
    if (estadoBotonAnterior == HIGH && estadoBotonActual == LOW) {
        Serial.println("\n[INTERACCIÓN] ¡Botón externo presionado! Cambiando de vista...");
        
        // Alternamos el estado interno del menú visual
        miPantalla.cambiarVista(); 
        
        // Forzamos la actualización para que el cambio gráfico se pinte en la TFT al instante
        tiempoUltimaActualizacion = 0; 
        
        // Retardo controlado para absorber los rebotes mecánicos de las láminas del botón.
        delay(250); 
    }
    
    // Guardamos el estado actual para la evaluación en el siguiente ciclo del loop
    estadoBotonAnterior = estadoBotonActual;
}