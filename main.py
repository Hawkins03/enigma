import curses
import sys
import textwrap
import copy
from enum import Enum
import encoding

'''
screen for the enigma project. if you have trouble with the screen just use encoding.py from the terminal

@author Hawkins Peterson
@version 12.19.21
'''

'''
Returns the y midpoint of the selected screen (note, rounds down)
@return the y midpoint of the selected screen (note, rounds down)
'''
def getYMid(screen):
    return int(screen.getmaxyx()[0] / 2)

'''
Returns the x midpoint of the selected screen (note, rounds down)
@return the x midpoint of the selected screen (note, rounds down)
'''
def getXMid(screen):
    return int(screen.getmaxyx()[1] / 2)

'''
Returns the last 12 lines of a string wrapped at length "width"
@return the last 12 lines of a string wrapped at length "width"
@param text the text to format
@param width the width of the text to wrap it at
'''
def formatString(text, width):
    return ''.join(textwrap.wrap(text, width = width)[-5:])

'''
ends the screen
@param screen the screen needed to end
'''
def endScreen(screen):
    screen.clear()
    curses.endwin()


'''
constructs a blank screen with only borders and the title
@param header the header subScreen
@param screen the main screen
'''
def drawBlankScreen(header, screen):
    screen.box()
    header.hline(8, 0, "-", screen.getmaxyx()[1])
    header.addstr(3, getXMid(header) - 9, "THE ENIGMA MACHINE")
    screen.refresh()



def drawSettings(screen, rotors, output, keyboard):
    screen.refresh()

    rotors.box()
    rotors.addstr(0, getXMid(rotors) - 7, "ROTOR SETTINGS:")
    rotors.refresh()

    output.box()
    output.addstr(0, getXMid(output) - 11, "PLAINTEXT / CIPHERTEXT")
    output.refresh()
    
    keyboard.box()
    keyboard.addstr(0, getXMid(keyboard) - 4, "KEYBOARD")
    keyboard.refresh()

def drawOutput(output, plaintext, ciphertext):
    yMax = output.getmaxyx()[0]
    xMax = output.getmaxyx()[1]
    output.addstr(0, 0, "Plaintext:")
    output.addstr(5, 0, "Ciphertext:")
    output.addstr(1, 0, formatString(plaintext, xMax))
    output.addstr(6, 0, formatString(ciphertext, xMax))
    output.refresh()

def drawRotors(rotors, rotorArray):
    for y in range(3):
        x = getXMid(rotors) + y * 5 - 5
        rotors.vline(4, x - 1, "|", 5)
        rotors.vline(4, x + 1, "|", 5)
        rotors.addch(2, x, str(rotorArray[0][y]))
        for z in range(3):
            rotors.addstr(4 + 2 * z, x, rotorArray[1][y][z])
    rotors.refresh()

'''
draws the keyboard
@param screen the screen used by the keyboard
@param keyPressed a int telling which key is currently pressed down
'''
def drawKeyboard(keyboard, highlightedKey = " "):
    keyAlphabet = "qwertyuiopasdfghjklzxcvbnm"
    xPosBases = [41, 45, 36]
    try:
        for i in range(26):
            row = int((i - 1) / 9)
            yPos = row * 6 + 1 #0-9 = 2, 10-18 = 8, 19-25 = 14
            xPos = getXMid(keyboard) - xPosBases[row] + (i - row * 9) * 9
            if highlightedKey == keyAlphabet[i]:
                keyboard.addch(yPos, xPos, ord(keyAlphabet[i]), curses.A_REVERSE)
            else:
                keyboard.addch(yPos, xPos, ord(keyAlphabet[i]))
        keyboard.refresh();
    except Exception as e:
        dims = keyboard.getmaxyx()
        yMax = dims[0]
        xMax = dims[1]
        endScreen(screen)
        print(e)
        print("yMax: " + str(yMax) + " xMax: " + str(xMax))
        print("yPos: " + str(yPos) + " xPos: " + str(xPos))

#try block because errors are a GIANT pain in curses
try:
    #init screen (blank canvas)
    screen = curses.initscr() #starting the screen
    screen.keypad(True) #accepts ctrl and other weird keys
    curses.echo(False) #doesn't show which key pressed
    curses.curs_set(False) #doesn't show cursor
    screen.clear() #clears the screen
    screen.refresh() #refresh
    dims = screen.getmaxyx() #get screen dims
    yMax = dims[0]
    xMax = dims[1]

    #constructing all the subscreens
    header = screen.subwin(9, xMax - 2, 0, 1)
    activeWin = screen.subwin(yMax - 8, xMax, 8, 0)

    keyboard = activeWin.subwin(15, 100, yMax - 16, int((xMax - 100) / 2))
    output = activeWin.subwin(10, xMax - 7, 21, 4)
    outLine = activeWin.subwin(12, xMax - 5, 20, 3)
    rotors = activeWin.subwin(11, 30, 9, getXMid(activeWin) - 15)
    
    encrypter = encoding.Encoding()
    rotorSettings = encrypter.getFancyRotorSettings()
    plaintext =  ""
    ciphertext = ""
    keyboardAlphabet = "qwertyuiopasdfghjklzxcvbnm"

    #drawing basic stuff
    drawBlankScreen(header, screen)
    #drawSettings(screen, rotors, outLine, keyboard)
    drawKeyboard(keyboard)
    drawRotors(rotors, encrypter.getFancyRotorArray())
    drawOutput(output, plaintext, ciphertext)
    

    #loop
    action = 0
    allowed = [".", ",", " ", "!", "?"]
    while (True):
        #getting keypress
        action = ord(chr(screen.getch()).lower()) #ord and chr to convert uppercase letters to lowercase letters
        if action == 27:
            break
        elif action >= 97 and action <= 122: #normal a-z lowercase letters
            plaintext += chr(action)
            ciphertext += encrypter.encrypt(chr(action), False)
        for a in allowed: #checking for punctuation and just adding it
            if action == ord(a):
                plaintext += a
                ciphertext += a
        drawKeyboard(keyboard, ciphertext[-1])#last encrypted thingey
        drawRotors(rotors, encrypter.getFancyRotorArray())
        drawOutput(output, plaintext, ciphertext)
        screen.refresh()
    
    endScreen(screen)
    encrypter.close(False)
    outputTxt = "%s \n\nplaintext:  %s \nciphertext: %s\n\n" % (rotorSettings, plaintext, ciphertext)
    print(outputTxt)
    print(action)
    f = open("output.txt", "a");
    f.write(outputTxt)
    f.close()
except Exception as e:
    endScreen(screen)
    print(e)

