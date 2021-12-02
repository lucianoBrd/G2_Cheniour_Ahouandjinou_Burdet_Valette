import paho.mqtt.client as mqtt
import time
from Home import Home
from Room import Room
from Element import Element

BROKER_ADDRESS = '127.0.0.1'
home = Home()

def on_message(client, userdata, message):
    message
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)

def init_mqtt_connection():
    # Init MQTT Client
    client = mqtt.Client('server_automation') #Create a client and set it ID
    client.connect(BROKER_ADDRESS) # Set the IP of the broker client and connect to it
    client.on_message=on_message #callback function
    client.loop_start() 

    # print("Subscribing to topic","house/bulbs/bulb1")
    # client.subscribe("house/bulbs/bulb1")
    # print("Publishing message to topic","house/bulbs/bulb1")
    # client.publish("house/bulbs/bulb1","OFF")
    # time.sleep(4) # wait
    #client.loop_stop() #stop the loop
    return client

def init_my_home(mqtt_client):
    
    #living room
        #sensor
    sensor_temperature_living_room = Element('sensor_temperature_living_room', 'sensor')
    sensor_humidity_living_room = Element('sensor_humidity_living_room', 'sensor')
        #actuator
    actuator_vmc_living_room = Element('actuator_vmc_living_room', 'actuator')
    actuator_entry_door_living_room = Element('actuator_entry_door_living_room ', 'actuator')
    actuator_shutter_living_room = Element('actuator_shutter_living_room', 'actuator')
    actuator_plug_living_room = Element('actuator_plug_living_room', 'actuator')

    living_room_elements_list = [sensor_temperature_living_room, sensor_humidity_living_room, actuator_vmc_living_room,
    actuator_entry_door_living_room, actuator_shutter_living_room, actuator_plug_living_room]
    #bed room 1
        #sensor
        #actuator
    

    rooms_list = [Room('living_room', living_room_elements_list), Room('bed_room_1', [])]
    home = Home('home', rooms_list, mqtt_client)

    # Creation of the topic
    home_topic = f'{home.name}/'
    print(home.rooms)
    for room_name, room in home.rooms.items():
        room_topic = home_topic + f'{room.name}/'
        for element_name, element in room.elements.items():
            element_topic = room_topic + f'{element.name}'.replace(f'_{room.name}', '')
            element.topic = element_topic
            print(element_topic)
    return home


if __name__ == "__main__":
    mqtt_client = init_mqtt_connection()
    home = init_my_home(mqtt_client)
    while True:
        pass
#publish : client.publish("house/light","ON")
#subscribe : client.subscribe("house/bulbs/bulb1")
