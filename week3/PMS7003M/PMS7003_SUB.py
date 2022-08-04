import os.path
import time
import pandas
import json
from typing import Final
from paho.mqtt import client as mqtt
import keyboard

# 브로커 서버 주소
BROKER_SERVER: Final = 'localhost'
PORT: Final = 1883
# 구독할 토픽 이름
TOPIC: Final = 'PMS7003'

json_data: dict = {}

def on_message(client, userdata, message):
    time.sleep(1)
    recv_data = str(message.payload.decode("utf-8"))
    print("received message =", recv_data)
    global json_data
    json_data = json.loads(recv_data)


def monitoring():
    cx_date = time.strftime('%Y%m%d', time.localtime(time.time()))
    out_file_name = "PMS7003M_Monitoring_Data_%s" % cx_date

    result: pandas.DataFrame

    if not os.path.exists("./%s.csv" % out_file_name):
        result = pandas.DataFrame(columns=["PM1_0", "PM2_5", "PM10_0"])
    else:
        result = pandas.read_csv("./%s.csv" % out_file_name, index_col=0, sep=",")

    cx_time = time.strftime("%H:%M:%S", time.localtime(time.time()))
    result_tmp = pandas.DataFrame(json_data, index=[cx_time])
    result = result.append(result_tmp)

    if not os.path.exists("./%s.csv" % out_file_name):
        result.to_csv("./%s.csv" % out_file_name, mode="w", encoding="utf-8")
    else:
        result_tmp.to_csv("./%s.csv" % out_file_name, mode="a", encoding="utf-8", header=False)


client = mqtt.Client()
client.on_message = on_message
client.connect(BROKER_SERVER)  # connect
client.loop_start()
client.subscribe(TOPIC)

try:
    while True:
        if keyboard.is_pressed("esc"):
            print("모니터링을 종료합니다")
            break

        monitoring()

        time.sleep(1)
except KeyboardInterrupt:
    print("모니터링을 종료합니다.")
    exit()


client.disconnect()  # disconnect
client.loop_stop() # stop loop



