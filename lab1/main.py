import subprocess
import sys

funcs = {}
cur_char = 'a'


def add_assert(s, file):
    file.write('(assert (' + s + '))\n')


def declare_and_assert(char, file):
    file.writelines(
        ['(declare-fun ' + char + str(i) + ' () Int)\n' for i in range(6)]
    )
    for i in range(6):
        add_assert('>= ' + char + str(i) + ' -1', file)

    # left upper corner in matrix != -inf
    add_assert('>= ' + char + '0 0', file)
    # upper coefficient in const term != inf or const term does not exist
    add_assert('or (>= ' + char + '4 0) (and (= ' + char + '4 -1) (= ' + char + '5 -1))', file)


def parse_rule(rule, file):
    global cur_char
    tpl = tuple(rule.split(' -> '))
    for s in tpl:
        for c in s:
            if c not in funcs:
                funcs[c] = [[cur_char + str(i) for i in range(4)], [cur_char + str(i) for i in range(4, 6)]]
                declare_and_assert(cur_char, file)
                cur_char = chr(ord(cur_char) + 1)
    return tuple(rule.split(' -> '))


def mul_matrix(left, right):
    res = []
    for i in range(4):
        res.append('(arc_plus (arc_mul ' + left[2 * (i // 2)] + ' ' + right[i % 2] + ') '
                                                                                     '(arc_mul ' + left[
                       2 * (i // 2) + 1] + ' ' + right[i % 2 + 2] + '))')
    return res


def mul_matrix_vector(mat, vec):
    res = ['', '']
    for i in range(2):
        res[i] = '(arc_plus (arc_mul ' + mat[2 * i] + ' ' + vec[0] + ') (arc_mul ' + mat[2 * i + 1] + ' ' + vec[
            1] + '))'
    return res


def plus_vectors(vec1, vec2):
    return [
        '(arc_plus ' + vec1[0] + ' ' + vec2[0] + ')',
        '(arc_plus ' + vec1[1] + ' ' + vec2[1] + ')'
    ]


def compose(seq):
    if len(seq) == 1:
        return [funcs[seq[0]][0], funcs[seq[0]][1]]
    left, right = seq[-2:]
    A, B = funcs[left][0], funcs[left][1]
    C, D = funcs[right][0], funcs[right][1]
    first = mul_matrix(A, C)
    second = plus_vectors(mul_matrix_vector(A, D), B)
    i = -3
    res = [first, second]
    while abs(i) <= len(seq):
        left = seq[i]
        A, B = funcs[left][0], funcs[left][1]
        res = [mul_matrix(A, res[0]), plus_vectors(mul_matrix_vector(A, res[1]), B)]
        i -= 1
    return res


if __name__ == '__main__':
    filename = sys.argv[1]
    smt2_file = open('to_solve.smt2', 'w')

    with open('base_smt2.txt', 'r') as file:
        smt2_file.writelines(file.readlines())

    with open(filename) as file:
        rules = [line.rstrip() for line in file]

    rules = list(map(lambda x: parse_rule(x, smt2_file), rules))
    compositions = []

    for i in range(len(rules)):
        compositions.append((compose(rules[i][0]), compose(rules[i][1])))

    for comp in compositions:
        for i in range(4):
            add_assert(
                'arc_gr ' + comp[0][0][i] + ' ' + comp[1][0][i],
                smt2_file
            )
        for i in range(2):
            add_assert(
                'arc_gr ' + comp[0][1][i] + ' ' + comp[1][1][i],
                smt2_file
            )

    smt2_file.write('(check-sat)\n')
    smt2_file.close()
    subprocess.call('z3 ' + smt2_file.name, shell=True)
