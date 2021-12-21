# enigma
A enigma machine I built using python. 
Requires python version 3.7+. Optimal usage is using terminal as I was able to make a tui using the curses module
Alternately use new.py

NOTE. This is still in progress, to see the working version (that has atrotious code I wish I didn't write (it was four years ago) check out the other branch

Todo:
- Show encrypted output durring programming
- Make ctrl+q exit tui
- Make exception handling
- Make settings window
    Encrypt side:
    - Make it read terminal args so that you can just run encode [plainText] and it will print the encoded text
        because of catting files in as args, I don't have to add args to pull from files or output to files :D
    - make british version (ie without the error of one letter not being encode to itself)
    - make updated german one (with extra rotor)
