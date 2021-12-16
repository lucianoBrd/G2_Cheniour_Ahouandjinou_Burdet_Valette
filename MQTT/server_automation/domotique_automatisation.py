import paho.mqtt.client as mqtt
import time
from Home import Home
from Room import Room
from Element import Element
from Api import Api
from decouple import config
from clint.textui import colored, puts, indent

BROKER_ADDRESS = '192.168.43.222'
TOPIC_DICT = {}
home = Home()
PASSWORD_MEMORY = ''

def on_message(client, userdata, message):
    
    print(colored.green(f"Data received from topic : {message.topic}"))
    if message.topic.split("/")[-1] != 'acquittement':
        print(colored.blue("Value received :"))
        element = TOPIC_DICT[message.topic]
        data_name = list(element.data.keys())[0]
        element.data[data_name] = str(message.payload.decode("utf-8"))
        print(colored.blue(f"{data_name} : {element.data[data_name]} from {element.name}"))
    else:
        print(colored.blue("Acquitement received :"))
        element = TOPIC_DICT[message.topic.replace('/acquittement', '')]
        element.acquittement = str(message.payload.decode("utf-8"))
        print(colored.blue(f"Acquittement : {element.acquittement} from {element.name}"))
    
    print('\n')



def init_mqtt_connection():
    # Init MQTT Client
    client = mqtt.Client('server_automation') #Create a client and set it ID
    client.connect(BROKER_ADDRESS) # Set the IP of the broker client and connect to it
    client.on_message=on_message #callback function
    client.loop_start() 

    return client

def init_my_home(mqtt_client, api_obj):
    
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

    #Creation data in BDD
    api_obj.create_home(home.name)

    for room_name, room_objr in home.rooms.items():
        api_obj.create_room(room_name, home.name)
        for element_name, element_obj in home.rooms[room_name].elements.items():
            api_obj.create_element(element_name, room_name, element_obj.type)
    
    #  Creation of the topics
    home_topic = f'{home.name}/'
    for room_name, room in home.rooms.items():
        room_topic = home_topic + f'{room.name}/'
        for element_name, element in room.elements.items():
            element_topic = room_topic + f'{element.name}'.replace(f'_{room.name}', '')
            element_topic += f'/{list(element.data.keys())[0]}'
            element.topic = element_topic
            TOPIC_DICT[element.topic] = element
            
            if element.type == "sensor":
                home.mqtt_client.subscribe(element.topic)
    return home

def check_and_format_actions_api (api_obj):
    unresolved_actions = api_obj.get_unresolved_actions()
    unresolved_formated_actions = []
    if unresolved_actions == [] or unresolved_actions == None:
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
    
def check_password_change(api_obj, password_memory):
    password = api_obj.get_home_by_name(home.name)['password']

    if password != password_memory:
        home.mqtt_client.publish("home/security/entry_code", password)
        return password
    else :
        return password_memory
    

def format_value_publish(value):
    dict_value = {"ON":1, 'OFF':0}
    if value == "ON" or value == "OFF":
        return dict_value[value]
    else:
        return value

def traitment_sensor_value(data_name, value_to_reach):
    if data_name == "temperature":
        temperature_to_reach = int(value_to_reach)
        pass
    elif data_name == "humidity":
        humidity_to_reach = int(value_to_reach)

def get_room_obj(home_obj, room_name):
    return home_obj.rooms[room_name]

def get_element_obj(home_obj, room_name, element_name):
    return home_obj.rooms[room_name].elements[element_name]

