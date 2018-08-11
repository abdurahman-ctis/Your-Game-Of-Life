import re

file = "Your Game of Life/Source.cpp"

def clearLife():
    f = open(file,'r')
    filedata = f.read()
    f.close()

    newdata = re.sub(r"void life.*void",
                     "void life(int i, int j) {\n\n}\n\nvoid", filedata, flags=re.DOTALL)

    print(newdata)
    #f = open(file,'w')
    #f.write(newdata)
    #f.close()

clearLife()
