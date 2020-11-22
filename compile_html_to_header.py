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
    o.write('const char* main_page = R"mainhtml(\n')
    for line, has_more in lookahead(i.readlines()):
        o.write(line)
        if not has_more:
            o.write('\n)mainhtml";\n')
    i.close()
    o.write('\nconst char* index_script = R"indexscript(\n')
    for line, has_more in lookahead(ij.readlines()):
        o.write(line)
        if not has_more:
            o.write('\n)indexscript";\n')
    ij.close()
    o.write('\nconst char* gauges_script = R"gaugesscript(\n')
    for line, has_more in lookahead(gj.readlines()):
        o.write(line)
        if not has_more:
            o.write('\n)gaugesscript";\n')
    gj.close()
    o.write('\nconst char* settings_page = R"settingshtml(\n')
    for line, has_more in lookahead(s.readlines()):
        o.write(line)
        if not has_more:
            o.write('\n)settingshtml";\n')
    s.close()
    o.write('\nconst char* settings_script = R"settingsscript(\n')
    for line, has_more in lookahead(sj.readlines()):
        o.write(line)
        if not has_more:
            o.write('\n)settingsscript";\n')
    sj.close()
    o.write('\nconst char* stylesheet = R"stylesheet(\n')
    for line, has_more in lookahead(ss.readlines()):
        o.write(line)
        if not has_more:
            o.write('\n)stylesheet";\n')
    ss.close()
    o.write('#endif\n')
    o.close()
    print("Done!")

raw2arduino()
