/**
 * @file PantallaManager.h
 * @brief Definición de la clase PantallaManager para el control de la interfaz gráfica.
 * @details Utiliza la librería TFT_eSPI para controlar el renderizado de datos,
 * la creación de recuadros y la alternancia cíclica de menús mediante software.
 * @date Junio 2026
 */

#ifndef PANTALLA_MANAGER_H
#define PANTALLA_MANAGER_H

#include <Arduino.h>
#include <TFT_eSPI.h> 

/**
 * @class PantallaManager
 * @brief Gestiona la renderización en la pantalla TFT y el flujo visual entre las distintas vistas.
 */
class PantallaManager {

private:

    TFT_eSPI tft;   /**< Instancia interna de la librería TFT_eSPI encargada de comunicarse con el hardware de la pantalla. */
    int vistaActual;    /**< Variable de estado del menú: 1 = Vista Principal (Tiempo/Temp), 2 = Vista Secundaria (Detalles). */

public:

    /**
     * @brief Constructor del mánager de la pantalla.
     * @details Inicializa por defecto el estado apuntando a la primera vista en el arranque del sistema.
     */
    PantallaManager();

    /**
     * @brief Inicializa el hardware gráfico de la pantalla.
     * @details Configura los registros de inicio, define la orientación de pantalla horizontal (Formato paisaje) y limpia el fondo.
     */
    void iniciar();

    /**
     * @brief Limpia la pantalla completa pintándola de un solo color sólido.
     * @param color Valor hexadecimal de 16 bits en formato RGB565 del color base.
     */
    void limpiarPantalla(uint16_t color);

    /**
     * @brief Utilidad para dibujar texto personalizado en coordenadas exactas del plano cartesiano de la pantalla.
     * @param texto Cadena de caracteres String con el mensaje que se va a mostrar.
     * @param x Ubicación en el eje horizontal (Píxeles de 0 a 320).
     * @param y Ubicación en el eje vertical (Píxeles de 0 to 240).
     * @param tamano Multiplicador o escala del tamaño de la fuente tipográfica incorporada.
     * @param color Color del texto en formato de color RGB565.
     * @param fondo Color del recuadro de fondo del texto en formato RGB565 para evitar efecto fantasma (Ghosting).
     */
    void mostrarTexto(String texto, int x, int y, int tamano, uint16_t color, uint16_t fondo);

    /**
     * @brief Cambia el estado del menú cíclicamente entre la Vista 1 y la Vista 2 de forma inmediata.
     * @details Borra por completo el búfer de la pantalla para repintar la nueva estructura gráfica sin dejar residuos de la vista anterior.
     */
    void cambiarVista(); 

    /**
     * @brief Renderiza y actualiza dinámicamente toda la información meteorológica en la pantalla TFT.
     * @details Dibuja los banners institucionales fijos y actualiza los recuadros de datos específicos según la vista que esté activa.
     * @param hora Cadena con el formato de tiempo hh:mm:ss.
     * @param fecha Cadena con la fecha actual formateada dd/mm/aaaa.
     * @param temp Valor de temperatura ambiente en grados Celsius (°C).
     * @param hum Valor del porcentaje de humedad relativa (% RH).
     * @param lux Valor de la intensidad luminosa capturada por el luxómetro.
     * @param presion Valor de la presión atmosférica actual calculada en Hectopascales (hPa).
     * @param estadoLuz Texto generado por el mánager del BH1750 que describe la iluminación.
     */
    void actualizarInterfaz(String hora, String fecha, float temp, float hum, float lux, float presion, String estadoLuz, String estadoPresion);
};

#endif