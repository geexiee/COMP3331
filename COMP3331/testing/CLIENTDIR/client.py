import socket
import sys
import json
import os
from os import listdir
import threading
import time

loginstatus = False
needpassword = False
newpassword = False
dwnfileexists = False
dwnfilesize = 0
shtdown = False
UPDthreadexists = False
goboom = False

# jsonifies and encodes a dictionary to get it ready to send over tcp
def jsonify(payload):
	payload = json.dumps(payload)
	payload = payload.encode()
	return payload

# converts received json payload to dictionary to be accessed
def dejsonify(payload):
	return json.loads(payload.decode())

# thread class for listening to the server
class ListeningThread(threading.Thread):
	def __init__(self, clientsocket):
		threading.Thread.__init__(self)
		self.clientsocket = clientsocket
	
	def run(self):
		global loginstatus
		global needpassword
		global newpassword
		global dwnfileexists
		global dwnfilesize
		global shtdown 
		global UPDthreadexists
		global goboom
			
		while True:
			try:
				payload = dejsonify(self.clientsocket.recv(4096))
				operation = payload['operation']
				if payload['operation'] == 'logincheck':
					if payload['verification'] == True:
						# set to true so that we can prompt user to input their pw
						needpassword = True
						continue
					else:
						# the user isnt registered, need to prompt them for new pw
						needpassword = True
						newpassword = True
						continue
				elif payload['operation'] == 'passwordcheck':
					username = payload['username']
					if payload['verification'] == True:
						print('Welcome to the forum')
						loginstatus = True
					elif payload['verification'] == 'logged in':
						print('User {} has already logged in'.format(username))
						loginstatus = False
						needpassword = False
						continue
					elif payload['verification'] == False:
						print('Invalid password')	
						loginstatus = False
						needpassword = False
						continue
					# registering a new user
					else:
						needpassword = 1
				elif operation == 'CRTcheck':
					if payload['ok'] == True:
						print('Thread successfully created')
					else:
						print('Thread {} exists'.format(payload['threadtitle']))
				# receiving list of threads
				elif operation == 'LSTcheck':
					if payload['ok'] == True:
						threadlist = payload['threadlist']
						if len(threadlist) == 0:
							print('There are no threads to list')
							continue
						print('The list of active threads: ')
						for t in threadlist:
							print(t)
				elif operation == 'MSGcheck':
					threadtitle = payload['threadtitle']
					if payload['ok'] == True:
						print('Message posted to {} thread'.format(threadtitle))
					else:
						print('Thread {} does not exist'.format(threadtitle))
				# receive the thread data from client
				elif operation == 'RDTcheck':
					threadtitle = payload['threadtitle']
					if payload['ok'] == False:
						print('Thread {} does not exist'.format(threadtitle))
						continue
					else:
						threadcontent = payload['threadcontent']
						if len(threadcontent) == 0:
							print('Thread {} is empty'.format(threadtitle))
							continue
						for m in threadcontent:
							print(m)
				elif operation == 'EDTcheck':
						threadtitle = payload['threadtitle']
						msgnum = payload['msgnum']
						if payload['ok'] == 'no thread':
							print('Thread {} does not exist'.format(threadtitle))
							continue
						elif payload['ok'] == 'no msg':
							print('Message number {} does not exist'.format(msgnum))
							continue
						elif payload['ok'] == 'not yours':
							print('This message belongs to another use and cannot be edited')
							continue
						elif payload['ok'] == True:
							print('The message has been edited')
							continue	
				elif operation == 'DLTcheck':
					threadtitle = payload['threadtitle']
					msgnum = payload['msgnum']
					if payload['ok'] == 'no thread':
						print('Thread {} does not exist'.format(threadtitle))
						continue
					elif payload['ok'] == 'no msg':
						print('Message number {} does not exist'.format(msgnum))
						continue
					elif payload['ok'] == 'not yours':
						print('This message belongs to another user and cannot be deleted')
						continue
					elif payload['ok'] == True:
						print('The message has been deleted')
						continue	
				elif operation == 'UPDcheck1':
					if payload['ok'] == True:
						UPDthreadexists = True
						continue	
					else:
						print('Thread {} does not exist'.format(payload['threadtitle']))
						continue
				elif operation == 'UPDcheck':
					if payload['ok'] == True:
						print('File {} successfully uploaded to {}'.format(payload['filename'], payload['threadtitle']))
						continue
				elif operation == 'SHTcheck':
					if payload['ok'] == True:
						print('Goodbye. Server has been shut down')
						goboom = True
						sys.exit()
					else:
						print('Incorrect password')
				elif operation == 'RMVcheck':
					threadtitle = payload['threadtitle']
					if payload['ok'] == 'not yours':
						print('Thread {} was not created by you and cannot be deleted'.format(threadtitle))
						continue
					elif payload['ok'] == False:
						print('Thread {} does not exist'.format(threadtitle))
						continue
					else:
						print('Thread {} successfully removed'.format(threadtitle))
						continue
				elif operation == 'DWNcheck':
					if payload['ok'] == False:
						print('Thread {} does not exist'.format(payload['threadtitle']))
						continue
					elif payload['ok'] == 'no file':
						print('File {} does not exist in thread {}'.format(payload['filename'], payload['threadtitle']))
						continue
					elif payload['ok'] == True:
						dwnfileexists = True
						dwnfilesize = payload['filesize']
						with open(payload['filename'], 'wb') as f:
							if dwnfilesize <= 4096:
								filecontents = self.clientsocket.recv(dwnfilesize)
								f.write(filecontents)			
							else:
								filecontents = self.clientsocket.recv(4096)
								f.write(filecontents)
								remaining_bytes = dwnfilesize - len(filecontents)
								while remaining_bytes > 0:
									next_buffer = min(4096, remaining_bytes)
									filecontents = self.clientsocket.recv(next_buffer)
									if len(filecontents) < 4096:
										remaining_bytes -= len(filecontents)
									else:
										remaining_bytes -= 4096
									f.write(filecontents)
			except:
				sys.exit()
				
