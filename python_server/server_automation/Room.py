
class Room:
    """
    Room class
    """
    def __init__(self, name = 'room_1', elements_list = []) -> None:
        """
        Init the Room class 

        :param name: The name of the room
        :type name: str

        :param elements: The list of elements (sensor : temperature, actuator : VMC etc...) instance which compose the house
        :type elements: list

        :returns: None
        """
        self.name = name
        self.elements = {}
        for element in elements_list:
            self.elements[element.name] = element
                
        