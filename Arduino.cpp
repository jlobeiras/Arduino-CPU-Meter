/// Only for Arduino
#ifdef LED_BUILTIN

const int PWM9 = 9;
static int blinky = 1;

void setup() {
    Serial.begin(9600); // Puerto de comunicación PC
    pinMode(LED_BUILTIN, OUTPUT); // Led de estado
    pinMode(PWM9, OUTPUT); // Pin salida (usar 5,6,9,10,11)
}

void loop() {
    // Comprobamos si existen datos a la espera
    while (Serial.available()) {

        // Si existen datos parpadeamos el led de estado
        digitalWrite(LED_BUILTIN, ++blinky & 0x01);

        // Leemos los datos del puerto COM en un buffer temporal
        char buffer[64] = "";
        Serial.readBytesUntil('\n', buffer, 60);

        int CPUload = 0;
        // Comprobamos si se recibió 1 valor entero con la carga de CPU
        if (sscanf(buffer, "LoadPercentage=%i", &CPUload) == 1) {

            // Normalizamos el valor a [0..255] y lo enviamos al pin
            if (CPUload > 100) CPUload = 100;
            CPUload = (CPUload * 255) / 100;
            analogWrite(PWM9, CPUload);

            // Confirmamos al PC que se recibió bien el dato (opcional)
            Serial.print("Output = ");
            Serial.println(CPUload);
        }
    }
}

#endif
