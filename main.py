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
        self.rotorSettings = self.encrypter.getFancyRotorSettings()
        self.plaintext = ""
        self.ciphertext = ""
        self.keyboardAlphabet = "qwertyuiopasdfghjklzxcvbnm"

    def getYMid(self, screen):
        return int(screen.getmaxyx()[0] / 2)

    def getXMid(self, screen):
        return int(screen.getmaxyx()[1] / 2)

    def formatString(self, text, width):
        return ''.join(textwrap.wrap(text, width)[-5:])

    def endScreen(self):
        self.screen.clear()
        curses.endwin()

    def drawBlankScreen(self):
        mid = self.getXMid(self.header)
        self.screen.box()
        self.header.hline(8, 0, "-", self.screen.getmaxyx()[1])
        self.header.addstr(3, mid - 9, "THE ENIGMA MACHINE")
        self.screen.refresh()

    def drawSettings(self, selected):
        self.screen.refresh()
        rMid = self.getXMid(self.rotors)
        kMid = self.getXMid(self.keyboard)
        self.keyboard.box()
        if selected % 2 == 0:
            rotors.box()
            mid = self.getXMid(rotors)
            rotors.addstr(0, mid-7, "ROTOR SETTINGS:", curses.A_REVERSE)
            rotors.refresh()
        else:
            self.keyboard.addstr(0, mid - 4, "KEYBOARD", curses.A_REVERSE)
            self.keyboard.refresh()
            rotors.box()
            rotors.addstr(0, mid - 7, "ROTOR SETTINGS:")
            rotors.refresh()

        mid = self.getXMid(self.output)
        self.output.box()
        self.output.addstr(0, mid-11, "PLAINTEXT / CIPHERTEXT")
        self.output.refresh()

        mid = self.getXMid(self.keyboard)
        if selected % 3 == 2:
                    else:
            self.keyboard.box()
            self.keyboard.addstr(0, mid - 4, "KEYBOARD")
            self.keyboard.refresh()

    def drawOutput(self):
        xMax = self.output.getmaxyx()[1]

        self.output.addstr(0, 0, "Plaintext:")
        self.output.addstr(5, 0, "Ciphertext:")
        self.output.addstr(1, 0, self.formatString(self.plaintext, xMax))
        self.output.addstr(6, 0, self.formatString(self.ciphertext, xMax))
        self.output.refresh()

    def drawRotors(self, rotorArray):
        for y in range(3):
            x = self.getXMid(self.rotors) + y * 5 - 5
            self.rotors.vline(4, x - 1, "|", 5)
            self.rotors.vline(4, x + 1, "|", 5)
            self.rotors.addch(2, x, str(rotorArray[0][y]))
            for z in range(3):
                self.rotors.addstr(4 + 2 * z, x, rotorArray[1][y][z])
        self.rotors.refresh()

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
            self.drawRotors(self.encrypter.getFancyRotorArray())
            self.drawOutput()
            self.screen.refresh()

    def settingsLoop(self):
        action = 0
        selected = 2
        while (True):
            self.drawSettings(selected)
            action = self.screen.getch()
            if action == 27:
                return -1
            elif action == curses.KEY_UP:
                selected -= 1
            elif action == curses.KEY_DOWN:
                selected += 1
            elif action == ord("\n"):
                return selected

    def run(self):
        self.drawBlankScreen()
        self.drawKeyboard()
        self.drawRotors(self.encrypter.getFancyRotorArray())
        self.drawOutput()

        try:
            while True:
                self.machineLoop()
                self.settingsLoop()
            self.endScreen(self.screen)
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
