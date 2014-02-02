
#include "transfer.h"
using namespace std;

Transfer::Transfer() {
}

Transfer::Transfer(SOCKET s) {
	this->s = s;
	PACKET_SIZE = 128;
	HEADER_LENGTH = 43;
}

bool Transfer::sendMessage(char* message) {

	// Get message size
	string messageString = string(message);
	int messageSize = messageString.size();

	// Get number of packets
	int numPackets = (messageSize / PACKET_SIZE) + 1;

	// Get last packet size
	int lastPacketSize = messageSize % PACKET_SIZE;

	// Pad number of packets with zeros
	char numPacketsChar[9] = "";
	sprintf(numPacketsChar, "%09d", numPackets);

	// Pad last packet size with zeros
	char lastPacketSizeChar[3] = "";
	sprintf(lastPacketSizeChar, "%03d", lastPacketSize);

	// Create header string
	string numPacketsString = string(numPacketsChar);
	string lastPacketSizeString = string(lastPacketSizeChar);
	string header = "num_packets:" + numPacketsString + ";last_packet_size:" + lastPacketSizeString + ";";
	const char *headerChar = header.c_str();

	// Send header
	memset(&szbuffer,0,sizeof(szbuffer));
	sprintf(szbuffer, headerChar);
	ibytessent=0;
	if(strlen(headerChar) != HEADER_LENGTH)
		throw "Header is corrupt..";
	ibufferlen = HEADER_LENGTH;
	ibytessent = send(this->s,szbuffer,ibufferlen,0);
	if (ibytessent == SOCKET_ERROR)
		throw "Send failed\n";

	// Send each packet
	for(int i = 0; i < numPackets; i++) {
		
		// Get the proper packet size
		int thisPacketSize = PACKET_SIZE;
		if(i == numPackets - 1) {
			thisPacketSize = lastPacketSize;
		}

		// Reset the buffer	
		memset(&szbuffer,0,sizeof(szbuffer));

		// Copy the correct number of packets
		strncpy(szbuffer, &message[i*PACKET_SIZE], thisPacketSize);
		ibytessent=0;
		ibufferlen = HEADER_LENGTH;
		ibytessent = send(this->s,szbuffer,ibufferlen,0);
		if (ibytessent == SOCKET_ERROR)
			throw "Send failed\n";
	}
}

char* Transfer::receiveMessage() {

	// Receive header
	memset(&szbuffer,0,sizeof(szbuffer));
	if((ibytesrecv = recv(s,szbuffer,PACKET_SIZE,0)) == SOCKET_ERROR)
		throw "Receive error in server program\n";

	// Extract number of packets
	char num_packets_char[10];
	memcpy(num_packets_char, &szbuffer[12], 9);
	num_packets_char[9] = '\0';
	int num_packets = atoi(num_packets_char);

	// Extract last packet size
	char last_packet_size_char[4];
	memcpy(last_packet_size_char, &szbuffer[39], 3);
	last_packet_size_char[3] = '\0';
	int last_packet_size = atoi(last_packet_size_char);

	// Create a buffer to empty the packets into
	const int total_message_size = (num_packets - 1) * PACKET_SIZE + last_packet_size;
	char* message = new char[total_message_size];

	cout << "receiving message";

	// Receive message
	for(int i = 0; i < num_packets; i++) {

		// Get incoming message size
		int packet_size = PACKET_SIZE;
		if(i == num_packets - 1) {
			packet_size = last_packet_size;
		}

		// Clear the buffer
		memset(&szbuffer,0,sizeof(szbuffer));

		// Receive the message
		if((ibytesrecv = recv(s,szbuffer,packet_size,0)) == SOCKET_ERROR)
			throw "Receive error in server program\n";

		// Copy the message to the buffer
		strcat(message, szbuffer);
	}

	return message;
}