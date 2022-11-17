FROM final-diplomv3:debian10

WORKDIR /usr/app/src

COPY input.csv

CMD ["python3", "./ProjektKode.py"]
