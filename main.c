/*
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
constructs a blank screen with only borders and the title
@param header the header subScreen
@param screen the main screen
'''
def drawBlankScreen(header, screen):
    screen.box()
    header.hline(8, 1, "-", screen.getmaxyx()[1] - 2)
    header.addstr(3, getXMid(header) - 9, "THE ENIGMA MACHINE")
    screen.refresh()

def drawPausedScreen(header, screen, sidebar):
    sidebar.vline(8, 28, "|", screen.getmaxyx()[0] - 8)
    sidebar.addstr(9, 10, "settings: ")
    drawBlankScreen(header, screen)

#def addStringOverflow()
'''
ends the screen
@param screen the screen needed to end
'''
def endScreen(screen):
    screen.clear()
    curses.endwin()


def drawStrings(screen, plaintext, ciphertext, offset = 0):
    try:
        screen.addstr(1, 27, plaintext)
        screen.addstr(13, 27, ciphertext)
        screen.refresh()
    except Exception as e:
        print("ERROR ERORR ERROR")
        endScreen(screen) 
        print(e)

'''
draws the keyboard
if xShifted, moves the keyboard slightly to the right otherwise it just centers it on the middle
@param screen the screen used by the keyboard
@param keyPressed a int telling which key is currently pressed down
'''
def drawKeyboard(screen, keyPressed = -1):
    listedKeyboard = "qwertyuiopasdfghjklzxcvbnm"
    if (keyPressed != -1):
        highlightKey = listedKeyboard.index(chr(keyPressed + 97))
    else:
        highlightKey = -1
    try:
        for i in range(26):
            dims = screen.getmaxyx()
            yMax = dims[0]
            xMax = dims[1]
            if i<=9:
                yPos = yMax - 15
                xPos = getXMid(screen) + i * 9 - 41
            elif i<=18:
                yPos = yMax - 8
                xPos = getXMid(screen) + (i - 9) * 9 - 43
            else:
                yPos = yMax - 2
                xPos = getXMid(screen) + (i - 18) * 9 - 34
            if highlightKey == i:
                screen.addch(yPos, xPos, ord(listedKeyboard[i]), curses.A_REVERSE)
            else:
                screen.addch(yPos, xPos, ord(listedKeyboard[i]))

        screen.refresh();
    except Exception as e:
        endScreen(screen)
        print("yMax: " + str(yMax) + " xMax: " + str(xMax))
        print("yPos: " + str(yPos) + " xPos: " + str(xPos))

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
    header = screen.subwin(9, xMax, 0, 0)
    pauseWin = screen.subwin(yMax - 8, 29, 8, 0)
    activeWin = screen.subwin(yMax - 8, xMax, 8, 0)
    inactiveWin = screen.subwin(yMax - 8, xMax - 28, 8, 28)
    
    halfYMain = getYMid(activeWin)
    activeKeyboard = screen.subwin(16, xMax, yMax - 16, 0)
    activeOutput = screen.subwin(25, xMax, 8, 0)
    inactiveKeyboard = screen.subwin(16, xMax - 8, yMax - 16, 8)
    inactiveOutput = screen.subwin(25, xMax - 8, 8, 8)

    #drawing basic stuff
    drawBlankScreen(header, screen)
    drawKeyboard(activeWin)
    encrypter = encoding.Encoding()
    plaintext = "Plaintext:  "
    ciphertext = "Ciphertext: "
    


    end = False
    while (not end):
        action = ord(chr(screen.getch()).lower())
        if action >= 97 and action <= 122:
            drawKeyboard(activeKeyboard, action - 97)
            plaintext += chr(action)
            ciphertext += encrypter.encrypt(chr(action))
        elif action == 10:
            plaintext += "\n";
            ciphertext += "\n";
        elif action == 8:
            plaintext = plaintext[0, len(plaintext) - 1]
            ciphertext = ciphertext[0, len(ciphertext) - 1]
        else:
            end = True;
        drawStrings(activeOutput, plaintext, ciphertext)
        screen.refresh()
    encrypter.close(False)
    endScreen(screen) #ends program
    print(plaintext + "\n\n" + ciphertext)

    f = open("output.txt", "w");
    f.write(plaintext + "\n\n" + ciphertext)
    f.close()
except Exception as e:
    endScreen(screen)
    print(e)
*/
