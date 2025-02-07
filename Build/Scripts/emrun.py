import sys
import subprocess


EMSDK_PATH = 'C:/SDK/emsdk'

def main():
    assert len(sys.argv) > 1, 'Please provide target'

    run = subprocess.run(f'{EMSDK_PATH}/emsdk activate latest & emrun ../Target/{sys.argv[1]}/index.html', capture_output=True, shell=True)

    if len(run.stdout) > 0:
        print(run.stdout.decode(errors='ignore'))

    if len(run.stderr) > 0:
        print(run.stderr.decode(errors='ignore'))


if __name__ == '__main__':
    main()
