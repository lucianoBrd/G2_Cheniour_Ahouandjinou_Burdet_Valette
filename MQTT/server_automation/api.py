import requests

class API:
    """
    API class
    """
    def __init__(self) -> None:
        """
        Init the API class
        """
        self.url= 'https://work.lucien-brd.com/api'

    def get_homes(self):
        """
        Get all homes

        :returns: json -- The json of all homes data
        """
        return requests.get(f"{self.url}/homes", headers= {'accept': 'application/json'}).json()
    
    def get_home(self, home_name):
        """
        Get one home with it name 

        :param home_name: The name of the home
        :type home_name: str

        :returns: json -- The json of the home data
        """
        return requests.get(f"{self.url}/home/{home_name}", headers= {'accept': 'application/json'}).json()

    def get_rooms(self):
        """
        Get all rooms

        :returns: json -- The json of all rooms data
        """
        return requests.get(f"{self.url}/rooms", headers= {'accept': 'application/json'}).json()
    
    def get_room(self, room_name):
        """
        Get one room with it name 

        :param room_name: The name of the room
        :type room_name: str

        :returns: json -- The json of the room data
        """
        return requests.get(f"{self.url}/room/{room_name}", headers= {'accept': 'application/json'}).json()

    def get_elements(self):
        """
        Get all elements

        :returns: json -- The json of all elements data
        """
        return requests.get(f"{self.url}/elements", headers= {'accept': 'application/json'}).json()
    
    def get_element(self, element_name):
        """
        Get one element with it name 

        :param element_name: The name of the element
        :type element_name: str

        :returns: json -- The json of the element data
        """
        return requests.get(f"{self.url}/element/{element_name}", headers= {'accept': 'application/json'}).json()

    def create_home(self, home_name = '', rooms_list= []):
        """
        Create a home

        :param home_name: The name of the home
        :type home_name: str

        :param rooms_list: The list of rooms in the home
        :type rooms_list: list 

        :returns: int -- The status code of the request post
        """

        payload = {
        "label": f"{home_name}",
        "rooms": rooms_list
            }
        
        request = requests.post(f"{self.url}/homes", data=payload)

        return request.status_code

    def create_room(self, room_name = '', elements_list = []):
        """
        Create a room

        :param room_name: The name of the room
        :type room_name: str

        :param elements_list: The list of elements in the room
        :type elements_list: list 

        :returns: int -- The status code of the request post
        """

        payload = {
        "label": f"{room_name}",
        "elements": elements_list
            }
        
        request = requests.post(f"{self.url}/rooms", data=payload)

        return request.status_code

    def create_element(self, element_name):
        """
        Create an element

        :param element_name: The name of the element
        :type element_name: str

        :returns: int -- The status code of the request post
        """

        payload = {
        "label": f"{element_name}"
            }
        
        request = requests.post(f"{self.url}/rooms", data=payload)

        return request.status_code
    
    def update_home(self, home_name, rooms_list = []):
        """
        Update a home

        :param home_name: The name of the home
        :type home_name: str

        :param rooms_list: The list of rooms in the home
        :type rooms_list: list 

        :returns: int -- The status code of the request post
        """

        payload = {
        "label": f"{home_name}",
        "rooms": rooms_list
            }
        
        request = requests.put(f"{self.url}/homes", data=payload)

        return request.status_code

    def update_room(self, room_name, elements_list = []):
        """
        Create a room

        :param room_name: The name of the room
        :type room_name: str

        :param elements_list: The list of elements in the room
        :type elements_list: list 

        :returns: int -- The status code of the request post
        """

        payload = {
        "label": f"{room_name}",
        "elements": elements_list
            }
        
        request = requests.put(f"{self.url}/rooms", data=payload)

        return request.status_code

    def update_element(self, element_name):
        """
        Create an element

        :param element_name: The name of the element
        :type element_name: str

        :returns: int -- The status code of the request post
        """

        payload = {
        "label": f"{element_name}"
            }
        
        request = requests.put(f"{self.url}/rooms", data=payload)

        return request.status_code

print(API().get_homes())
print('===========================================')
print(API().get_room('Chambre a laver'))