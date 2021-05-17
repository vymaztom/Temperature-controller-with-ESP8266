
import socket




hostname = socket.gethostname()
print(hostname)

local_ip = socket.gethostbyname("esp8266")

print(local_ip)

byte_message = bytes("Hello, World! Hello, World!", "utf-8")
opened_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
opened_socket.sendto(byte_message, ("192.168.1.255", 4210))
