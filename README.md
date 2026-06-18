# Drone-desde-cero

En este proyecto armaré un drone de 5 pulgadas desde cero utilizando un esp32 en vez de comprar una computadora de vuelo comercial, además se diseñará una pcb personalizada para montar los componentes y hacer que sea mucho más profesional, también armare el control y utilizaré esp-now para la comunicación a distancia, todo esto se desarollará en lenguaje c++ con el entorno oficial del esp32 conocido como esp-idf.

Los materiales que vamos a necesitar son los siguientes (en diagonal aparecen opciones alternativas):

- 4 motores RS2205 2300Kv / A2212 2200 Kv.
- 4 ESC (electronic speed controller) 30A con BEC 5v (pueden ser genericos o también recomiendo de BLHeli).
- 4 Helices de 5 pulgadas y 3 palas.
- 1 BMI160 sensor con giroscopio y acelerómetro.
- 1 HCM5883L sensor magnetómetro.
- 1 Batería Lipo 11.1v 3s 2200 mAh 25C.
- 2 Esp32 DevKitC 32U WROOM (se caracteriza por que no tiene antena propia sino una entrada para conectar una antena externa).
- 2 Antena de 2.4 GHz 3dBi SMA Macho con Cable SMA Hembra a UFL (IPEX).
- 2 Joystick con eje X,Y - KY-023.
- 1 Cables XT60 12AWG.
- 1 Chasis de drone de 5 pulgadas (yo compré este modelo en especifico, sin embargo puedes diseñarlo en 3D). https://www.amazon.com.mx/dp/B0FR4VZ7NK?ref=ppx_yo2ov_dt_b_fed_asin_title
- 1 PCB personalizada.
- 1 Kit de tornillos allen M2,M3,M4.
- 1 regulador de voltage LM2596.
- 1 Batería lipo 2s.
- 1 LiPo Cargador de batería 2S-3S RC Balance Charger Cargador compacto para baterías LiPo 7.4-11.1V (comprado en amazon con ese mismo nombre).
- Display OLED 128x64 0.96 pulgadas I2C SSD1315.

La primera versión que diseñé fue con impresión 3D en donde modelé el chasis del drone como se puede observar en lsa siguientes imágenes:

<img width="1346" height="715" alt="8a511ea6-0dac-41ea-b9e2-b2e4cc2c4f41" src="https://github.com/user-attachments/assets/5b151760-5664-4ef3-a02c-53e7e96663cf" />

<img width="823" height="573" alt="Captura de pantalla 2025-12-12 192946" src="https://github.com/user-attachments/assets/3856ebe7-74ec-4b3f-9d01-28403a7c2c94" />



