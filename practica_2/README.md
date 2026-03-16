# Práctica 2

**Alumno: Luca Lazcano**

Esta práctica implementa dos bloques principales:

- Un módulo de **delay no bloqueante** basado en `HAL_GetTick()`.
- Un módulo de **blink pattern** para generar secuencias de parpadeo configurables.

## Módulo `delay`

### Objetivo
Permitir temporizaciones sin frenar el `while(1)`, para mantener la lógica reactiva y escalable.

### Interfaz
- `delayInit(delay_t *delay, tick_t duration)`: inicializa la estructura y duración base.
- `delayRead(delay_t *delay)`: consulta si venció el tiempo configurado.
- `delayWrite(delay_t *delay, tick_t duration)`: actualiza la duración para el próximo ciclo.

### Cómo funciona internamente
`delayRead()` opera como una pequeña máquina de estados:

1. Si el delay no está corriendo, guarda el tick actual (`startTime`) y arranca.
2. Si está corriendo, compara `HAL_GetTick() - startTime` con `duration`.
3. Cuando se cumple el tiempo, devuelve `true` y deja el delay listo para reiniciar en la próxima llamada.

De esta forma no se usa `HAL_Delay()` y el loop principal sigue ejecutándose continuamente.

## Módulo `blink pattern`

### Objetivo
Generar un patrón de blink con:

- Lista de duraciones (`durations[]`)
- Repeticiones por paso (`repeats_per_step`)
- Duty cycle (`duty_cycle`)

### Interfaz
- `blinkPatternInit(...)`: carga parámetros y estado inicial del patrón.
- `blinkPatternStep(...)`: avanza la lógica del patrón y devuelve el próximo tiempo de espera (`tick_t`).

### Cómo funciona internamente
El módulo mantiene estado interno (`position`, `counter`, `high`, `millis_high`):

- Si estaba en alto, pasa a bajo y devuelve el tiempo bajo restante del período actual.
- Si estaba en bajo, pasa a alto, incrementa contador y devuelve `millis_high`.
- Cada `repeats_per_step`, avanza a la siguiente duración del arreglo (cíclicamente).

En el `main`, el valor retornado por `blinkPatternStep()` se carga en `delayWrite()`, logrando un parpadeo configurable sin bloqueos.

