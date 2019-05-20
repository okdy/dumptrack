import socket

def run_server():

	with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:

		sock.bind(('127.0.0.1', 1234))

		while 1:
			msg = sock.recv(1024)
			msg = msg.decode()

			if msg == 'q':
				sock.close()

			print(msg)

if __name__ == '__main__':

	run_server()