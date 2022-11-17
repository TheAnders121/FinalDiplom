# FinalDiplom

"Dockerfile" used to create the container with proper configurations <br>
"ProjektKode.ipynb" is used for creating and running a GRU for forecasting power output based on weather data <br>
"crontab.txt" is the content of the timescheduler (crontab) running on Raspberry Pi <br>
"data.csv" is the historical weather data used for training the GRU during construction <br>
"dependencies.txt" contains the necessary packages and versions to create a Tensorflow compatible environment <br>
"main.sh" main script run on Raspberry Pi to create a container build with Tensorflow compatible image <br>

"MQQT_to_csv" contains code to establish a MQTT connection to a broker in order to fetch data from a temperature sensor
