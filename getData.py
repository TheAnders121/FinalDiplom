from MQTT2csv import ConvertMQTT

IP_Address = "0.0.0.0" #MQTT broker IP-address

m2csv = ConvertMQTT(IP_Address,"input.csv")

m2csv.setReadings(5) #5 readings, can be changed to more if needed

# More topics can be added here
m2csv.addTopics("/esp/temp")

m2csv.start()