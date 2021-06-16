import json
import os
import signal
import socket
import sys
import threading
import time

existing_threads = [] # list to keep track of threads created threads
loggedin_users = [] # list to keep track of users that are currently logged in
threadsocketlist = []
processlist = []

# jsonifys and encodes a dictionary to get it ready to send via tcp
def jsonify(payload):
	payload = json.dumps(payload)
	payload = payload.encode()
	return payload

# converts json payload into dictionary so it can be read/accessed
def dejsonify(payload):
	return json.loads(payload.decode())	

class ClientSocket:
	def __init__(self, clientsocket, threadlock):
		self.clientsocket = clientsocket
		self.threadlock = threadlock
	
	# Helper function which reads credentials.txt into a dictionary and return the dictionary
	# Usernames are keys, passwords are values
	def readcredentials(self):
		logins = {}
		try:
			f = open('credentials.txt', 'r')
			for x in f.readlines():
				username = x.split(' ')[0]
				password = x.split(' ')[1].strip()
				logins[username] = password
			f.close()
			return logins
		except:
			return logins

	# returns list of all existing threads
	def LST(self):
		return existing_threads

	# appends a message to a given thread. returns false if thread doesn't exist
	def MSG(self, user, threadtitle, message):
		if threadtitle not in self.LST():
			return False
		# get line number for new message
		line_number = 1
		with open(threadtitle, 'r') as f1:
			for l in f1:
				# check that the line is an actual message (not upload or creator name)
				try:
					num = int(l.split(' ')[0])
					line_number += 1
				except:
					continue
		# append message to end of thread 
		with open(threadtitle, 'a+') as f:
			f.write('\n{} {}: {}'.format(line_number, user, message))	
		return True

	# returns contents of a thread excluding first line as a list. returns false if thread doesn't exist
	def RDT(self, threadtitle):
		if threadtitle not in self.LST():
			return False
		f = open(threadtitle, 'r')
		next(f)
		threadcontent = []
		for l in f:
			threadcontent.append(l.strip())
		return threadcontent

	# edits a specific line in a specified thread. returns message based on error or true if successful
	def EDT(self, threadtitle, msgnum, msg, username):
		if threadtitle not in self.LST():
			return 'no thread'
		flag = 0
		with open(threadtitle, 'r') as f1:
			newfile = []
			oldfile = f1.readlines()
			length = len(oldfile)
			# find the matching msgnum and username, then replace the msg
			for l in oldfile:
				length -= 1
				msgparts = l.split(' ')
				try:
					if int(msgparts[0]) == msgnum:
						if msgparts[1].rstrip(':') == username:
							if length == 0:
								newfile.append('{} {}: {}'.format(msgnum, username, msg))
							else:
								newfile.append('{} {}: {}\n'.format(msgnum, username, msg))
							# flag to let us know a message was found and edited
							flag = 1
						else:
							return 'not yours'
					else:
						newfile.append(l)
				except:
					newfile.append(l)
					continue
		if flag == 0:
			return 'no msg'
		with open(threadtitle,'w') as f:
			for l in newfile:
				f.writelines(l)
		return True	

	# deletes a specific line in a specified thread
	# returns one of: 'no thread', 'not yours', 'no msg', True
	def DLT(self, threadtitle, username, msgnum):
		if threadtitle not in self.LST():
			return 'no thread'
		with open(threadtitle, 'r') as f:
			oldfile = f.readlines()
			newfile = []
			length = len(oldfile)
			lastline = False
			for l in oldfile:
				length -= 1
				msg = l.split(' ')
				try:
					curr_msgnum = int(msg[0])
					if curr_msgnum == msgnum:
						if msg[1].rstrip(':') == username:
							if length == 0:
								lastline = True
							continue # if it's the same msg then just dont add it to the new file
						else:
							return 'not yours'
					else:
						if curr_msgnum > msgnum:
							old_msg = l.split(' ', 1)
							new_msg = '{} {}'.format(curr_msgnum-1, old_msg[1])
							newfile.append(new_msg)
						else:		
							newfile.append(l)
				# if it's not a numbered message, just add it in
				except:
					newfile.append(l)
			# if no line was skipped, the message must not exist
		if len(newfile) == len(oldfile):
			return 'no msg'
		# overwrite the thread with the new file
		if lastline == True:
			newfile[-1] = newfile[-1].strip()
		print(newfile)
		with open(threadtitle, 'w') as f:
			for l in newfile:
				f.write(l)					 
		return True

	# uploads a file from client to server
	def UPD(self, threadtitle, username, filename, filesize):
		new_filename = threadtitle + '-' + filename
		# receiving the file itself
		print('receiving file of size {}'.format(filesize))
		with self.threadlock:
			with open(new_filename, 'wb') as f:
				if filesize <= 4096:
					filecontents = self.clientsocket.recv(filesize)
					f.write(filecontents)
				else:
					filecontents = self.clientsocket.recv(4096)
					f.write(filecontents)
					remaining_bytes = filesize - len(filecontents)
					while remaining_bytes > 0:
						next_buffer = min(4096, remaining_bytes)
						filecontents = self.clientsocket.recv(4096)
						if len(filecontents) < 4096:
							remaining_bytes -= len(filecontents)
						else:
							remaining_bytes -= 4096
						f.write(filecontents)
			return True

	# downloads a file from a thread (from server to client)
	# returns False if thread doesnt exist, no file if file doesnt exist, True if successful
	def DWN(self, threadtitle, filename, filesize):
		with self.threadlock:
			with open(filename, 'rb') as f:
				if filesize <= 4096:
					filecontents = f.read(filesize)
					self.clientsocket.send(filecontents)
				else:
					filecontents = f.read(4096)
					self.clientsocket.send(filecontents)
					remaining_bytes = filesize - len(filecontents)
					while remaining_bytes > 0:
						buffsize = min(4096, remaining_bytes)
						filecontents = f.read(buffsize)
						self.clientsocket.send(filecontents)
						remaining_bytes -= len(filecontents)
						print('remaining bytes: {}'.format(remaining_bytes))

	def SHT(self, password, shutdown_password):
		if password != shutdown_password:
			return False
		else:
			for f in os.listdir():
				if f != 'server.py':
					os.remove(f)
			return True

	def RMV(self, threadtitle, username):
		if threadtitle not in self.LST():
			return False
		with self.threadlock:
			with open(threadtitle, 'r') as f:
				threadlines = f.readlines()
				if threadlines[0].strip() != username:
					return 'not yours'
				else:
					# delete thread 
					os.remove(threadtitle)
					existing_threads.remove(threadtitle)	
					# delete associated files
					for l in os.listdir():
						if l.split('-')[0] == threadtitle:
							os.remove(l)
			return True

