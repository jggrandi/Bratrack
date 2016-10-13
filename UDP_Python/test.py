# Server program
# UDP VERSION

from socket import *
from struct import *

# Set the socket parameters
host = "143.54.13.40"
port = 3000
buf = 1024
addr = (host,port)

# Create socket and bind to address
UDPSock = socket(AF_INET,SOCK_DGRAM)
UDPSock.bind(addr)

# Receive messages
while 1:
	data,addr = UDPSock.recvfrom(buf)
	if not data:
		print ("Client has exited!")
		break
	else:
		print ("\nTimestamp '", unpack('l', data[0:8]),"'")
		print ("\nName '", unpack('8c', data[8:16]),"'")
		print ("\nx '", unpack('d', data[16:24]),"'")
		print ("\ny '", unpack('d', data[24:32]),"'")

# Close socket
UDPSock.close()
