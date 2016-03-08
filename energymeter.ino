#include <SoftwareSerial.h>

#define AV       459.0            // reference peak value (arduino value)
#define MV       122.0            // reference rms voltage (multimeter voltage)
#define SQ       1.41421356237    // square root of 2
#define PT       (MV * SQ)        // peak tension
#define K        ((PT / AV) / SQ) // constant to me multiplied by the read value
#define CHANNELS 4

//#define OSCILLOSCOPE_MODE

int pins [CHANNELS] = {A0, A1, A2, A3};
int v0   [CHANNELS];
int v1   [CHANNELS];
int v2   [CHANNELS];
int peaks[CHANNELS];
unsigned long resolution = 0;
unsigned long prev_millis = 0;

#define TX_PIN 2
SoftwareSerial LCD = SoftwareSerial(0, TX_PIN);

void setup()
{
    // ---------------------------------------
    // serial display manual
    // http://www.tato.ind.br/files/TS-216.pdf
    // ---------------------------------------
    delay(500);
    LCD.begin(9600);
    LCD.write(0x0D);
    LCD.write(0xFE); // command
    LCD.write(0x0C); // turn off cursor
    delay(5);
    LCD.write(0xFE); // command
    LCD.write(0x80); // line 1
    delay(5);
    LCD.write(0xFE); // command
    LCD.write(0x01); // clear
    delay(5);
    LCD.print("Res    Tens Pico");
    Serial.begin(115200);
    ADMUX = B01000000; // REFS1, REFS0, ADLAR, MUX4, MUX3, MUX2, MUX1, MUX0
    ADCSRA = B11101101; // ADEN, ADSC, ADATE, ADIF, ADIE, ADPS2, ADPS1, ADPS0
    memset(v0, 0, sizeof(v0));
    memset(v1, 0, sizeof(v1));
    memset(v2, 0, sizeof(v2));
    memset(peaks, 0, sizeof(peaks));
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

ISR(ADC_vect)
{
    // --------------------------------
    static uint8_t discard_reading = 0;
    discard_reading = !discard_reading;
    if (discard_reading)
    {
        volatile uint16_t v = ADCH;
        return;
    }
    // --------------------------------
    int i = ADMUX & B00000011;
    switch (i)
    {
    case 0:
        ADMUX++;
        break;
    case 1:
        ADMUX++;
        break;
    case 2:
        ADMUX++;
        break;
    case 3:
        resolution++;
        ADMUX &= B11111100;
        break;
    }
    uint16_t v = ADCL | (ADCH << 8);
    if (v > peaks[i])
    {
        peaks[i] = v;
    }
    // saves the history
    v2[i] = v1[i];
    v1[i] = v0[i];
    v0[i] = v;
    // detects the begin of the half sine
    if (v2[i] == 0 && v1[i] > 0 && v0[i] > v1[i])
    {
        // TODO
    }
}

void loop()
{
#ifdef OSCILLOSCOPE_MODE
    Serial.println(v0[0]);
#endif
    if (millis() - prev_millis >= 1000)
    {
        char text[64];
        float voltage_rms = K * (float)peaks[0];
        sprintf(text, "%04luHz %03dV %04d", resolution, (int)voltage_rms, peaks[0]);
#ifndef OSCILLOSCOPE_MODE
        Serial.println(text);
#endif
        LCD.write(0xFE); // command
        LCD.write(0xC0); // line 2
        LCD.print(text);
        memset(peaks, 0, sizeof(peaks));
        resolution = 0;
        prev_millis = millis();
    }
}

