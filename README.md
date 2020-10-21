# Regulátor teploty s ESP8266

### Zadání Práce
Prostudujte problematiku elektronického měření teploty. Navrhněte a vyrobte elektronický regulátor teploty s ESP8266. Ten bude spínáním externího topení udržovat teplotu ve stanoveném intervalu. Uvažujte rozsah teplot alespoň -20 až 50 stupňů Celsia. Zařízení bude napájené z externího zdroje. Regulátor se bude konfigurovat prostřednictvím webového rozhraní, kde bude také vidět průběh teplot a stav sepnutí externího topení. Naměřená data budou také odesílána na cloudovou službu pro IoT analýzu dat typu ThingSpeak apod. V rámci semestrálního projektu prostudujte problematiku elektronického měření teploty. Navrhněte schéma a navrhněte a vyrobte desku plošných spojů regulátoru teploty.

### Rozdělení projektu na dílčí části
* prostudování problematiky výber komponet
* výběr vhodného teplotního čidla
* rozbor cloudové služby pro IoT
* návrh DPS (prototyp)
* napsání frimwaru
* návrh finální DPS výroba
* Sepsání dokumentace/práce   

### Realizace projektu

Pro projekt jsem se rozhodl použít ESP8266 a to v provedení ESP-12F. Jedná se o malou destičku, na které je osazen IC ESP8266EX (od něho je odvozen výsledný název) a EEPROM pamět (Flash). Na destičce dále nalezneme několik rezistorů, kondenzátorů, cívek a jeden krystal který taktuje celý obvod. viz schéma zapojení destiky. Za povšimnutí také stojí anténa pro pásmo 2.4GHz, která je realizovaná na DPS a je dopněma LC filtrem typu horní propust (C6, L1, L2).

#### Schéma destičky ESP-XX

![ESPdeskSchema](Image/ESP8266-module-circuit-diagram.ppm)   

Na obrázku můžme vidět pomocí kterých pinů (SPI sběrnice) je k IC připojena Flash pamět, která u mnou zvolené destičky má velikost 4MB.

#### Parametry IC ESP8266EX

* **CPU:** Tensilica L106 32-bit processor
* **I/O rozhraní:** UART/SDIO/SPI/I2C/I2S/IR Remote Contro GPIO/ADC/PWM/LED Light & Button
* **Napájecí napětí:** 2,5 – 3,6V
* **Odběr proudu: průměrně:** 80mA
* **Teplotní rozsah:** -40 – 125°C
* **Pouzdro:** QFN32-pin (5 mm x 5 mm)

![ESP-12E PINOUT](Image/ESP-12E_PINOUT.png)
*!Na obrázku můžete vidět ESP-12E to se od ESP-12F liší pouze tvarem antény*


| **GPIO15**<br>**(MTDO)** | **GPIO0** | **GPIO2** | **Mode** | **Comment** |
| :-: | :-: | :-: | :-: | :-: |
|L| H | H | Flash | Boot form SPI Flash |
|L| L | H | UART  | Programing by UART  |
|H| X | X | SDIO  | Boot form SD-card   |


#### Výběr vhodného teplotního čidla

##### Rozbor problematiky

Teplota je jednou ze základních veličin SI soustavy, její jednotka je [K]. V našich končinách se spíše setkáme s jednoutkou stpeň Celsia [°C], která je odvozena od trojného bodu vody. Přepočet mezi těmito jednotkami je velice snadný:

![obr](https://latex.codecogs.com/gif.latex?%5Cinline%20T_%7BC%7D%20%3D%20T_%7BK%7D%20-%20273%2C15)

Teplotní čidlo slouží k eletrickému měření teploty, jeho hlavním úkolem je převod okolní teploty na nějakou elektrickou veličinu (odpor, poud, napětí). Čidla můžeme dělit podle mnoha faktorů asi nejzásadnější z nich je jestli je čidlo dotykové nebo ne. V naší aplikaci by bezdotykoké čidlo nemělo smysl, stejně tak by nemělo smysl se tu zaobýrat čidly, které nejsou elektrická, proto se dále zaměřím pouze na dělení dotykových elektrických čidel.  

##### Eleketrická dotykové čidla:

* Odporová kovová
* Odporová polovodičová
* Termoelektrická
* Polovodičová s NP přechodem


##### Citace

* http://skriptum.wz.cz/autom/teplota.htm


#### IoT cloudové sluby

* https://thingspeak.com/
* https://tmep.cz/
* https://iotguru.live/
* https://login.arduino.cc/
* https://www.openhab.org/
* https://aws.amazon.com/free/
* https://cloud.ibm.com/login
* https://cloud.google.com/iot-core
* https://www.fogwing.io/
* https://www.axonize.com/platform/
* https://blynk.io/
* https://www.torryharris.com/products/iot-glue-integration-platform
* https://www.datadoghq.com/product/
* https://www.ixon.cloud/iiot-platform
* https://www.bevywise.com/iot-platform/



##### Citace

* LinuxDays - 3.10.2020 - IoT domov aneb pozvěte si zloděje domů (Petr Šrámek)
* LinuxDays - 3.10.2020 - PiHome - automatizace domácnosti s OpenHAB běžící na Raspberry Pi & Arduino (Michal Novotný)
* LinuxDays - 10.10.2015 - MQTT, protokol pro IoT (Adam Hořčica)
