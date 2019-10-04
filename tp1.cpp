#include<stdio>
int main(){
int y=1;
int s=0;
int x=0;
while(x<y){
	x=x+3;
	y=y+2;
	if (x+y<10)
		s=s+x+y;
	else
		s=s+x-y;
}
}