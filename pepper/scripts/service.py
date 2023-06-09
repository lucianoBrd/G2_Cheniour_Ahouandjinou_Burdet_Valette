#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ssh nao@pepper02
# ifconfig
# qicli call ALTabletService._openSettings

# export PYTHONPATH=${PYTHONPATH}:/softwares/INFO/Pepper/pynaoqi-python2.7-2.5.5.5-linux64/lib/python2.7/site-packages

# run with python2 scripts/service.py

# http://134.214.51.43/apps/domotique/


import qi
import json
from decouple import config
from api import Api

class APIModule:
    
    def __init__(self, session):
        self.session = session
        self.memory = self.session.service("ALMemory")

        self.api = Api()

    def get_label_home_by_id(self, home_id):
        home = {}

        try:
            home = self.api.get_home_by_id(home_id)
        except:
            home = {}

        if home != None and "label" in home :
            return home["label"]

        return 'Inconnu'

    def get_active_mode_label_by_home_name(self, home_name):
        mode = {}

        try:
            mode = self.api.get_active_mode(home_name)
        except:
            mode = {}

        if mode != None and "label" in mode :
            return mode["label"]

        return 'Normal'

    def get_temperature(self):
        try:
            value = self.api.get_last_value_by_element_name(config("SENSOR_TEMPERATURE_LIVING_ROOM"))
            if value != None and "value" in value :
                return value["value"]
        except:
            pass

        return 'Inconnu'

    def get_humidity(self):
        try:
            value = self.api.get_last_value_by_element_name(config("SENSOR_HUMIDITY_LIVING_ROOM"))
            if value != None and "value" in value :
                return value["value"]
        except:
            pass

        return 'Inconnu'

    def get_home_by_id(self, home_id):
        home = {}
        try:
            home = self.api.get_home_by_id(home_id)
        except:
            pass
        """
        if "rooms" in home :
            rooms = []

            for r in home["rooms"]:
                room = self.api.get_room_by_iri(r)

                if "elements" in room :
                    elements = []

                    for e in room["elements"]:
                        element = self.api.get_element_by_iri(e)

                        if "label" in element :
                            element["elementValue"] = self.api.get_last_value_by_element_name(element["label"])
                            element["action"] = self.api.get_last_action_by_element_name(element["label"])

                        if "type" in element :
                            element["type"] = self.api.get_type_by_iri(element["type"])

                        elements.append(element)

                    room["elements"] = elements
                
                rooms.append(room)

            home["rooms"] = rooms
        """
        #print json.dumps(home) 

        return json.dumps(home)

    def get_homes(self):
        homes = {}
        try:
            homes = self.api.get_homes()
        except:
            pass

        #print json.dumps(homes)

        return json.dumps(homes)

    def create_action(self, parameter):
        parameter = json.loads(parameter)
        status_code = 404

        if parameter != None and "elementName" in parameter and "value" in parameter :
            try:
                status_code = self.api.create_action(parameter["value"], parameter["elementName"])
            except:
                pass

        #print status_code

        return status_code

    def create_action_alarme(self):
        status_code = 404

        try:
            status_code = self.api.create_action("ALARME", "actuator_lum_living_room")
        except:
            pass

        #print status_code

        return status_code

    def create_action_on(self):
        status_code = 404

        try:
            status_code = self.api.create_action("ON", "actuator_lum_living_room")
        except:
            pass

        #print status_code

        return status_code

    def update_mode(self, home_name, mode_name, state):
        status_code = 404

        try:
            mode = self.api.get_mode_by_name(home_name, mode_name)
            if mode != None and "id" in mode :
                status_code = self.api.update_mode(mode["id"], state)
        except:
            pass

        return status_code

def main():
    app = qi.Application(url="tcp://134.214.51.43:9559")
    app.start()

    s = app.session
    my_module = APIModule(s)
    s.registerService("API", my_module)

    app.run()

if __name__ == "__main__":
    main()