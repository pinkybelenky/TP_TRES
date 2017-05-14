#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	FILE *pf;
	if ((pf = fopen("pruebaDB.dat","rb"))==NULL)
	{
		printf("%s\n", "sssss");
		return 1;
	}
	return 0;
}