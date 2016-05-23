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

var_table = []

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
            result.append(Token('end_st', None))
        else:
            print Error
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
    if token[cur].type == 'start': 
        tmp = Node('Block')
        tmp.left = get_S()
        if token[cur].type != 'stop':
           print 'Error'
           exit(1)

def get_S():
    tmp = Node('S')
    tmp.left = get_E()
    tmp.right = get_S()

def get_E():
    tmp = Node('E')
    tmp.left = get_P()
    return tmp

def get_N():
    global program
    global cur_token
    tmp = Node('-1')
    print 'get_n ', program[cur_token]
    if program[cur_token].type == 'num':
        tmp.data = program[cur_token]
        cur_token += 1
        return tmp

def get_P():
    global program
    global cur_token
    tmp = Node('-1')
    tmp.left = get_T()
    if (program[cur_token].type == 'add'): 
        tmp.data = '+'
        cur_token += 1
    elif (program[cur_token] == 'sub'):
        tmp.data = '-'
        cur_token += 1
    else:
        return tmp
    tmp.right = get_P()
    return tmp

def get_T():
    global program
    global cur_token
    tmp = Node('-1')
    tmp.left = get_B() #get_N() 
    if (program[cur_token] == 'mul'):
        tmp.data = '*'
        cur_token += 1
    elif (program[cur_token] == 'div'):
        tmp.data = '/'
        cur_token += 1
    else:
        return tmp
    tmp.right = get_T()
    return tmp

def get_B():
    global program
    global cur_token
    tmp = Node('-1')
    if (program[cur_token] == '('):
        cur_char += 1
        print 'found bracket'
        tmp.left = get_E()
        if (program[cur_token] != ')'):
            print 'wrong brackets'
            return None
        cur_char += 1
    else:
        print 'looking fo num'
        tmp.left = get_N()
    return tmp


def dump(n):
    if n.left != None:
        dump(n.left)
    if n.right != None:
        dump(n.right)
    if n.data.isdigit():
        print 'PUSH', n.data
    elif n.data == '+':
        print 'ADD'
    elif n.data == '-':
        print 'SUB'
    elif n.data == '*':
        print 'MUL'
    elif n.data == '/':
        print 'DIV'
    


program = open(sys.argv[1])
program = program.readlines()
program = lex()
#tree = build_tree()
#tree.print_tree()
#dump(tree.root)
