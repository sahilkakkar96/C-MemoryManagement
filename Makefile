part1:	main	run
main:	main.c
	gcc	-Wall	-Werror	-g	-o	part1	main.c	-lrt	-lpthread
run:	run.c
	gcc	-Wall	-Werror	-g	-o	run	run.c	-lrt	-lpthread
part2:	main2
main2:	main2.c
	gcc	-Wall	-Werror	-g	-o	part2	main2.c	-lrt	-lpthread	  									 	
clean:
	rm	-f	part1	run