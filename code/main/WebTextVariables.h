#ifndef WebTextVariables_h
#define WebTextVariables_h

#include <Arduino.h>

char WebTextVariable[2][40][40] = {
	{
		"Regulátor teploty",
		"Regulátor teploty",
		"AP mód",
		"mDNS název zařízení",
		"IP adresa zařízení",
		"Maska sítě", //5
		"Výchozí brána",
		"SSID",
		"Heslo",
		"ThingSpeak nastavení",
		"Konfiurace Wifi sítí", //10
		"Číslo kanálu",
		"Klíč aplikace",
		"Interval zpráv (s)",
		"Odesílání zpráv",
		"Počet bodů v grafu",
		"Čidlo odpojeno", //16
		"Připojeno",
		"Chyba",
		"Výstup", // 19
		"Regulace",
		"Nastaveni",
		"Regulator VYP/ZAP",
		"Teplota", //23
		"Nastavená teplota",
		"Hystereze",
		"Kontrolní ledky",
		"Uložit", //27
		"Přidej",
		"Graf",
		"Možnosti zařízení", //30
		"Uložení konfigurace",
		"Načtení konfigurace", //32
		"Restartovat",
		"Poslední DHCP adresa"
	},{
		"Temperature controller",
		"Temperature controller",
		"AP mode",
		"mDNS device name",
		"Device IP address",
		"Netmask",
		"Default gateway",
		"SSID",
		"Password",
		"ThingSpeak settings",
		"Wifi connection",
		"Chanel Number",
		"Api Key",
		"Time interval (s)",
		"Send messages",
		"Number of graph points",
		"Sensor not connected",
		"Connected",
		"Error",
		"Output",
		"Status",
		"Config",
		"Controller OFF/ON",
		"Temperature",
		"Setted temperature",
		"Hysteresis",
		"Status",
		"Save",
		"Add",
		"Graph",
		"Device options", //30
		"Save configuration",
		"Load configuration", //32
		"reboot",
		"Last DHCP address"
	}
};



#endif
