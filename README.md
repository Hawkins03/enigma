A enigma machine I built using python origionally (check the "2018 version" branch), and then rebuilt in C.
- REQUIRED MODULE: ncurses (see below on how to install.)

There are two ways to use this program. First you run "Make tui" and then run
./tui. Then type in whatever you want to and you're good.

Hitting escape once gets you to the main menu. From there you can hit escape
again, or just select quit using the enter or space buttons.
Otherwise, you can go back by selecting "go back"
or edit settings. Within settings, first select a row using the up and down
keys, press enter to lock in the selection, and then select the specific
variable you want to edit using the left and right keys (and press enter once
again.) From there use the up and down keys to change the variable and press
escape or enter once you're done.

Option 2 is just to use the terminal command. To do so you type "make" and then
./encrypt [WHATEVER YOU WANT].

In terms of settings, you can use the tui, or edit the .session.txt file.
The first line is the rotor selections [0-8), the second is the rotor settings
[0-26), and the rest are the plugboard (a-z) (note, they go both ways.) If you have
problems with the encryption, just uncomment the printf statments in encrypt.c,
and you should be able to see what is going wrong with the program.

In terms of which file contains what:

init contains any functions required to edit, fetch and store session data,
encrypt stores anything required to encrypt the data
term stores anything required to interact with the terminal.
tui contains everything needed to run the text based ui (using ncurses)

This project relies on ncurses for c. To install it, paste the following command into your terminal
"sudo apt install libncurses5-dev libncursesw5-dev -y"


git auto log in (when pushing on WSL):
  git config --global credential.helper "/mnt/c/Program\ Files/Git/mingw64/libexec/git-core/git-credential-wincred.exe"
(you're welcome future me / other person reading this :D)
