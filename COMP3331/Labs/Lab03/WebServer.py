# WEBSERVER FOR RECEIVING AND SENDING HTTP GET REQUESTS
import socket
import sys

# CREATE A SOCKET FOR LISTENING
portnumber = int(sys.argv[1])
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind(('127.0.0.1',portnumber))
serversocket.listen(1)

def parserequest(requeststr):
	print(requeststr)
	if requeststr[:3] == 'GET':
		filename = requeststr.split(' ')[1]
		filename = filename[1:]
		return filename
	else:
		return '404 Not Found'

def createresponse(body, code, content_type):
	if content_type == 'text/html':
		response_headers = {
			'Content-Type': content_type,
			'Content-Length': len(body.encode(encoding="utf-8")),
			'Connection': 'close',
		}	
	else:
		response_headers = {
			'Content-Type': content_type,
			'Content-Length': len(body.encode()),
			'Connection': 'close',
		}	

	response_headers_raw = ''.join('%s: %s\r\n' % (k, v) for k, v in response_headers.items())
	response_proto = 'HTTP/1.1'
	if code == 200:
		response_proto = response_proto + ' 200 OK\r\n'
	else:
		response_proto = response_proto + ' 404 NOT FOUND\r\n'	
	response_headers_raw = response_proto + response_headers_raw
	final_response = response_headers_raw + '\r\n' + body
	return final_response

# USE THE SOCKET TO LISTEN FOR HTTP GET REQUESTS
while True:
	(clientsocket, address) = serversocket.accept()	
#	clientsocket.bind(('127.0.0.1',portnumber))
	recdata = clientsocket.recv(4096)
	filename = parserequest(recdata)
	if filename != '404 Not Found':
		try:
			f = open(filename, 'r')
			response_body = f.read()
			print(response_body)
			if filename.split('.')[1] == 'png':
				response = createresponse(response_body, 200, 'image/png')
			else:
				response = createresponse(response_body, 200, 'text/html')
			clientsocket.sendall(response.encode('utf-8'))
			print(response)
		except Exception as e:
			response = createresponse('not found', 404, 'text/html')
			clientsocket.sendall(response.encode('utf-8'))
			print(e)
			print(response)
		clientsocket.close()
	else:
		clientsocket.close()
		continue
