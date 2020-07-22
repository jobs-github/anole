# -*- coding: UTF-8 -*-
#/usr/bin/python
import os
import json
import string
import sys   
reload(sys)
sys.setdefaultencoding('utf-8')

iglist = [
    'linux-vdso.so', 
    'libpthread.so', 
    'libstdc++.so', 
    'libm.so', 
    'libgcc_s.so', 
    'libc.so', 
    'libdl.so', 
    'librt.so',
    'libz.so',
    'liblzma.so'
]

def manual(): 
    print """
    usage:
        python ldd.py [elf] [dir] [dir] ...
    sample:
        python ldd.py anole /usr/ /lib/x86_64-linux-gnu/ /usr/local/lib/ /usr/local/boost_1_73_0/lib/
        """

def acceptable(line):
    for item in iglist:
        if -1 != line.find(item):
            return False
    return True

def find_libs(url, dirs):
    files = {}
    keys = []
    with os.popen('ldd %s' % url) as f:
        libs = [{
            'key': line.split('.so')[0].split('\t')[-1],
            'name': line.split(' => ')[0].split('\t')[-1]
        } for line in filter(acceptable, f.readlines())]
        for lib in libs:
            if lib['key'][0] == '/':
                continue
            print lib['name']
            keys.append(lib['name'])
            with os.popen('find %s -name "*.so*" | grep %s.so' % (string.join(dirs, ' '), lib['key'])) as fp:
                files[lib['name']] = fp.readlines()
    return [files, keys]

def check_keys(files, keys):
    for key in keys:
        if not key in files:
            print '%s not found!' % key
            return False
    return True

def locate_libs(files, keys, dbs):
    for key in keys:
        links = set()
        nonlinks = set()
        for lib in files[key]:
            lib = lib.split('\n')[0]
            if os.path.islink(lib):
                realpath = os.path.realpath(lib)
                if not realpath in links:
                    links.add(realpath)
            else:
                if not lib in nonlinks:
                    nonlinks.add(lib)
        if len(links) < 1 and len(nonlinks) < 1:
            print '%s locate fail!' % key
            return False
        dbs[key] = { 'links': list(links), 'nonlinks': list(nonlinks) }
    return True

def write_libs(keys, dbs, url):
    with open(url, 'w') as f:
        for key in keys:
            nonlinks = len(dbs[key]['nonlinks'])
            if nonlinks > 0:
                for lib in dbs[key]['nonlinks']:
                    f.write('%s %s\n' % (key, lib))
                if nonlinks > 1:
                    print '%s duplicated' % key
            else:
                links = len(dbs[key]['links'])
                for lib in dbs[key]['links']:
                    f.write('%s %s\n' % (key, lib))
                if links > 1:
                    print '%s duplicated' % key

def ldd(url, dirs):
    [files, keys] = find_libs(url, dirs)
    print ''
    keys.sort()
    if not check_keys(files, keys):
        return True
    dbs = {}
    if not locate_libs(files, keys, dbs):
        return True
    write_libs(keys, dbs, 'ldd.txt')
    return True

def main(argv):
    size = len(argv)
    if size > 3:
        return ldd(argv[1], argv[2:])
    return False

if __name__ == "__main__":
    if not main(sys.argv):
        manual()