import sys
output = open('output.txt','w')
output.close()
inpt = open('input.txt','r').read()
'''

changing settings:

'''

settings = open('settings.txt','r')

nrp = settings.readlines()[0:4]
new_nrp = []
def settings(txt):
  nrp = []
  c_loc = [0]
  for i in range(0,len(txt)):
    if txt[i] == ' ':
      c_loc.append(i)
  try:
    nrp.append(int(txt[0:c_loc[1]]))
  except:
    nrp.append(txt[0:c_loc[1]])    
  for i in range(1,len(c_loc)-1):
    try:
      nrp.append(int(txt[c_loc[i]+1:c_loc[i+1]]))
    except:
      nrp.append(txt[c_loc[i]+1:c_loc[i+1]])      
    c = c_loc[len(c_loc)-1]
  try:
    nrp.append(int(txt[c+1:c+2]))
  except:
    nrp.append(txt[c+1:c+2])
  return(nrp)
for i in range(0,len(nrp)):
  new_nrp.append(settings(nrp[i]))
nrp = new_nrp
'''

done changing settings:

'''
start = True
r_1_s = nrp[0][0]
r_2_s = nrp[0][1]
r_3_s = nrp[0][2]

def ltr_numb(ltr):
  return(ord(ltr)-97)
graphics = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]


def plugboard(action,pairs):
  for i in range(0,9):
    if action == pairs[0][i]:
      action = pairs[1][i]
    elif action == pairs[1][i]:
      action = pairs[0][i]
  return(action)



def encode(action,r_1_s=0,r_2_s=0,r_3_s=0):
  rotors=['ekmflgdqvzntowyhxuspaibrcj','ajdksiruxblhwtmcqgznpyfvoe','bdfhjlcprtxvznyeiwgakmusqo','esovpzjayquirhxlnftgkdcmwb','vzbrgityupsdnhlxawmjqofeck','jpgvoumfyqbenhzrdkasxlictw','nzjhgrcxmyswboufaivlpekqdt','fkqhtlxocbjspdzramewniuygv']
  alpha='abcdefghijklmnopqrstuvwxyz'
  reflector = 'ejmzalyxvbwfcrquontspikhgd'
  rotor1 = rotors[0]
  rotor2 = rotors[1]
  rotor3 = rotors[2]
  rotor4 = rotors[3]
  pairs = [['a','b','c','d','e','f','g','h','i','j'],['z','y','x','w','v','u','t','s','r','q']]
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
  return(action)
opt = []
for action in inpt:
  if start == True:
    start = False
  else:
    r_1_s = (r_1_s+1)%26
    if r_1_s == 0:
      r_2_s+=1
    r_2_s = r_2_s%26
    if r_2_s == 0 and r_1_s == 0:
      r_3_s+=1
    r_3_s = r_3_s%26
  nrp[0][0] = r_1_s
  nrp[0][1] = r_2_s
  nrp[0][2] = r_3_s
  action = encode(action,r_1_s,r_2_s,r_3_s)
  opt.append(action)
output = open('output.txt','a')
for i in opt:
	output.write(i)
print('your encrypted/decrypted text: '+''.join(opt))
