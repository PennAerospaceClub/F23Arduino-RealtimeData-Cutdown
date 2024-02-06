import csv
import re
import numpy as np
import matplotlib.pyplot as plt

latit = 3
longit = 4
altit = 5
course = 6
speed = 7

def fillin(startx, endx, starty, endy, time):
     slope = (endy-starty) / (endx-startx)
     result = slope * (time - startx) + starty
     return result

def run(col):

    with open('datalog.csv', 'r') as f:
        file = list(csv.reader(f))
     
    datao = []
    xindex=120
    xodata = []
    timestamps = []
    lastentry = 0
    zerosection = False

    for row in file[120:len(file)-1]:
            relevantrow = []
            for i in range(8):
                relevantrow.append(float(str(row[i])[str(row[i]).index(":")+1:]))
            if (relevantrow[col] == 0):
                if (not zerosection):
                    zerosection = True
            else:
                if (zerosection):
                    zerosection = False
                    timestamps.append([lastentry, xindex])
                lastentry = xindex
            datao.append(relevantrow[col])
            xodata.append(xindex)
            xindex += 1
            
    data = []
    xindex=120
    xdata = []
    zdata = []

    for row in file[120:len(file)-1]:
            relevantrow = []
            for i in range(8):
                relevantrow.append(float(str(row[i])[str(row[i]).index(":")+1:]))
            if (relevantrow[col] != 0.0):
                data.append(relevantrow[col])
                xdata.append(xindex)
            else:
                for pair in timestamps:
                    if (xindex > pair[0] and xindex < pair[1]):
                        point = fillin(pair[0], pair[1], datao[pair[0]-120], datao[pair[1]-120], xindex)
                        data.append(point)
                        xdata.append(xindex)
            xindex += 1

    return (data, datao, xdata, xodata)

plt.rcParams["figure.figsize"] = [7.50, 3.50]
plt.rcParams["figure.autolayout"] = True

#fill in longit, latit, altit, course, or speed
latcol = run(latit)[0]
longcol =run(longit)[0]
altcol = run(altit)[0]
coursecol = run(course)[0]
speedcol = run(speed)[0]

cleaneddata = []
timestamp = 0

with open('CLEANED.CSV', 'w', newline='') as endfile:
     writer = csv.writer(endfile)

     for i in range(721):
        writer.writerow(["entry: " + str(timestamp), "latitude: " + str(latcol[i]), "longitude: " + str(longcol[i]), 
                         "altitude: " + str(altcol[i]), "course: " + str(coursecol[i]), "speed: " + str(speedcol[i])])
        timestamp += 1