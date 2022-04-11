#include "init.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

static char g_rotors[8][27] = {"ekmflgdqvzntowyhxuspaibrcj",
                               "ajdksiruxblhwtmcqgznpyfvoe",
                               "bdfhjlcprtxvznyeiwgakmusqo",
                               "esovpzjayquirhxlnftgkdcmwb",
                               "vzbrgityupsdnhlxawmjqofeck",
                               "jpgvoumfyqbenhzrdkasxlitcw",
                               "nzjhgrcxmyswboufaivlpekqdt",
                               "fkqhtlxocbjspdzramewniuygv"};

void print_settings(settings_struct_t *settings) {
  if (settings == 0) {
    return;
  }
  printf("Enigma settings:\n");

  printf("Rotors:\n");
  for (int i = 0; i < 3; i++) {
    printf("%d: %s %d\n", i, settings->rotors[i], settings->r_set[i]);
  }

  printf("Plugboard:\n");
  for (int i = 0; i < 10; i++) {
    printf("%c-%c ", settings->plug_top[i], settings->plug_bot[i]);
  }
  printf("\n");

}

settings_struct_t *get_settings() {
  FILE *in_ptr = NULL;
  in_ptr = fopen(".settings.txt", "r");
  if (in_ptr == NULL) {
    printf("Error, no file found.");
    return NULL;
  }

  settings_struct_t *settings = malloc(sizeof(settings_struct_t));
  if (settings == NULL) {
    return NULL;
  }

  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = fscanf(in_ptr, "%d ", &(settings->r_pos[i]));
    if (status != 1) {
      return NULL;
    }
  }

  for (int i = 0; i < 3; i ++) {
    strncpy(settings->rotors[i], g_rotors[settings->r_pos[i] % 8], 27);
  }

  for (int i = 0; i < 3; i++) {
    status = fscanf(in_ptr, "%d ", &(settings->r_set[i]));
    if(status != 1) {
      return NULL;
    }
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(in_ptr, "%c ", &(settings->plug_top[i]));
    if(status != 1) {
      return NULL;
    }
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(in_ptr, "%c ", &(settings->plug_bot[i]));
    if(status != 1) {
      return NULL;
    }
  }

  printf("close: %d\n", fclose(in_ptr));
  in_ptr = NULL;

  return settings;
}

int set_settings(settings_struct_t *settings) {
  FILE *out_ptr = NULL;
  out_ptr = fopen(".settings.txt", "w");
  if (out_ptr == NULL) {
    printf("Error, no file found.");
    return STARTUP_ERR;
  }

  if (settings == NULL) {
    return STARTUP_ERR;
  }

  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = fprintf(out_ptr, "%ls ", &(settings->r_pos[i]));
    if (status != 1) {
      return READ_ERR;
    }
  }

  for (int i = 0; i < 3; i++) {
    status = fscanf(out_ptr, "%ls ", &(settings->r_set[i]));
    if(status != 1) {
      return READ_ERR;
    }
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(out_ptr, "%c ", &(settings->plug_top[i]));
    if(status != 1) {
      return READ_ERR;
    }
  }

  for (int i = 0; i < 10; i++) {
    status = fscanf(out_ptr, "%c ", &(settings->plug_bot[i]));
    if(status != 1) {
      return READ_ERR;
    }
  }

  printf("close: %d\n", fclose(out_ptr));
  out_ptr = NULL;

  return 0;
}

int main() {
  settings_struct_t *settings = get_settings();
  print_settings(settings);
  printf("%d\n", set_settings(settings));
  settings = get_settings();
  print_settings(settings);
}

/*
import curses
import sys
from enum import Enum
import util


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
        self.concatEncryptions += self.ciphertext + "\n"
        return self.ciphertext

    def update_rotors(self):
        self.settings[0][0]+=1
        for i in range(3):
            for j in range(len(self.rotor_updates[i])-1):
                if ord(self.rotor_updates[i][j])-97 == self.settings[0][i] and i != 3:
                    self.settings[0][i+1]+=1
                self.settings[0][i] = self.settings[0][i]%26

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
            f = open("output.txt", "w")
            f.write(self.concatEncryptions)
            f.close()


encoder = Encoding()
print(encoder.encrypt('a'))
*/
