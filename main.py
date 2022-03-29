import curses
import textwrap
import encoding


'''
screen for the enigma project.
if you have trouble with the screen just use enoding.py from the terminal

@author Hawkins Peterson
@version 12.19.21
'''


class enigma:
    def __init__(self):
        self.screen = curses.initscr()      # starting the screen
        self.screen.keypad(True)            # accepts ctrl and other weird keys
        curses.echo(False)                  # doesn't show which key pressed
        curses.curs_set(False)              # doesn't show cursor
        self.screen.clear()                 # clears the screen
        self.screen.refresh()               # refresh
        dims = self.screen.getmaxyx()            # get screen dims
        yMax = dims[0]
        xMax = dims[1]

        # constructing all the subscreens
        self.header = self.screen.subwin(9, xMax - 2, 0, 1)
        activeWin = self.screen.subwin(yMax - 8, xMax, 8, 0)

        self.keyboard = activeWin.subwin(15, 100, yMax - 16, 43)
        self.output = activeWin.subwin(10, xMax - 7, 21, 4)
        self.outLine = activeWin.subwin(12, xMax - 5, 20, 3)
        self.rotors = activeWin.subwin(11, 30, 9, self.getXMid(activeWin) - 15)

        self.encrypter = encoding.Encoding()
        self.rotorSettings = self.encrypter.getRotorSettings()
        self.plaintext = ""
        self.ciphertext = ""
        self.keyboardAlphabet = "qwertyuiopasdfghjklzxcvbnm"

    # gets middle y value (because I'm lazy)
    def getYMid(self, screen):
        return int(screen.getmaxyx()[0] / 2)

    # gets middle x value (because I'm lazy)
    def getXMid(self, screen):
        return int(screen.getmaxyx()[1] / 2)

    # easy printing things
    def formatString(self, text, width):
        return ''.join(textwrap.wrap(text, width)[-5:])

    # needs to be called, otherwise bad news bears
    def endScreen(self):
        self.screen.clear()
        curses.endwin()

    # basis for all draw stuff/-
    def drawBlankScreen(self):
        self.screen.clear()
        mid = self.getXMid(self.header)
        self.screen.box()
        self.header.hline(8, 0, "-", self.screen.getmaxyx()[1])
        self.header.addstr(3, mid - 9, "THE ENIGMA MACHINE")
        self.screen.refresh()

    # when you press exit,6
    def drawEscape(self, selected):
        self.screen.refresh()
        mid = self.getXMid(self.output)
        self.output.box()
        self.output.addstr(0, mid-11, "PLAINTEXT / CIPHERTEXT")
        self.output.refresh()
        self.keyboard.box()
        self.rotors.box()
        self.header.refresh()

        if selected % 3 == 0:
            self.keyboard.addstr(0, 46, "KEYBOARD")
            self.rotors.addstr(0, 8, "ROTOR SETTINGS:", curses.A_REVERSE)
            self.header.addstr(5, mid - 7, "EXIT")
        elif selected % 3 == 1:
            self.keyboard.addstr(0, 46, "KEYBOARD", curses.A_REVERSE)
            self.rotors.addstr(0, 8, "ROTOR SETTINGS:")
            self.header.addstr(5, mid - 7, "EXIT")
        else:
            self.keyboard.addstr(0, 46, "KEYBOARD")
            self.rotors.addstr(0, 8, "ROTOR SETTINGS:")
            self.header.addstr(5, mid - 2, "EXIT", curses.A_REVERSE)

        self.keyboard.refresh()
        self.rotors.refresh()

    # draws the middle section (the output if you will).
    def drawOutput(self):
        xMax = self.output.getmaxyx()[1]

        self.output.addstr(0, 0, "Plaintext:")
        self.output.addstr(5, 0, "Ciphertext:")
        self.output.addstr(1, 0, self.formatString(self.plaintext, xMax))
        self.output.addstr(6, 0, self.formatString(self.ciphertext, xMax))
        self.output.refresh()

    # draws the rotors (top section)
    def drawRotors(self, rotorArray):
        for y in range(3):
            x = 15 + y * 5 - 5
            self.rotors.vline(4, x - 1, "|", 5)
            self.rotors.vline(4, x + 1, "|", 5)
            self.rotors.addch(2, x, str(rotorArray[0][y]))
            for z in range(3):
                self.rotors.addstr(4 + 2 * z, x, rotorArray[1][y][z])
        self.rotors.refresh()

    # draws the keyboard (bottom section)
    def drawKeyboard(self, highlightedKey=" "):
        keyAlphabet = "qwertyuiopasdfghjklzxcvbnm"
        xPosBases = [41, 45, 36]
        try:
            for i in range(26):
                row = int((i - 1) / 9)
                yPos = row * 6 + 1  # 0-9 = 2, 10-18 = 8, 19-25 = 14
                mid = self.getXMid(self.keyboard)
                xPos = mid - xPosBases[row] + (i - row * 9) * 9
                if highlightedKey == keyAlphabet[i]:
                    o = ord(keyAlphabet[i])
                    self.keyboard.addch(yPos, xPos, o, curses.A_REVERSE)
                else:
                    self.keyboard.addch(yPos, xPos, ord(keyAlphabet[i]))
            self.keyboard.refresh()
        except Exception as e:
            self.endScreen()
            print(e)
            print("yMax: " + str(self.yMax) + " xMax: " + str(self.xMax))
            print("yPos: " + str(yPos) + " xPos: " + str(xPos))

    # draws section where you can edit sections (in progress)
    def drawEdit(y, x, selected):
        """if y % 2 == 0:
            yPos = 2
        else:
            yPos = 4
        xPos = (x % 3) * 5 + 10
        """

    def drawReset(self):
        self.screen.clear()
        self.screen.refresh()
        self.drawBlankScreen()
        self.drawKeyboard()
        self.drawRotors(self.encrypter.getRotorArray())
        self.drawOutput()

    def machineLoop(self):
        # loop
        action = 0
        allowed = [".", ",", " ", "!", "?"]
        while (True):
            # getting keypress
            action = ord(chr(self.screen.getch()).lower())
            if action == 27:
                return 0
            elif action >= 97 and action <= 122:
                self.plaintext += chr(action)
                self.ciphertext += self.encrypter.encrypt(chr(action), False)
            for a in allowed:
                if action == ord(a):
                    self.plaintext += a
                    self.ciphertext += a
            self.drawKeyboard(self.ciphertext[-1])
            self.drawRotors(self.encrypter.getRotorArray())
            self.drawOutput()
            self.screen.refresh()

    def settingsLoop(self):
        action = 0
        selected = 1
        while (True):
            self.drawEscape(selected)
            action = self.screen.getch()
            if action == 27:
                return -1
            elif action == curses.KEY_UP:
                selected -= 1
            elif action == curses.KEY_DOWN:
                selected += 1
            elif action == ord("\n"):
                return selected

    def editLoop(self):
        y = 0
        x = 0
        selected = False
        while (True):
            self.drawSettings(y, x, selected)
            action = self.screen.getch()
            if selected:
                if action == 27:
                    selected = False
                elif action == curses.KEY_UP:
                    array[y][x] += 1
                elif action == curses.KEY_DOWN:
                    array[y][x] -= 1
                elif action == ord("\n"):
                    return (y, x)
            else:
                if action == 27:
                    return -1
                elif action == curses.KEY_UP:
                    y -= 1
                elif action == curses.KEY_DOWN:
                    y += 1
                elif action == curses.KEY_LEFT:
                    x -= 1
                elif action == curses.KEY_RIGHT:
                    x += 1
                elif action == ord("\n"):
                    selected = True

    def run(self):
        self.drawBlankScreen()
        self.drawKeyboard()
        self.drawRotors()
        self.drawOutput()

        try:
            editing = 1
            while True:
                if editing == -1:
                    break
                elif editing == 0:
                    self.editLoop(self.encrypter.getRotorSettings())
                elif editing == 1:
                    self.machineLoop()
                editing = self.settingsLoop()
                self.drawReset()
            self.endScreen()
            self.encrypter.close(False)
            i = (self.rotorSettings, self.plaintext, self.ciphertext)
            outputTxt = "%s \n\nplaintext:  %s \nciphertext: %s\n\n" % i
            print(outputTxt)
            f = open("output.txt", "a")
            f.write(outputTxt)
            f.close()

        except Exception as e:
            self.endScreen()
            print(e)


enigma = enigma()
enigma.run()
