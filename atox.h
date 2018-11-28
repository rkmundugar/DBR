#ifndef ATOX_H_
#define ATOX_H_

#include<stdlib.h>

inline unsigned short int atox(char *str, int size)
{
	unsigned short int temp[4],result =0x0000,hex_wt=1;
	int i,j;
	for(i=0;i<size;i++)
	{
		switch(str[i])
		{
			case 'A':
					if(str[i] == 'A')
						temp[i] = 10;
					break;
			case 'B':
					if(str[i] == 'B')
						temp[i] = 11;
					break;
			case 'C':
					if(str[i] == 'C')
						temp[i] = 12;
					break;
			case 'D':
					if(str[i] == 'D')
						temp[i] = 13;
					break;
			case 'E':
					if(str[i] == 'E')
						temp[i] = 14;
					break;
			case 'F':
					if(str[i] == 'F')
						temp[i] = 15;
					break;
			case '0':
					if(str[i] == '0')
						temp[i] = 0;
					break;
			case '1':
					if(str[i] == '1')
						temp[i] = 1;
					break;
			case '2':
					if(str[i] == '2')
						temp[i] = 2;
					break;
			case '3':
					if(str[i] == '3')
						temp[i] = 3;
					break;
			case '4':
					if(str[i] == '4')
						temp[i] = 4;
					break;
			case '5':
					if(str[i] == '5')
						temp[i] = 5;
					break;
			case '6':
					if(str[i] == '6')
						temp[i] = 6;
					break;
			case '7':
					if(str[i] == '7')
						temp[i] = 7;
					break;
			case '8':
					if(str[i] == '8')
						temp[i] = 8;
					break;
			case '9':
					if(str[i] == '9')
						temp[i] = 9;
					break;
			default:
					printf("Invalid ID\n");
					exit(0);
		}
	}
	for(i=0;i<size;i++)
	{
		hex_wt = 1;
		for(j=0;j<(size-1-i);j++)
			hex_wt = 16*hex_wt;
		result = temp[i]*hex_wt + result;
	}
	return result;
}

#endif 











