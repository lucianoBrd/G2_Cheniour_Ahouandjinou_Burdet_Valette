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
MODE = "normal"

def on_message(client, userdata, message):
    
    print(colored.green(f"Data received from topic : {message.topic}"))
    if message.topic.split("/")[-1] != 'acquittement':
        element = TOPIC_DICT[message.topic]
        print(colored.blue(f"Value received from {element.name}: "), end='')
        data_name = list(element.data.keys())[0]
        element.data[data_name] = str(message.payload.decode("utf-8"))
        print(colored.magenta(f"{data_name} = {element.data[data_name]}"))
    else:
        print(colored.cyan("Acquitement received :"), end = '')
        element = TOPIC_DICT[message.topic.replace('/acquittement', '')]
        element.acquittement = str(message.payload.decode("utf-8"))
        print(colored.cyan(f"Acquittement : {element.acquittement} from {element.name}"))
    
    print('\n')

def init_mqtt_connection():
    # Init MQTT Client
    client = mqtt.Client('server_automation') #Create a client and set it ID
    client.connect(BROKER_ADDRESS) # Set the IP of the broker client and connect to it
    client.on_message=on_message #callback function
    client.loop_start() 

    return client

def init_my_home(mqtt_client, api_obj):
    
    api_obj.create_type("sensor")
    api_obj.create_type("actuator")

    #living room
        #sensor
    sensor_temperature_living_room = Element('sensor_temperature_living_room', 'sensor', {"temperature":0}) #temperature in degrès Celcius
    sensor_humidity_living_room = Element('sensor_humidity_living_room', 'sensor', {"humidity":0}) #percentage of humidity
    sensor_luminosity_living_room = Element('sensor_luminosity_living_room', 'sensor', {"luminosity":0}) #percentage of humidity
    sensor_entry_door_authentification_face_living_room = Element('sensor_entry_door_authentification_face_living_room', 'sensor', {'authentification': ''})
    sensor_entry_door_authentification_mdp_living_room = Element('sensor_entry_door_authentification_mdp_living_room', 'sensor', {'authentification': ''})
        #actuator
    actuator_vmc_living_room = Element('actuator_vmc_living_room', 'actuator', {"state": ""}) #state OFF or ON
    actuator_entry_door_living_room = Element('actuator_entry_door_living_room', 'actuator', {"state": ""}) #state OPEN or CLOSE
    actuator_shutter_living_room = Element('actuator_shutter_living_room', 'actuator', {"state": ""}) #state OPEN or CLOSE
    actuator_heating_living_room = Element('actuator_heating_living_room', 'actuator', {"state": ""}) #state OFF or ON
    actuator_lum_living_room = Element('actuator_lum_living_room', 'actuator', {"state": ""}) #state OFF or ON


    living_room_elements_list = [sensor_temperature_living_room, sensor_humidity_living_room, sensor_luminosity_living_room, sensor_entry_door_authentification_mdp_living_room, sensor_entry_door_authentification_face_living_room, actuator_vmc_living_room,
    actuator_entry_door_living_room, actuator_shutter_living_room, actuator_heating_living_room, actuator_lum_living_room]
    #bed room 1
        #sensor
        #actuator
    
    rooms_list = [Room(config("ROOM_NAME_1"), living_room_elements_list), Room(config("ROOM_NAME_2"), [])]
    home = Home(config("HOME_NAME"), rooms_list, mqtt_client)

    #Creation data in BDD
    api_obj.create_home(home.name)

    api_obj.create_mode(home.name, "ABSENT")
    api_obj.create_mode(home.name, "FETE")

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
    dict_value = {'OFF':0,"ON":1}
    mode_lum_dict = {"ROUGE" : 2, "VERT" : 3, "JAUNE": 4, "FETE" : 5, "ALARME" : 6}

    if value == "ON" or value == "OFF":
        return dict_value[value]
    else:
        return mode_lum_dict[value]

def state_to_boolean(state):
    dict_state = {"ON" : True, "OFF":False, "ROUGE" : True, "VERT" : True, "JAUNE": True, "FETE" : True, "ALARME" : True}
    return dict_state[state]

