#include<linux/syscalls.h>
#include<linux/kernel.h>
#include<linux/linkage.h>

int stack[50]; //stack declaration
int top=0; //top initialization
void print_stack(void);


SYSCALL_DEFINE1(os2023_push, int, a){
	int i;

	//if stack overflow occured 
	if (top>=50){
		printk("[System Call] os2023_push: Stack overflow\n"); //use 'printk' instead of 'printf'

		return;
	}

	//if the stack  already has input value->do not push input
	for (i=0;i<top;i++){
		if(stack[i]==a){
			printk("[System Call] os2023_push:\n");
			print_stack();
			return;			
		}
	}

	//stack push
	stack[top++]=a;
	printk("[System Call] os2023_push:\n");
	print_stack();

	return;
}
	
SYSCALL_DEFINE0(os2023_pop){

	int output;

	//if stack Underflow occured
	if (top<=0){
		printk("[System Call] os2023_pop: Stack underflow\n");
	}

	//stack pop
	output=stack[--top];
	printk("[System Call] os2023_pop:\n");
	print_stack();

	return output;
}

void print_stack(void){
	
	int i;
	
	printk("Stack Top--------------\n");
	for (i=top-1;i>=0;i--){
		printk("%d\n",stack[i]);
	}                                     //print stack from top to bottom
	printk("Stack Bottom------------\n");
	
	return;
}
