<img width="753" height="152" alt="PCRB" src="https://github.com/user-attachments/assets/9f4f3cb9-690d-47b0-bd87-d9adeb2d7b35" />

Flash de Firmware Badge RP2040 con VS Code + PlatformIO 
 
Requisitos del Entorno:

- VS Code. 
- Extensión VS Code PlatformIO IDE. 

Flash Bootloader

1. Clona el repositorio y abre la carpeta en VS Code
2. Conecta el Badge a tu PC mientras mantienes presionado el botón **BOOTSEL (B)**.
3. Aparecerá como una unidad de disco llamada `RPI-RP2`.
4. En la barra inferior de VS Code, haz clic en el icono de la flecha hacia la derecha (→) (Upload).
5. PlatformIO compilará el código, generará el archivo `.uf2` y lo copiará automáticamente al badge.


