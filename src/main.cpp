#include <Arduino.h>
#include "PantallaManager.h"

#include "RelojManager.h"

// Instanciamos los objetos bajo arquitectura POO
PantallaManager miPantalla;
RelojManager miReloj;

unsigned long tiempoUltimaActualizacion = 0;
unsigned long tiempoUltimoCambioVista = 0;

 // El reloj se refresca cada segundo para ver los segundos avanzar
const unsigned long INTERVALO_TFT = 1000;      

 // Cada 7 segundos rota la vista entre pantalla 1 y 2
const unsigned long INTERVALO_VISTA = 7000;    

//bandera de control para limpiar el letrero de carga
bool primeraActualizacion = true;  

//Se inicia la pantalla tft
void setup() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println("[SISTEMA] Iniciando TFT ...");
    miPantalla.iniciar();
    miReloj.iniciar();
    
    // Mostramos tu texto de carga de forma temporal
    miPantalla.mostrarTexto("Iniciando Panel TFT ...", 60, 100, 2, TFT_WHITE, TFT_NAVY);
    delay(1500);
}


void loop() {
    if (millis() - tiempoUltimaActualizacion >= INTERVALO_TFT) {
        tiempoUltimaActualizacion = millis();
        
        //Se borra el texto de iniciando pantalla
        if (primeraActualizacion) {
            // Se usa la función limpiarPantalla
            miPantalla.limpiarPantalla(TFT_NAVY);
            
            // Volvemos a llamar a iniciar para que redibuje los marcos fijos y el título
            miPantalla.iniciar(); 
            
            // Apagamos la bandera para que no lo vuelva a hacer
            primeraActualizacion = false; 
        }
        
        float t_simulada = 26.7;
        float h_simulada = 55.2;
        float l_simulada = 385.0;
        
        Serial.println("[SISTEMA] Refrescando interfaz grafica.");
        miPantalla.actualizarInterfaz(t_simulada, h_simulada, l_simulada);
    }
}