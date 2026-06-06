#include "RelojManager.h"

RelojManager::RelojManager() {
    // Constructor vacío, el bus I2C se inicia en el método iniciar
}

bool RelojManager::iniciar() {

    // Intentar enlazar con el chip físico por la dirección I2C nativa
    if (!rtc.begin()) {
        Serial.println("[ERROR] No se detecta el hardware del DS1307.");
        return false;
    }

    // Si es la primera vez que se monta
    if (!rtc.isrunning()) {
        Serial.println("[RTC] El reloj no estaba corriendo. Sincronizando con la hora de compilacion...");

        // Ajusta el reloj con la fecha y hora exacta en la que se le dio "Upload" 
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    Serial.println("[RTC] Inicializado correctamente.");
    return true;
}
