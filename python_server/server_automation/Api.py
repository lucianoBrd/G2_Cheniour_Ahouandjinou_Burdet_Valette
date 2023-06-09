import requests
import json

class Api:
    """
    API class
    """
    def __init__(self):
        """
        Init the API class
        """
        self.url= 'https://work.lucien-brd.com/api'
        self.url_strict= 'https://work.lucien-brd.com'
        self.header = {'accept': 'application/json', 'Content-Type' : 'application/json'}
        self.header_patch = {'accept': 'application/json', 'Content-Type' : 'application/merge-patch+json'}

    def get_homes(self):
        """
        Get all homes

        :returns: json -- The json of all homes data
        """
        return requests.get(self.url + "/homes", headers = self.header).json()
    
    def get_home_by_name(self, home_name = ''):
        """
        Get one home with it name 

        :param home_name: The name of the home
        :type home_name: str

        :returns: json -- The json of the home data
        """
        return requests.get(self.url + "/home/" + home_name, headers = self.header).json()
    
    def get_home_by_id(self, home_id = 0):
        """
        Get one home with it ID 

        :param home_id: The ID of the home
        :type home_id: int

        :returns: json -- The json of the home data
        """
        return requests.get(self.url + "/homes/ " + str(home_id), headers = self.header).json()
    
    def get_home_name_by_id(self, home_id = 0):
        """
        Get home name by id

        :param home_id: The ID of the home
        :type home_id: int

        :returns: str -- The name of the home
        """
        try:
            name = self.get_home_by_id(home_id)['label']
        except:
            name = None

        return name

    def get_home_id_by_name(self, home_name = ''):
        """
        Get home id by name

        :param home_name: The name of the home
        :type home_name: str

        :returns: id -- The ID of the home
        """
        try:
            id = int(self.get_home_by_name(home_name)['id'])
        except:
            id = None

        return id

    def get_rooms(self):
        """
        Get all rooms

        :returns: json -- The json of all rooms data
        """
        return requests.get(self.url + "/rooms", headers = self.header).json()
    
    def get_room_by_name(self, room_name = ''):
        """
        Get one room with it name 

        :param room_name: The name of the room
        :type room_name: str

        :returns: json -- The json of the room data
        """
        return requests.get(self.url + "/room/" + room_name, headers = self.header).json()
    
    def get_room_by_id(self, room_id = 0):
        """
        Get one room with it ID 

        :param room_id: The ID of the room
        :type room_id: int

        :returns: json -- The json of the room data
        """
        return requests.get(self.url + "/rooms/" + str(room_id), headers = self.header).json()

    def get_room_by_iri(self, room_iri = ''):
        """
        Get one room with it IRI 

        :param room_iri: The IRI of the room, exemple /api/rooms/1
        :type room_iri: str

        :returns: json -- The json of the room data
        """
        return requests.get(self.url_strict + str(room_iri), headers = self.header).json()
    
    def get_room_name_by_id(self, room_id = 0):
        """
        Get room name by id

        :param room_id: The ID of the room
        :type room_id: int

        :returns: str -- The name of the room
        """
        try:
            name = self.get_room_by_id(room_id)['label']
        except:
            name = None

        return name

    def get_room_id_by_name(self, room_name = ''):
        """
        Get room id by name

        :param room_name: The name of the room
        :type rom_name: str

        :returns: id -- The ID of the room
        """
        try:
            id = int(self.get_room_by_name(room_name)['id'])
        except:
            id = None

        return id

    def get_modes(self):
        """
        Get all modes

        :returns: json -- The json of all modes data
        """
        return requests.get(self.url + "/modes", headers = self.header).json()
    
    def get_active_mode(self, home_name):
        """
        Get the active mode

        :param home_name: The name of the home's mode
        :type home_name: str

        :returns: str -- The name of the active mode
        """

        return requests.get(self.url + "/get/mode/active/" + home_name, headers = self.header).json()

    def get_elements(self):
        """
        Get all elements

        :returns: json -- The json of all elements data
        """
        return requests.get(self.url + "/elements", headers = self.header).json()
    
    def get_element_by_name(self, element_name = ''):
        """
        Get one element with it name 

        :param element_name: The name of the element
        :type element_name: str

        :returns: json -- The json of the element data
        """
        return requests.get(self.url + "/element/" + element_name, headers = self.header).json()

    def get_element_by_id(self, element_id = 0):
        """
        Get one element with it id 

        :param element_id: The ID of the element
        :type element_id: ID

        :returns: json -- The json of the element data
        """
        return requests.get(self.url + "/elements/" + str(element_id), headers = self.header).json()

    def get_element_by_iri(self, element_iri = ''):
        """
        Get one element with it IRI 

        :param element_iri: The IRI of the element, exemple /api/elements/1
        :type element_iri: str

        :returns: json -- The json of the element data
        """
        return requests.get(self.url_strict + str(element_iri), headers = self.header).json()
    
    def get_element_name_by_id(self, element_id = 0):
        """
        Get element name by id

        :param element_id: The ID of the element
        :type element_id: int

        :returns: str -- The name of the element
        """
        try:
            name = self.get_element_by_id(element_id)['label']
        except:
            name = None

        return name

    def get_element_id_by_name(self, element_name = ''):
        """
        Get element id by name

        :param element_name: The name of the element
        :type element_name: str

        :returns: id -- The ID of the element
        """
        try:
            id = int(self.get_element_by_name(element_name)['id'])
        except:
            id = None

        return id
    
    def get_types(self):
        """
        Get all types

        :returns: json -- The json of all types data
        """
        return requests.get(self.url + "/types", headers = self.header).json()
    
    def get_type_by_name(self, type_name = ''):
        """
        Get one type with it name 

        :param type_name: The name of the type
        :type type_name: str

        :returns: json -- The json of the type data
        """
        return requests.get(self.url + "/type/" + type_name, headers = self.header).json()

    def get_type_by_id(self, type_id = 0):
        """
        Get one type with it id 

        :param type_id: The ID of the type
        :type type_id: int

        :returns: json -- The json of the type data
        """
        return requests.get(self.url + "/types/" + str(type_id), headers = self.header).json()

    def get_type_by_iri(self, type_iri = ''):
        """
        Get one type with it iri 

        :param type_iri: The IRI of the type, exemple /api/types/1
        :type type_iri: str

        :returns: json -- The json of the type data
        """

        request = requests.get(self.url_strict + str(type_iri), headers = self.header)
        
        if request.status_code == 200:
            return request.json()

        return None
    
    def get_type_name_by_id(self, type_id = 0):
        """
        Get type name by id

        :param type_id: The ID of the type
        :type type_id: int

        :returns: str -- The name of the type
        """
        try:
            name = self.get_type_by_id(type_id)['label']
        except:
            name = None

        return name

    def get_type_id_by_name(self, type_name = ''):
        """
        Get type id by name

        :param type_name: The name of the type
        :type type_name: str

        :returns: id -- The ID of the type
        """
        try:
            id = int(self.get_type_by_name(type_name)['id'])
        except:
            id = None

        return id
    
    def get_values(self):
        """
        Get all values

        :returns: json -- The json of all values data
        """
        return requests.get(self.url + "/values", headers = self.header).json()
    
    def get_value_by_id(self, value_id = 0):
        """
        Get one value with it id 

        :param value_id: The ID of the value
        :type value_id: ID

        :returns: json -- The json of the value data
        """
        return requests.get(self.url + "/values/" + str(value_id), headers = self.header).json()

    def get_last_value_by_element_name(self, element_name = ''):
        """
        Get the last value of an element (by name element name) 

        :param element_name: The element name
        :type element_name: ID

        :returns: json -- The json of the value data
        """
        return requests.get(self.url + "/value/last/" + element_name, headers = self.header).json()
    
    def get_actions(self):
        """
        Get all actions

        :returns: json -- The json of all actions data
        """
        return requests.get(self.url + "/actions", headers = self.header).json()
    
    def get_action_by_id(self, action_id = 0):
        """
        Get one action with it id 

        :param action_id: The ID of the action
        :type action_id: ID

        :returns: json -- The json of the action data
        """
        return requests.get(self.url + "/actions/" + str(action_id), headers = self.header).json()

    def get_unresolved_actions_by_element_name(self, element_name = ''):
        """
        Get all unresolved action of an element (by name element name) 

        :param element_name: The name of the element
        :type element_name: ID

        :returns: json -- The json of the actions data
        """
        request = requests.get(self.url + "/actions/unresolved/" + element_name, headers = self.header).json()
        
        if type(request) == list:
            return request
        else:
            return []
    
    def get_unresolved_actions(self):
        """
        Get all unresolved actions

        :returns: json -- The json of the actions data
        """
        request = requests.get(self.url + "/actions/get/unresolved", headers = self.header).json()

        if type(request) == list:
            return request
        else:
            return []

    def get_last_action_by_element_name(self, element_name = ''):
        """
        Get the last action of an element (by name element name) 

        :param element_name: The element name
        :type element_name: ID

        :returns: json -- The json of the value data
        """
        return requests.get(self.url + "/action/last/" + element_name, headers = self.header).json()
    
    def create_home(self, home_name = '', rooms_list= []):
        """
        Create a home

        :param home_name: The name of the home
        :type home_name: str

        :param rooms_list: The list of rooms in the home
        :type rooms_list: list 

        :returns: int -- The status code of the request post
        """
        
        payload = json.dumps({
        "label": home_name,
        "rooms": rooms_list 
            })

        request = requests.post(self.url + "/homes", data=payload, headers = self.header)

        return request.status_code

    def create_room(self, room_name = '', parent_home_name = '', elements_list = []):
        """
        Create a room

        :param room_name: The name of the room
        :type room_name: str

        :param parent_home_name: The name of the parent home
        :type parent_home_name: str

        :param elements_list: The list of elements in the room
        :type elements_list: list 

        :returns: int -- The status code of the request post
        """

        parent_home_id = self.get_home_id_by_name(parent_home_name)

        payload = json.dumps({
        "label": room_name,
        "home": '/api/homes/' + str(parent_home_id),
        "elements": elements_list
            })
                
        request = requests.post(self.url + "/rooms", data=payload, headers = self.header)

        return request.status_code

    def create_mode(self, parent_home_name = '', mode_name = ''):
        """
        Create a mode

        :param element_name: The name of the mode
        :type element_name: str

        :param parent_home_name: The name of the parent home
        :type parent_home_name: str

        :returns: int -- The status code of the request post
        """

        parent_home_id = self.get_home_id_by_name(parent_home_name)

        payload = json.dumps({
        "home" : '/api/homes/' + str(parent_home_id),
        "label": mode_name,
            })
        
        request = requests.post(self.url + "/modes", data=payload, headers = self.header)
        
        return request.status_code

    def create_element(self, element_name = '', parent_room_name = '', type_name = ''):
        """
        Create an element

        :param element_name: The name of the element
        :type element_name: str

        :param parent_room_name: The name of the room parent
        :type parent_room_name: str

        :param type_name: The type of the element
        :type type_name: str

        :returns: int -- The status code of the request post
        """

        parent_room_id = self.get_room_id_by_name(parent_room_name)
        type_id = self.get_type_id_by_name(type_name)

        payload = json.dumps({
        "label": element_name,
        "room" : '/api/rooms/' + str(parent_room_id),
        "type" : '/api/types/' + str(type_id)
            })
        
        request = requests.post(self.url + "/elements", data=payload, headers = self.header)

        return request.status_code
    
    def create_type(self, type_name = ''):
        """
        Create a type

        :param type_name: The name of the type
        :type type_name: str

        :returns: int -- The status code of the request post
        """

        payload = json.dumps({"label": type_name})
        
        request = requests.post(self.url + "/types", data=payload, headers = self.header)

        return request.status_code
    
    def create_value(self, value = '', element_name = ''):
        """
        Create a value

        :param value: The value
        :type value: str or int

        :param element_name: The element name of the value
        :type element_name: str

        :returns: int -- The status code of the request post
        """
        element_id = self.get_element_id_by_name(element_name)

        payload = json.dumps({
            "value": str(value),
            "element" : '/api/elements/' + str(element_id)
        })
        
        request = requests.post(self.url + "/values", data=payload, headers = self.header)

        return request.status_code
    
    def create_action(self, value = '', element_name = ''):
        """
        Create an action

        :param value: The value
        :type value: str or int

        :param element_name: The element name of the value
        :type element_name: str

        :returns: int -- The status code of the request post
        """
        element_id = self.get_element_id_by_name(element_name)

        payload = json.dumps({
            "value": str(value),
            "element" : '/api/elements/' + str(element_id)
        })
        
        request = requests.post(self.url + "/actions", data=payload, headers = self.header)
        
        return request.status_code
    
    def update_home(self, home_id = 0, home_name = '', rooms_list = None):
        """
        Update a home

        :param home_id: The ID of the home
        :type home_id: int

        :param home_name: The name of the home
        :type home_name: str

        :param rooms_list: The list of rooms in the home
        :type rooms_list: list 

        :returns: int -- The status code of the request put
        """

        payload = json.dumps({
        "label": home_name,
            })
        
        if rooms_list != None:
            rooms_list_iri = []
            for room in rooms_list:
                room_id = self.get_room_id_by_name(room)
                rooms_list_iri.append('/api/rooms/' + str(room_id))

            payload["rooms"] = rooms_list_iri

        payload = json.dumps(payload)

        request = requests.put(self.url + "/homes/" + str(home_id), data=payload, headers = self.header)

        return request.status_code

    def update_room(self, room_id = 0, room_name = '', parent_home_name = '', elements_list = None):
        """
        Update a room

        :param room_id: The ID of the room
        :type room_id: int

        :param room_name: The name of the room
        :type room_name: str

        :param elements_list: The list of elements in the room
        :type elements_list: list 

        :param parent_home_name: The name of the parent home
        :type parent_home_name: str

        :returns: int -- The status code of the request put
        """

        parent_home_id = self.get_home_id_by_name(parent_home_name)

        payload = {
            "label": room_name,
                }
        if parent_home_id != None :
            payload["home"] = '/api/homes/' + str(parent_home_id)
        if elements_list != None:
            elements_list_iri = []

            for element in elements_list:
                element_id = self.get_element_id_by_name(element)
                elements_list_iri.append('/api/elements/' + str(element_id))
        
            payload["elements"] = elements_list_iri

        payload = json.dumps(payload)

        request = requests.put(self.url + "/rooms/" + str(room_id), data=payload, headers = self.header)

        return request.status_code

    def update_element(self, element_id = 0, element_name = '', parent_room_name = '', type_name = '', actions_id = None, values_id = None, state = None):
        """
        Update an element

        :param element_id: The ID of the element
        :type element_id: int

        :param element_name: The name of the element
        :type element_name: str

        :param parent_room_name: The name of the parent room
        :type parent_room_name: str

        :param actions_id: The list of actions 
        :type actons_id: list

        :param values_id: The list of values
        :type values_id: list

        :param state: The state of the element (for actuator)
        :type state: bool

        :returns: int -- The status code of the request put
        """

        parent_room_id = self.get_room_id_by_name(parent_room_name)
        type_id = self.get_type_id_by_name(type_name)

        payload = {
            "label": element_name,
                }

        if parent_room_id != None:
            payload["room"] = '/api/rooms/' + str(parent_room_id)
        if type_id != None:
            payload["type"] = '/api/types/' + str(type_id)
        if actions_id != None:
            payload["actions"] = ["/api/actions/" + str(action) for action in actions_id]
        if values_id != None:
            payload["elementValues"] = ["/api/values/" + str(value) for value in values_id]
        if state != None:
            payload["state"] = state
        payload = json.dumps(payload)

        request = requests.put(self.url + "/elements/" + str(element_id), data=payload, headers = self.header)
        
        return request.status_code
    
    def update_type(self, type_id = 0, type_name = '', elements_list = None):
        """
        Update a type

        :param type_id: The ID of the type
        :type type_id: int

        :param type_name: The name of the type
        :type type_name: str

        :param elements_list: The list of elements in the room
        :type elements_list: list 

        :returns: int -- The status code of the request put
        """

        payload = {
            "label": type_name,
            }
        
        if elements_list != None :
            elements_list_iri = []

            for element in elements_list:
                element_id = self.get_element_id_by_name(element)
                elements_list_iri.append('/api/elements/' + str(element_id))

            payload["elements"] = elements_list_iri

        payload = json.dumps(payload)

        request = requests.put(self.url + "/types/" + str(type_id), data=payload, headers = self.header)

        return request.status_code

    def update_action(self, action_id = 0, value = '', element_name = '', state = False):
        """
        Update an action

        :param action_id: The ID of the action
        :type action_id: int

        :param value: The value
        :type value: str or int

        :param element_name: The element name of the value
        :type element_name: str

        :param state: The state of the action
        :type state: bool

        :returns: int -- The status code of the request put
        """
        element_id = self.get_element_id_by_name(element_name)

        payload = {
            "value":str(value),
            "state": state
        }

        if element_id != None:
            payload["element"] =  '/api/elements/' + str(element_id),

        payload = json.dumps(payload)
        
        request = requests.put(self.url + "/actions/" + str(action_id), data=payload, headers = self.header)
        
        return request.status_code
    
    def update_mode(self, mode_id = 0, state = False):
        """
        Update a mode

        :param mode_id: The ID of the mode
        :type mode_id: int

        :param state: The state of the action
        :type state: bool

        :returns: int -- The status code of the request put
        """

        payload = {
            "state": state
        }

        payload = json.dumps(payload)
        
        request = requests.put(self.url + "/modes/" + str(mode_id), data=payload, headers = self.header)
        
        return request.status_code
