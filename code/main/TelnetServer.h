#ifndef TelnetServer_h
#define TelnetServer_h

#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <vector>

#define TCP_TELNET_PORT 23


class TelnetServer {


public:
	TelnetServer();
	bool begin();

private:
	AsyncServer* telnet_;

	static void handleError(void* arg, AsyncClient* client, int8_t error);
	static void handleData(void* arg, AsyncClient* client, void *data, size_t len);
	static void handleDisconnect(void* arg, AsyncClient* client);
	static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time);
	static void handleNewClient(void* arg, AsyncClient* client);

	static char* procesor(void *data, size_t len);

};


#endif