def get_room_obj(home_obj, room_name):
    return home_obj.rooms[room_name]

def get_element_obj(home_obj, room_name, element_name):
    return home_obj.rooms[room_name].elements[element_name]

if __name__ == "__main__":
    print(colored.blue("Lancement du server domotique...\n"))

    print(colored.cyan(f"Initialisation du server MQTT"))
    mqtt_client = init_mqtt_connection()
    print(colored.green("Connexion au server MQTT réussi\n"))
    web_api = Api()
    print(colored.cyan(f"Initialisation de la maison"))
    home = init_my_home(mqtt_client, web_api)
    print(colored.green(f"Initialisation de la maison {home.name} réussie\n\n"))

    vmc_living_room = get_element_obj(home, 'living_room', 'actuator_vmc_living_room')
    heating_living_room = get_element_obj(home, 'living_room', 'actuator_heating_living_room')
    lum_living_room = get_element_obj(home, 'living_room', 'actuator_lum_living_room')
    entry_door = get_element_obj(home, 'living_room','actuator_entry_door_living_room') 

    sensor_temperature = get_element_obj(home, 'living_room', 'sensor_temperature_living_room')
    sensor_humidity = get_element_obj(home, 'living_room', 'sensor_humidity_living_room')
    sensor_luminosity = get_element_obj(home, 'living_room', 'sensor_luminosity_living_room')
    sensor_entry_door_authentification_face = get_element_obj(home, 'living_room', 'sensor_entry_door_authentification_face_living_room')
    sensor_entry_door_authentification_mdp = get_element_obj(home, 'living_room', 'sensor_entry_door_authentification_mdp_living_room')


    while True:

        #region check temperature in living room
        temperature_living_room = float(sensor_temperature.data['temperature'])
        web_api.create_value(temperature_living_room, sensor_temperature.name)

        humidity_living_room = float(sensor_humidity.data['humidity'])
        web_api.create_value(humidity_living_room, sensor_humidity.name)

        luminosity_living_room = float(sensor_luminosity.data['luminosity'])
        web_api.create_value(luminosity_living_room, sensor_luminosity.name)


        #uniquement en mode manuel ??
        unresolved_actions = check_and_format_actions_api(web_api)
        if unresolved_actions != None:
            for action in unresolved_actions:
                print(colored.yellow(f"Traitment of the action {action}"))
                if action["value"] != "":
                    element = get_element_obj(home, action['room'], action['element'])
                    if element.type == "actuator":
                        if element.data["state"] != action["value"]:
                            home.mqtt_client.subscribe(element.topic +"/acquittement")
                            home.mqtt_client.publish(element.topic, format_value_publish(action["value"]))
                            sender_cpt = 0
                            flag_acquittement = True
                            while ( element.acquittement != f"{action['value']} RECEIVED") :
                                time.sleep(0.2)
                                home.mqtt_client.publish(element.topic, format_value_publish(action["value"]))
                                sender_cpt += 1
                                if sender_cpt == 20:
                                    flag_acquittement = False
                                    break
                            if flag_acquittement :
                                home.mqtt_client.unsubscribe(element.topic +"/acquittement")
                                element.data["state"] = action["value"]

                                web_api.update_element(web_api.get_element_id_by_name(element.name), element_name = element.name,state = state_to_boolean(element.data["state"]))
                                web_api.update_action(action["action_id"], value = action["value"], state = True)
                        elif element.data["state"] == action["value"]:
                            web_api.update_action(action["action_id"], value = action["value"], state = True)

                    elif element.type == "sensor":
                        if list(element.data.keys())[0] == "temperature":
                            temperature_to_reach = float(action["value"])
                            if temperature_living_room < temperature_to_reach + 0.5 and temperature_living_room > temperature_to_reach - 0.5:
                                # switch off the VMC
                                if vmc_living_room.data['state']  != "OFF" :
                                    web_api.create_action("OFF", vmc_living_room.name)

                                # switch off the heating
                                if heating_living_room.data['state'] != "OFF" :
                                    web_api.create_action("OFF", heating_living_room.name)


                                web_api.update_action(action["action_id"], state = True, value = action["value"])

                            if temperature_living_room > temperature_to_reach + 0.5:
                                # powered the VMC
                                if vmc_living_room.data['state'] != "ON" :
                                    web_api.create_action("ON", vmc_living_room.name)

                                # switch off the heating))
                                if heating_living_room.data['state'] != "OFF" :
                                    web_api.create_action("OFF", heating_living_room.name)

                            elif temperature_living_room < temperature_to_reach - 0.5:
                                # switch offthe VMC
                                if vmc_living_room.data['state'] != "OFF" :
                                    web_api.create_action("OFF", vmc_living_room.name)
                                # powered the heating
                                if heating_living_room.data['state'] != "ON" :
                                    web_api.create_action("ON", heating_living_room.name)
                            #endregion

                        #region check humidity in living room
                        if list(element.data.keys())[0] == "humidiy":
                            humidity_to_reach = float(action["value"])
                            if humidity_living_room >= 0 and humidity_living_room < humidity_to_reach:
                                # switch offthe VMC
                                if vmc_living_room.data['state'] != "OFF" :
                                    web_api.create_action("OFF", vmc_living_room.name)

                            if humidity_living_room >= humidity_to_reach and humidity_living_room <= 100:
                                # powered the VMC
                                if vmc_living_room.data['state'] != "ON" :
                                    web_api.create_action("ON", vmc_living_room.name)

                                web_api.update_action(action["action_id"], state = True, value = action["value"])

                        #endregion

                        #region check luminosity in living room
                        if list(element.data.keys())[0] == "luminosity":
                            luminosity_to_reach = float(action["value"])
                            if luminosity_living_room >= 0 and luminosity_living_room < luminosity_to_reach:
                                # switch offthe VMC
                                if lum_living_room.data['state'] != "OFF" :
                                    web_api.create_action("OFF", lum_living_room.name)

                            if luminosity_living_room >= luminosity_to_reach and luminosity_living_room <= 100:
                                # powered the VMC
                                if lum_living_room.data['state'] != "ON" :
                                    web_api.create_action("ON", lum_living_room.name)

                                web_api.update_action(action["action_id"], state = True, value = action["value"])

                        #endregion

                        #region check face entry door in living room

                else :
                    web_api.update_action(action["action_id"], state = True)
            
        if sensor_entry_door_authentification_face.data["authentification"] == "authorized":
            if entry_door.data['state'] != "ON" :
                web_api.create_action("ON", entry_door.name)
                web_api.create_action("OFF", entry_door.name)
            sensor_entry_door_authentification_face.data["authentification"] = ""
        
        if sensor_entry_door_authentification_mdp.data["authentification"] == "authorized":
            if entry_door.data['state'] != "ON" :
                web_api.create_action("ON", entry_door.name)
                web_api.create_action("OFF", entry_door.name)
            sensor_entry_door_authentification_mdp.data["authentification"] = ""

        # envoi du new password s'il est modifié en BDD: 
        try:
            PASSWORD_MAMORY = check_password_change(web_api, PASSWORD_MEMORY)
        except:
            pass

        try :
            MODE = web_api.get_active_mode(home.name)["label"]
        except:
            MODE = "normal"

        if MODE == "ABSENT":

            if lum_living_room.data['state'] != "OFF" :
                web_api.create_action("OFF", lum_living_room.name)
            
            if vmc_living_room.data['state'] != "OFF" :
                web_api.create_action("OFF", vmc_living_room.name)
            
            if entry_door.data['state'] != "OFF" :
                web_api.create_action("OFF", entry_door.name)

        try :
            MODE = web_api.get_active_mode(home.name)["label"]
        except:
            MODE = "normal"
        
        if MODE == "FETE":
            if lum_living_room.data['state'] != "FETE" :
                web_api.create_action("FETE", lum_living_room.name)

        

