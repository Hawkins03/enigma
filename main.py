import curses
import sys
a = open('encoded_txt.txt','w')
a.close()
'''

changing settings:

'''

#opening settings.txt and moving the contents of the first four lines into a list called settings_list
settings = open('settings.txt','r')
settings_list = settings.readlines()[0:4]
settings.close()
nrp = []
new_nrp=[]
print("settings")
print(settings)

def settings(input_txt):
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

'''

done changing settings:

'''
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
'''
  screen.addch(y_3_4+5,x_3_4-41,'q',graphics[1][ltr_numb('q')])
  screen.addch(y_3_4+5,x_3_4-32,'w',graphics[1][ltr_numb('w')])
  screen.addch(y_3_4+5,x_3_4-23,'e',graphics[1][ltr_numb('e')])
  screen.addch(y_3_4+5,x_3_4-14,'r',graphics[1][ltr_numb('r')])
  screen.addch(y_3_4+5,x_3_4-4,'t',graphics[1][ltr_numb('t')])
  screen.addch(y_3_4+5,x_3_4+5,'y',graphics[1][ltr_numb('y')])
  screen.addch(y_3_4+5,x_3_4+14,'u',graphics[1][ltr_numb('u')])
  screen.addch(y_3_4+5,x_3_4+23,'i',graphics[1][ltr_numb('i')])
  screen.addch(y_3_4+5,x_3_4+32,'o',graphics[1][ltr_numb('o')])
  screen.addch(y_3_4+5,x_3_4+41,'p',graphics[1][ltr_numb('p')])
  screen.addch(y_3_4+11,x_3_4-38,'a',graphics[1][ltr_numb('a')])
  screen.addch(y_3_4+11,x_3_4-29,'s',graphics[1][ltr_numb('s')])
  screen.addch(y_3_4+11,x_3_4-20,'d',graphics[1][ltr_numb('d')])
  screen.addch(y_3_4+11,x_3_4-11,'f',graphics[1][ltr_numb('f')])
  screen.addch(y_3_4+11,x_3_4-2,'g',graphics[1][ltr_numb('g')])
  screen.addch(y_3_4+11,x_3_4+8,'h',graphics[1][ltr_numb('h')])
  screen.addch(y_3_4+11,x_3_4+17,'j',graphics[1][ltr_numb('j')])
  screen.addch(y_3_4+11,x_3_4+26,'k',graphics[1][ltr_numb('k')])
  screen.addch(y_3_4+11,x_3_4+35,'l',graphics[1][ltr_numb('l')])
  screen.addch(y_3_4+17,x_3_4-35,'z',graphics[1][ltr_numb('z')])
  screen.addch(y_3_4+17,x_3_4-26,'x',graphics[1][ltr_numb('x')])
  screen.addch(y_3_4+17,x_3_4-17,'c',graphics[1][ltr_numb('c')])
  screen.addch(y_3_4+17,x_3_4-8,'v',graphics[1][ltr_numb('v')])
  screen.addch(y_3_4+17,x_3_4+1,'b',graphics[1][ltr_numb('b')])
  screen.addch(y_3_4+17,x_3_4+11,'n',graphics[1][ltr_numb('n')])
  screen.addch(y_3_4+17,x_3_4+20,'m',graphics[1][ltr_numb('m')])
'''

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
