all: pipe msgq;

pipe: pipe.c;
	gcc -o pipe.out pipe.c;

msgq: msgq.c;
	gcc -o msgq.out msgq.c;