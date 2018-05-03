part1:	main
main:	main.c
	gcc	-Wall	-Werror	-g	-o	part1	main.c	-lrt	-lpthread		  									 	
clean:
	rm	-f	part1