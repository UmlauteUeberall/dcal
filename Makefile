CC=gcc
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

all: dcal.c
	$(CC) dcal.c -o dcal

install: dcal dcal.1
	install -d $(PREFIX)/bin/
	install -m 755 dcal $(PREFIX)/bin/
	gzip -k dcal.1
	install -d $(PREFIX)/share/man/man1/
	install -m 644 dcal.1.gz $(PREFIX)/share/man/man1/	

clean:
	rm dcal
	rm dcal.1.gz

uninstall:
	rm $(PREFIX)/bin/dcal
	rm $(PREFIX)/share/man/man1/dcal.1.gz
