# program for sending UDP packets to ping server
from datetime import datetime
import socket
import sys

# setting up the target IP and host
try:
    UDP_IP = sys.argv[1]
    UDP_PORT = int(sys.argv[2])
except:
    print("Please enter target IP and port as command line arguments 1 and 2")
    print("e.g. python PingClient.py 127.0.0.1 1069")
    sys.exit()

# setting up the socket for sending/receiving packets
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.settimeout(0.6)

count = 0
totalrtt = 0
maxrtt = -999999
minrtt = 999999

# sending 15 packets
for i in range(0,15):
	seq_num = 3331 + i
	ts = datetime.now()
	send_message = 'ping to {}, seq = {}, {}\r\n'
	sock.sendto(bytes(send_message.format(UDP_IP, seq_num, ts)), (UDP_IP, UDP_PORT))

	# receiving the packet	
	try:
		data, addr = sock.recvfrom(1024)
		currtime = datetime.now()
		rtt = (currtime-ts).microseconds/1000
		maxrtt = max(rtt, maxrtt)
		minrtt = min(rtt, minrtt)
		totalrtt += rtt
		rec_message = "ping to {}, seq = {}, {} ms"
		count += 1
		print(rec_message.format(UDP_IP, seq_num, rtt))
	except socket.timeout:
		print("ping to {}, seq = {}, time out".format(UDP_IP, seq_num))

closing_message = "rtt min/max/avg = {}/{}/{}"
if count != 0:
	print(closing_message.format(minrtt, maxrtt, totalrtt/count))
else:
	print('all pings timed out')

sock.close()
