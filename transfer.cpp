
#include "transfer.h"
using namespace std;

Transfer::Transfer() {

}

Transfer::Transfer(SOCKET s) {
	this->s = s;
}

bool Transfer::sendMessage(char* message) {

	memset(&szbuffer,0,sizeof(szbuffer));
	sprintf(szbuffer, message);
	ibytessent=0;
	ibufferlen = strlen(message);
	ibytessent = send(this->s,szbuffer,ibufferlen,0);
	if (ibytessent == SOCKET_ERROR)
		throw "Send failed\n";
}

char* Transfer::receiveMessage() {
	memset(&szbuffer,0,sizeof(szbuffer));
	//Fill in szbuffer from accepted request.
	if((ibytesrecv = recv(s,szbuffer,128,0)) == SOCKET_ERROR)
		throw "Receive error in server program\n";

	//Print reciept of successful message. 
	return szbuffer;
}