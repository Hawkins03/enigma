# enigma
A enigma machine I built using python origionally, and am now rebuilding in C.

To work, run "make" and then use the ./encrypt followed by whatever you want to encrypt.

init contains any functions required to edit, fetch and store session data,
encrypt stores anything required to encrypt the data
term stores anything required to interact with the terminal.
tui contains everything needed to run the text based ui (using ncurses)

This project relies on ncurses for c. To install it, paste the following command into your terminal
"sudo apt install libncurses5-dev libncursesw5-dev -y"


todo:
  make program to store buff into a text file when it overwrites.

git auto log in (when pushing on WSL):
  git config --global credential.helper "/mnt/c/Program\ Files/Git/mingw64/libexec/git-core/git-credential-wincred.exe"
(you're welcome future me / other person reading this :D)
