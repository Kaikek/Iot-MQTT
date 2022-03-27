# python 3.6

import random
import time
import adafruit_dht
from gpiozero import LED

from paho.mqtt import client as mqtt_client

dht_device = adafruit_dht.DHT11(23)
led = LED(20)

broker = '192.168.0.208'
port = 1883
topic = "bruh/lmao"
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 1000)}'
username = 'wail'
password = 'Admin'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def publish(client):
    flag = 0
    while True:
        try:
            temperature = dht_device.temperature
        except RuntimeError:
            continue
        if temperature is not None:
            print(f'Temp is {temperature}')
            client.publish("home/cpu/hons", temperature)
        else:
            print('nope')
        time.sleep(3)


        if temperature >= 25 and flag == 0:
            client.publish("home/cpu/hins", "0.2")
            flag = 1
        elif temperature < 25 and flag == 1:
            client.publish("home/cpu/hins", "1.2")
            flag = 0


def run():
    client = connect_mqtt()
    client.loop_start()
    publish(client)


if __name__ == '__main__':
    run()

