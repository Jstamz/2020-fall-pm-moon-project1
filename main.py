import math
import sys

class calculatedEquation:
    def __init__(self, value = -math.inf, str = ""):
        self.val = value
        self.s = str
    def set(self, value, str):
        self.val = value
        self.s = str
    def __del__(self):
        self.s = ""

class indexedEquation:
    def __init__(self, str = ""):
        self.s = str
        self.idx = 0
    def __del__(self):
        self.s = ""

MAX_OPERATOR_LEVEL = 3
max = calculatedEquation()

def StringToNumber(ieq):
    start = ieq.idx

    if ieq.s[ieq.idx] == '-':
        ieq.idx += 1

    while ((ieq.s[ieq.idx] >= '0') and (ieq.s[ieq.idx] <= '9')) or ieq.s[ieq.idx] == '.':
        ieq.idx += 1
        if ieq.idx == len(ieq.s):
            break

    number = float(ieq.s[start:ieq.idx])

    return number

def EvaluateTerm(ieq):
    if ieq.s[ieq.idx] == '(':
        ieq.idx += 1
        temp = ieq.idx
        depth = 1

        while depth > 0:
            if ieq.s[ieq.idx] == '(':
                depth += 1
            elif ieq.s[ieq.idx] == ')':
                depth -= 1
            ieq.idx += 1

        ss = indexedEquation()
        ss.s = ieq.s[temp:ieq.idx - 1]

        return CalculateExpression(ss)

    else:
        return StringToNumber(ieq)

def OperatorPriority(op):
    if op == '+':
        return 1
    elif op == '-':
        return 1
    elif op == '*':
        return 2
    elif op == 'X':
        return 2
    elif op == 'x':
        return 2
    elif op == '/':
        return 2
    elif op == '^':
        return 3

def CalculateOperator(left, right, op):
    if op == '+':
        return left + right
    elif op == '-':
        return left - right
    elif op == '*':
        return left * right
    elif op == 'X':
        return left * right
    elif op == 'x':
        return left * right
    elif op == '/':
        return left / right
    elif op == '^':
        return math.pow(left, right)

def CalculateExpression(ieq, level = 1):
    if level > MAX_OPERATOR_LEVEL:
        return EvaluateTerm(ieq)

    left = CalculateExpression(ieq, level + 1)
    if ieq.idx == len(ieq.s):
        return left

    op = ieq.s[ieq.idx]

    while (ieq.idx < len(ieq.s)) and (OperatorPriority(op) == level):
        ieq.idx += 1

        right = CalculateExpression(ieq, level + 1)

        left = CalculateOperator(left, right, op)

        if ieq.idx < len(ieq.s):
            op = ieq.s[ieq.idx]
        else:
            return left

    return left

def FindSpace(ieq):
    for i in range(ieq.idx, len(ieq.s)):
        if ieq.s[i] == ' ':
            return i

    return len(ieq.s)

def Solve(ieq, add, sub, mul, div, pow, seq_pow):
    new_idx = FindSpace(ieq)

    if ieq.idx == 0:
        max.val = -math.inf
        seq_pow = False
    
    if len(ieq.s) == new_idx:
        temp = ieq.idx
        ieq.idx = 0

        value = CalculateExpression(ieq)

        if math.isfinite(value):
            if value > max.val:
                max.val = value
                max.s = ieq.s
        
        ieq.idx = temp

    else:
        temp = ieq.idx
        ieq.idx = new_idx

        if add > 0:
            ieq.s = ieq.s[0:new_idx] + '+' + ieq.s[new_idx+1:]
            Solve(ieq, add - 1, sub, mul, div, pow, False)

        if sub > 0:
            ieq.s = ieq.s[0:new_idx] + '-' + ieq.s[new_idx+1:]
            Solve(ieq, add, sub - 1, mul, div, pow, False)

        if mul > 0:
            ieq.s = ieq.s[0:new_idx] + '*' + ieq.s[new_idx+1:]
            Solve(ieq, add, sub, mul - 1, div, pow, False)

        if div > 0:
            ieq.s = ieq.s[0:new_idx] + '/' + ieq.s[new_idx+1:]
            Solve(ieq, add, sub, mul, div - 1, pow, False)

        if (pow > 0) and (not seq_pow):
            ieq.s = ieq.s[0:new_idx] + '^' + ieq.s[new_idx+1:]
            Solve(ieq, add, sub, mul, div, pow - 1, True)

        ieq.s = ieq.s[0:new_idx] + ' ' + ieq.s[new_idx+1:]
        ieq.idx = temp

def main():
    eq = indexedEquation()

    print("Enter your equation:")
    eq.s = sys.stdin.readline().strip()
    print(eq.s)
    if eq.s == "exit":
        print("Exiting..")
        sys.exit(1)
    
    print("Number of add sub mul div pow: (splitted by blank)")
    add, sub, mul, div, pow = map(float, sys.stdin.readline().split())

    Solve(eq, add, sub, mul, div, pow, False)

    if math.isfinite(max.val):
        print("Maximum Value: {0}".format(max.val))
        print("Expression: {0}".format(max.s))
    else:
        print("Error while evaluating!")

    sys.exit(0)

if __name__ == "__main__":
    main()