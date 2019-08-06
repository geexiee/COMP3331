#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char *argv[])
{
	char ch,ans;
	int i;
	int zz[26];
	double sum=0;
	double percent;
	for(i = 0;i<26;i++){
			zz[i]=0;
	}
	while( (ch = getchar()) != EOF){
		for(i = 0;i<26;i++){
			if(ch-97 == i)
				zz[i]++;
		}


	}
	for(i = 0;i<26;i++){
		sum += zz[i];
	}
	printf("%f\n",sum);
	for(i = 0;i<26;i++){
		percent = zz[i]/sum;
		ans = i+97;
		printf("'%c' %lf %d\n",ans,percent,zz[i]);
	}
	return 0;
}

