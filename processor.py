import sys

def CalcRevPol(task):
    global errno
    
    register = {"Gly": 0, "Ala": 0, "Val": 0, "Leu": 0, "Ser": 0, "Thr": 0, "Asp": 0, "Asn": 0, "Glu": 0, "Gln": 0,  "Lys": 0,"Arg": 0, "Cys": 0, "Met": 0, "Fen": 0, "Tyr": 0, "Trp": 0, "Hys": 0, "Pro": 0} 
    
    
    task = task.readlines()
    
    label = {}
    for j in task:
        if j[0] == ':':
            index = task.index(j)
            j = j.replace('\n', '')
            j = j.replace(':', '')
            label[j] = index
    
    stack = []
    
    i = 0
    while i < len(task):
        temp = task[i]
        temp = temp.replace('\n', '')
        temp = temp.split(' ')
        if temp[0] == 'push':
            if temp[1].isdigit() or (temp[1][0] == '-' and temp[1][1:].isdigit()):
                stack.append(int(temp[1]))
            elif temp[1] in register.keys():
                a = GetValue(stack)
                register[temp[1]] = a
            else:
                errno = error.index('wtf')
                return Report("'wtf'", i + 1, task[i])    
        elif temp[0][0] == ':':
          a = 3  
        elif temp[0] == 'in':
            a = raw_input()
            if a.isdigit() or (a[0] == '-' and a[1:].isdigit()):
                stack.append(int(a))
            else:
                errno = error.index('bad input')
                return Report("'bad input'", i + 1, task[i])
        elif temp[0] == 'out':
            print GetValue(stack)
        elif temp[0] == 'tr':
            if temp[1] in label.keys():
                    i = label[temp[1]]
            else:
                    errno = error.index('no such string')
                    return Report("'no such string'", i, task[i])
        elif temp[0] == 'triz': #'travel if zero'
            a = GetValue(stack)
            if a == 0:
                if temp[1] in label.keys():
                    i = label[temp[1]]
                else:
                    errno = error.index('no such string')
                    return Report("'no such string'", i + 1, task[i])
        elif temp[0] == 'trip': #'travel if positive'
            a = GetValue(stack)
            if a > 0:
                if temp[1] in label.keys():
                    i = label[temp[1]]
                else:
                    errno = error.index('no such string')
                    return Report("'no such string'", i + 1, task[i])
        elif temp[0] == 'trin': #'travel if negative'
            a = GetValue(stack)
            if a < 0:
                if temp[1] in label.keys():
                    i = label[temp[1]]
                else:
                    errno = error.index('no such string')
                    return Report("'no such string'", i + 1, task[i])
     	elif temp[0] == 'pop':
            if temp[1] in register.keys():
                stack.append(register[temp[1]])
            else:
                errno = error.index('no such register')
                return Report("'no such register'", i + 1, task[i])
        elif temp[0] == 'add':
            a = GetValue(stack)
            b = GetValue(stack)
            stack.append(a + b)
        elif temp[0] == 'mul':
            a = GetValue(stack)
            b = GetValue(stack)
            stack.append(a * b)
        elif temp[0] == 'sub':
            a = GetValue(stack)
            b = GetValue(stack)
            stack.append(- a + b)
        elif temp[0] == 'div':
            a = GetValue(stack)
            b = GetValue(stack)
            if a != 0:
                stack.append(b / a)
            else:
                errno = error.index('div by 0')
                return Report("'div by 0'", i + 1, task[i])
        else:
            errno = error.index('wtf')
            return Report("'wtf'", i + 1, task[i])
        i += 1

def IsOk():
    if errno:
        return 1
    else:
        return 0

def GetValue(stack):
    if stack:
        a = stack.pop()
    else:
        a = 0
    return a

def Report(error, strnum, wrongstring):
    return "Error {} in string {}:\n{}".format(error, strnum, wrongstring) 

error = ['ok','div by 0','wtf', 'bad input', 'no such string', 'no such register']
errno = 0
task = open(sys.argv[1])
error_report = CalcRevPol(task)
if IsOk():
    print error_report
    sys.exit(errno)
