
import sys

debug = False

def parse_paren(tokens):
    if debug: print("parse paren:", tokens)
    if tokens[0] == '(':
        tokens.pop(0)
        val = parse_sum(tokens)
        if tokens[0] != ')':
            print("error, mismatched parens")
        tokens.pop(0)
        return val
    else:
        return tokens.pop(0)

def parse_factor(tokens):
    if debug: print("parse factor:", tokens)
    if len(tokens) == 1 or tokens[1] == ')':
        return tokens.pop(0)
    lhs = parse_paren(tokens)
    if len(tokens) > 0 and tokens[0] in '*/':
        op = tokens.pop(0)
        rhs = parse_paren(tokens)
        part = (op, lhs, rhs)
        while len(tokens) > 0 and tokens[0] in '*/':
            op = tokens.pop(0)
            rhs = parse_paren(tokens)
            part = (op, part, rhs)
        return part
    else:
        return lhs

def parse_sum(tokens):
    if debug: print("parse sum:", tokens)
    lhs = parse_factor(tokens)
    if len(tokens) == 0 or tokens[0] == ')':
        return lhs
    if tokens[0] in '+-':
        op = tokens.pop(0)
        rhs = parse_factor(tokens)
        part = (op, lhs, rhs)
        while len(tokens) > 0 and tokens[0] in '+-':
            op = tokens.pop(0)
            rhs = parse_factor(tokens)
            part = (op, part, rhs)
        return part
    else:
        assert False

def eval(ast):
    if type(ast) is int:
        return ast
    if ast[0] == '+':
        return eval(ast[1]) + eval(ast[2])
    if ast[0] == '-':
        return eval(ast[1]) - eval(ast[2])
    if ast[0] == '*':
        return eval(ast[1]) * eval(ast[2])
    if ast[0] == '/':
        return eval(ast[1]) / eval(ast[2])

def tokenize(s):
    tokens = []
    i = 0
    while True:
        if s[i].isdigit():
            token = ""
            while s[i].isdigit():
                token += s[i]
                i += 1
                if len(s[i:]) == 0:
                    break
            tokens.append(int(token))
            i -= 1
        elif s[i] == ' ':
            pass
        elif s[i] in '+-*/()':
            tokens.append(s[i])
        else:
            print("tokenization error: unexpected '" + s[i] + "'")

        if len(s[i:]) == 1:
            break
        i += 1
    return tokens

def run(s):
    tokens = tokenize(s)
    print(tokens)
    t = parse_sum(tokens)
    print(t)
    print(eval(t))

def main():
    while True:
        v = input('> ')
        if len(v) == 0:
            continue
        run(v)

if __name__ == "__main__":
    main()

