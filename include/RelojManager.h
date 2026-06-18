/**
 * @file RelojManager.h
 * @brief Control de tiempo unificado mediante RTC DS1307 y sincronización Wi-Fi NTP.
 * @details Esta clase se encarga de gestionar el reloj de tiempo real (RTC) de la estación.
 * Permite conectarse a una red inalámbrica para sincronizar la hora exacta desde internet 
 * y luego mantenerla de forma local.
 * @date Junio 2026
 */

#ifndef RELOJ_MANAGER_H
#define RELOJ_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

/**
 * @class RelojManager
 * @brief Clase mánager para la administración de la fecha, hora y sincronización de la estación.
 */
class RelojManager {
private:
    RTC_DS1307 rtc; /**< Instancia interna de la librería RTClib para controlar el chip físico DS1307. */

    /**
     * @brief Función de utilidad para anteponer ceros a los números menores a 10.
     * @details Asegura que los minutos, segundos, días o meses siempre tengan dos dígitos en la pantalla (ej: convierte "5" en "05").
     * @param numero El valor entero que se desea formatear.
     * @return String La cadena formateada con el formato "XX".
     */
    String formatearDosDigitos(int numero);

public:

    /**
     * @brief Constructor del mánager de tiempo.
     * @details Inicializa la estructura del reloj y prepara las variables internas del sistema.
     */
    RelojManager();

    /**
     * @brief Inicializa la comunicación I2C con el chip RTC DS1307.
     * @details Levanta la conexión física y verifica si el reloj está respondiendo o si ha perdido la hora.
     * @return true si el RTC arrancó con éxito, false si no se encuentra en el bus I2C.
     */
    bool iniciar();

    /**
     * @brief Se conecta temporalmente a internet para calibrar la hora interna del RTC.
     * @details Levanta el módulo Wi-Fi del ESP32 con las credenciales dadas, consulta un servidor de tiempo NTP 
     * confiable y actualiza el chip físico DS1307 para que quede al segundo exacto.
     * @param ssid Nombre de red wifi
     * @param password Contraseña de la red Wi-Fi
     */
    void sincronizarHoraPorWiFi(const char* ssid, const char* password);

    /**
     * @brief Entrega la hora actual formateada para la pantalla TFT.
     * @details Consulta los registros actuales del RTC y devuelve una cadena lista con formato de reloj digital.
     * @return String Cadena de texto con el formato "HH:MM:SS".
     */
    String obtenerHora();

    /**
     * @brief Entrega la fecha actual formateada para la pantalla TFT.
     * @details Consulta los registros actuales del RTC y devuelve una cadena lista con formato de calendario.
     * @return String Cadena de texto con el formato "DD/MM/AAAA".
     */
    String obtenerFecha();
};

#endif