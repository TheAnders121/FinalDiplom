import sys
import time
import paho.mqtt.client as mqtt #Connect to the MQTT Broker
import xlwt #https://pypi.org/project/xlwt/ - help with creating xls/csv files

class ConvertMQTT:

    def __init__(self,mqtt_server,fname):
        self.topics = []
        self.number = 10
        self.mqtt_server = mqtt_server 
        self.fname = fname
        self.readings = 0

        self.wb = xlwt.Workbook()
        self.ws = self.wb.add_sheet("Data from MQTT",cell_overwrite_ok=True)
        self.ws.write(0, 0, "Data from MQTT")
        self.columns = ["Date Time"]        

        self.files = 1
        
    def addTopics(self,topic):
        self.topics.append(topic)

    def setReadings(self,number):
        self.number = number

    def on_connect(self,client, userdata, flags, rc):
        for topic in self.topics:
            print("Suscribe to "+topic)
            client.subscribe(topic)

    def on_message(self,client, userdata, msg):
        self.ws.write(self.files, 0, time.strftime("%d-%m-%Y %H:%M:%S", time.localtime()))
        
        topic = str(msg.topic)
        idx_topic = self.topics.index(topic)
        self.ws.write(self.files, (idx_topic+1), str(msg.payload))
        self.readings = self.readings + 1
        self.files = self.files + 1

        if(self.readings == self.number):        
            self.wb.save(self.fname)
            sys.exit("Done! "+str(self.number)+" the readings")
        
    
    def start(self):
        self.ws.write(0, 0, "Date Time")
        c = 1
        for topic in self.topics:
            self.ws.write(0, c, topic)
            c = c + 1

        client = mqtt.Client()
        client.on_connect = self.on_connect
        client.on_message = self.on_message

        client.connect(self.mqtt_server, 1883, 60)
        client.loop_forever()




