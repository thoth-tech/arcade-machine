#Note this script requires super user privilages if the the user is not in the "input" group
from time import sleep
from datetime import datetime, timedelta
from os import set_blocking, listdir, path
from subprocess import call, Popen

pollTime = 10 #Seconds
timeout = 10  #minutes
processName = 'emulationstation'
lastInput = datetime.now()
inputs = []

def has_input(file): 
    data = file.read()
    return data is not None #Event file may be empty if no input has been registered for some time

def get_inputs():
    dir = r'/dev/input/'
    for file in listdir(dir):
        if(not path.isdir(dir+file) and "event" in file):
            f = open(dir+file, "rb")
            set_blocking(f.fileno(), False)
            inputs.append(f)

get_inputs()#Find all input devices
while True:
    for f in inputs:
        if(has_input(f)):
            lastInput = datetime.now()

    if(datetime.now() - lastInput > timedelta(minutes=timeout)):
        lastInput += timedelta(days=1)#Add time to the delay counter to stop the program restarting endlessly
        #print("Idle timeout reached")
        call(['pkill', '-f', processName]) #kill old process
        sleep(1)
        Popen([processName])
    # else:
    #     print("Idle time: " + str(datetime.now() - lastInput))
    sleep(pollTime)
