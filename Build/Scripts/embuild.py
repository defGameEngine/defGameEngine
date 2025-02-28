"""
    It's a build script that you can use for the Emscripten
"""

import sys
from helpers import *


EMSDK_PATH = 'C:/SDK/emsdk'
ASSETS_FOLDER = None


def build(path, filename):
    builder = Builder()

    create_dir_if_not_exists('../Target/Web')

    command_initialise = Command(f'{EMSDK_PATH}/emsdk')

    command_initialise.add_argument('activate')
    command_initialise.add_argument('latest')

    create_dir_if_not_exists(f'../Target/Web/{filename}')

    c = Command('em++')

    c.add_flag('O2')
    c.add_flag('Wall')
    c.add_flag('pedantic')
    c.add_flag('std', 'c++20')

    c.add_flag('sALLOW_MEMORY_GROWTH', '1')
    c.add_flag('sMIN_WEBGL_VERSION', '2')
    c.add_flag('sMAX_WEBGL_VERSION', '2')

    c.add_flag(f'I"../../Engine/Vendor/stb"')
    c.add_flag('I"../../Engine/Include"')

    for source in get_files('../../Engine/Sources'):
        if source != 'PlatformGLFW3.cpp':
            c.add_argument(f'../../Engine/Sources/{source}')

    c.add_argument(f'{path}{filename}.cpp')

    c.add_flag('o')
    c.add_argument(f'../Target/Web/{filename}/index.js')

    if ASSETS_FOLDER and is_dir(ASSETS_FOLDER):
        c.add_argument('--preload-file')
        c.add_argument(ASSETS_FOLDER)

    builder.add_command(command_initialise.combine(c))

    builder.execute(True, True, True)
    copy_file('Emscripten/template.html', f'../Target/Web/{filename}/index.html')


if __name__ == '__main__':
    assert len(sys.argv) > 2, 'Please provide a path (without the filename) and the filename'
    build(sys.argv[1], sys.argv[2])
