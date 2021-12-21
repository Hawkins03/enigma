def remove_newlines(string):
    return string[:len(string)-1]

def find_breaks(string,break_chr = ' '):
    breaks = [0]
    for i in range(len(string)):
        if string[i] == break_chr:
            breaks.append(i+1)
    breaks.append(len(string)+1)
    return breaks

def string_to_list(string,breaks):
    formatted_string = []
    for i in range(len(breaks)-1):
        try:
            formatted_string.append(int(string[breaks[i]:breaks[i+1]-1]))
        except:
            formatted_string.append(string[breaks[i]:breaks[i+1]-1])
    return formatted_string
