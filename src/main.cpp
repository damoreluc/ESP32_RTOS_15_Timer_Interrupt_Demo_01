/*
 * FreeRTOS Esempio 15: lampeggio di un LED mediante interrupt da timer hardware.
 *
 * Il led viene comandato nella ISR del timer hardware, periodo 2000ms
 *
 * Nota: nel file soc.h sono definiti i riferimenti ai due core della ESP32:
 *   #define PRO_CPU_NUM (0)
 *   #define APP_CPU_NUM (1)
 *
 * Qui viene adoperata la APP_CPU
 *
 */

#include <Arduino.h>

// pin driver del Led
#define pinLed GPIO_NUM_23

// variabili globali
// divisore del timer hardware (per tick = 1us)
static const uint16_t timer_divider = 80;
// costante di tempo del timer hardware
static const uint64_t timer_max_count = 1000000;

// Globals
// handler del timer hardware
static hw_timer_t *timer = NULL;

//*****************************************************************************
// Interrupt Service Routines (ISRs)

// ISR del timer hardware, eseguita quando il timer raggiunge il valore timer_max_count
void IRAM_ATTR onTimer() {

  // Toggle LED
  int pin_state = digitalRead(pinLed);
  digitalWrite(pinLed, !pin_state);
}

//*****************************************************************************
// Main (sul core 1, con priorità 1)

// configurazione del sistema
void setup() {

  // Configurazione del pin del led
  pinMode(pinLed, OUTPUT);

  // Crea e avvia il timer hardware num. 0 (num, divider, countUp)
  timer = timerBegin(0, timer_divider, true);

  // Associa la ISR al timer (timer, function, edge)
  timerAttachInterrupt(timer, &onTimer, true);

  // Imposta il valore di conteggio al quale eseguire la ISR (timer, count, autoreload)
  timerAlarmWrite(timer, timer_max_count, true);

  // Abilita la generazione degli interrupt del timer
  timerAlarmEnable(timer);
}

void loop() {
  // lasciare vuoto
}