COMPILER = ../build/bin/clang
CFLAG = -fsanitize=address -L. -lpostgres -g -O0
ALL:
	$(COMPILER) -c postgres.c -g -o postgres.o
	$(COMPILER) postgres.o -shared -fPIC -o libpostgres.so #生成动态链接库
	$(COMPILER) -c test.c -fsanitize=address -g -o test.o
	$(COMPILER) test.o $(CFLAG) -o a.out #动态链接
clean:
	rm a.out libpostgres.so test.o postgres.o -rf