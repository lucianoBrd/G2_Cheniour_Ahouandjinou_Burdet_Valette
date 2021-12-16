import face_recognition
import os
import urllib.request
from PIL import Image
import time
import paho.mqtt.client as mqtt



os.mkdir("Faces")
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
            'http://172.20.10.6/capture',
            "Faces/" + prenom + ".jpg")
        new_image = face_recognition.load_image_file("Faces/" + prenom + ".jpg")
        new_detected_faces = face_recognition.face_encodings(new_image)
        if new_detected_faces :
            img = Image.open("Faces/" + prenom + ".jpg")
            img.show()
            Q1 = input("Etes vous satisfait (o/n)?")
            if Q1 == "n":
                os.remove("Faces/" + prenom + ".jpg")
        else :
            print("Pas de visage detecté")
            os.remove("Faces/" + prenom + ".jpg")
            Q2 = input("Voulez vous réessayer (o/n)?")
            if Q2 == "n":
                print("Lancement de la detection")
                satisfait = 1
            

    elif Question == ("non"):
        print("Lancement de la detection")
        satisfait = 1


while 1 : 
    urllib.request.urlretrieve(
    'http://172.20.10.6/capture',
    "capture.jpg")

    """img = Image.open("capture.jpg")"""


    directory = 'Faces'

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
                print(results)
            else :
                print("no face")
    
    time.sleep(2)

