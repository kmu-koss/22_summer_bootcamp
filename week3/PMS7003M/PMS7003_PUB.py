"""
*******************************************
* PMS7003 데이터 수신 프로그램 import 예제
* 수정 : 2018. 08. 27
* 제작 : eleparts 부설연구소
* SW ver. 1.0.1
*******************************************
# unpack_data(buffer)
# data list
HEADER_HIGH            = 0x42
HEADER_LOW             = 0x4d
FRAME_LENGTH           = 2x13+2(data+check bytes)
DUST_PM1_0_CF1         = PM1.0 concentration unit μ g/m3（CF=1，standard particle）
DUST_PM2_5_CF1         = PM2.5 concentration unit μ g/m3（CF=1，standard particle）
DUST_PM10_0_CF1        = PM10 concentration unit μ g/m3（CF=1，standard particle）
DUST_PM1_0_ATM         = PM1.0 concentration unit μ g/m3（under atmospheric environment）
DUST_PM2_5_ATM         = PM2.5 concentration unit μ g/m3（under atmospheric environment）
DUST_PM10_0_ATM        = PM10 concentration unit μ g/m3  (under atmospheric environment)
DUST_AIR_0_3           = indicates the number of particles with diameter beyond 0.3 um in 0.1 L of air.
DUST_AIR_0_5           = indicates the number of particles with diameter beyond 0.5 um in 0.1 L of air.
DUST_AIR_1_0           = indicates the number of particles with diameter beyond 1.0 um in 0.1 L of air.
DUST_AIR_2_5           = indicates the number of particles with diameter beyond 2.5 um in 0.1 L of air.
DUST_AIR_5_0           = indicates the number of particles with diameter beyond 5.0 um in 0.1 L of air.
DUST_AIR_10_0          = indicates the number of particles with diameter beyond 10 um in 0.1 L of air.
RESERVEDF              = Data13 Reserved high 8 bits
RESERVEDB              = Data13 Reserved low 8 bits
CHECKSUM               = Checksum code
# CF=1 should be used in the factory environment
"""
import serial
import json
from paho.mqtt import client as mqtt
from PMS7003 import PMS7003
from typing import Final

dust = PMS7003()

# 싱수 선언
# Baud Rate
Speed: Final = 9600
# UART / USB Serial
USB0: Final = '/dev/ttyUSB0'
UART: Final = '/dev/tty.usbserial-14330'
# 발행할 토픽 이름
TOPIC: Final = 'PMS7003'
# 브로커 서버 주소
BROKER_SERVER: Final = 'localhost'
PORT: Final = 1883
# USE PORT
SERIAL_PORT: Final = UART

# serial setting
ser = serial.Serial(SERIAL_PORT, Speed, timeout=1)


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connected OK")
    else:
        print("Bad connection Returned code=", rc)

def on_disconnect(client, userdata, flags, rc=0):
    print(str(rc))

def on_publish(client, userdata, mid):
    print("In on_pub callback mid= ", mid)


client = mqtt.Client()

client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_publish = on_publish

client.connect(BROKER_SERVER, PORT)
client.loop_start()

while (True):
    buffer = ser.read(1024)
    if dust.protocol_chk(buffer):
        data = dust.unpack_data(buffer)

        json_object = {
            "PM1_0": data[dust.DUST_PM1_0_ATM],
            "PM2_5": data[dust.DUST_PM2_5_ATM],
            "PM10_0": data[dust.DUST_PM10_0_ATM]
        }

        json_string = json.dumps(json_object)
        print(json_string)
        client.publish(TOPIC, json_string, 1)
    else:
        print("data read Err")
client.loop_stop()
ser.close()
