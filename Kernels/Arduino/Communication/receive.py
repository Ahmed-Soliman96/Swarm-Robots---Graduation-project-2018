import subprocess
import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

#subprocess.call("/home/pi/bowlingsampler/RPi/music_play.sh", shell=True)

GPIO.setmode(GPIO.BCM)

#pipes = [[0xF0, 0xF0, 0xF0, 0xF0, 0xD2], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]
#pipes = [[0xC3, 0x3C, 0x33, 0xCE, 0x3E, 0xE3, 0xEC], [0xC3, 0x3C, 0x33, 0xCE, 0x3E, 0xE3, 0xEC]]
pipes = [[0xF0, 0xF0, 0xF0, 0xF0, 0xD2], [0xF0, 0xF0, 0xF0, 0xF0, 0xD2]]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 17)

radio.setPayloadSize(32)
radio.setChannel(0x76)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)

radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()

radio.openWritingPipe(pipes[0])
radio.openReadingPipe(1, pipes[1])
radio.printDetails()
time.sleep(1)


def position(robot_no):
	#getting_position_function
	pass

def listen():
	while(1):
		radio.startListening()
		try:
			received = [-1]
			received_list = []

			radio.read(received, radio.getDynamicPayloadSize())
			for i in range(len(received)):
				received_list.append(int(chr(received[i])))

			if ((received_list[0] > 0) and (received_list[1] == 0)): #check if it's from a device to the RPI or not
				print(received_list)
				radio.stopListening()
				radio.write(position(received_list[0]))

		except KeyboardInterrupt:
			GPIO.cleanup()
			break

listen()
