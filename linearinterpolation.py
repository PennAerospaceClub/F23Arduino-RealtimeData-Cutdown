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
output = run(altit)

#prints interpolated data to the console
print(output[0])
plt.plot(output[3], output[1], color="blue") #displays original data, feel free to comment this out if it's messing with the graph scale
plt.plot(output[2], output[0], color="red") #displays interpolated data
plt.show() 