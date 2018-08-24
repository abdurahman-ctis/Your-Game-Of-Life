import re
import sys
import os

source = "Your-Game-of-Life/Source.cpp"
        
def show():
    f = open("templates.txt", 'r')
    data = f.read()
    f.close()
    data = re.findall("NAME:(.*):", data)
    i=1
    for d in data:
        print(i,"- "+d)
        i+=1

def explore():
    show()
    s = input("Enter name of the template you want to explore: ")
    f = open("templates.txt", 'r')
    data = f.read()
    f.close()
    data = re.search(s+'(.*)end', data, flags=re.DOTALL)
    if data:
        data = data.group(1)
        alive = re.search("E:(.*)",data)
        alive = alive.group(1)
        dead = re.search("D:(.*)", data)
        dead = dead.group(1)
        print("\nProperties of " + s + " are:")
        print("Alive cells die if " + alive)
        print("Dead cells come to life if " + dead)
        print("Note: n is the number of alive neighbours(adjacent cells).")
    else:
        explore()
    

def choose():
    show()
    s = input("Enter the desired template: ")
    f = open("templates.txt", 'r')
    data = f.read()
    f.close()
    data = re.search(s+'(.*)end', data, flags=re.DOTALL)
    if data:
        data = data.group(1)
        alive = re.search("E:(.*)",data)
        alive = alive.group(1)
        alive = alive.replace('OR','||')
        alive = alive.replace('AND','&&')
        dead = re.search("D:(.*)", data)
        dead = dead.group(1)
        dead = dead.replace('OR','||')
        dead = dead.replace('AND','&&')
        f = open(source,'r')
        filedata = f.read()
        f.close()
        newdata='''void life(int i, int j) {{

            int n = countNeighbours(cells[i][j], i, j);

            /*
                    Write instructions
            */

            if (cells[i][j]) {{
                    if ({live})
                            newCells[i][j] = false;
            }}

            else {{
                    if ({die})
                            newCells[i][j] = true;
            }}\n}}\n\nvoid on'''.format(live=alive, die=dead)
        newdata = re.sub(r"void life.*void on",
                         newdata, filedata, flags=re.DOTALL)
        f = open(source,'w')
        f.write(newdata)
        f.close()
        print("Chosen template was uploaded to the Source.cpp file.")
        
    else:
        choose()
    
    
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
    show()
    s = input("Enter name of the template you want to delete: ")
    f = open("templates.txt", 'r')
    data = f.read()
    f.close()
    data = re.sub(r"NAME:"+s+"(.*)end","", data, flags=re.DOTALL)
    f = open("templates.txt", 'w')
    f.write(data)
    f.close()
    

def clearLife(): # resets life() to classic rules.
    f = open(source,'r')
    filedata = f.read()
    f.close()

    newdata = re.sub(r"void life.*void on",
                     """void life(int i, int j) {

        int n = countNeighbours(cells[i][j], i, j);

        /*
                Write instructions
        */

        if (cells[i][j]) {
                if (n < 2 || n>3)
                        newCells[i][j] = false;
        }

        else {
                if (n == 3)
                        newCells[i][j] = true;
        }\n}\n\nvoid on""", filedata, flags=re.DOTALL)
    f = open(source,'w')
    f.write(newdata)
    f.close()
    sys.exit()

if __name__=="__main__":
    print("Welcome to Your Game of Life!")
    c = -1
    while c!=4:
        print("""
What would you like to do?

0. Explore template list.
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
            explore()
        elif c==1:
            choose()
        elif c==2:
            add()
        elif c==3:
            delete()
    clearLife()
