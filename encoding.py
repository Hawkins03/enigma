import curses
import sys
import util
from enum import Enum

a = open('encoded_txt.txt','w')
a.close()

'''

changing settings:

'''
class Backend():
    def __init__(self): 
        self.generate_settings()
        self.initalize_settings()        

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

    def rotor(self, rotor_number, plaintext):
        for i in range(0,len(self.alpha)):
            if self.ciphertext == self.alpha[i]:
                if rotor_number < 3:
                    return self.rotors[rotor_number][(i+self.settings[0][rotor_number])%26]
                else:
                    return self.rotors[rotor_number][i]

    def plugboard(self,plaintext):
        for i in range(10):
            if plaintext == self.pairs[0][i]:
                plaintext = self.pairs[1][i]
            if plaintext == self.pairs[1][i]:
                plaintext = self.pairs[0][i]
        return plaintext

    def encrypt(self, plaintext):
        self.ciphertext = plaintext
        ciphertext = ""
        self.update_rotors()
        self.ciphertext = self.plugboard(self.ciphertext)
        for i in range(4):
            self.ciphertext = self.rotor(i,self.ciphertext)
        for i in range(2,-1):
            self.ciphertext = self.rotor(i,self.ciphertext)
        self.ciphertext = self.plugboard(self.ciphertext)
        return self.ciphertext

    def update_rotors(self):
        self.settings[0][0]+=1
        for i in range(3):
            for j in range(len(self.rotor_updates[i])-1):
                if ord(self.rotor_updates[i][j])-97 == self.settings[0][i] and i != 3:
                    self.settings[0][i+1]+=1
                self.settings[0][i] = self.settings[0][i]%26
    def writeSettings(self):



backend = Backend()
print(backend.encrypt('a'))
