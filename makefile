makefile	:	driver.o
	gcc driver.o -o compiler
driver.o	:	driver.c
	gcc -c driver.c