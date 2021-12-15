import paho.mqtt.client as mqtt
import time
from Home import Home
from Room import Room
from Element import Element
from api import API
from decouple import config


BROKER_ADDRESS = '192.168.43.222'
TOPIC_DICT = {}
home = Home()

def on_message(client, userdata, message):
    
    if message.topic.split("/")[-1] != 'acquitement':
        element = TOPIC_DICT[message.topic]
        data_name = list(element.data.keys())[0]
        element.data[data_name] = str(message.payload.decode("utf-8"))
        print(element.name,element.data)
    else:
        element = TOPIC_DICT[message.topic.replace('/acquitement', '')]
        element.acquitement = str(message.payload.decode("utf-8"))


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
    
    rooms_list = [Room(config("ROOM_NAME_1"), living_room_elements_list), Room(config("ROOM_NAME_2"), [])]
    home = Home(config("HOME_NAME"), rooms_list, mqtt_client)

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

def check_and_format_actions_api (api_obj):
    api_obj = API()
    unresolved_actions = api_obj.get_unresolved_actions()

    unresolved_formated_actions = []
    if unresolved_actions == []:
        return None
    else :
        for action in unresolved_actions:
            action_id = action["id"]
            element_id = action["element"].split("/")[-1]
            element_name = api_obj.get_element_name_by_id(element_id)

            room_id = api_obj.get_element_by_name(element_name)["room"].split("/")[-1]
            room_name = api_obj.get_room_name_by_id(room_id)

            home_id = api_obj.get_room_by_name(room_name)["home"].split("/")[-1]
            home_name = api_obj.get_home_name_by_id(home_id)

            value = action["value"]

            action_formated = {"action_id" : action_id, "home" : home_name, "room" : room_name, "element" : element_name, "value" : value}

            unresolved_formated_actions.append(action_formated)
        return unresolved_formated_actions
    
def get_room_obj(home_obj, room_name):
    return home_obj.rooms[room_name]

def get_element_obj(home_obj, room_name, element_name):
    return home_obj.rooms[room_name].elements[element_name]


if __name__ == "__main__":
    #mqtt_client = init_mqtt_connection()
    #home = init_my_home(mqtt_client)
    home = init_my_home(mqtt.Client("test"))
    web_api = API()

    while True:
        time.sleep(0.5)

        #uniquement en mode automatique ??
        #region check temperature in living room
        #########CREER UN FONCTION POUR RECUPERER OBJETS EN FONCTION DU NOM ETC
        temperature_living_room = float(get_element_obj(home, 'living_room', 'sensor_temperature_living_room').data['temperature'])
        
        vmc_living_room = get_element_obj(home, 'living_room', 'actuator_vmc_living_room')
        heating_living_room = get_element_obj(home, 'living_room', 'actuator_heating_living_room')
        
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
        humidity_living_room = float(get_element_obj(home, 'living_room', 'sensor_humidity_living_room').data['humidity'])
        
        if humidity_living_room >= 0 and humidity_living_room < 25:
            # switch offthe VMC
            if vmc_living_room.data['state']  != "OFF" :
                home.mqtt_client.publish(vmc_living_room.topic , "OFF")
        if humidity_living_room >= 25 and humidity_living_room <= 100:
            # powered the VMC
            if vmc_living_room.data['state'] != "ON" :
                home.mqtt_client.publish(vmc_living_room.topic , "ON")
        #endregion

        #uniquement en mode manuel ??
        unresolved_actions = check_and_format_actions_api(web_api)
        if unresolved_actions != None:
            for action in unresolved_actions:
                element = get_element_obj(home, action['room'], action['element'])
                home.mqtt_client.subcribe(element.topic +"/acquitement")
                while (not (element.acquitement == "ON RECEIVED" or element.acquitement == "OFF RECEIVED")) :
                    home.mqtt_client.publish(element.topic, action["value"])
                home.mqtt_client.unsubcribe(element.topic +"/acquitement")
                web_api.update_action(action["id"], action["value"], element.name, True)

