from Room import Room

class Home:
    """
    Home class
    """
    def __init__(self, name = 'MyHome', rooms_list = [], mqtt_client=object()) -> None:
        """
        Init the Home class 

        :param name: The name of the home
        :type name: str

        :param rooms_list: The list of rooms instance which compose the home
        :type rooms_list: list

        :param mqtt_client: The instance of the mqtt client
        :type mqtt_client: object

        :returns: None
        """
        self.name = name
        self.rooms = {}
        self.mqtt_client = mqtt_client
        self.topics_list = []
        for room in rooms_list:
            self.rooms[room.name] = room
        
        self.mqtt_client = mqtt_client
        