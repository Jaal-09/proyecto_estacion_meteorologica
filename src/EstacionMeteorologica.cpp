#include "EstacionMeteorologica.h"
#include "SensorTemperatura.h"
#include "SensorHumedad.h"
#include "SensorPresion.h"
#include "SensorLuz.h"

EstacionMeteorologica::EstacionMeteorologica(int pinBoton, const char* wifiSSID, const char* wifiPASS)
    : pinBoton(pinBoton), INTERVALO_TFT(1000), ssid(wifiSSID), password(wifiPASS) {
    
    tiempoUltimaActualizacion = 0;
    primeraActualizacion = true;
    estadoBotonAnterior = HIGH;

    // Aquí ocurre la herencia real y la restructuración por función:
    misSensores[0] = new SensorTemperatura(&bmeFisico);
    misSensores[1] = new SensorHumedad(&bmeFisico);
    misSensores[2] = new SensorPresion(&bmeFisico);
    misSensores[3] = new SensorLuz(); 
}

EstacionMeteorologica::~EstacionMeteorologica() {
    for (int i = 0; i < 4; i++) {
        delete misSensores[i];
    }
}

void EstacionMeteorologica::iniciar() {
    Serial.begin(115200);
    delay(500);
    
    pinMode(pinBoton, INPUT_PULLUP);
    pantalla.iniciar();
    reloj.iniciar();
    
    // 1. Levantar Bus Secundario Wire1 (Pines 18 y 19) para el integrado físico BME280
    Wire1.begin(18, 19);
    if (!bmeFisico.begin(0x76, &Wire1)) {
        Serial.println("[ERROR] ¡No se encontró el chip físico BME280!");
    }

    // 2. Levantar Bus Primario Wire nativo (Pines 21 y 22) e inicializar el sensor de luz heredado
    Wire.begin(21, 22);
    misSensores[3]->iniciar(); 
    
    pantalla.mostrarTexto("Iniciando Sistema NTP...", 15, 100, 2, TFT_WHITE, TFT_NAVY);
    reloj.sincronizarHoraPorWiFi(ssid, password);
    delay(1000);
}

void EstacionMeteorologica::procesar() {
    if (millis() - tiempoUltimaActualizacion >= INTERVALO_TFT) {
        tiempoUltimaActualizacion = millis();
        
        if (primeraActualizacion) {
            pantalla.limpiarPantalla(TFT_NAVY);
            pantalla.iniciar(); 
            primeraActualizacion = false; 
        }
        
        String horaActual  = reloj.obtenerHora();
        String fechaActual = reloj.obtenerFecha();
        
        // Ejecución de llamadas polimórficas puras por su función heredada
        float t_real = misSensores[0]->leerValor();
        float h_real = misSensores[1]->leerValor();
        float p_real = misSensores[2]->leerValor();
        float l_real = misSensores[3]->leerValor();
        
        String estadoPresion = misSensores[2]->obtenerEstadoTexto();
        String estadoLuz     = misSensores[3]->obtenerEstadoTexto();

        Serial.printf("[TELEMETRIA OO] Temp: %.1f C | Hum: %.1f %% | Pres: %.1f hPa (%s) | Luz: %.1f lx | %s\n", 
                      t_real, h_real, p_real, estadoPresion.c_str(), l_real, estadoLuz.c_str());
        
        pantalla.actualizarInterfaz(horaActual, fechaActual, t_real, h_real, l_real, p_real, estadoLuz, estadoPresion);
    }

    // Monitoreo del botón
    bool estadoBotonActual = digitalRead(pinBoton);
    if (estadoBotonAnterior == HIGH && estadoBotonActual == LOW) {
        pantalla.cambiarVista();
        tiempoUltimaActualizacion = 0; 
        delay(250); 
    }
    estadoBotonAnterior = estadoBotonActual;
}