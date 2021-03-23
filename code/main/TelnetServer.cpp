#include "TelnetServer.h"

TelnetServer::TelnetServer() {

}

bool TelnetServer::begin() {
	telnet_ = new AsyncServer(TCP_TELNET_PORT);
	telnet_->onClient(&handleNewClient, telnet_);
	telnet_->begin();
}


/* clients events */
void TelnetServer::handleError(void* arg, AsyncClient* client, int8_t error){
   Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

void TelnetServer::handleData(void* arg, AsyncClient* client, void *data, size_t len){
   Serial.printf("\n data received from client %s \n", client->remoteIP().toString().c_str());
   Serial.write((uint8_t*)data, len);

   // reply to client
   if (client->space() > 32 && client->canSend()){
	   char reply[32];
	   Serial.printf("\n data received from client %i \n", client->space());
	   sprintf(reply, "this is from %s", "SERVER_HOST_NAME");
	   client->add(reply, strlen(reply));
	   client->send();
   }
}

void TelnetServer::handleDisconnect(void* arg, AsyncClient* client){
   Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());
}

void TelnetServer::handleTimeOut(void* arg, AsyncClient* client, uint32_t time){
   Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}


/* server events */
void TelnetServer::handleNewClient(void* arg, AsyncClient* client){
   Serial.printf("\n new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());


   // register events
   client->onData(&handleData, NULL);
   client->onError(&handleError, NULL);
   client->onDisconnect(&handleDisconnect, NULL);
   client->onTimeout(&handleTimeOut, NULL);
}

char* TelnetServer::procesor(void *data, size_t len){

}
