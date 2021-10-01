import curses
import sys
from enum import Enum

a = open('encoded_txt.txt','w')
a.close()

'''

changing settings:

'''
class backend():
    def __init__(self): 
        self.generate_setting_list()
        self.inatalize_settings()        

    def generate_setting_list(self):
        '''
        Line 1: rotor settings
        Line 2: rotor selections
        Line 3: Plugboard pt 1
        Line 4: plugboard pt 2
        '''
        settings = open('settings.txt','r')
        settings_list = settings.readlines()[0:4]
        settings.close()
        
        formatted_settings_list = []

        #format settings list (remove \n) and find spaces
        
        for i in settings_list:
            formatted_settings_list.append(i[0:len(i)-1])
            breaks=[0]
            for j in range(0,len(formatted_settings_list)):
                if i[j] == ' ':
                    breaks.append(j)
        self.settings_list = formatted_settings_list 
        print(settings_list)

    def initalize_settings(self):
        #initalizing rotors
        self.rotor_settings = []
        for i in range(3):
            self.rotor_settings.append(self.settings_list[0][i])

        #generating rotors
        self.rotors = []
        for i in settings_list[1]:
            self.rotors.append(rotor_list[i]) # reading in the selected rotors
        self.rotors.append('ejmzalyxvbwfcrquontspikhgd')

        #other rotors settings
        self.alpha='abcdefghijklmnopqrstuvwxyz'

        #plugboard pairs
        self.pairs = [settings_list[2],settings_list[3]]    
    
    def rotor(self, rotor_number, plaintext):
        for i in range(0,len(plaintext)):
            if plaintext == self.alpha[i]:
                return rotor[rotor_number][i-rotor_settings[rotor_number]

    def plugboard(self,plaintext):
        for i in range(10):
            if plaintext == self.pairs[0][i]:
                plaintext = self.pairs[1][i]
            if plaintext == self.pairs[1][i]:
                plaintext = self.pairs[0][i]
        return plaintext

    def encrypt(plaintext):
        plaintext = plugboard(plaintext)
        for i in range(4):
            rotor(i,plaintext)
        for i in range(2,-1):
            rotor(i,plaintext)
        plaintext = plugboard(plaintext)
        print(plaintext)
        return plaintext


'''
def plugboard(action,pairs):
  for i in range(0,10):
    if action == pairs[0][i]:
      action = pairs[1][i]
    if action == pairs[1][i]:
      action = pairs[0][i]
  return(action)


def encode(action,nrp,r_1_s=0,r_2_s=0,r_3_s=0):
  pairs=[]
  rotors=['ekmflgdqvzntowyhxuspaibrcj','ajdksiruxblhwtmcqgznpyfvoe','bdfhjlcprtxvznyeiwgakmusqo','esovpzjayquirhxlnftgkdcmwb','vzbrgityupsdnhlxawmjqofeck','jpgvoumfyqbenhzrdkasxlictw','nzjhgrcxmyswboufaivlpekqdt','fkqhtlxocbjspdzramewniuygv']
  alpha='abcdefghijklmnopqrstuvwxyz'
  reflector = 'ejmzalyxvbwfcrquontspikhgd'
  rotor1 = rotors[nrp[1][0]-1]
  rotor2 = rotors[nrp[1][1]-1]
  rotor3 = rotors[nrp[1][2]-1]
  pairs.append(nrp[2])
  pairs.append(nrp[3])
  action = plugboard(action,pairs)
  for i in range(0,len(alpha)):
    if action == alpha[i]:
      action = rotor1[(i-r_1_s)%26]
      break
  for i in range(0,len(alpha)):
    if action == alpha[i]:
      action = rotor2[(i-r_2_s)%26]
      break
  for i in range(0,len(alpha)):
    if action == alpha[i]:
      action = rotor3[(i-r_3_s)%26]
      break
  for i in range(0,len(alpha)):
    if action == alpha[i]:
      action = reflector[i]
      break
  for i in range(0,len(alpha)):
    if action == rotor3[i]:
      action = alpha[(i+r_3_s)%26]
      break
  for i in range(0,len(alpha)):
    if action == rotor2[i]:
      action = alpha[(i+r_2_s)%26]
      break
  for i in range(0,len(alpha)):
    if action == rotor1[i]:
      action = alpha[(i+r_1_s)%26]
      break
  action = plugboard(action,pairs)
  print(action)
  return(action)

while True:
  draw_scr()
  try:
    action = screen.getkey()
    screen.addch(15,1,action)
    graphics = [[0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
    r_1_s = (r_1_s+1)%26
    nrp[0][0] = r_1_s
    nrp[0][1] = r_2_s
    nrp[0][2] = r_3_s
    if r_1_s == 0:
      r_2_s+=1
    r_2_s = r_2_s%26
    if r_2_s == 0 and r_1_s ==0:
        r_3_s+=1
    r_3_s = r_3_s%26
    action = encode(action,nrp,r_1_s,r_2_s,r_3_s)
    a = open('encoded_txt.txt','a')
    a.write(str(action))
    a.close()
    screen.addstr(1,1,str(nrp[0][0]))
    screen.addstr(2,1,str(nrp[0][1]))
    screen.addstr(3,1,str(nrp[0][2]))
    screen.addstr(4,1,str(action))
    graphics[1][ord(str(action))-97] = 262144
    draw_scr()
    screen.clear()
  except:
    break
screen.clear()
curses.endwin()
print(nrp)
'''
