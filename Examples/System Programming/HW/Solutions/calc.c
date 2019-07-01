#include "stdio.h"
#include "stdlib.h"

int main(int ac, char* args[])
{
	if(ac != 4)
	{
		printf("USAGE:  %s OPERATOR OPERAND1 OPERAND2\n", args[0]);
	}
	else 
	{
		double op1 = atof(args[2]);
		double op2 = atof(args[3]);
		double answer;
		
		switch(args[1][0])
		{
			case '+':
				answer = op1 + op2;
			break;
			case '-':
				answer = op1 - op2;
			break;
			case '/':
				answer = op1 / op2;
			break;
		}
		
		printf("%f\n", answer);
	}
}
