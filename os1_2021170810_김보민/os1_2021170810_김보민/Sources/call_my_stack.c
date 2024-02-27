#include<unistd.h>
#include<stdio.h>

#define my_stack_push 335 //declare system number
#define my_stack_pop 336

int main(){

	int r;
	
	syscall(my_stack_push,1);
	printf("PUSH %d\n",1);

	syscall(my_stack_push,1);
	printf("PUSH %d\n",1);

	syscall(my_stack_push,2);
	printf("PUSH %d\n",2);

	
	syscall(my_stack_push,3);
	printf("PUSH %d\n",3);


	r=syscall(my_stack_pop);
	printf("POP  %d\n",r);


	r=syscall(my_stack_pop);
	printf("POP  %d\n",r);

	r=syscall(my_stack_pop);
	printf("POP  %d\n",r);



	return 0;
}
