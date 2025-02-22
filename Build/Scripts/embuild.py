"""
    It's a build script that you can use for the Emscripten (works only on Windows)
"""

from helpers import *


EMSDK_PATH = 'C:/SDK/emsdk'
STB_PATH = '../../Engine/Vendor/stb'
ASSETS_FOLDER = None
ENGINE_INCLUDE_PATH = '../../Engine/Include'
ENGINE_SOURCES_PATH = '../../Engine/Sources'


def construct_engine_sources() -> list[str]:
    return [file_name for file_name in get_files(ENGINE_SOURCES_PATH)]


def build(targets):
    builder = Builder()

    create_dir_if_not_exists('../Target/Web')

    command_initialise = Command(f'{EMSDK_PATH}/emsdk')

    command_initialise.add_argument('activate')
    command_initialise.add_argument('latest')

    for target in targets:
        create_dir_if_not_exists(f'../Target/Web/{target}')

        c = Command('em++')

        c.add_flag('O2')
        c.add_flag('Wall')
        c.add_flag('pedantic')
        c.add_flag('std', 'c++20')

        c.add_flag('sALLOW_MEMORY_GROWTH', '1')
        c.add_flag('sMIN_WEBGL_VERSION', '2')
        c.add_flag('sMAX_WEBGL_VERSION', '2')

        c.add_flag(f'I"{STB_PATH}"')
        c.add_flag(f'I"{ENGINE_INCLUDE_PATH}"')

        for source in construct_engine_sources():
            if source != 'PlatformGLFW3.cpp':
                c.add_argument(f'{ENGINE_SOURCES_PATH}/{source}')

        # !!!!!! CHANGE A PATH TO YOUR SOURCE FILES !!!!!!
        c.add_argument(f'../../Examples/{target}.cpp')

        c.add_flag('o')
        c.add_argument(f'../Target/Web/{target}/index.js')

        if ASSETS_FOLDER and is_dir(ASSETS_FOLDER):
            c.add_argument('--preload-file')
            c.add_argument(ASSETS_FOLDER)

        builder.add_command(command_initialise.combine(c))

    builder.execute(True, True, True)
    copy_file('Emscripten/template.html', f'../Target/Web/{target}/index.html')


if __name__ == '__main__':
    build([]) # provide targets here
