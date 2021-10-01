import curses
import sys
from enum import Enum

a = open('encoded_txt.txt','w')
a.close()

class Direction(Enum): # list is [x_direction,y_direction]
    LEFT  = [-1,0]
    RIGHT = [1, 0]
    UP    = [0,-1]
    DOWN  = [0, 1]

'''

changing settings:

'''
class enigma():
    def __init__(self): 
        self.generate_settings()

        #init screen

        screen = curses.initscr()
        screen.keypad(1)
        curses.echo(0)
        curses.curs_set(0)
        screen.clear()
        screen.refresh()
        dims = screen.getmaxyx()
        
        #initalizing rotors
        self.rotor_1_settings = self.settings_list[0][0]
        self.rotor_2_settings = self.settings_list[0][1]
        self.rotor_3_settings = self.settings_list[0][2]

        self.end_screen(screen)

    def generate_settings(self):
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
    
    def end_screen(self, screen):
        screen.clear()
        curses.endwin()



    def draw_line(x_start, y_start, direction):
        if not isinstance(direction, Direction):
            raise TypeError("direction must be an instance of Direction Enum")


        


enigma()

'''
def settings(input_txt):
    dims = 
    print(input_txt)
    breaks = [0]
    for i in range(0,len(txt)):
        if txt[i] == ' ':
            breaks.append(i) #looking for spaces and appending them to the breaks list
    try:
        nrp.append(int(txt[0:breaks[1]])) # appending the first two captured in the breaks list to nrp
    except:
        print(breaks)
        nrp.append(txt[0:breaks[1]])    
    for i in range(1,len(breaks)-1):
        try:
            nrp.append(int(txt[breaks[i]+1:breaks[i+1]]))
        except:
            nrp.append(txt[breaks[i]+1:breaks[i+1]])      
    c = breaks[len(breaks)-1]
    try:
        nrp.append(int(txt[c+1:c+2]))
    except:
        nrp.append(txt[c+1:c+2])
    return(nrp)
for i in range(0,4):
    new_nrp.append(settings(nrp[i]))
nrp = new_nrp


Done Changing Settings


screen = curses.initscr()
screen.keypad(1)
curses.echo(0)
curses.curs_set(0)
screen.clear()
screen.refresh()
dims = screen.getmaxyx()

r_1_s = nrp[0][0]
r_2_s = nrp[0][1]
r_3_s = nrp[0][2]

def draw_rotors(y,x,nrp,n):
  screen.addstr(y,x,'__')
  for i in range(0,3):
    screen.addch(y+2+2*i,x,'-')
    screen.addch(y+2+2*i,x+1,'-')
  for i in range(0,5):
    screen.addch(y+1+i,x-1,'|')
    screen.addch(y+1+i,x+2,'|')
  for i in range(0,3):
    if len(str(nrp[0][n]-1+i))==1:
      screen.addch(y+1+2*i,x,'0')
      screen.addch(y+1+2*i,x+1,str((nrp[0][n]-1+i)%26))
    else:
      screen.addstr(y+1+2*i,x,str((nrp[0][n]-1+i)%26))
def draw_plugs(y,x,nrp):
  screen.addch(y-1,x+1,'_')
  screen.addch(y-1,x+2,'_')
  screen.addch(y-1,x+3,'_')
  for i in range(1,11):
    screen.addch(y+2*i-1,x+3,'-')
    screen.addch(y+2*i-1,x+2,'-')
    screen.addch(y+2*i-1,x+1,'-')
  for i in range(0,19):
    screen.addch(y+i,x,'|')
    screen.addch(y+i,x+4,'|')
    screen.addch(y+i,x+2,'|')

  for i in range(0,10):
    screen.addch(y+2*i,x+1,nrp[2][i])
    screen.addch(y+2*i,x+3,nrp[3][i])

graphics = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
def draw_scr():
  y_half = int(dims[0]/2-2)
  y_three_quarters = int(y__2+y__2/2-5)
  y_one_quarter = int(y__2-y__2/2)
  x_half = int((dims[1]-1)/2)
  x_one_quarter = int(x__2/2)
  x_three_quarters = x__2+int(x_1_4/2)
  banner = "THE WWII ENIGMA MACHINE"
  screen.addstr(1,int(x__2-(len(banner)/2),banner))
  for i in range(0,dims[1]):
    screen.addch(5,i,'_')
  for i in range(6,dims[0]):
    screen.addch(i,x_1_4,'|')
  for i in range(x_1_4+1,dims[1]):
    screen.addch(y_3_4,i,'-')


def draw_keyboard():
    keyboard_layout="qwertyuiopasdfghjklzxcvbnm"
    #switcher={10:(5,-41),18:(11,-38),26:(17,-35)} #index of last value:(y offset, first x offset)
    for i in range(0,26):
        #updating the chordinates of the letter (y_chord,starting x chord, smallest i value in the group
        if i<=10:
            switcher = (5,-41,0)
        elif i<=18:
            switcher = (11,-38,11)
        else:
            switcher = (17,-35,18)
        curr_letter = keyboard_layout[i]
        screen.addch(y_3_4+switcher[0],x_3_4+switcher[1]+i*9,curr_letter,graphics[1][ord(curr_letter)-97])


  x__2 = x_3_4-2
  y__2 = y_1_4-1
  screen.addstr(y_1_4+6,x__2-12,'0'+str(nrp[1][2]))
  screen.addstr(y_1_4+6,x__2,'0'+str(nrp[1][1]))
  screen.addstr(y_1_4+6,x__2+12,'0'+str(nrp[1][0]))

  draw_rotors(y__2,x__2,nrp,1)
  draw_rotors(y__2,x__2-12,nrp,2)
  draw_rotors(y__2,x__2+12,nrp,0)

  draw_plugs(y__2-2,x__2-36,nrp)
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
