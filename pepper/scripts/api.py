#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ssh nao@pepper01
# export PYTHONPATH=${PYTHONPATH}:/softwares/INFO/Pepper/pynaoqi-python2.7-2.5.5.5-linux64/lib/python2.7/site-packages
# run with python2 scripts/api.py
# http://134.214.50.49/apps/domotique/

import qi
import requests

class APIModule:
    
    def __init__(self, session):
        self.session = session
        self.memory = self.session.service("ALMemory")

        self.url = "https://work.lucien-brd.com/api"


    def get_home(self):
        label = "home"

        info = requests.get(self.url + "/home/" + label)

        info_json = info.json()

        print info_json

        main_info = info_json["label"]
        print "main_info: %s" % main_info

        return main_info


def main():
    app = qi.Application(url="tcp://134.214.51.44:9559")
    app.start()

    s = app.session
    my_module = APIModule(s)
    s.registerService("Api", my_module)

    app.run()



if __name__ == "__main__":
    main()
