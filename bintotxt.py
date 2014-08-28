#! /usr/bin/python
# Note:  The -u option allows unbuffered binary stdout and stderr.
# This can be done programatically by:
# Windows only: msvcrt.setmode(sys.stdin.fileno(), os.O_BINARY)
# sys.stdin = os.fdopen(sys.stdin.fileno(), 'rb', 0)
# sys.stdin.detach() - to disable buffering from within python.


import sys
import getopt

def binToAscii(bin_in):
    """
    Careful using this function stand alone, since it is practically impossible
    to feed in a binary input from the command prompt.
    """
    bin_list = list(bin_in)
    #print bin_list
    ascii_list = [ ]
    #print 'At beginning of binToAscii'

    for byte in bin_list:
        #print 'In loop'
        #print byte
        #if byte[0:2] == '\\x':
        """
        if ord(byte) < 48 or ord(byte) > 126:
        #if ord(byte) < 48:
            # maybe a form of '{0:02X}'.format(0x1c)
            ascii_list.append(byte)
        else:
            ascii_list.append(ord(byte))
        """
        ascii_list.append('{0:02X}'.format(ord(byte)))
    ascii_out = ''.join(ascii_list)
    # print ascii_out
    return ascii_out + '\n' # Added New Line to make compatible with manually created ASCII strings.


def usage():
    print "Usage:  bintoascii.py"


def main(argv):
    try:
        opts, args = getopt.getopt(argv, "h:", ["help"])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            sys.exit()

    bin_in = sys.stdin.read()
    ascii_out = binToAscii(bin_in)
    sys.stdout.write(ascii_out)

if __name__ == "__main__":
    main(sys.argv[1:])
