# python 3.6

import random
import time
from gpiozero import Button

from paho.mqtt import client as mqtt_client

button = Button(16)

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
    flag = 1
    while True:
        if button.is_pressed:
            if flag == 1:
                msg = "on"
                client.publish(topic, msg)
                print(f"Send {msg} to topic {topic}")
                client.publish("home/cpu/hans", "1.1")
                time.sleep(1)
                flag = 0
        if button.is_pressed:
            if flag == 0:
                msg = "off"
                client.publish(topic, msg)
                print(f"Send {msg} to topic {topic}")
                client.publish('home/cpu/hans','0.1')
                time.sleep(1)
                flag = 1

def run():
    client = connect_mqtt()
    client.loop_start()
    publish(client)


if __name__ == '__main__':
    run()

