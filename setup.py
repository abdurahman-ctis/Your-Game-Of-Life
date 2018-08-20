import re
import sys

file = "Your Game of Life/Source.cpp"

def show():
    f = open("templates.txt", 'r')
    data = f.read()
    f.close()
    data = re.findall("NAME:(.*):", data)
    i=1
    for d in data:
        print(i,"- "+d)
        i+=1

def choose():
    print("ERROR")
    choice = int(input())
def add():
    name = input("Enter name: ")
    print("\nFORMAT FOR CONDITION INPUT:\nn[</>/==]somenumber [AND/OR] n[</>/==]somenumber [AND/OR]...\n")
    live = input("Enter neighbour conditions that kill alive cells: ")
    live.replace('n','')
    die = input("Enter neighbour conditions that bring dead cells to life: ")
    die.replace('n','')
    with open("templates.txt", "a") as f:
        f.write("""
NAME:%s:
    ALIVE:%s
    DEAD:%s
end
    """%(name,live,die))
    
def delete():
    print("ERROR")
    

def clearLife(): # resets life() to classic rules.
    f = open(file,'r')
    filedata = f.read()
    f.close()

    newdata = re.sub(r"void life.*void on",
                     """void life(int i, int j) {

        int n = countNeighbours(cells[i][j], i, j);

        /*
                TODO: Write instructions
        */

        if (cells[i][j]) {
                if (n < 2 || n>3)
                        newCells[i][j] = false;
        }

        else {
                if (n == 3)
                        newCells[i][j] = true;
        }\n}\n\nvoid on""", filedata, flags=re.DOTALL)
    f = open(file,'w')
    f.write(newdata)
    f.close()
    print("ERROR")
    sys.exit()

if __name__=="__main__":
    print("Welcome to Your Game of Life!")
    c = -1
    while c!=4:
        print("""
What would you like to do?

0. Show template list.
1. Choose existing life template.
2. Add a template.
3. Delete a template.
4. Exit
    """)
        while True:
            c = int(input("Enter your choice: "))
            if c<0 or c>4:
                print("Wrong input!")
                continue
            else:
                break
        if c==0:
            show()
        elif c==1:
            choose()
        elif c==2:
            add()
        elif c==3:
            delete()
    
