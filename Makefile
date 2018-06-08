all: pipe msgq;

pipe: pipe.c;
	gcc -o pipe pipe.c;

msgq: msgq.c;
	gcc -o msgq msgq.c;