# thread class for taking input and sending messages to the server	
class SendingThread(threading.Thread):
	def __init__(self, clientsocket):
		threading.Thread.__init__(self)
		self.clientsocket = clientsocket
	
	def run(self):
		global loginstatus
		global needpassword
		global shtdown
		global newpassword
		global UPDthreadexists
		global goboom
		while True:
			if goboom == True:
				sys.exit()
			if loginstatus == False:
				if needpassword == False:
					username = input('Enter username: ')
					payload = {
								'operation':'login',
								'username':username
								}
					clientsocket.send(jsonify(payload))
					time.sleep(0.2)
				# user has entered username, needs to enter password 
				elif needpassword == True:
					# prompt new user for pw
					if newpassword == True:
						newpw = input('Enter new password for {}: '.format(username))
						payload = {
							'operation':'newpw',
							'password':newpw
						}
						clientsocket.send(jsonify(payload))
						loginstatus = True
						time.sleep(0.1)
						continue
					pw = input('Enter password: ')
					payload = {
								'operation':'password',
								'password':pw
								}
					clientsocket.send(jsonify(payload))
					time.sleep(0.2)
			######### user has logged in, now we do normal operations #############
			else:
				commandlist = input('Enter one of the following commands: CRT, MSG, DLT, EDT, LST, RDT, UPD, DWN, RMV, XIT, SHT: ').split(' ')
				operation = commandlist[0]
				if operation == 'XIT':
					if len(commandlist) != 1:
						print('Invalid syntax for XIT')
						continue
					payload = {
								'operation':'XIT'
								}
					print('Goodbye')
					clientsocket.send(jsonify(payload))	
					sys.exit()
				elif operation == 'CRT':
					if len(commandlist) != 2 or commandlist[1] == '':
						print('Invalid syntax for CRT')
						continue
					payload = {
								'operation':'CRT',
								'threadtitle':commandlist[1],
								'username':username
								}
					clientsocket.send(jsonify(payload))
					time.sleep(0.1)
				elif operation == 'LST':
					if len(commandlist) != 1:
						print('Invalid syntax for LST')
						continue
					payload = {
								'operation':'LST'
								}
					clientsocket.send(jsonify(payload))
					time.sleep(0.1)
				elif operation == 'MSG':
					if len(commandlist) < 3 or commandlist[2] == '':
						print('Invalid syntax for MSG')
						continue
					threadtitle = commandlist[1]
					message = commandlist[2]
					if len(commandlist) > 3:
						for i in range (3,len(commandlist)):
							message = message + ' ' + commandlist[i] 
					payload = {
								'operation':'MSG',
								'threadtitle':threadtitle,
								'message':message,
								'username':username
								}
					# send message to server
					clientsocket.send(jsonify(payload))	
					time.sleep(0.1)		
				elif operation == 'RDT':
					if len(commandlist) != 2:
						print('Invalid syntax for RDT')
						continue
					threadtitle = commandlist[1]
					payload = {
								'operation':'RDT',
								'threadtitle':threadtitle
								}
					# tell server the name of the thread we want to read
					clientsocket.send(jsonify(payload))
					time.sleep(0.1)		
				elif operation == 'EDT':
					if len(commandlist) < 4 or commandlist[3] == '':
						print('Incorrect syntax for EDT')
						continue
					try:
						msgnum = int(commandlist[2])
					except:
						print('Please enter an integer for message number')
						continue
					threadtitle = commandlist[1]
					msg = commandlist[3]
					if len(commandlist) > 4:
						for i in range(4, len(commandlist)):
							msg = msg + ' ' + commandlist[i]
					payload = {
								'operation':'EDT',
								'threadtitle':threadtitle,
								'msgnum':msgnum,
								'msg':msg,
								'username':username
								}
					clientsocket.send(jsonify(payload))
					time.sleep(0.1)
				elif operation == 'DLT':
					if len(commandlist) != 3:
						print('Invalid syntax for DLT')
						continue
					try:
						msgnum = int(commandlist[2])
					except:
						print('Please enter an integer for message number')
						continue
					threadtitle = commandlist[1]
					payload = {
								'operation':'DLT',
								'threadtitle':threadtitle,
								'msgnum':msgnum,
								'username':username
								}
					clientsocket.send(jsonify(payload))
					time.sleep(0.1)
				elif operation == 'UPD':
					if len(commandlist) != 3:
						print('Incorrect syntax for UPD')
						continue
					threadtitle = commandlist[1]
					filename = commandlist[2]
					try:
						filesize = os.path.getsize(filename)
					except:
						print('File {} does not exist'.format(filename))
						continue
					payload = {
								'operation':'UPD',
								'threadtitle':threadtitle,
								'filename':filename,
								'username':username,
								'filesize':filesize
								}
					# first send the payload with filename 
					clientsocket.send(jsonify(payload))
					time.sleep(0.5)
					# send the file immediately after
					if UPDthreadexists:
						print('Uploading {} of size {} to {}'.format(filename, filesize, threadtitle))
						with open(filename, 'rb') as f:
							if filesize < 4096:	
								filecontents = f.read(filesize)
								self.clientsocket.send(filecontents)
							else:
								filecontents = f.read(4096)
								remaining_bytes = filesize - len(filecontents)
								self.clientsocket.send(filecontents)
								while remaining_bytes > 0:
									filecontents = f.read(min(4096, remaining_bytes))
									remaining_bytes -= len(filecontents)
									self.clientsocket.send(filecontents)
						time.sleep(0.2)
						UPDthreadexists = False
				elif operation == 'DWN':
					if len(commandlist) != 3:
						print('Incorrect syntax for DWN')
						continue
					threadtitle = commandlist[1]
					filename = commandlist[2]
					# tell the server what file we want
					payload = {
								'operation':'DWN',
								'threadtitle':threadtitle,
								'filename':filename
								}
					self.clientsocket.send(jsonify(payload))
					time.sleep(0.2)
				elif operation == 'SHT':
					if len(commandlist) != 2:
						print('Incorrect syntax for SHT')
						continue
					pw = commandlist[1]
					payload = {
								'operation':'SHT',
								'password':pw
								}
					clientsocket.send(jsonify(payload))
					time.sleep(0.1)
				elif operation == 'RMV':
					if len(commandlist) != 2:	
						print('Incorrect syntax for RMV')
						continue
					threadtitle = commandlist[1]
					payload = {
								'operation':'RMV',
								'threadtitle':threadtitle,
								'username':username
								}
					clientsocket.send(jsonify(payload))
					time.sleep(0.1)
				else:
					print('Invalid command')
				
if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('Usage: python3 client.py [server_ip] [server_port]')
		sys.exit()

	targetserverip = sys.argv[1]
	targetserverport = int(sys.argv[2])

	clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	clientsocket.connect((targetserverip,targetserverport))

	listeningthread = ListeningThread(clientsocket)
	sendingthread = SendingThread(clientsocket)
	sendingthread.setDaemon(True)
#	listeningthread.setDaemon(True)
	
	sendingthread.start()
	listeningthread.start()

