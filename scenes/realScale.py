import re


regex="(\w+) = (.+)"

file = open ("realScale.txt")


size = 10

distance = 100
sizeSun = 0

res = ""

xml = open("sistemaSolar_realScale.xml")
res = xml.read()

def sub(name,value,res):
    return re.sub(f"#{name}",f'"{value}"',res)
day = 1
year = day * 365

for line in file:
    match = re.match(regex,line)
    if match is not None :
        value = float(match[2])
        if "distance" in match[1]:
            value = size*sizeSun + distance * value
            if "Neptune" in match[1]:
                rMax = value + distance * 3
                res = sub("rMax",rMax,res)
                res = sub("minusRmax",-rMax,res)
                res = sub("doubleRmax",rMax*2,res)

        if "sizeSun" in match[1]:
            sizeSun = float(match[2])
        if "year" in match[1]:
            value *= year
        if "day" in match[1]:
            value *= day
        res = sub(match[1],value,res)


rMin = (sizeSun/2) * size
res = sub("rMin",rMin,res)
res = sub("lua",size+2,res)

newFile = open("realScale.xml",'w+')
newFile.write(res)
    

