# Copyright 2020 The Hush Developers
# just type make to compile all dapps
all: zmigrate oraclefeed

subatomic:
	$(CC) subatomic.c -o subatomic -lm

zmigrate:
	$(CC) zmigrate.c -o zmigrate -lm

oraclefeed:
	$(CC) oraclefeed.c -o oraclefeed -lm

clean:
	rm zmigrate oraclefeed

