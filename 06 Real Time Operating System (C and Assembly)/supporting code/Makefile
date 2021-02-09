# ECEn 425 Lab 8 Makefile

lab8.bin:	lab8final.s
		nasm lab8final.s -o lab8.bin -l lab8.lst

lab8final.s:	clib.s myisr.s myinth.s lab8app.s yakc.s yaks.s
		cat clib.s simptris.s myisr.s lab8app.s myinth.s yakc.s yaks.s > lab8final.s

myinth.s:	clib.h yakk.h myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

yakc.s: 	clib.h yakk.h yaku.h yakc.c
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

lab8app.s:	clib.h simptris.h yakk.h yaku.h lab8defs.h lab8app.c
		cpp lab8app.c lab8app.i
		c86 -g lab8app.i lab8app.s

clean:
		rm lab8.bin lab8.lst lab8final.s myinth.s myinth.i \
                lab8app.s lab8app.i yakc.s yakc.i
