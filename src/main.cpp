#include <Arduino.h>
#include "PantallaManager.h"

PantallaManager miPantalla;

unsigned long tiempoUltimaActualizacion = 0;
const unsigned long INTERVALO_TFT = 2500; 
bool primeraActualizacion = true;

//Se inicia la pantalla tft
void setup() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println("[SISTEMA] Iniciando TFT ...");
    miPantalla.iniciar();
    
    // Mostramos el texto de carga de forma temporal
    miPantalla.mostrarTexto("Iniciando Panel TFT ...", 60, 100, 2, TFT_WHITE);
    delay(1500);
}