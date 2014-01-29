FileTransferProtocol
====================

#### A C++ project for a networking course (COMP 445 at Concordia)

### Requirements

Using existing code for client and server applications which can communicate using WinSock2, do the following

- enable file transfer between client and server applications such that files are packed into equal sized packets at the source and unpacked at the destination
- modify the server application to be multi-threaded and support simultaneous client connections

The client application must support the following commands:

- __LIST__:
    * list files in a default directory on the client
    * list files in a default directory on the server
- __PUT__: transfer a file from the client to the server
- __GET__: transfer a file from the server to the client
