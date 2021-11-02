from math import sqrt
from queue import PriorityQueue

start='806524731'
goal='123456780'

n=int(sqrt(len(goal))) # 보드 크기

class Node():
    def __init__(self,state,path,stateList):
        self.state=state
        self.path=path
        self.val=self.f()
        self.stateList=stateList

    def f_mismatch(self):
        return sum(self.state[i]!=goal[i] for i in range(len(goal)))

    def f_manhattan(self):
        dist=0
        for i in range(len(goal)):
            r,c=i//n,i%n
            r1,c1=self.state.index(goal[i])//n,self.state.index(goal[i])%n
            dist+=abs(r1-r)+abs(c1-c)
        return dist

    def f(self):
        return len(self.path)+self.f_manhattan()

    def __lt__(self,other):
        return self.val<other.val

def print_puzzle(state):
    for i in range(len(state)//9):

        tmp = state[i*9:i*9+9]
        zeroPosition = tmp.index('0')
        
        if i == 0:
            print("initial state:")

        elif i == len(state)//9 - 1:
            print("Goal state:")
        else:
            print("Step: %d" % i)
        
        print('ㅡㅡㅡㅡㅡㅡㅡㅡ')

        for j in range(9):
            if j == zeroPosition:
                print("ㅣ   ", end='')
            else:
                print("ㅣ %c " % tmp[j], end='')
            if j % 3 == 2:
                print("ㅣ")
                print('ㅡㅡㅡㅡㅡㅡㅡㅡ')
        
        if i != len(state)//9 -1:
            print('        ↓       \n')
        


def Move(node):
    p=node.state
    i=p.index('0') # 빈 칸 위치
    child=[]
    if not (i%n==0): # 좌변이 아니면
        q=p[:i-1]+p[i]+p[i-1]+p[i+1:]
        child.append(Node(q,node.path+'L', node.stateList+q))
    if not (i%n==n-1): # 우변이 아니면
        q=p[:i]+p[i+1]+p[i]+p[i+2:]
        child.append(Node(q,node.path+'R', node.stateList+q))
    if i>=n: # 상변이 아니면
        q=p[:i-n]+p[i]+p[i-n+1:i]+p[i-n]+p[i+1:]
        child.append(Node(q,node.path+'U', node.stateList+q))
    if i<n*n-n: # 하변이 아니면
        q=p[:i]+p[i+n]+p[i+1:i+n]+p[i]+p[i+n+1:]
        child.append(Node(q,node.path+'D', node.stateList+q))
    return child


Q=PriorityQueue()
root=Node(start,'-', start) # 루트 노드
Q.put(root)
V=[root.state]
while not Q.empty():
    node=Q.get()
    
    if node.state==goal:
        print_puzzle(node.stateList)
        break
    else:
        child=Move(node)
        for j in range(len(child)):
            if child[j].state not in V:
                Q.put(child[j])
                V.append(node.state)


