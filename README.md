# Redes - Laboratorio 2
## Esquemas de detección y corrección de errores
**Integrantes:**
- Diego Leiva
- Pablo Orellana
- - - 
### Instrucciones de Uso
- - -
**Primera Parte del Laboratorio**


Para poder iniciar la primera parte del laboratorio, primero deberemos abrir la carpeta de nuestra preferencia para hacer la prueba. Ya sea la carpeta Hamming o la carpeta CRC-32
- Primero deberá ejecutar "receptor" independientemente el que haya seleccionado (Hamming/CRC-32).
- Segundo deberá ejecutar "emisor" independientemente el que haya seleccionado (Hamming/CRC-32).
- El emisor solicitará que ingrese un dato.
- El receptor recibirá el mensaje.
- - -

**Segunda Parte del Laboratorio**

Para poder probar un mensaje se debe hacer lo siguiente:
- Ejecuatr primero main_receiver.py e indicar que tipo de algoritmo va a recibir (Hamming/CRC-32)
- ejecutar main_sender.cpp e indicar que tipo de algoritmo a usar y luego ingresar mensaje.
- - -

**para testing**
ejecutar primero receiver_test.py y modificar variables de message_lenght y probability para generar los titulos del grafico.
luego ejecutar sender_test.cpp y modificar variables de message_amount para cantidad de pruebas, message_lengt para longitud de cada mensaje, probability para probabilidad de error y choice (1 para hamming, 2 para crc-32)
