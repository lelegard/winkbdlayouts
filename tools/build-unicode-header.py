#!/usr/bin/env python
#---------------------------------------------------------------------------
#
# Windows Keyboards Layouts (WKL)
# Copyright (c) 2023, Thierry Lelegard
# BSD-2-Clause license, see the LICENSE file.
#
# Utility generate a header containing "SYM" from "#define".
#
#---------------------------------------------------------------------------

import sys, re

if len(sys.argv) != 3:
    print('Usage: %s in-file out-file' % sys.argv[0], file=sys.stderr)
    exit(1)

input_file = sys.argv[1]
output_file = sys.argv[2]

with open(output_file, 'w') as output:
    with open(input_file, 'r', encoding='utf-8') as input:
        for line in input:
            match = re.search(r'#define\s+(UC_\w+)\s+', line.strip())
            if match is not None:
                print('    SYM(%s),' % match.group(1), file=output)
