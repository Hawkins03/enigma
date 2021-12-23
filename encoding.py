import curses
import sys
from enum import Enum
import util

'''

changing settings:

'''
class Encoding():
    def __init__(self): 
        self.generate_settings()
        self.initalize_settings()
        self.concatEncryptions = "";

    def generate_settings(self):
        '''
        Line 1: rotor settings
        Line 2: rotor selections
        Line 3: Plugboard pt 1
        Line 4: plugboard pt 2
        '''

        #reading settings and setting self.settings = to the first 4 lines of the settings file
        f = open('settings.txt','r')
        self.settings = f.readlines()[:4]
        f.close()

        formatted_settings = []
        breaks = []
        #removing newlines, finding the spaces, and then changing the string into lists
        for i in range(4):
            self.settings[i] = util.remove_newlines(self.settings[i])
            breaks.append(util.find_breaks(self.settings[i],' '))
            self.settings[i] = util.string_to_list(self.settings[i],breaks[i])

    def initalize_settings(self):
        #initalizing rotors
        self.rotor_settings = []
        for i in range(3):
            self.rotor_settings.append(self.settings[0][i-1])

        #generating rotors
        rotor_list = open('.rotors.txt','r').readlines()
        for i in range(len(rotor_list)):
            rotor_list[i] = util.remove_newlines(rotor_list[i])

        self.rotors = []
        for i in self.settings[1]:
            self.rotors.append(rotor_list[i]) # reading in the selected rotors
        self.rotors.append('ejmzalyxvbwfcrquontspikhgd') #reflector

        self.alpha='abcdefghijklmnopqrstuvwxyz'

        #plugboard pairs
        self.pairs = [self.settings[2],self.settings[3]]

        allRotorUpdates = [['r'],['f'],['w'],['k'],['a'],['a','n'],['a','n'],['a','n']]
        
        self.rotor_updates = []
        for i in range(3):
            self.rotor_updates.append(allRotorUpdates[self.settings[1][i]])
    
    def getFancyRotorSettings(self):
        toReturn = "rotors chosen:  #%d  #%d  #%d" % (self.settings[1][0], self.settings[1][1], self.settings[1][2])
        toReturn += "\nrotor settings: #" + str(self.settings[0][0]).zfill(2) + " #" + str(self.settings[0][1]).zfill(2) + " #" + str(self.settings[0][2]).zfill(2)
        return toReturn


    def getFancyRotorArray(self):
        rotorSettings = self.settings[0]
        toReturn = [self.settings[1], [["a", "a", "a"], ["b", "b", "b"], ["c", "c", "c"]]]
        for y in range(3):
            for x in range(3):
                toReturn[1][y][x] = "a"
                toReturn[1][y][x] = self.rotors[y][(self.settings[0][y] + x - 1) % 26]
        return toReturn

    def rotor(self, rotor_number, plaintext):
        for i in range(0,len(self.alpha)):
            if plaintext == self.alpha[i]:
                if rotor_number < 3:
                    position = self.settings[0][rotor_number]
                    return self.rotors[rotor_number][(i+self.settings[0][rotor_number])%26]
                else:
                    return self.rotors[rotor_number][i]


    def updateRotors(self):
        self.settings[0][0] = (self.settings[0][0] + 1) % 26
        for y in range(3):
            for x in self.rotor_updates[y]:
                if self.rotors[y][self.settings[0][y]] == x:
                    if y < 2:
                        self.settings[0][y] = (self.settings[0][y] + 1) % 26
                        self.settings[0][y + 1] = (self.settings[0][y + 1] + 1) % 26

    def plugboard(self, plaintext):
        for i in range(10):
            if plaintext == self.settings[2][i]:
                plaintext = self.settings[3][i]
            elif plaintext == self.settings[3][i]:
                plaintext = self.settings[2][i]
        return plaintext

    def encrypt(self, plaintext, output = True):
        outputStr = plaintext + " "
        ciphertext = self.plugboard(plaintext)
        outputStr += ciphertext + " "
        for i in range(4):
            ciphertext = self.rotor(i, ciphertext)
            outputStr += ciphertext
        for i in range(3):
            ciphertext = self.rotor(2 - i, ciphertext)
            outputStr += ciphertext
        self.updateRotors()
        ciphertext = self.plugboard(ciphertext)
        outputStr += " " + ciphertext + " " + " ".join(map(str, self.settings[0]))
        if output:
            print(outputStr)
        self.concatEncryptions += ciphertext + "\n"
        return ciphertext
    
    def close(self, save = True):
        settingLines = open("settings.txt", "r").readlines();
        settingLines[0] = ' '.join(map(str, self.settings[0])) + "\n"
        settingLines[1] = ' '.join(map(str, self.settings[1])) + "\n"
        settingLines[2] = ' '.join(map(str, self.settings[2])) + "\n"
        settingLines[3] = ' '.join(map(str, self.settings[3])) + "\n"
        f = open("settings.txt", "w")
        f.writelines(settingLines)
        f.close()
        if save:
            f = open("output.txt", "a")
            f.write(self.concatEncryptions)
            f.close()

#debug by pprint("rotor setting: " + str(self.settings[0][rotor_number]))
encoder = Encoding()
#for i in range(26):
    #letter = chr(i + 97)
    #encoder.encrypt(letter)
#encoder.close()
