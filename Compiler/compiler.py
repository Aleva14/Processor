import sys

class Node:
    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None
    def add_left(self, data):
        self.left = Node(data)
    def add_right(self, data):
        self.left = Node(data) 
    def print_node(self):
        print self
        if (self.right != None):
            print 'r'
            self.right.print_node()   
        if (self.left != None):
            print 'l'
            self.left.print_node()
    def __str__(self):
        return str(self.data)

class Tree:
    def __init__(self, data):
        self.root = Node(data[0])
        del data[0]
        if data != []:
            self.add(data)
    def print_tree(self):
        self.root.print_node()
t_type = ['start', 'stop', 'end_st', '(', ')', 'add', 'sub', 'mul', 'div', 'num', 'def', 'undef', 'var', 'assign']

num_of_variables = 8
var_table = [0 for i in range(0, num_of_variables)]

class Token:
    def __init__(self, token_type, value):
        if token_type in t_type:
            self.type = token_type
        else:
            print 'Wrong token type'
        self.value = value
    def __str__(self):
        return (str(self.type) + str(self.value))

def lex():
    result = []
    print program
    for i in program:
        i = i.replace('\n', '')
        if i == 'start':
            result.append(Token('start', None))
        elif i == 'stop':
            result.append(Token('stop', None))
        elif i[-1] == '!':
            i = i.replace('!', '')
            i = i.split(' ')
            if i[0] == 'def':
                if i[1].isalpha():
                    result.append(Token('def', i[1]))
                    var_table.append(i[1])
            if i[0] == 'undef':
                if i[1] in var_table:
                    var_table.remove(i[1])
                    result.append(Token('undef', i[1]))
            if i[0].isalpha() and i[1] == '=':
                result.append(Token('assign', i[0]))
                for j in i[2:]:
                    if j.isdigit():
                        result.append(Token('num', j))
                    elif j == '(':
                        result.append(Token('(', None))
                    elif j == ')':
                        result.append(Token(')', None))
                    elif j == '+':
                        result.append(Token('add', None))
                    elif j == '-':
                        result.append(Token('sub', None))
                    elif j == '/':
                        result.append(Token('div', None))
                    elif j == '*':
                        result.append(Token('mul', None))
                    elif j in var_table:
                        result.append(Token('var', j))
            result.append(Token('end_st', None))
        else:
            print 'Error'
            exit(1)
    for i in result:
        print i.type, i.value
    return result

def build_tree():
    tree = Tree(['G0'])
    cur_node = tree.root
    cur_node.left = get_Block()
    return tree

def get_Block():
    global program
    global cur
    if program[cur].type == 'start': 
        tmp = Node('Block')
        cur += 1
        tmp.left = get_S()
        if cur >= len(program):
           print 'Error'
           exit(1)
        print 'END BLOCK', program[cur].type
        if program[cur].type != 'stop':
           print 'Error'
           exit(1)
        return tmp
    else:
        print 'Error'
        exit(1)

def get_S():
    global program
    global cur
    tmp = Node('S')
    tmp.left = get_Def()
    if tmp.left == None:
        tmp.left = get_Undef()
        if tmp.left == None:
            tmp.left = get_Assignment()
    if (program[cur].type == 'end_st'):
        cur += 1
        tmp.right = get_S()
        return tmp
    else:
        return None

def get_Def():
    global program
    global cur
    tmp = Node('-1')
    if (program[cur].type == 'def'):
        tmp.data = 'def'
        tmp.left = Node(program[cur].value)
        cur += 1
        return tmp
    return None

def get_Undef():
    global program
    global cur
    tmp = Node('-1')
    if (program[cur].type == 'undef'):
        tmp.data = 'undef'
        tmp.left = Node(program[cur].value)
        cur += 1
        return tmp
    return None

def get_Assignment():
    global program
    global cur
    tmp = Node('-1')
    if program[cur].type == 'assign':
        tmp.data = 'assign'
        tmp.left = Node(program[cur].value)
        cur += 1
        tmp.right = get_E()
        return tmp
    return None
            

def get_E():
    tmp = Node('E')
    tmp.left = get_P()
    return tmp

def get_N():
    global program
    global cur
    if program[cur].type == 'num':
        tmp = Node(program[cur].value)
        cur += 1
        return tmp
    return None
    

def get_V():
    global program
    global cur
    if program[cur].type == 'var':
        tmp = Node('var')
        tmp.left = Node(program[cur].value)
        cur += 1
        return tmp
    return None 

def get_P():
    global program
    global cur
    tmp = Node('-1')
    tmp.left = get_T()
    if (program[cur].type == 'add'): 
        tmp.data = '+'
        cur += 1
        tmp.right = get_P()
    elif (program[cur].type == 'sub'):
        tmp.data = '-'
        cur += 1
        tmp.right = get_P()
    return tmp

def get_T():
    global program
    global cur
    tmp = Node('-1')
    tmp.left = get_B()
    if (program[cur].type == 'mul'):
        tmp.data = '*'         
        cur += 1
        tmp.right = get_T()
    elif (program[cur].type == 'div'):
        tmp.data = '/'
        cur += 1
        tmp.right = get_T()
    return tmp

def get_B():
    global program
    global cur
    tmp = Node('-1')
    if (program[cur].type == '('):
        tmp.type = 'B'
        cur += 1
        tmp.left = get_E()
        if (program[cur].type != ')'):
            print 'wrong brackets'
            exit(1)
        cur += 1
    else:
        tmp.left = get_N()
        if tmp.left == None:
            tmp.left = get_V()
    return tmp

def dump_def(n):
    var = n.left.data
    if var in var_table:
        print 'Already defined'
    else:
        for i in var_table:
            print 'elem', i
            if i == 0:
                var_table[var_table.index(i)] = var
                break;
            if not(i in var_table):
                print 'Too many variables'

def dump_undef(n):
    var = n.left.data
    i = var_table.index(var)
    var_table[i] = 0

def dump(n):
    if n.left != None:
        dump(n.left)
    if n.right != None:
        dump(n.right)
    if n.data.isdigit():
        print 'PUSH', n.data
    elif n.data == 'var':
        register = 'v_' + str(var_table.index(n.left.data))
        print 'PUSH', register
    elif n.data == 'assign':
        register = 'v_' + str(var_table.index(n.left.data))
        print 'POP', register
    elif n.data == '+':
        print 'ADD'
    elif n.data == '-':
        print 'SUB'
    elif n.data == '*':
        print 'MUL'
    elif n.data == '/':
        print 'DIV'
    elif n.data == 'def':
        dump_def(n)
    elif n.data == 'undef':
        dump_undef(n)


program = open(sys.argv[1])
program = program.readlines()
program = lex()
cur = 0
tree = build_tree()
dump(tree.root)
