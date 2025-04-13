import tkinter as tk
import serial


#Main Screen
root = tk.Tk()
root.state('zoomed')
# root.attributes('-fullscreen', True)
# root.geometry(str(root.winfo_screenwidth())+"x"+str(root.winfo_screenheight()))
root.minsize(800,800)
root.title("EtchASketch")
canvas = tk.Canvas(root, width=800, height=800, bg = "white")
canvas.place(relx=0.5, rely=0.5, anchor=tk.CENTER)

comport = 'COM10'
baudrate = 9600

ser = serial.Serial(comport, baudrate, timeout=1)

cursor = 0;

def clearCanvas():
    global cursor

    canvas.delete("all")

    #Draw Grid
    for i in range(0,801,res_size):
        canvas.create_line(i,0,i,800,fill="grey")
        canvas.create_line(0,i,800,i,fill="grey")

    #Draw Cursor
    cursor = canvas.create_rectangle(
                            cursor_x, 
                            cursor_y, 
                            cursor_x + res_size, 
                            cursor_y + res_size, 
                            fill = "white",
                            outline = "grey"
    )


#Parameters
res_size = 10
cursor_x = 400
cursor_y = 400
filled_pixels = []
X,Y,LBut,cursor_drawing = 0,0,False,False

clearCanvas()
def update_canvas():

    global X,Y,LBut,cursor_drawing,filled_pixels

    #Take Serial Input
    try:
        data = ser.readline().decode('utf-8')
    except:
        update_canvas()
    data = ser.readline().decode("utf-8")
    data = "{x = 0, y = 400, LBut = 0, RBut = 0}"
    X += 5
    print(data)


    #Parse Serial Input
    X = int(data[(data.find("x")+4) : data.find(",")])
    Y = int(data[(data.find("y")+4) : data.find(",", data.find("y"))])
    LBut = bool(int(data[(data.find("RBut")+7)]))
    cursor_drawing = bool(int(data[(data.find("LBut")+7)]))


    #Add filled rectangles to array
    if cursor_drawing : 
        canvas.create_rectangle(
            X,
            Y,
            X + res_size,
            Y + res_size,
            fill = "grey"
        )

    if LBut:
        clearCanvas()

        
    #Update Cursor Location
    canvas.moveto(cursor,X,Y)
    canvas.itemconfig(cursor, fill = "blue" if cursor_drawing else "red")


    root.after(22,update_canvas)

root.after(22,update_canvas)
root.mainloop()
