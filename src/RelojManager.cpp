#include "RelojManager.h"
#include <WiFi.h>
#include "time.h"

RelojManager::RelojManager() {
    // Constructor vacío
}

bool RelojManager::iniciar() {
    if (!rtc.begin()) {
        Serial.println("[ERROR] No se detecta el hardware del DS1307.");
        return false;
    }
    return true;
}

void RelojManager::sincronizarHoraPorWiFi(const char* ssid, const char* password) {
    Serial.println("[NTP] Conectando a Wi-Fi para actualizar hora...");
    WiFi.begin(ssid, password);

    // Esperar máximo 15 segundos a que se conecte a la red física
    int intentosWifi = 0;
    while (WiFi.status() != WL_CONNECTED && intentosWifi < 30) {
        delay(500);
        Serial.print(".");
        intentosWifi++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[NTP] ¡Conectado al Wi-Fi con éxito!");
        
        // ⏳ TIEMPO DE ESPERA CRÍTICO: 
        // Dejamos pasar 3 segundos para que el router estabilice la IP del ESP32
        Serial.println("[NTP] Esperando estabilidad de red (3s)...");
        delay(3000); 
        
        // Configurar la zona horaria 
        configTime(-18000, 0, "pool.ntp.org", "time.nist.gov");
        
        struct tm timeinfo;
        bool horaObtenida = false;
        int intentosNTP = 0;
        
        // Bucle de reintentos para el servidor de internet
        while (!horaObtenida && intentosNTP < 5) {
            intentosNTP++;
            Serial.printf("[NTP] Intentando obtener hora (Intento %d de 5)...\n", intentosNTP);
            
            if (getLocalTime(&timeinfo)) {
                horaObtenida = true;
                // Inyectar la hora de internet directo al chip físico DS1307
                rtc.adjust(DateTime(
                    timeinfo.tm_year + 1900, 
                    timeinfo.tm_mon + 1, 
                    timeinfo.tm_mday, 
                    timeinfo.tm_hour, 
                    timeinfo.tm_min, 
                    timeinfo.tm_sec
                ));
                Serial.println("[NTP] ¡Módulo DS1307 sincronizado con éxito!");
            } else {
                Serial.println("[WARNING] Servidor ocupado. Esperando 2 segundos para reintentar...");
                delay(2000); 
            }
        }
        
        if (!horaObtenida) {
            Serial.println("[ERROR FINAL] No se pudo obtener la hora tras 5 intentos.");
        }
        
    } else {
        Serial.println("\n[ERROR Wi-Fi] No se pudo conectar a la red.");
    }

    // Dejamos el Wi-Fi encendido 2 segundos más por seguridad antes de tumbarlo
    delay(2000);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("[NTP] Wi-Fi apagado correctamente. Bucle local iniciado.");
}

String RelojManager::formatearDosDigitos(int numero) {
    if (numero < 10) {
        return "0" + String(numero);
    }
    return String(numero);
}

String RelojManager::obtenerHora() {
    DateTime ahora = rtc.now();
    return formatearDosDigitos(ahora.hour()) + ":" +
           formatearDosDigitos(ahora.minute()) + ":" +
           formatearDosDigitos(ahora.second());
}

String RelojManager::obtenerFecha() {
    DateTime ahora = rtc.now();
    return formatearDosDigitos(ahora.day()) + "/" +
           formatearDosDigitos(ahora.month()) + "/" +
           String(ahora.year());
}