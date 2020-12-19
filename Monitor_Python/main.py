from tkinter import messagebox
import serial
import threading
from tkinter import *

# Set display sizes
BUTTON_SIZE = 100
NUMBER_BUTTON = 3
MARGIN = 5
WINDOW_WIDTH = MARGIN + ((BUTTON_SIZE + MARGIN) * NUMBER_BUTTON)
WINDOW_HEIGHT = (2 * MARGIN) + BUTTON_SIZE
# Set colours
# R G B
WHITE = "#FFFFFF"
RED = "#FF0000"
GREEN = "#008000"
BLUE = "#0000FF"
NEXT = b'n'
PREV = b'p'
global STATE
global root
BAUD = 115200
PORT_COM4 = "COM4"
PARITY = serial.PARITY_NONE
BYTE_SIZE = serial.EIGHTBITS
STOP_BITS = serial.STOPBITS_ONE


def new_state():
    # Changes the LED lights displayed on the Monitor Gui
    if STATE == 0:
        root.led1 = root.canvas.create_oval(x, MARGIN, x + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=WHITE)
        root.led2 = root.canvas.create_oval(y, MARGIN, y + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=WHITE)
    elif STATE == 1:
        root.led1 = root.canvas.create_oval(x, MARGIN, x + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=RED)
        root.led2 = root.canvas.create_oval(y, MARGIN, y + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=BLUE)
    elif STATE == 2:
        root.led1 = root.canvas.create_oval(x, MARGIN, x + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=WHITE)
        root.led2 = root.canvas.create_oval(y, MARGIN, y + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=RED)
    elif STATE == 3:
        root.led1 = root.canvas.create_oval(x, MARGIN, x + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=RED)
        root.led2 = root.canvas.create_oval(y, MARGIN, y + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=GREEN)


def change_state(increment):
    # Changes the State on the Monitor Gui
    global STATE
    if STATE <= 3:
        if increment == 1:
            STATE = STATE + 1
        elif increment == 0:
            STATE = STATE - 1

    if STATE == -1:
        STATE = 3
    if STATE == 4:
        STATE = 0
    root.lbl_value.config(text="State: " + str(STATE))
    new_state()


class UARTLINK:
    def __init__(self):
        # Create a serial instance
        self.connection = serial.Serial()
        self.connection.port = PORT_COM4
        self.connection.baudrate = BAUD
        self.connection.parity = PARITY
        self.connection.bytesize = BYTE_SIZE
        self.connection.stopbits = STOP_BITS
        # Opening serial connection
        self.connection.open()
        self.running = True
        self.read_thread = threading.Thread(target=self.state(all))
        self.read_thread.start()

    def state(self, args):
        # Writes into UART NEXT/PREV (1/0)
        if args == 1:
            self.connection.write(NEXT)
            change_state(1)
        elif args == 0:
            self.connection.write(PREV)
            change_state(0)


if __name__ == "__main__":
    # Initializing a UART Connection
    uart = UARTLINK()
    # Initializing python Tkinter gui
    root = Tk()
    root.canvas = Canvas(width=220, height=150)
    root.canvas.grid(row=1, column=1)
    root.resizable(False, False)

    x = MARGIN
    y = MARGIN + ((MARGIN + BUTTON_SIZE) * 1)
    # (STATE 0: 00) (STATE 1: 11) (STATE 2: 02) (STATE 3: 13)

    # Add some "lights" to the canvas
    root.led1 = root.canvas.create_oval(x, MARGIN, x + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=WHITE)
    root.led2 = root.canvas.create_oval(y, MARGIN, y + BUTTON_SIZE, BUTTON_SIZE + MARGIN, fill=WHITE)

    # Setting up previous button
    root.btn_prev = Button(master=root, text="PREV", command=lambda: uart.state(0))
    root.btn_prev.grid(row=0, column=0, sticky="nsew")

    # Setting up current state label
    STATE = 0
    root.lbl_value = Label(master=root, text="State: " + str(STATE))
    root.lbl_value.grid(row=0, column=1)

    # Setting up next button
    root.btn_next = Button(master=root, text="NEXT", command=lambda: uart.state(1))
    root.btn_next.grid(row=0, column=2, sticky="nsew")


    def on_closing():
        if messagebox.askokcancel("Quit", "Do you want to quit?"):
            # Resets the MCU before closing
            if STATE == 1:
                uart.state(0)
            elif STATE == 2:
                uart.state(0)
                uart.state(0)
            elif STATE == 3:
                uart.state(1)
            root.destroy()


    root.protocol("WM_DELETE_WINDOW", on_closing)
    root.mainloop()
