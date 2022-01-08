import os
import pwd
import psutil


def get_username():
    '''Find the user name of the current process'''
    return pwd.getpwuid( os.getuid() ).pw_name


def find_ancestor(pid=None, username=None):
    '''Find the ancestor of the process with ID pid thatis owned by the user with
    the given user name'''
    if pid is not None and not psutil.pid_exists(pid):
        raise ValueError(f'PID {pid} does not exist')
    if username is None:
        username = get_username()
    process = psutil.Process(pid)
    parents = process.parents()
    for parent in reversed(parents):
        if parent.username() == username:
            return parent
    return process


def find_pids(process_name, user=None, ancestor=False):
    pids = []
    for process in psutil.process_iter():
        if process_name == process.name():
            pids.append(process.pid)
    if ancestor:
        pid = pids[0] if len(pids) > 0 else None
        pids = [find_ancestor(pid, user).pid]
    return pids
