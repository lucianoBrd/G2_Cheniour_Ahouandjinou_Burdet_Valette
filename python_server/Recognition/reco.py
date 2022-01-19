import face_recognition
import os
import urllib.request
from PIL import Image
import time
import paho.mqtt.client as mqtt

directory = 'Faces'
CAMERA_ADDRESS = "192.168.12.1"
BROKER_ADDRESS = "192.168.12.222"

def init_mqtt_connection():
    # Init MQTT Client
    client = mqtt.Client('Recognition') #Create a client and set it ID
    client.connect(BROKER_ADDRESS) # Set the IP of the broker client and connect to it
    client.loop_start() 
    return client
print("Connexion au broker MQTT")

TOPIC = "home/entry_door/detection"
mqtt_client = init_mqtt_connection()

if not os.path.exists(directory):
    os.mkdir(directory)



satisfait = 0
while satisfait == 0:
    Question = input("Voulez vous enregistré une nouvelle personne(oui/non)?")
    if Question == ("oui"):
        prenom = input("Comment vous appellez vous : ")
        print("Mettez vous devant la caméra")
        time.sleep(3)
        print("Souriez!!!!")
        time.sleep(1)
        print("3")
        time.sleep(1)
        print("2")
        time.sleep(1)
        print("1")
        time.sleep(1)
        print("FLASHHH!!!!!!!!")
        
        urllib.request.urlretrieve(
            "http://" + CAMERA_ADDRESS + "/capture",
            directory + "/" + prenom + ".jpg")
        
        new_image = face_recognition.load_image_file(directory + "/" + prenom + ".jpg")
        new_detected_faces = face_recognition.face_encodings(new_image)
        if new_detected_faces :
            img = Image.open(directory + "/" + prenom + ".jpg")
            img.show()
            Q1 = input("Etes vous satisfait (o/n)?")
            if Q1 == "n":
                os.remove(directory + "/" + prenom + ".jpg")
        else :
            print("Pas de visage detecté")
            os.remove(directory + "/" + prenom + ".jpg")
            Q2 = input("Voulez vous réessayer (o/n)?")
            if Q2 == "n":
                print("Lancement de la detection")
                satisfait = 1
            

    elif Question == ("non"):
        print("Lancement de la detection")
        satisfait = 1


while 1 : 
    try :
        urllib.request.urlretrieve(
        'http://' + CAMERA_ADDRESS + '/capture',
        "capture.jpg")
    except:
        pass
    

    for filename in os.scandir(directory):
        if filename.is_file():
            known_image = face_recognition.load_image_file(filename.path)

            unknown_image = face_recognition.load_image_file("capture.jpg")

            users_encoding = face_recognition.face_encodings(known_image)[0]

            detected_faces = face_recognition.face_encodings(unknown_image)
            if detected_faces :
                unknown_encoding = face_recognition.face_encodings(unknown_image)[0]
                results = face_recognition.compare_faces(
                [users_encoding], unknown_encoding)
                print(results[0])
                if results[0] == True :
                    print("authorizé")
                    mqtt_client.publish(TOPIC , "authorized")
                elif results[0] == False :
                    print("non authorizé")
            else :
                print("pas de visage")
    
    time.sleep(0.5)