# run the client socket so that they can start receiving commands from user
	def run(self):
		while True:
			payload = dejsonify(self.clientsocket.recv(4096))
			operation = payload['operation']
			if operation == 'login':
				currentuser = payload['username']
				logins = self.readcredentials()
				if currentuser in logins:
					# now that the username is validated, tell the client 
					payload = {
								'operation':'logincheck',
								'verification':True
								}
					self.clientsocket.send(jsonify(payload))
				else:
					# username isn't in the file so we add it in
					print('New user')
					payload = {
								'operation':'logincheck',
								'verification':False
							}
					# tell the client that the user isn't registered
					self.clientsocket.send(jsonify(payload))	
					payload = dejsonify(self.clientsocket.recv(4096))
					if payload['operation'] == 'newpw':
						newpw = payload['password']
						f = open('credentials.txt', 'a+')
						newuser = currentuser + ' ' + newpw +'\n'
						f.write(newuser)
						f.close()
					loggedin_users.append(currentuser)
			# received pw from client, check against credentials
			elif operation == 'password':
				logins = self.readcredentials()
				currentpw = payload['password']
				if logins[currentuser] == currentpw:
					if currentuser in loggedin_users:
						print('{} has already logged in'.format(currentuser))
						verification = 'logged in'
					# successful login
					else:
						print('{} successful login'.format(currentuser))
						verification = True
						loggedin_users.append(currentuser)	
				else:
					print('Incorrect password')
					verification = False
				payload = {
							'operation':'passwordcheck',
							'username':currentuser,
							'verification':verification
							}
				self.clientsocket.send(jsonify(payload))
			# XIT
			elif operation == 'XIT':
				print('{} exited'.format(currentuser))
				payload = {'operation':'XITcheck'}
				self.clientsocket.send(jsonify(payload))
				threadsocketlist.remove(self.clientsocket)
				self.clientsocket.close()
				loggedin_users.remove(currentuser)
				sys.exit()
				break
			# CRT
			elif operation == 'CRT':
				print('CRT received')
				username = payload['username']
				threadtitle = payload['threadtitle']
				if threadtitle in self.LST():
					print('Thread already exists')
					# tell client thread already exists 
					payload = {
								'operation':'CRTcheck',
								'threadtitle':threadtitle,
								'ok':False
								}
					self.clientsocket.send(jsonify(payload))
				else:
					print('creating new thread')
					# if thread doesn't exist, create new one with user as first line
					with open(threadtitle, 'a')	as f:
						f.write(username)	
					# tell client thread created
					payload = {
								'operation': 'CRTcheck',
								'ok':True
								}
					self.clientsocket.send(jsonify(payload))
					# add thread name to list of existing threads
					existing_threads.append(threadtitle)
					print('Thread created')
			elif operation == 'LST':
				print('{} issued LST command'.format(currentuser))
				payload = {
							'operation':'LSTcheck',
							'ok':True,
							'threadlist':self.LST()
							}
				self.clientsocket.send(jsonify(payload))
			elif operation == 'MSG':
				print('{} issued MSG command'.format(currentuser)) 	
				# if the message was successfully posted, let the client know
				ok = self.MSG(payload['username'], payload['threadtitle'], payload['message'])
				payload = {
							'operation':'MSGcheck',
							'threadtitle':payload['threadtitle'],
							'ok':ok
							}
				self.clientsocket.send(jsonify(payload))
			elif operation == 'RDT':
				print('{} issued RDT command'.format(currentuser))
				threadtitle = payload['threadtitle']
				threadcontent = self.RDT(threadtitle)
				if threadcontent == False:
					payload = {
								'operation':'RDTcheck',
								'threadtitle':threadtitle,
								'ok':False
								}
				else:
					print('Thread {} read'.format(threadtitle))
					payload = {
								'operation':'RDTcheck',
								'threadtitle':threadtitle,
								'ok':True,
								'threadcontent':threadcontent
								}
				self.clientsocket.send(jsonify(payload))
			elif operation == 'EDT':
				print('{} issued EDT command'.format(currentuser))
				threadtitle = payload['threadtitle']
				msgnum = payload['msgnum']
				msg = payload['msg']
				username = payload['username']
				ok = self.EDT(threadtitle, msgnum, msg, username)
				payload = {
							'operation':'EDTcheck',
							'ok':ok,
							'threadtitle':threadtitle,
							'msgnum':msgnum
							}
				# let the client know if the edit was successful
				self.clientsocket.send(jsonify(payload))
			elif operation == 'DLT':
				print('{} issued DLT command'.format(currentuser))
				threadtitle = payload['threadtitle']
				username = payload['username']
				msgnum = payload['msgnum']
				ok = self.DLT(threadtitle, username, msgnum)	
				payload = {
							'operation':'DLTcheck',
							'threadtitle':threadtitle,
							'msgnum':msgnum,
							'ok':ok
							}					
				# let the client know if the delete was successful
				self.clientsocket.send(jsonify(payload))
			elif operation == 'UPD':
				print('{} issued UPD command'.format(currentuser))
				threadtitle = payload['threadtitle']
				filename = payload['filename']
				username = payload['username']
				filesize = payload['filesize']
				if threadtitle not in self.LST():
					ok = False
				else:
					ok = True
				payload = {'operation':'UPDcheck1',
							'threadtitle':threadtitle,
							'ok':ok}
				# notify client if the thread exists
				self.clientsocket.send(jsonify(payload))
				if ok == False:
					print('Thread {} does not exist'.format(threadtitle))
					continue
				# if thread does exist, receive the file
				if ok == True:
					self.UPD(threadtitle, username, filename, filesize)
					payload = {
								'operation':'UPDcheck',
								'ok':True,
								'filename':filename,
								'threadtitle':threadtitle
								}
					# add message to thread showing the file was upladed
					threadmsg = '{} uploaded {}'.format(username, filename)
					with open(threadtitle, 'a') as f:
						f.write('\n')
						f.write(threadmsg)	
					print('File {} successfully uploaded to {}'.format(filename, threadtitle))
					self.clientsocket.send(jsonify(payload))
			elif operation == 'SHT':
				print('{} issued SHT command'.format(currentuser))
				password = payload['password']
				ok = self.SHT(password, shutdown_password)
				payload = {
							'operation':'SHTcheck',
							'ok':ok
							}
				if ok == True:
					for c in threadsocketlist:
						c.send(jsonify(payload))
					print('Server shutting down')
