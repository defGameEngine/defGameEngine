from helpers import *
import sys
import embuild as em


WEBSITE_SOURCES_PATH = '../../../../js/defini7.github.io'


def send(target):
    em.build([target])

    copy_dirs(f'../Target/{target}', f'{WEBSITE_SOURCES_PATH}/{target.lower()}')

    builder = Builder()
    
    add = Command('git')
    add.add_argument('add')
    add.add_argument('--all')

    commit = Command('git')
    commit.add_argument('commit')
    commit.add_flag('m', f'"added {target}"', True)

    push = Command('git')
    push.add_argument('push')
    push.add_argument('origin')
    push.add_argument('master')
    push.add_argument('--force')
    
    builder.add_command(add)
    builder.add_command(commit)
    builder.add_command(push)

    builder.execute(True, True, True, True)


if __name__ == '__main__':
    assert len(sys.argv) > 1, 'Please provide a target'
    send(sys.argv[1])
