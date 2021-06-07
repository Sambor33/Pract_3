#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void ExceptionHandler (const string &err)
{
    cout<<err<<endl;
}
int main()
{
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family=AF_INET;
    selfAddr->sin_port = 0;
    selfAddr->sin_addr.s_addr = 0;
    sockaddr_in *remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family=AF_INET;
    remoteAddr->sin_port = htons(13);
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.12");
    char *buffer = new char [256];
    strcpy(buffer,"Скажи мне время");
    int msglen = strlen(buffer);
    int mySocket = socket(AF_INET,SOCK_DGRAM,0);
    if (mySocket==-1) {
        ExceptionHandler("Error open socket");
    }
    int rc = bind(mySocket,(const sockaddr*)selfAddr,sizeof(sockaddr_in));
    if (rc==-1) {
        close(mySocket);
        ExceptionHandler("Error bind socket with local address");
    }
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        ExceptionHandler("Error connect socket with remote server");
    }
    rc = send (mySocket,buffer,msglen,0);
    if (rc==-1) {
        close(mySocket);
        ExceptionHandler("Error send message");
    }
    cout<<"We send: "<<buffer<<endl;
    rc = recv (mySocket,buffer,1024,0);
    if (rc==-1) {
        close(mySocket);
        ExceptionHandler("Error receive answer");
    }
    buffer[rc] = '\0';
    cout<<"We receive: "<<buffer<<endl;
    close (mySocket);
    delete selfAddr;
    delete remoteAddr;
    delete[] buffer;
    return 0;
}
