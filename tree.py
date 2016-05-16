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

def build_tree():
    tree = Tree(['G0'])
    cur_node = tree.root
    cur_node.left = get_E()
    return tree

def get_E():
    tmp = Node('E')
    tmp.left = get_P()
    return tmp

def get_N():
    global expr
    global cur_char
    tmp = Node('-1')
    print 'get_n ', expr[cur_char]
    if expr[cur_char].isdigit():
        tmp.data = expr[cur_char]
        cur_char += 1
        return tmp

def get_P():
    global expr
    global cur_char
    tmp = Node('-1')
    tmp.left = get_T()
    if (expr[cur_char] == '+'): 
        tmp.data = '+'
        cur_char += 1
    elif (expr[cur_char] == '-'):
        tmp.data = '-'
        cur_char += 1
    else:
        return tmp
    tmp.right = get_P()
    return tmp

def get_T():
    global expr
    global cur_char
    tmp = Node('-1')
    tmp.left = get_B() #get_N() 
    if (expr[cur_char] == '*'):
        tmp.data = '*'
        cur_char += 1
    elif (expr[cur_char] == '/'):
        tmp.data = '/'
        cur_char += 1
    else:
        return tmp
    tmp.right = get_T()
    return tmp

def get_B():
    global expr
    global cur_char
    tmp = Node('-1')
    if (expr[cur_char] == '('):
        cur_char += 1
        print 'found bracket'
        tmp.left = get_E()
        if (expr[cur_char] != ')'):
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
    

s = raw_input()
expr = s.split(' ')
cur_char = 0
expr = s.split(' ')
expr += ['\0']
tree = build_tree()
tree.print_tree()
dump(tree.root)
