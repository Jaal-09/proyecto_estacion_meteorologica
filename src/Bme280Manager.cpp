/**
 * @file Bme280Manager.cpp
 * @brief Implementación de los métodos de control y lectura para el sensor BME280.
 * @details Contiene la lógica para levantar el bus I2C secundario (Wire1) en los pines 18 y 19,
 * verificar la presencia física del chip en la dirección 0x76 y extraer los datos ambientales.
 * @date Junio 2026
 */

#include "Bme280Manager.h"

Bme280Manager::Bme280Manager() {

    // Por seguridad, asumimos que no está listo hasta pasar el setup
    sensorOperativo = false;
}

bool Bme280Manager::iniciar() {
    Serial.println("[BME280] Iniciando bus I2C secundario...");
    
    // Configuración de los pines asignados para el bus secundario Wire1 (SDA=18, SCL=19)
    bool busIniciado = Wire1.begin(18, 19); 
    
    if (!busIniciado) {
        Serial.println("[ERROR] No se pudo inicializar el bus I2C en pines 18 y 19.");
        return false;
    }

    Serial.println("[BME280] Buscando sensor físico...");

    // Intentamos despertar al sensor en su dirección estándar 0x76 pasándole el bus secundario Wire1
    if (!bme.begin(0x76, &Wire1)) {
        Serial.println("[ERROR] ¡No se encontró el BME280! Revisa conexiones.");
        sensorOperativo = false;
        return false;
    }
    
    Serial.println("[BME280] ¡Sensor detectado en pines 18 y 19 con éxito!");

    // Todo OK, habilitamos el paso de lecturas
    sensorOperativo = true;
    return true;

}

float Bme280Manager::obtenerTemperatura() {

    // Si el sensor falló en el arranque, evitamos lecturas basura y devolvemos cero seguro
    if (!sensorOperativo) return 0.0;
    return bme.readTemperature(); // Retorna la temperatura real en grados Celsius (°C)
}

float Bme280Manager::obtenerHumedad() {

    // Protección en caso de desconexión física del sensor en caliente
    if (!sensorOperativo) return 0.0;
    return bme.readHumidity(); // Retorna el porcentaje de humedad relativa (% RH)
}

float Bme280Manager::obtenerPresion() {
    if (!sensorOperativo) return 0.0;

    // La librería lee en Pascales en bruto (Pa). Dividimos por 100.0F para transformarlo 
    // a Hectopascales (hPa), que es la unidad que espera la interfaz gráfica.
    return bme.readPressure() / 100.0F; 
}

String Bme280Manager::obtenerEstadoTexto() {
    // Si el sensor se desconecta o falla, devolvemos un aviso seguro
    if (!sensorOperativo) return "❌ ERROR";

    float presion = obtenerPresion();
    
    // Clasificación en cascada según los rangos calibrados para la altitud
    if (presion < 775.0)  return "Lluvioso";
    if (presion <= 784.0) return "Nublado";
    
    return "Despejado";
}

