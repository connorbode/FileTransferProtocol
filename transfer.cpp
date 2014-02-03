
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
	memset(&szbuffer,0,sizeof(szbuffer));
	sprintf(szbuffer, message); 
	ibytessent=0;
	ibufferlen = strlen(message);
	ibytessent = send(s,szbuffer,ibufferlen,0);
	if (ibytessent == SOCKET_ERROR)
		throw "Send failed\n";  
	else {
		cout << "\n\r";
		cout << "SENDING MESSAGE: " << szbuffer;
		cout << "\n\r\n\r";
	}
}

char* Transfer::receiveMessage() {
	memset(&szbuffer,0,sizeof(szbuffer));
	//Fill in szbuffer from accepted request.
	if((ibytesrecv = recv(s,szbuffer,128,0)) == SOCKET_ERROR)
		throw "Receive error in server program\n";

	//Print reciept of successful message. 
	return szbuffer;
}

bool Transfer::sendFile(FILE *stream, string filename) {

	// Get the filesize
	long fileSize;
	fseek (stream, 0, SEEK_END);
    fileSize=ftell (stream);
    printf ("Size of myfile.txt: %ld bytes.\n", fileSize);

	// Figure out how many packets are needed
	long numPackets = (fileSize / PACKET_SIZE) + 1;
	printf("Number of packets required: %ld. \n", numPackets);

	// Figure out the size of the last packet
	long lastPacketSize = (fileSize % PACKET_SIZE);
	printf("Last packet size: %ld. \n", lastPacketSize);

	// Compose header message
	char numPacketsChar[128];
	itoa(numPackets, numPacketsChar, 10);
	char lastPacketSizeChar[128];
	itoa(lastPacketSize, lastPacketSizeChar, 10);
	char header[128] = "";
	strcat(header, "put;num_packets:");
	strcat(header, numPacketsChar);
	strcat(header, ";last_packet_size:");
	strcat(header, lastPacketSizeChar);
	strcat(header, ";filename:");
	strcat(header, filename.c_str());
	strcat(header, ";");
	cout << "Header: " << header << "\n";

	// Send header
	sendMessage(header);

	// Wait for response
	char response[128];
	strcpy(response, receiveMessage());

	// if the response is not "ok", exit
	if(strcmp(response, "ok") != 0) {
		cout << "something went wrong... quitting...";
		exit(0);
	}

	// Read file to memory
	long size;
	size_t result;
	fseek(stream, 0, SEEK_END);
	size = ftell(stream);
	rewind(stream);
	char *fileBuffer = new char[size];
	result = fread(fileBuffer, 1, size, stream);
	printf("Read %ld bytes to memory", result);

	// Send packets
	for(int i = 0; i < numPackets; i++) {

		// set packet size
		int packetSize = PACKET_SIZE;
		if(i == numPackets - 1) packetSize = lastPacketSize;

		// Clear buffer
		memset(&szbuffer,0,sizeof(szbuffer));

		// Get the current position in the file
		memcpy(szbuffer, &fileBuffer[i * PACKET_SIZE], PACKET_SIZE);

		// Read from fi

		ibytessent=0;
		ibytessent = send(s,szbuffer, packetSize,0);
		if (ibytessent == SOCKET_ERROR)
			throw "Send failed\n";
		else {
			//cout << "\n\r";
			//cout << "Sending packet " << i << ", size " << packetSize << ", bytes sent " << ibytessent;
		}
	}




	return true;
}


void Transfer::receiveFile(FILE *stream, int numPackets, int lastPacketSize) {

	// Send OK
	sendMessage("ok");

	// Calculate total size
	int filesize = ((numPackets - 1) * PACKET_SIZE) + lastPacketSize;
	char *fileBuffer = new char[filesize];

	// Receive packets
	for(int i = 0; i < numPackets; i++) {

		// Set packet size
		int packetSize = PACKET_SIZE;
		if(i == numPackets - 1) packetSize = lastPacketSize;

		// Reset buffer
		memset(&szbuffer,0,sizeof(szbuffer));

		// Receive packet to buffer
		if((ibytesrecv = recv(s,szbuffer, packetSize,0)) == SOCKET_ERROR)
			throw "Receive error in server program\n";

		// Print received packet
		printf("Received packet %ld, expected size %ld, bytes received %ld\n", i, packetSize, ibytesrecv);

		// Copy to file buffer
		memcpy(&fileBuffer[i * PACKET_SIZE], szbuffer, packetSize);

	}

	// Empty buffer to file stream
	int byteswritten = fwrite(fileBuffer, 1, filesize, stream);

	printf("Wrote %ld bytes \n", byteswritten);
}


bool Transfer::sendMessage2(char* message) {

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

char* Transfer::receiveMessage2() {

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