# -*- coding:utf-8 -*-
""" Setup script for CRC8/CRC16/CRC24/CRC32/CRC64/CRC82 library. """
from os import path
from setuptools import setup, find_packages, Extension
from codecs import open

# !/usr/bin/python
# Python:   3.5.2+
# Platform: Windows/Linux/MacOS/ARMv7
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Library CRC8/CRC16/CRC24/CRC32/CRC64/CRC82 Modules.
# History:  2017-08-09 Wheel Ver:0.0.1 [Heyn] Initialize
#           2017-08-10 Wheel Ver:0.0.2 [Heyn] New add CRC8 (IntelHex/BCC/LRC/CRC8(VERB))
#           2017-08-17 Wheel Ver:0.0.3 [Heyn] Optimized Code.
#                      Wheel Ver:0.0.4 [Heyn] New CRC16-SICK / CRC16-DNP.
#           2017-08-17 Wheel Ver:0.0.5 [Heyn] New CRC64-ISO / CRC64-ECMA182.
#           2017-08-22 Wheel Ver:0.1.2 [Heyn] Optimization code for the C99 standard.
#           2017-09-19 Wheel Ver:0.1.3 [Heyn] New CRC16-X25.
#           2020-03-17 Wheel Ver:1.0   [Heyn] New hacker16 / hacker32 / hacker64
#           2020-04-17 Wheel Ver:1.1   [Heyn] Issues #1
#           2020-08-04 Wheel Ver:1.4   [Heyn] Issues #4
#           2021-03-17 Wheel Ver:1.7   [Heyn] Developing.
#           2022-03-04 Wheel Ver:1.8   [Heyn] New add modbus(ASCII) checksum(LRC).

here = path.abspath(path.dirname(__file__))

with open(path.join(here, 'README.rst'), encoding='utf-8') as f:
    long_description = f.read()
    long_description = long_description.replace("\r", "")

setup(
    name='libscrc',
    version='1.8.1',

    description='Library for calculating CRC3/CRC4/CRC8/CRC16/CRC24/CRC32/CRC64/CRC82',
    long_description=long_description,

    url='https://github.com/hex-in/libscrc',

    author='Heyn',
    author_email='heyunhuan@gmail.com',

    # license='GNU General Public License v3 or later (GPLv3+)',

    platforms='any',

    classifiers=[
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        'Development Status :: 4 - Beta',

        'Intended Audience :: Developers',
        'Topic :: Software Development :: Libraries :: Python Modules',

        'License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)',

        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: Python :: Implementation :: PyPy',
    ],

    keywords=['CRC3', 'CRC4', 'CRC5', 'CRC6', 'CRC7', 'CRC8', 'CRC16', 'CRC24', 'CRC32', 'CRC64', 'CRC82'],

    packages=['libscrc'],

    install_requires=['wheel'],

    include_package_data=True,

    ext_modules=[Extension('libscrc._crcx',  sources=['src/crcx/_crcxmodule.c',   'src/crcx/_crcxtables.c'], include_dirs=['src/crcx' ]),
                 Extension('libscrc._canx',  sources=['src/canx/_canxmodule.c',   'src/canx/_canxtables.c'], include_dirs=['src/canx' ]),
                 Extension('libscrc._crc8',  sources=['src/crc8/_crc8module.c',   'src/crc8/_crc8tables.c'], include_dirs=['src/crc8' ]),
                 Extension('libscrc._crc16', sources=['src/crc16/_crc16module.c', 'src/crc16/_crc16tables.c'], include_dirs=['src/crc16' ]),
                 Extension('libscrc._crc24', sources=['src/crc24/_crc24module.c', 'src/crc24/_crc24tables.c'], include_dirs=['src/crc24' ]),
                 Extension('libscrc._crc32', sources=['src/crc32/_crc32module.c', 'src/crc32/_crc32tables.c'], include_dirs=['src/crc32' ]),
                 Extension('libscrc._crc64', sources=['src/crc64/_crc64module.c', 'src/crc64/_crc64tables.c'], include_dirs=['src/crc64' ]),
                ],
)
