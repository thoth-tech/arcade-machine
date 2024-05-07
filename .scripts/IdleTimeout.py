#Note this script requires super user privilages if the the user is not in the "input" group
import struct
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
    data = file.read(24)
    if data != None: #Event file may be empty if no input has been registered for some time
        return True
    else:
        return False

def get_last_input(file):
    try: #Just in case the file is reset in between calling has_input() and here
        data = file.read()[:-24]#Read LAST input
        time = struct.unpack('4IHHI', data) 
        time = datetime.fromtimestamp(time[0])
    except:
        time = datetime.now()
    return time

def get_inputs():
    dir = r'/dev/input/'
    for file in listdir(dir):
        if not path.isdir(dir+file) and "event" in file:
            f = open(dir+file, "rb")
            set_blocking(f.fileno(), False)
            inputs.append(f)

get_inputs()#Find all input devices
while True:
    for f in inputs:
        if(has_input(f)):
            #print("\nInput detected: " + f.name)
            temp = get_last_input(f)
            if(temp > lastInput): #Get most recent input from all inputs
                lastInput = temp

    if(datetime.now() - lastInput > timedelta(minutes=timeout)):
        lastInput += timedelta(days=1)#Add time to the delay counter to stop the program restarting endlessly
        #print("Idle timeout reached")
        call(['pkill', '-f', processName]) #kill old process
        sleep(1)
        Popen([processName])
    #else:
        #print("Idle time: " + str(datetime.now() - lastInput))
    sleep(pollTime)
