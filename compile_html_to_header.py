#!/usr/bin/python
#
# "Compiles" static html files to header file for inclusion
#
import os

def lookahead(iterable):
    """Pass through all values from the given iterable, augmented by the
    information if there are more values to come after the current one
    (True), or if it is the last value (False).
    """
    # Get an iterator and pull the first value.
    it = iter(iterable)
    last = next(it)
    # Run the iterator to exhaustion (starting from the second value).
    for val in it:
        # Report the *previous* value (more to come).
        yield last, True
        last = val
    # Report the last value.
    yield last, False

def raw2arduino():
    try:
        os.remove("lib/html.h")
    except OSError:
        pass
    s = open("html/settings.html", 'r')
    sj = open("html/js/settings.js", 'r')
    i = open("html/index.html", 'r')
    ij = open("html/js/index.js", 'r')
    gj = open("html/js/gauges.js", 'r')
    ss = open("html/css/style.css", 'r')
    o = open("lib/html.h", 'a+')
    o.write('#ifndef html_h\n')
    o.write('#define html_h\n\n')
    o.write('static String main_page=\n')
    for line, has_more in lookahead(i.readlines()):
        line = line.replace( '"', '\\"' )
        o.write('"')
        o.write(line.strip("\n"))
        if has_more:
            o.write('\\r\\n"\n')
        else:
            o.write('\\r\\n";\n')
    i.close()
    o.write('\nstatic String index_script=\n')
    for line, has_more in lookahead(ij.readlines()):
        line = line.replace( '"', '\\"' )
        o.write('"')
        o.write(line.strip("\n"))
        if has_more:
            o.write('\\r\\n"\n')
        else:
            o.write('\\r\\n";\n')
    ij.close()
    o.write('\nstatic String gauges_script=\n')
    for line, has_more in lookahead(gj.readlines()):
        line = line.replace( '"', '\\"' )
        o.write('"')
        o.write(line.strip("\n"))
        if has_more:
            o.write('\\r\\n"\n')
        else:
            o.write('\\r\\n";\n')
    gj.close()
    o.write('\nstatic String settings_page=\n')
    for line, has_more in lookahead(s.readlines()):
        line = line.replace( '"', '\\"' )
        o.write('"')
        o.write(line.strip("\n"))
        if has_more:
            o.write('\\r\\n"\n')
        else:
            o.write('\\r\\n";\n')
    s.close()
    o.write('\nstatic String settings_script=\n')
    for line, has_more in lookahead(sj.readlines()):
        line = line.replace( '"', '\\"' )
        o.write('"')
        o.write(line.strip("\n"))
        if has_more:
            o.write('\\r\\n"\n')
        else:
            o.write('\\r\\n";\n')
    sj.close()
    o.write('\nstatic String stylesheet=\n')
    for line, has_more in lookahead(ss.readlines()):
        line = line.replace( '"', '\\"' )
        o.write('"')
        o.write(line.strip("\n"))
        if has_more:
            o.write('\\r\\n"\n')
        else:
            o.write('\\r\\n";\n')
    ss.close()
    o.write('#endif\n')
    o.close()
    print("Done!")

raw2arduino()