#					os.kill(os.getpid(), signal.SIGTERM)
#					sys.exit()
					os._exit(os.EX_OK)
				else:
					self.clientsocket.send(jsonify(payload))
					print('Incorrect password')
			elif operation == 'RMV':
				print('{} issued RMV command'.format(currentuser))
				threadtitle = payload['threadtitle']
				username = payload['username']
				ok = self.RMV(threadtitle, username)
				payload = {
							'operation':'RMVcheck',
							'threadtitle':threadtitle,
							'ok':ok
						}
				self.clientsocket.send(jsonify(payload))
				if ok == True:
					print('Thread {} removed'.format(threadtitle))
			elif operation == 'DWN':
				print('{} issued DWN command'.format(currentuser))
				threadtitle = payload['threadtitle']
				filename = payload['filename']
				# check if file/thread exists, if they do then tell the client how big the file is before sending
				dwn_file_name = threadtitle + '-' + filename
				ok = True
				filesize = -1
				if not os.path.exists(dwn_file_name):
					ok = 'no file'
				else:
					filesize = os.path.getsize(dwn_file_name)
					print('FILESIZE: {}'.format(filesize))
				if threadtitle not in self.LST():
					ok = False	
				payload = {
							'operation':'DWNcheck',
							'ok':ok,
							'filesize':filesize,
							'threadtitle':threadtitle,
							'filename':filename
							}
				print('sending dwncheck')
				self.clientsocket.send(jsonify(payload))
				print('sent dwncheck')
				if ok != True: # stop the operation if file or thread doesn't exist
					continue
