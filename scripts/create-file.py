#!/usr/bin/env python

import re
import sys

SRC = './src'
HDR = './include'
SRC_TEMPLATE = '#include "%s"\n'
HDR_TEMPLATE = '#ifndef %s\n#define %s\n\n#endif  // %s\n'


def main():
    if len(sys.argv) < 2:
        raise ValueError('Please specify a filename!')

    # Parse filename and tokens
    filename = sys.argv[1]
    tokens = map(str.upper, re.split('-|_', filename))
    tokens = filter(lambda s: len(s) > 0, tokens)
    if len(tokens) < 1:
        raise ValueError('Invalid filename!')

    # Generate header
    header_filename = filename + '.hpp'
    guard = '_'.join(tokens) + '_HPP_'
    print('Creating header file %s...' % (header_filename))

    header = open(HDR + '/' + header_filename, 'w')
    header.write(HDR_TEMPLATE % (guard, guard, guard))
    header.close()

    # Generate source
    source_filename = filename + '.cpp'
    print('Creating source file %s...' % (source_filename))

    source = open(SRC + '/' + source_filename, 'w')
    source.write(SRC_TEMPLATE % (header_filename))
    source.close()

    print('Done!')


if __name__ == '__main__':
    main()
