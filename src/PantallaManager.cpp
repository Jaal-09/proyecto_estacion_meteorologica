#include "PantallaManager.h"

//Los pines se cargan directamente desde el platformio.ini
PantallaManager::PantallaManager() {
    
}

void PantallaManager::iniciar() {

    tft.init();
    tft.setRotation(1); 

    tft.fillRect(0, 0, 320, 240, TFT_NAVY); 
    delay(50);

    //Banner
    tft.fillRect(0, 0, 320, 38, TFT_BLUE);

    // Configuración del título
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setTextSize(2);
    tft.drawString("ESTACION METEOROLOGICA", 30, 10);



}