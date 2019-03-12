SRCFILE = shox96_0_2.c
OUTFILE = shox96

default:
	gcc -o $(OUTFILE) $(SRCFILE)

install: default
	cp $(OUTFILE) /usr/bin/

clean:
	$(RM) $(OUTFILE)
