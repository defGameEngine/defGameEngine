import sys
import subprocess


EMSDK_PATH = 'C:/SDK/emsdk'

def run(target):
    run = subprocess.run(f'{EMSDK_PATH}/emsdk activate latest & emrun ../Target/{target}/index.html', capture_output=True, shell=True)

    if len(run.stdout) > 0:
        print(run.stdout.decode(errors='ignore'))

    if len(run.stderr) > 0:
        print(run.stderr.decode(errors='ignore'))


if __name__ == '__main__':
    assert len(sys.argv) > 1, 'Please provide target'
    run(sys.argv[1])
