#!/bin/bash

#Read data
python3 getData.py
#Create image with new data file
sudo docker build -t temp-image .

#Execute container 
sudo docker run -it temp-image > ~/Documents/Docker/predict.txt

#"Cleanup"
sudo docker rmi -f temp-image
