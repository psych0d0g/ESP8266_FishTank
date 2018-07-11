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
                os.remove("ESP8266_FishTank.h")
        except OSError:
                pass
	s = open("html/settings.html", 'r')
        i = open("html/index.html", 'r')
	o = open("ESP8266_FishTank.h", 'a+')
        o.write('#ifndef header_h\n')
        o.write('#define header_h\n\n')
        o.write('String main_page=\n')
	for line, has_more in lookahead(i.readlines()):
                line = line.replace( '"', '\\"' )
		o.write('"')
		o.write(line.strip("\n"))
                if has_more:
                        o.write('\\r\\n"\n')
                else:
                        o.write('\\r\\n";\n')
	i.close()
        o.write('\nString settings_page=\n')
        for line, has_more in lookahead(s.readlines()):
                line = line.replace( '"', '\\"' )
                o.write('"')
                o.write(line.strip("\n"))
                if has_more:
                        o.write('\\r\\n"\n')
                else:
                        o.write('\\r\\n";\n')
        s.close()
	o.write('\n#endif\n')
	o.close()
	print("Done!")

raw2arduino()
