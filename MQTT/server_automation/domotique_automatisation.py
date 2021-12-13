import paho.mqtt.client as mqtt
import time
from Home import Home
from Room import Room
from Element import Element

BROKER_ADDRESS = '192.168.43.222'
TOPIC_DICT = {}
home = Home()

def on_message(client, userdata, message):
    
    element = TOPIC_DICT[message.topic]
    data_name = list(element.data.keys())[0]
    element.data[data_name] = str(message.payload.decode("utf-8"))
    print(element.name,element.data)


def init_mqtt_connection():
    # Init MQTT Client
    client = mqtt.Client('server_automation') #Create a client and set it ID
    client.connect(BROKER_ADDRESS) # Set the IP of the broker client and connect to it
    client.on_message=on_message #callback function
    client.loop_start() 

    return client

def init_my_home(mqtt_client):
    
    #living room
        #sensor
    sensor_temperature_living_room = Element('sensor_temperature_living_room', 'sensor', {"temperature":0}) #temperature in degrès Celcius
    sensor_humidity_living_room = Element('sensor_humidity_living_room', 'sensor', {"humidity":0}) #percentage of humidity
        #actuator
    actuator_vmc_living_room = Element('actuator_vmc_living_room', 'actuator', {"state": "OFF"}) #state OFF or ON
    actuator_entry_door_living_room = Element('actuator_entry_door_living_room', 'actuator', {"state": "OPEN"}) #state OPEN or CLOSE
    actuator_shutter_living_room = Element('actuator_shutter_living_room', 'actuator', {"state": "OPEN"}) #state OPEN or CLOSE
    actuator_heating_living_room = Element('actuator_heating_living_room', 'actuator', {"state": "OFF"}) #state OFF or ON

    living_room_elements_list = [sensor_temperature_living_room, sensor_humidity_living_room, actuator_vmc_living_room,
    actuator_entry_door_living_room, actuator_shutter_living_room, actuator_heating_living_room]
    #bed room 1
        #sensor
        #actuator
    

    rooms_list = [Room('living_room', living_room_elements_list), Room('bed_room_1', [])]
    home = Home('home', rooms_list, mqtt_client)

    # Creation of the topic
    home_topic = f'{home.name}/'
    for room_name, room in home.rooms.items():
        room_topic = home_topic + f'{room.name}/'
        for element_name, element in room.elements.items():
            element_topic = room_topic + f'{element.name}'.replace(f'_{room.name}', '')
            element_topic += f'/{list(element.data.keys())[0]}'
            element.topic = element_topic
            TOPIC_DICT[element.topic] = element
        
            home.mqtt_client.subscribe(element.topic)
    return home


if __name__ == "__main__":
    mqtt_client = init_mqtt_connection()
    home = init_my_home(mqtt_client)
    while True:
        time.sleep(0.5)

        #region check temperature in living room
        temperature_living_room = float(home.rooms['living_room'].elements['sensor_temperature_living_room'].data['temperature'])
        
        vmc_living_room = home.rooms['living_room'].elements['actuator_vmc_living_room']
        heating_living_room = home.rooms['living_room'].elements['actuator_heating_living_room']
        
        if temperature_living_room < 25 and temperature_living_room > 18:
            # switch offthe VMC
            if vmc_living_room.data['state']  != "OFF" :
                home.mqtt_client.publish(vmc_living_room.topic , "OFF")
            # switch off the heating
            if heating_living_room.data['state'] != "OFF" :
                home.mqtt_client.publish(heating_living_room.topic, "OFF")
        if temperature_living_room > 25:
            # powered the VMC
            if vmc_living_room.data['state'] != "ON" :
                home.mqtt_client.publish(vmc_living_room.topic , "ON")
            # switch off the heating
            if heating_living_room.data['state'] != "OFF" :
                home.mqtt_client.publish(heating_living_room.topic, "OFF")
        elif temperature_living_room <18:
            # switch offthe VMC
            if vmc_living_room.data['state']  != "OFF" :
                home.mqtt_client.publish(vmc_living_room.topic , "OFF")
            # powered the heating
            if heating_living_room.data['state'] != "ON" :
                home.mqtt_client.publish(heating_living_room.topic, "ON")
        #endregion

        #region check humidity in living room
        humidity_living_room = float(home.rooms['living_room'].elements['sensor_humidity_living_room'].data['humidity'])
        
        if humidity_living_room >= 0 and humidity_living_room < 25:
            # switch offthe VMC
            if vmc_living_room.data['state']  != "OFF" :
                home.mqtt_client.publish(vmc_living_room.topic , "OFF")
        if humidity_living_room >= 25 and humidity_living_room <= 100:
            # powered the VMC
            if vmc_living_room.data['state'] != "ON" :
                home.mqtt_client.publish(vmc_living_room.topic , "ON")
        #endregion

