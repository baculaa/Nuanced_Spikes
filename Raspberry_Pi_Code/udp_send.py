import socket

msgFromClient       = "Hello UDP Server"
bytesToSend         = str.encode(msgFromClient)
serverAddressPort   = ('', 20001)
bufferSize          = 1024


# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

while(True):
    print("Provide text to send: ")
    new_text = input()
    # Send to server using created UDP socket
    UDPClientSocket.sendto(new_text.encode('utf_8','strict'), serverAddressPort)