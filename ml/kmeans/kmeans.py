import matplotlib.pyplot as plt
import subprocess
import fileinput
import random
import re
import argparse
import time

parser = argparse.ArgumentParser(description='kmeans')
parser.add_argument('-a', type=int, help='run for M:[2, a) (default 16)')
parser.add_argument('-m', type=int, help='run M')
parser.add_argument('-i', type=str, help='input executable')
parser.add_argument('-p', action='store_true', help='export plot png')
args = parser.parse_args()

def read_data(filename):
    with open(filename, 'r') as file:
        M, data = int(file.readline().strip()), []
        for line in file:
            values = line.strip().split('\t')
            data.append([float(v) for v in values])

        return data[:M], data[M:]

def plot_data(filename):
    cent, data = read_data(f"{filename}.txt")

    plt.scatter([d[0] for d in data], [d[1] for d in data], color=tuple(random.randint(155, 255) / 255.0 for _ in range(3)))
    plt.scatter([c[0] for c in cent], [c[0] for c in cent], color=tuple(random.randint(0, 100) / 255.0 for _ in range(3)))
    plt.title('Σύνολο δεδομένων ομαδοποίησης M=%d' % len(cent))
    plt.gcf().set_size_inches(800/80, 600/80)
    plt.savefig(f'{filename}.png', dpi=80) 

    archive(filename, 'data')

def archive(name, folder):
    ## unix
    subprocess.run(f''' [ -d "{folder}" ] || mkdir -p "{folder}" ''', shell=True)
    subprocess.run(f'''mv {name}.txt {name}.png {folder} ''', shell=True)
    ##

def replace_define(filename, new_value):
    pattern = re.compile(r'#define M \d+')

    with fileinput.FileInput(filename, inplace=True, backup='.bak') as file:
        for line in file:
            print(pattern.sub(f'#define M {new_value}', line), end='')

def init(M):
    exe_file = args.i if args.i else 'kmeans.c'
    out_file = f"kmeans_{M}_{int(time.time())}"
    replace_define(exe_file, M)

    ## unix
    subprocess.run(f'gcc {exe_file} -lm && ./a.out {out_file}.txt', shell=True)
    subprocess.run(f'rm {exe_file}.bak', shell=True)
    ##

    if(args.p): 
        plot_data(out_file)

if args.m:
    init(args.m)
else:
    mm = args.a if args.a else 16
    for i in range(2, args.a):
        init(args.m)


