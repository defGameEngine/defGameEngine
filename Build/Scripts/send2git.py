from helpers import *
import sys
import embuild as em


WEBSITE_SOURCES_PATH = '../../../../js/defini7.github.io'


def send(target):
    em.build([target])

    copy_dirs(f'../Target/{target}', f'{WEBSITE_SOURCES_PATH}/{target.lower()}')

    builder = Builder()

    builder.add_command(Command('cd', [WEBSITE_SOURCES_PATH]))
    builder.add_command(Command('git', ['add', '--all']))
    builder.add_command(Command('git', ['commit', '-m', f'"added {target}"']))
    builder.add_command(Command('git', ['push']))

    builder.execute(True, True, True, True)


if __name__ == '__main__':
    assert len(sys.argv) > 1, 'Please provide a target'
    send(sys.argv[1])
