#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ssh nao@pepper01
# export PYTHONPATH=${PYTHONPATH}:/softwares/INFO/Pepper/pynaoqi-python2.7-2.5.5.5-linux64/lib/python2.7/site-packages
# run with python2 scripts/service.py
# http://134.214.50.49/apps/domotique/
# qicli call ALTabletService._openSettings

import qi
import json
from decouple import config
from api import API

class APIModule:
    
    def __init__(self, session):
        self.session = session
        self.memory = self.session.service("ALMemory")

        self.api = API()

    def get_label_home_by_id(self, home_id):
        home = {}

        try:
            home = self.api.get_home_by_id(home_id)
        except:
            pass

        if "label" in home :
            return home["label"]

        return 'Inconnu'

    def get_temperature(self):
        element = {}

        try:
            element = self.api.get_element_by_name(config("SENSOR_TEMPERATURE_LIVING_ROOM"))
        except:
            pass

        if "value" in element :
            return element["value"]

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

        if "elementName" in parameter and "value" in parameter :
            try:
                status_code = self.api.create_action(parameter["value"], parameter["elementName"])
            except:
                pass

        #print status_code

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