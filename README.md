# enigma
A enigma machine I built using python origionally, and am now rebuilding in C.

To work, run "make" and then use the ./encrypt followed by whatever you want to encrypt.

init contains any functions required to edit, fetch and store session data,
encrypt stores anything required to encrypt the data
term stores anything required to interact with the terminal.
tui contains everything needed to run the text based ui (using ncurses)

Ncurses instilation:
"sudo apt install libncurses5-dev libncursesw5-dev"


todo:
  make program to store buff into a text file when it overwrites.
