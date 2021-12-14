#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ssh nao@pepper01
# export PYTHONPATH=${PYTHONPATH}:/softwares/INFO/Pepper/pynaoqi-python2.7-2.5.5.5-linux64/lib/python2.7/site-packages
# run with python2 scripts/api.py
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
        self.get_home()


    def get_home(self):
        label = config("HOME_NAME")

        home = self.api.get_home_by_name(label)

        print json.dumps(home) 

        #if not home["label"] is None:
        #    label = home["label"]

        return json.dumps(home)

    def get_room(self, room_name):
        room = self.api.get_room_by_name(room_name)

        print room

        return room

    def get_value(self, element_name):
        value = self.api.get_last_value_by_element_name(element_name)

        print value

        return value

    def create_action(self, value, element_name):
        status_code = self.api.create_action(value, element_name)

        print status_code

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