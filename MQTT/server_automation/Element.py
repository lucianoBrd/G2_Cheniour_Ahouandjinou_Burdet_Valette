class Element:
    """
    Element class
    """
    def __init__(self, name = 'elements_1', type = 'sensor', data = {}) -> None:
        """
        Init the Element class 

        :param name: The name of the elements
        :type name: str

        :param type: The type of the element : must be 'sensor' or 'actuator'
        :type type: str

        :returns: None
        """
        self.name = name

        if type == 'sensor' or 'actuator':
            self.type = type
        else : 
            raise ValueError(f'The type muste be "sensor" or "actuator" and not {type}')

        self.topic = ''
        self.acquitement = ''
        self.data = data

        