if __name__ == "__main__":
    mqtt_client = init_mqtt_connection()
    web_api = Api()
    home = init_my_home(mqtt_client, web_api)
    
    while True:
        time.sleep(0.5)

        #uniquement en mode automatique ??
        #region check temperature in living room
        sensor_temperature = get_element_obj(home, 'living_room', 'sensor_temperature_living_room')
        temperature_living_room = float(sensor_temperature.data['temperature'])
        web_api.create_value(temperature_living_room, sensor_temperature.name)

        vmc_living_room = get_element_obj(home, 'living_room', 'actuator_vmc_living_room')
        heating_living_room = get_element_obj(home, 'living_room', 'actuator_heating_living_room')
        
        sensor_humidity = get_element_obj(home, 'living_room', 'sensor_humidity_living_room')
        humidity_living_room = float(sensor_humidity.data['humidity'])
        web_api.create_value(humidity_living_room, sensor_humidity.name)

        #uniquement en mode manuel ??
        unresolved_actions = check_and_format_actions_api(web_api)
        if unresolved_actions != None:
            print(unresolved_actions)
            for action in unresolved_actions:
                print(action)
                if action["value"] != "":
                    element = get_element_obj(home, action['room'], action['element'])
                    if element.type == "actuator":
                        home.mqtt_client.subscribe(element.topic +"/acquittement")
                        home.mqtt_client.publish(element.topic, format_value_publish(action["value"]))
                        time.sleep(0.1)
                        while ( element.acquittement != f"{action['value']} RECEIVED") :
                            time.sleep(0.5)
                            home.mqtt_client.publish(element.topic, format_value_publish(action["value"]))
                        home.mqtt_client.unsubscribe(element.topic +"/acquittement")
                        web_api.update_action(action["action_id"], value = action["value"], state = True)

                    elif element.type == "sensor":
                        if element.data.keys()[0] == "temperature":
                            temperature_to_reach = int(element.data["temperature"])
                            if temperature_living_room < temperature_to_reach + 0.5 and temperature_living_room > temperature_to_reach - 0.5:
                                # switch offthe VMC
                                if vmc_living_room.data['state']  != "OFF" :
                                    home.mqtt_client.publish(vmc_living_room.topic , "OFF")
                                    vmc_living_room.data['state'] = "OFF"

                                # switch off the heating
                                if heating_living_room.data['state'] != "OFF" :
                                    home.mqtt_client.publish(heating_living_room.topic, "OFF")
                                    heating_living_room.data["state"] = "OFF"

                                web_api.update_action(action["action_id"], state = True)

                            if temperature_living_room > temperature_to_reach + 0.5:
                                # powered the VMC
                                if vmc_living_room.data['state'] != "ON" :
                                    home.mqtt_client.publish(vmc_living_room.topic , "ON")
                                    vmc_living_room.data['state'] = "ON"

                                # switch off the heating))
                                if heating_living_room.data['state'] != "OFF" :
                                    home.mqtt_client.publish(heating_living_room.topic, "OFF")
                                    heating_living_room.data['state'] = "OFF"

                            elif temperature_living_room < temperature_to_reach - 0.5:
                                # switch offthe VMC
                                if vmc_living_room.data['state'] != "OFF" :
                                    home.mqtt_client.publish(vmc_living_room.topic , "OFF")
                                    vmc_living_room.data['state'] = "OFF"
                                # powered the heating
                                if heating_living_room.data['state'] != "ON" :
                                    home.mqtt_client.publish(heating_living_room.topic, "ON")
                                    heating_living_room.data['state'] = "ON"
                            #endregion

                        #region check humidity in living room
                        if element.data.keys()[0] == "humidiy":
                            humidity_to_reach = int(element.data["humidity"])
                            if humidity_living_room >= 0 and humidity_living_room < humidity_to_reach:
                                # switch offthe VMC
                                if vmc_living_room.data['state'] != "OFF" :
                                    home.mqtt_client.publish(vmc_living_room.topic , "OFF")
                                    vmc_living_room.data['state'] = "OFF"

                            if humidity_living_room >= humidity_to_reach and humidity_living_room <= 100:
                                # powered the VMC
                                if vmc_living_room.data['state'] != "ON" :
                                    home.mqtt_client.publish(vmc_living_room.topic , "ON")
                                    vmc_living_room.data['state'] = "ON"

                                web_api.update_action(action["action_id"], state = True)

                        #endregion
                else :
                    web_api.update_action(action["action_id"], state = True)
            
        # envoi du new password s'il est modifié en BDD: 
        PASSWORD_MAMORY = check_password_change(web_api, PASSWORD_MEMORY)
        

