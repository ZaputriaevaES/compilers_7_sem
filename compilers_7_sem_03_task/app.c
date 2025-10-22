#include "sim.h"

void app() 
{
    int logo_width = 60;
    int logo_height = 40;
    
    int logo_x = SIM_X_SIZE / 2 - logo_width / 2;
    int logo_y = SIM_Y_SIZE / 2 - logo_height / 2;
    
    int speed_x = 4;
    int speed_y = 5;
    
    int bg_color = 0xFF000000;
    int logo_color = 0xCD5C5C;

    // отрисовываем 200 кадров анимации
    for (int step = 0; step < 200; ++step) 
    {
        // очищаем весь экран (закращиваем все черным)
        for (int y = 0; y < SIM_Y_SIZE; ++y) 
        {
            for (int x = 0; x < SIM_X_SIZE; ++x) 
            {
                simPutPixel(x, y, bg_color);
            }
        }
        
        // проверяем на столкновение с границами (левой или правой соответственно)
        if (logo_x <= 0 || logo_x + logo_width >= SIM_X_SIZE - 1) 
        {
            speed_x = -speed_x;
            logo_color = (logo_color == 0xCD5C5C) ? 0x90EE90 : 0xCD5C5C;
        }
        
        if (logo_y <= 0 || logo_y + logo_height >= SIM_Y_SIZE - 1) 
        {
            speed_y = -speed_y;
            logo_color = (logo_color == 0xCD5C5C) ? 0x90EE90 : 0xCD5C5C;
        }
        
        // смещаем логотип
        logo_x += speed_x;
        logo_y += speed_y;
        
        // отрисовываем логотип
        for (int y = logo_y; y < logo_y + logo_height; ++y) 
        {
            for (int x = logo_x; x < logo_x + logo_width; ++x) 
            {
                if (x >= 0 && x < SIM_X_SIZE && y >= 0 && y < SIM_Y_SIZE) 
                {
                    simPutPixel(x, y, logo_color);
                }
            }
        }
        
        simFlush();
    }
}