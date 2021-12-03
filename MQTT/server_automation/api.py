import requests
import json

class API:
    """
    API class
    """
    def __init__(self) -> None:
        """
        Init the API class
        """
        self.url= 'https://work.lucien-brd.com/api'
        self.header = {'accept': 'application/json', 'Content-Type' : 'application/json'}

    def get_homes(self):
        """
        Get all homes

        :returns: json -- The json of all homes data
        """
        return requests.get(f"{self.url}/homes", headers = self.header).json()
    
    def get_home_by_name(self, home_name = ''):
        """
        Get one home with it name 

        :param home_name: The name of the home
        :type home_name: str

        :returns: json -- The json of the home data
        """
        return requests.get(f"{self.url}/home/{home_name}", headers = self.header).json()
    
    def get_home_by_id(self, home_id = 0):
        """
        Get one home with it ID 

        :param home_id: The ID of the home
        :type home_id: int

        :returns: json -- The json of the home data
        """
        return requests.get(f"{self.url}/homes/{home_id}", headers = self.header).json()
    
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
        return requests.get(f"{self.url}/rooms", headers = self.header).json()
    
    def get_room_by_name(self, room_name = ''):
        """
        Get one room with it name 

        :param room_name: The name of the room
        :type room_name: str

        :returns: json -- The json of the room data
        """
        return requests.get(f"{self.url}/room/{room_name}", headers = self.header).json()
    
    def get_room_by_id(self, room_id = 0):
        """
        Get one room with it ID 

        :param room_id: The ID of the room
        :type room_id: int

        :returns: json -- The json of the room data
        """
        return requests.get(f"{self.url}/rooms/{room_id}", headers = self.header).json()
    
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

    def get_elements(self):
        """
        Get all elements

        :returns: json -- The json of all elements data
        """
        return requests.get(f"{self.url}/elements", headers = self.header).json()
    
    def get_element_by_name(self, element_name = ''):
        """
        Get one element with it name 

        :param element_name: The name of the element
        :type element_name: str

        :returns: json -- The json of the element data
        """
        return requests.get(f"{self.url}/element/{element_name}", headers = self.header).json()

    def get_element_by_id(self, element_id = 0):
        """
        Get one element with it id 

        :param element_id: The ID of the element
        :type element_id: ID

        :returns: json -- The json of the element data
        """
        return requests.get(f"{self.url}/elements/{element_id}", headers = self.header).json()
    
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
        return requests.get(f"{self.url}/types", headers = self.header).json()
    
    def get_type_by_name(self, type_name = ''):
        """
        Get one type with it name 

        :param type_name: The name of the type
        :type type_name: str

        :returns: json -- The json of the type data
        """
        return requests.get(f"{self.url}/type/{type_name}", headers = self.header).json()

    def get_type_by_id(self, type_id = 0):
        """
        Get one type with it id 

        :param type_id: The ID of the type
        :type type_id: int

        :returns: json -- The json of the type data
        """
        return requests.get(f"{self.url}/types/{type_id}", headers = self.header).json()
    
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
        return requests.get(f"{self.url}/values", headers = self.header).json()
    
    def get_value_by_id(self, value_id = 0):
        """
        Get one value with it id 

        :param value_id: The ID of the value
        :type value_id: ID

        :returns: json -- The json of the value data
        """
        return requests.get(f"{self.url}/values/{value_id}", headers = self.header).json()

    def get_last_value_by_element_name(self, element_name = ''):
        """
        Get the last value of an element (by name element name) 

        :param element_name: The element name
        :type element_name: ID

        :returns: json -- The json of the value data
        """
        return requests.get(f"{self.url}/value/last/{element_name}", headers = self.header).json()
    
    def get_actions(self):
        """
        Get all actions

        :returns: json -- The json of all actions data
        """
        return requests.get(f"{self.url}/actions", headers = self.header).json()
    
    def get_action_by_id(self, action_id = 0):
        """
        Get one action with it id 

        :param action_id: The ID of the action
        :type action_id: ID

        :returns: json -- The json of the action data
        """
        return requests.get(f"{self.url}/actions/{action_id}", headers = self.header).json()

    def get_unresolved_actions_by_element_name(self, element_name = ''):
        """
        Get all unresolved action of an element (by name element name) 

        :param element_name: The name of the element
        :type element_name: ID

        :returns: json -- The json of the actions data
        """
        return requests.get(f"{self.url}/actions/unresolved/{element_name}", headers = self.header).json()
    
    def get_unresolved_actions(self):
        """
        Get all unresolved actions

        :returns: json -- The json of the actions data
        """
        return requests.get(f"{self.url}/actions/get/unresolved", headers = self.header).json()
    
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

        request = requests.post(f"{self.url}/homes", data=payload, headers = self.header)

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
                
        request = requests.post(f"{self.url}/rooms", data=payload, headers = self.header)

        return request.status_code

    def create_element(self, element_name = '', parent_room_name = '', type_name = ''):
        """
        Create an element

        :param element_name: The name of the element
        :type element_name: str

        :returns: int -- The status code of the request post
        """

        parent_room_id = self.get_room_id_by_name(parent_room_name)
        type_id = self.get_type_id_by_name(type_name)

        payload = json.dumps({
        "label": element_name,
        "room" : '/api/rooms/' + str(parent_room_id),
        "type" : '/api/types/' + str(type_id)
            })
        
        request = requests.post(f"{self.url}/elements", data=payload, headers = self.header)

        return request.status_code
    
    def create_type(self, type_name = ''):
        """
        Create a type

        :param type_name: The name of the type
        :type type_name: str

        :returns: int -- The status code of the request post
        """

        payload = json.dumps({"label": type_name})
        
        request = requests.post(f"{self.url}/types", data=payload, headers = self.header)

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
        
        request = requests.post(f"{self.url}/values", data=payload, headers = self.header)

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
        
        request = requests.post(f"{self.url}/actions", data=payload, headers = self.header)

        return request.status_code
    
    def update_home(self, home_name = '', rooms_list = []):
        """
        Update a home

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
        
        request = requests.put(f"{self.url}/homes", data=payload, header = self.header)

        return request.status_code

    def update_room(self, room_name = '', parent_home_name = '', elements_list = []):
        """
        Create a room

        :param room_name: The name of the room
        :type room_name: str

        :param elements_list: The list of elements in the room
        :type elements_list: list 

        :param parent_home_name: The name of the parent home
        :type parent_home_name: str

        :returns: int -- The status code of the request post
        """

        payload = json.dumps({
        "label": room_name,
        "home": parent_home_name,
        "elements": elements_list
            })
        
        request = requests.put(f"{self.url}/rooms", data=payload, header = self.header)

        return request.status_code

    def update_element(self, element_name = '', parent_room_name = ''):
        """
        Create an element

        :param element_name: The name of the element
        :type element_name: str

        :param parent_room_name: The name of the parent room
        :type parent_room_name: str

        :returns: int -- The status code of the request post
        """

        payload = json.dumps({
        "label": element_name,
        "room": parent_room_name,
            })
        
        request = requests.put(f"{self.url}/elements", data=payload, headers = self.header)

        return request.status_code

print(API().create_action('oiqusfhgqudjfisqgjgfijqgs','capteur_1'))