#				time.sleep(0.2)
				# receive ok from client to start sending file  
				if self.DWN(threadtitle, dwn_file_name, filesize):
					print('File successfully downloaded')
					continue		

# thread class which just creates clientsocket and runs it
class ClientThread(threading.Thread):
	def __init__(self, clientsocket, threadlock):
		threading.Thread.__init__(self)
		self.clientsocket = clientsocket
		self.threadlock = threadlock
	
	def run(self):
		clientsocket = ClientSocket(self.clientsocket, self.threadlock)
		clientsocket.run()

# socket which communicates with client. Create a new one of these for each thread/connected client
class Server:
	def __init__(self, server_port, shutdown_password):
		self.server_port = server_port
		self.shutdown_password = shutdown_password
		self.serverip = '127.0.0.1'
		self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	def start(self):
		self.serversocket.bind((self.serverip, self.server_port))
		self.serversocket.listen(8)
		print('Opened TCP port for listening on ip {} and port number {}'.format(self.serverip, self.server_port))  
		threadlock = threading.Lock()
		while True:
			(clientsocket, address) = self.serversocket.accept()
			print('Client connected')
			thread = ClientThread(clientsocket, threadlock)
			thread.setDaemon(True)
			thread.start()
			threadsocketlist.append(clientsocket)
			processlist.append(thread)	
		
if __name__ == '__main__':
	if len(sys.argv) != 3:
		print('Usage: python server.py [server_port] [admin_password]')
		sys.exit()
	try:			
		portnumber = int(sys.argv[1])
		shutdown_password = sys.argv[2]
		# create a listening socket which is always on
		serversocket = Server(portnumber, shutdown_password)
		serversocket.start()
	except Exception as e:
		print('Invalid syntax')
		sys.exit()
