#!/usr/bin/python
import random, sys

numberOfProcess = int(sys.argv[1])
numberOfTimes = int(sys.argv[2])
file = open("saida", "w")
times = []
cpuBurstSum = 0

##### defining functions
def cpuBurstDefinition(times):
	time = int(random.uniform(1,40))
	times.append(time)
	return time

def ioBurstDefinition(times):
	time = int(random.uniform(1, 200))
	times.append(time)
	return 
def setTimesOnFile(file, times):
	for time in times:
		file.write(str(time)+"\t")
	return


for i in range(numberOfProcess):
	file.write(str(int(random.uniform(0,30)))+"\t")
	file.write("processZ\t")
	for j in range(numberOfTimes) :
		cpuBurstSum += cpuBurstDefinition(times)
		ioBurstDefinition(times)
	file.write(str(cpuBurstSum/numberOfTimes)+"\t")
	setTimesOnFile(file, times)
	file.write("\n")
	cpuBurstSum = 0;
	times=[]
file.close()



