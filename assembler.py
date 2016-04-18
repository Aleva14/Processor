import sys
import struct

command = {"push_num": 0, "push_reg": 1, "pop": 2, "in": 3, "out": 4, "tr": 5, 
"triz": 6, "trip": 7, "trin": 8, "add": 9, "mul": 10, "sub": 11, "div": 12, "gsp": 13, "asp": 14, "rsp": 15}
register = {"Gly": 0, "Ala": 1, "Val": 2, "Leu": 3, "Ser": 4, "Thr": 5, 
"Asp": 6, "Asn": 7, "Glu": 8, "Gln": 9,  "Lys": 10,"Arg": 11, "Cys": 12, 
"Met": 13, "Fen": 14, "Tyr": 15, "Trp": 16, "Hys": 17, "Pro": 18, "Npc": 19}


def Assemble(program):
    global errno
    global result
    global label
    
    result = []

    empty = 1
    for i in program:
        if not i.isspace() and i:
            empty = 0
    if empty:
        errno = error.index('no program')
        return 'No program -('

    for j in program:
        if j[0] == ':':
            index = program.index(j)
            j = j.replace('\n', '')
            j = j.replace(':', '')
            if not (j in label.keys()):
                label[j] = index   
    i = 0
    while i < len(program):
        temp = program[i]
        temp = temp.replace('\n', '')
        temp = temp.split(' ')
        if temp[0] == 'push':
            if temp[1].isdigit() or (temp[1][0] == '-' and temp[1][1:].isdigit()):
                result.append(command[temp[0] + '_num'])
                result.append(int(temp[1]))
            elif temp[1] in register.keys():
                result.append(command[temp[0] + '_reg'])
                result.append(register[temp[1]])
            else:
                errno = error.index('wtf')
                return Report("'wtf'", i + 1, program[i])  
        elif temp[0][0] == '/':
            pass  
        elif temp[0][0] == ':':
            temp[0] = temp[0][1:]
            label[temp[0]] = len(result)
        elif temp[0] == 'in':
            result.append(command[temp[0]])
        elif temp[0] == 'out':
            result.append(command[temp[0]])
        elif temp[0] == 'tr':
            if temp[1] in label.keys():
                    result.append(command[temp[0]])
                    result.append(label[temp[1]])
            else:
                    errno = error.index('no such label')
                    return Report("'no such label'", i, program[i])
        elif temp[0] == 'triz': #'travel if zero'
            if temp[1] in label.keys():
                result.append(command[temp[0]])
                result.append(label[temp[1]])
            else:
                errno = error.index('no such label')
                return Report("'no such label'", i + 1, program[i])
        elif temp[0] == 'trip': #'travel if positive'
            if temp[1] in label.keys():
                result.append(command[temp[0]])
                result.append(label[temp[1]])
            else:
                errno = error.index('no such label')
                return Report("'no such label'", i + 1, program[i])
        elif temp[0] == 'trin': #'travel if negative'
            if temp[1] in label.keys():
                result.append(command[temp[0]])
                result.append(label[temp[1]])
            else:
                errno = error.index('no such label')
                return Report("'no such label'", i + 1, program[i])
     	elif temp[0] == 'pop':
            if temp[1] in register.keys():
                result.append(command[temp[0]])
                result.append(register[temp[1]])
            else:
                errno = error.index('no such register')
                return Report("'no such register'", i + 1, program[i])
        elif temp[0] == 'add':
            result.append(command[temp[0]])
        elif temp[0] == 'mul':
            result.append(command[temp[0]])
        elif temp[0] == 'sub':
            result.append(command[temp[0]])
        elif temp[0] == 'div':
            result.append(command[temp[0]])
        elif temp[0] == 'gsp':
            result.append(command[temp[0]])
	elif temp[0] == 'asp':
            if temp[1] in register.keys():
                result.append(command[temp[0]])
                result.append(register[temp[1]])
            else:
                errno = error.index('wtf')
                return Report("'wtf'", i + 1, program[i])
        elif temp[0] == 'rsp':
            if temp[1] in register.keys():
                result.append(command[temp[0]])
                result.append(register[temp[1]])
            else:
                errno = error.index('wtf')
                return Report("'wtf'", i + 1, program[i])

        else:
            errno = error.index('wtf')
            return Report("'wtf'", i + 1, program[i])

        i += 1

def IsNotOk():
    if errno:
        return 1
    else:
        return 0

def Report(error, str_num, wrong_string):
    return "Error {} in string {}:\n{}".format(error, str_num, wrong_string) 

def Transform(num):
    if num >= 0 and num <= max_int / 2 - 1:
        return struct.pack("H", num)
    elif num < 0 and num >= -max_int / 2:
        num = max_int + num
        return struct.pack("H", num)
    else:
        return 'error'


error = ['ok','wtf', 'no such label', 'no such register', 'too large int', 'no program']
num_of_bits = 16
max_int = 2 ** num_of_bits
errno = 0

result = []
label = {}
result_bin = []

program = open(sys.argv[1])
program = program.readlines()

error_report = Assemble(program)
if IsNotOk():
    print error_report
    sys.exit(errno)

Assemble(program)
for i in result:
    a = Transform(i)
    result_bin.append(a)
    if a == 'error':
        errno = error.index('too large int')
        print 'too large int', i


if len(sys.argv) > 2:
    path = sys.argv[2]
else:
    path = 'result.bin' 
f = open(path, 'wb')
for i in result_bin:
    f.write(i)
print result
print result_bin

