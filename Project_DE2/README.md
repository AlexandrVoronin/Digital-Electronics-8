# Project_DE2

### Členové týmu

Michal Jemelka

Ondřej Pišťák 

[Odkaz na repositář v GitHubu](https://github.com/xjemel06/Digital-Electronics-2/tree/master/Project_DE2)


### Cíle projektu
- Cílem projektu je vytvořit funkční aplikaci parkovacího asistenta s využitím ultrazvukových senzorů a Arduina. Tato aplikace využívá periferie k zobrazování vzdálenosti objektu od senzoru. Pužité periferie jsou: LCD displej, LED panel a reproduktor. Následně je využit i komunikační protokol UART. Veškeré vzdálenosti jsou zobrazovány v centimetrech.

## Popis Hardwaru
- Schéma zapojení
![Schéma zapojení](Schema.png)

- Jak si můžete všimnout v zapojení se nachází dva ultrazvukové senzory HC-SR04 připojené k mikroprocesoru ATmega328P. Dále se zde nachází LCD displej, který je připojený ve čtyř-bitovém módu a zobrazuje vzdálenost snímanou oběma senzory. Dále je k mikroprocesoru přes rezistory připojen LED panel s pěti LED, které začnou signalizovat pokud je překročena zadaná hranice vzdálenosti. Společně s LED panelem se aktivuje i reproduktor měnící kmitočet tónu v závislosti na vzálenosti.


## Popis kódu a simulace
- Stavový diagram
![Stavový diagram](state_diagram.png) 

### Hlavní program [main.c](https://github.com/xjemel06/Digital-Electronics-2/blob/master/Project_DE2/Project_DE2/Project_DE2/Project_DE2/main.c) se dělí na: 
- import knihoven
- hlavní smyčku
- obslužné funkce přerušení. 

#### Import knihoven
- propojí knihovny
- nadefinují globální proměnné a kmitočet procesoru 

#### Hlavní smyčka
- inicializuje porty, komunikační protokol UART, LCD displej
- nastavení časovačů, a požadavek o přerušení
- nekonečný cyklus zaručující stálou funkčnost programu 

#### Funkce přerušení 
- obsahuje dvě přerušení založené na principu změny pinu (PinChange) a dvě založené na časovačích (Timer) 

- Přerušení založené na PinChange reagují na vzestupnou hranu na dvou EchoPinech
  - PinChange0 reprezentuje levý senzor a PinChange1 pravý senzor.
  - PinChange0 a PinChange1 mají prakticky stejnou funkci. V obou případech se měří šířka přijatého pulzu, která se dále přepočítává na reálnou vzdálenost v cm. Dále se veškerá potřebná data odesílají na LCD diplej a UART. 

- Přerušení založené na typu Timer používaji  časovače Timer1 a Timer2
  - Požadavek přerušení od Timer1 vyvolá funkci, která vyšle na oba senzory 10 us pulzy, které vygenerují burst. Tato funkce se opakuje jednou za 1 s.
  - Požadavek přerušení od Timer2 vyhodnocuje vzdálenost a podle toho spouští LED panel a zvukovou signalizaci. 

![Screenshot simulace](Screenshot.png)

## Video

[Odkaz na video na YouTube](https://youtu.be/cEXJesjSIcg)

## References

1. [Popis senzoru](https://components101.com/ultrasonic-sensor-working-pinout-datasheet)

2. [Datasheet HC-SR04](https://components101.com/sites/default/files/component_datasheet/HCSR04%20Datasheet.pdf)

3. [Mikroprocesor ATmega328P](https://www.microchip.com/wwwproducts/en/ATmega328p)

4. [Datasheet mikroprocesoru](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf)

5. [Schéma Arduino shield](https://github.com/tomas-fryza/Digital-electronics-2/blob/master/Docs/arduino_shield.pdf)
