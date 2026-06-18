# Drone-desde-cero

En este proyecto armaré un drone de 5 pulgadas desde cero utilizando un esp32 en vez de comprar una computadora de vuelo comercial, además se diseñará una pcb personalizada para montar los componentes y hacer que sea mucho más profesional, también armare el control y utilizaré esp-now para la comunicación a distancia, todo esto se desarollará en lenguaje c++ con el entorno oficial del esp32 conocido como esp-idf sin utilizar el entorno de desarrollo de arduino.

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
- led RGB tierra común.
- 3 resistencias 330 ohms (para proteger el led rgb).
- 2 resistencias de 10k ohms (la comunicación I2C necesita tener pull up).
- 4 capacitores ceramicos de 0.1 uF 25v.
- 2 capacitores electroliticos de 10 uF 25v.
- 1 capacitor ceramico de 1 uF 25v.
- 1 capacitor electrolitico de 470 uF 25v.
- 5 borneras.
- muchas tiras de headers macho y hembra.
- 1 resistencia de 30k omhs y 10k omhs (divisor de voltage para medir batería).

La primera versión que diseñé fue con impresión 3D en donde modelé el chasis del drone como se puede observar en lsa siguientes imágenes:

<img width="1346" height="715" alt="8a511ea6-0dac-41ea-b9e2-b2e4cc2c4f41" src="https://github.com/user-attachments/assets/5b151760-5664-4ef3-a02c-53e7e96663cf" />

<img width="823" height="573" alt="Captura de pantalla 2025-12-12 192946" src="https://github.com/user-attachments/assets/3856ebe7-74ec-4b3f-9d01-28403a7c2c94" />

También diseñe su respectiva pcb cuyo archivo podremos encontrar listo para imprimir en la carpeta llamada pcbs bajo el nombre de "Drone_PCB_GERBER_Y1" y se ve de la siguiente manera:

<img width="3000" height="2000" alt="Drone_VIP_image v38" src="https://github.com/user-attachments/assets/31c1b594-b32d-4067-bd18-d399526ee990" />

<img width="610" height="660" alt="image" src="https://github.com/user-attachments/assets/c0ef89e3-5d7a-45ed-a964-8ece3165f5e6" />

al soldar los componentes a la pcb obtenemos este resultado:

https://github.com/user-attachments/assets/316fa69b-e04b-4bce-a329-7272a76162d6

Ahora solo falta montar los motores y el chasis.

<img width="1151" height="751" alt="image" src="https://github.com/user-attachments/assets/55f71348-76c7-4cd0-9f5f-f94becb49e50" />


El código lo vamos a ver más adelante, pero me gustaría que vean este video para entender que es lo que esta pasando.

https://www.youtube.com/watch?v=hIrbeEAcsL4

Básicamente el material que utilice es demasiado debil y pesado para aguantar caidas por lo que para este punto ya había roto varios chasis del drone por lo que decidí rediseñar el drone para ser mas ligero y resistente a impactos, fue entonces cuando compré el chasis de fibra de carbono junto con unos motores más livianos y de mejor calidad, además el drone se estrellaba por que el pid no estaba suficientemente calibrado.

Primero empece diseñando la nueva pcb que para ahorrar espacio decidi crear 2 pcbs separadas que se unen como un sandwich la primera pcb es dedicada a potencia del drone y la segunda pcb con tiene las pistas de logica. al unirlas se ve la siguiente manera:


https://github.com/user-attachments/assets/0f2d87ef-f514-440b-8249-9a66fb56a2bb

PCB de potencia
<img width="923" height="670" alt="image" src="https://github.com/user-attachments/assets/a7d20ce1-ce21-4658-b259-3e190fdb9f63" />


PCB de lógica
<img width="482" height="739" alt="image" src="https://github.com/user-attachments/assets/558cb06b-12ec-4d97-8c05-4144d658c8bd" />

Así es como se vería completamente montado:

<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/a9c03141-f08b-46aa-a4dd-e783acd629ae" />
















