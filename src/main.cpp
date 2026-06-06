#include <Arduino.h>
#include "PantallaManager.h"
#include "RelojManager.h"

PantallaManager miPantalla;
RelojManager miReloj;

unsigned long tiempoUltimaActualizacion = 0;
unsigned long tiempoUltimoCambioVista = 0;

const unsigned long INTERVALO_TFT = 1000;       
const unsigned long INTERVALO_VISTA = 7000;     
bool primeraActualizacion = true;               

// Configuracion WIFI
const char* WIFI_SSID = "";
const char* WIFI_PASS = "";

void setup() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println("[SISTEMA] Iniciando TFT y Reloj...");
    miPantalla.iniciar();
    miReloj.iniciar();
    
    miPantalla.mostrarTexto("Sincronizando hora ...", 10, 100, 2, TFT_WHITE, TFT_NAVY);
    
    // Llamamos a la sincronización automática por Wi-Fi
    miReloj.sincronizarHoraPorWiFi(WIFI_SSID, WIFI_PASS);
    
    delay(1000);
}

void loop() {
    if (millis() - tiempoUltimaActualizacion >= INTERVALO_TFT) {
        tiempoUltimaActualizacion = millis();
        
        if (primeraActualizacion) {
            miPantalla.limpiarPantalla(TFT_NAVY);
            miPantalla.iniciar(); 
            primeraActualizacion = false; 
        }
        
        String horaActual  = miReloj.obtenerHora();
        String fechaActual = miReloj.obtenerFecha();
        
        float t_simulada = 26.7;
        float h_simulada = 55.2;
        float l_simulada = 385.0;
        
        miPantalla.actualizarInterfaz(horaActual, fechaActual, t_simulada, h_simulada, l_simulada);
    }

    if (millis() - tiempoUltimoCambioVista >= INTERVALO_VISTA) {
        tiempoUltimoCambioVista = millis();
        miPantalla.cambiarVista();
    }
}
