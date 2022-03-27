import re
from typing import NamedTuple
from gpiozero import LED
import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient

led = LED(21)
ledg = LED(20)

INFLUXDB_ADDRESS = '192.168.0.208'
INFLUXDB_USER = 'wail'
INFLUXDB_PASSWORD = 'Admin'
INFLUXDB_DATABASE = 'home'

MQTT_ADDRESS = '192.168.0.208'
MQTT_USER = 'wail'
MQTT_PASSWORD = 'Admin'
MQTT_TOPIC = 'home/+/+'
MQTT_REGEX = 'home/([^/]+)/([^/]+)'
MQTT_CLIENT_ID = 'home'

influxdb_client = InfluxDBClient(INFLUXDB_ADDRESS, 8086, INFLUXDB_USER, INFLUXDB_PASSWORD, None)

class SensorData(NamedTuple):
    location: str
    measurement: str
    value: float

def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    client.subscribe(MQTT_TOPIC)

def _parse_mqtt_message(topic, payload):
    match = re.match(MQTT_REGEX, topic)
    if match and topic != 'home/cpu/measurement' and payload != 'nan':
        location = match.group(1)
        measurement = match.group(2)
        if measurement == 'status':
            return None
	
        return SensorData(location, measurement, float(payload))
    else:
        return None

def _send_sensor_data_to_influxdb(sensor_data):
    json_body = [
        {
            'measurement': sensor_data.measurement,
            'tags': {
                'location': sensor_data.location
            },
            'fields': {
                'value': sensor_data.value
            }
        }
    ]
    influxdb_client.write_points(json_body)

def on_message(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    if msg.topic != 'home/cpu/measurement' and msg.payload != 'nan':
        message = msg.payload.decode('utf-8')
        print(msg.topic + ' ' + message)
        sensor_data = _parse_mqtt_message(msg.topic, msg.payload.decode('utf-8'))
        if float(message) == 1:
            led.on()
            client.publish('bruh/lmao', 'red ON')
        elif float(message) == 0:
            led.off()
            client.publish('bruh/lmao', 'red OFF')
        if float(message) == 3:
            print('yes')

        if float(message) == 1.2:
            ledg.on()
            client.publish('bruh/lmao', 'green ON')
        elif float(message) == 0.2:
            ledg.off()
            client.publish('bruh/lmao', 'green OFF')

        if sensor_data is not None:
            _send_sensor_data_to_influxdb(sensor_data)

def _init_influxdb_database():
    databases = influxdb_client.get_list_database()
    if len(list(filter(lambda x: x['name'] == INFLUXDB_DATABASE, databases))) == 0:
        influxdb_client.create_database(INFLUXDB_DATABASE)
    influxdb_client.switch_database(INFLUXDB_DATABASE)

def main():
    _init_influxdb_database()

    mqtt_client = mqtt.Client(MQTT_CLIENT_ID)
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
