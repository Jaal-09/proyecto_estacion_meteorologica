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

    // Lógica de refresco de datos y tiempo real
    if (millis() - tiempoUltimaActualizacion >= INTERVALO_TFT) {
        tiempoUltimaActualizacion = millis();
        
        //Se borra el texto de iniciando pantalla en el primer ciclo
        if (primeraActualizacion) {

            // Se usa la función limpiarPantalla
            miPantalla.limpiarPantalla(TFT_NAVY);
            
            // Volvemos a llamar a iniciar para que redibuje los marcos fijos y el título
            miPantalla.iniciar(); 
            
            // Apagamos la bandera para que no lo vuelva a hacer
            primeraActualizacion = false; 
        }
        
        // Obtenemos los datos de tiempo de la clase especializada
        String horaActual  = miReloj.obtenerHora();
        String fechaActual = miReloj.obtenerFecha();
        
        // Datos simulados (Los que heredarán los sensores reales)
        float t_simulada = 26.7;
        float h_simulada = 55.2;
        float l_simulada = 385.0;
        
        Serial.println("[SISTEMA] Refrescando interfaz grafica.");
        // Enviamos hora, fecha y clima directo al manager de pantalla
        miPantalla.actualizarInterfaz(horaActual, fechaActual, t_simulada, h_simulada, l_simulada);
    }

    // 2. Control automático de cambio de vista lógico (Simulación táctil)
    if (millis() - tiempoUltimoCambioVista >= INTERVALO_VISTA) {
        tiempoUltimoCambioVista = millis();
        Serial.println("[SISTEMA] Cambiando de vista...");
        miPantalla.cambiarVista();
    }
}
