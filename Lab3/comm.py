#!/usr/bin/python

import sys
from optparse import OptionParser

SP = "\t"
EMPTY = ""

class comm:
    def __init__(self, args, first_only, second_only, both, unsorted):
        if args[0] == "-":
            self.lines1 = sys.stdin.readlines()
        else:
            file1 = args[0]
            f1 = open(file1, 'r')
            self.lines1 = f1.readlines()
            f1.close()

        if args[1] == "-":
            self.lines2 = sys.stdin.readlines()
        else:
            file2 = args[1]
            f2 = open(file2, 'r')
            self.lines2 = f2.readlines()
            f2.close()

        self.first_only = bool(first_only)
        self.second_only = bool(second_only)
        self.both = bool(both)
        self.unsorted = bool(unsorted)

        if len(self.lines1)==0:            
            self.lines1.append("\n")
        else:
            temp = self.lines1[-1]
            if "\n" not in temp:
                temp += "\n"
            self.lines1[-1] = temp
        
        if len(self.lines2)==0:            
            self.lines2.append("\n")
        else:
            temp = self.lines2[-1]
            if "\n" not in temp:
                temp += "\n"
            self.lines2[-1] = temp

    def writeline(self, line, column):
        stream = ""
        if column == 1:
            if not self.first_only:
                return
        elif column == 2:
            if not self.second_only:
                return
            if self.first_only:
                stream += SP
        elif column == 3:
            if not self.both:
                return
            if self.first_only:
                stream += SP
            if self.second_only:
                stream += SP
        else:
            sys.stderr.write("incorrect argument\n")
        sys.stdout.write(stream + line)


    def issorted(self):
        sorted_line1= sorted(self.lines1)
        sorted_line2= sorted(self.lines2)

        files_sorted = True

        if sorted_line1 != self.lines1:
            files_sorted = False

        if sorted_line2 != self.lines2:
            files_sorted = False

        return files_sorted


def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
    Compare files FILE1 and FILE2 line by line.

    When FILE1 or FILE2 (not both) is -, read standard input.
    """

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", "--nofile1",
                      action="store_false", dest="first_only", default=True,
                      help="make first column disappear")
    parser.add_option("-2", "--nofile2",
                      action="store_false", dest="second_only", default=True,
                      help="make second column disappear")
    parser.add_option("-3", "--nocommon",
                      action="store_false", dest="both", default=True, help=
                      "make third column disappear")
    parser.add_option("-u", "--unsorted",
                      action="store_true", dest = "unsorted",
                      default=False, help="unsorted files")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        first_only = bool(options.first_only)
    except:
        parser.error("invalid argument for -1".
                     format(options.first_only))
    try:
        second_only = bool(options.second_only)
    except:
        parser.error("invalid argument for -2".
                     format(options.second_only))
    try:
        both = bool(options.both)
    except:
        parser.error("invalid argument for -3".
                     format(options.both))
    try:
        unsorted = bool(options.unsorted)
    except:
        parser.error("invalid argument for -u".
                     format(options.unsorted))

    if len(args) != 2:
        parser.error("missing operand")

    if args[0] == "-" and args[1] == "-":
        parser.error("only one file can be read from standard input")
    try:
        run = comm(args, first_only, second_only, both, unsorted)
    except OSError as err:
        parser.error("OS Error: {0}".format(err))

    if run.unsorted:
            diff_line2 = run.lines2
            for line1 in run.lines1:
                if line1 not in run.lines2:
                    run.writeline(line1, int(1))
                else:
                    run.writeline(line1, int(3))
                    diff_line2.remove(line1)

            for line2 in diff_line2:
                run.writeline(line2, int(2))


    issorted = run.issorted()

    if issorted:
            i, j = 0, 0
            len_line1, len_line2 = len(run.lines1), len(run.lines2)

            while i in range(len_line1) or j in range(len_line2):
                line1, line2 = EMPTY, EMPTY

                if i < len_line1:
                    line1 = run.lines1[i]
                if j < len_line2:
                    line2 = run.lines2[j]

                if line1 == EMPTY or (line2 != EMPTY and line1 > line2):
                    run.writeline(line2, 2)
                    j += 1
                elif line2 == EMPTY or line1 < line2:
                    run.writeline(line1, 1)
                    i += 1
                else:
                    run.writeline(line2, 3)
                    i += 1
                    j += 1

if __name__ == "__main__":
    main()