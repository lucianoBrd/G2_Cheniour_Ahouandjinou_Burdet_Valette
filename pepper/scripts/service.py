#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ssh nao@pepper01
# export PYTHONPATH=${PYTHONPATH}:/softwares/INFO/Pepper/pynaoqi-python2.7-2.5.5.5-linux64/lib/python2.7/site-packages
# run with python2 scripts/api.py
# http://134.214.50.49/apps/domotique/

import qi
from api import API

class APIModule:
    
    def __init__(self, session):
        self.session = session
        self.memory = self.session.service("ALMemory")

        self.api = API()


    def get_home(self):
        label = "home"

        home = self.api.get_home_by_name(label)

        print(home)

        main_info = home["label"]
        print("main_info: %s" % main_info)

        return main_info


def main():
    app = qi.Application(url="tcp://134.214.51.44:9559")
    app.start()

    s = app.session
    my_module = APIModule(s)
    s.registerService("API", my_module)

    app.run()

if __name__ == "__main__":
    main()