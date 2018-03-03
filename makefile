makefile	:	driver.o
	gcc driver.o -o stage1exe
driver.o	:	driver.c
	gcc -c driver.c