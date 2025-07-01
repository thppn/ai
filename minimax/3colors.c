#include<stdio.h>
#include<unistd.h>
#include <sys/time.h>
#define K1 14
#define K2 13
#define K3 14 
#define M 15
#define end_val 3*M

#define MIN -end_val
#define MAX end_val

typedef struct
{
	int red,
		green,
		yellow,
		turn;
} game;

typedef struct
{
	int choice,
	    choice_val;
} move;

int end(game a)
{
	if(a.red || a.green || a.yellow) return 0;
	return 1;
}
void print_(game a)
{
	int i;
	printf("\n==========");
	printf("\nRed: ");
	for(i = 0;i < a.red;i++) printf("O");
	printf("\nGreen: ");
	for(i = 0;i < a.green;i++) printf("O");
	printf("\nYellow: ");
	for(i = 0;i < a.yellow;i++) printf("O");
	printf("\n==========\n");
}	
int play(int choice,game* a)
{
	switch(choice)								
	{
		case 1:
			if(a->red < 1) return 0;
			a->red --;
			break;
		case 2:
			if(a->green < 1) return 0;
			a->green --;
			break;
		case 3:
			if(a->yellow < 1) return 0;
			a->yellow --;
			break;
		case 4:
			if(a->red < K1) return 0;
			a->red -= K1;
			break;
		case 5:
			if(a->green < K2) return 0;
			a->green -= K2;
			break;
		case 6:
			if(a->yellow < K3) return 0;
			a->yellow -= K3;
			break;
		default:									
			return 0;
		}
		return 1;														
}

	
move minimax(game now, int depth,int alpha,int beta)
{
	move best_move;
	
	if(end(now))
	{
		if(depth%2) best_move.choice_val = end_val-depth;
		else best_move.choice_val = depth-end_val;
		//printf("\nval is %d\n",best_move.choice_val);
		return best_move;
	}
	
	int choice;
	
	move next_move;
	
	int max = MIN,
		min = MAX;
		
	for(choice = 7;choice > 1;choice--)
	{
		game next;
		next = now;
		
		if(!play(choice,&next)) continue;
 
		//printf("\nNext choice %d\n",choice);
		//printf("%s\n",depth%2?"Min":"Max");
		
		next_move = minimax(next,depth+1,alpha,beta);
		next_move.choice = choice;
		
		if(depth%2) //minimizer
		{
			if(next_move.choice_val < min)
			{
				min = next_move.choice_val;
				best_move = next_move;	
			}
			if(beta > min) beta = min;
		}
		else //maximizer
		{
			if(next_move.choice_val > max)
			{
				max = next_move.choice_val;
				best_move = next_move;
			}
			if(alpha < max) alpha = max;
		}
		if(beta <= alpha) break;
	}
	
	best_move.choice_val = depth%2?min:max;
	//printf("\nFor %s best choice %d\n",depth%2?"Min":"Max",best_move.choice);
	return best_move;
}
void computer(game* a)
{
	struct timeval t1, t2;
	gettimeofday(&t1,NULL);
	move m = minimax(*a,0,MIN,MAX);
	gettimeofday(&t2,NULL);
	timersub(&t2,&t1,&t2);
	play(m.choice,a);
	printf("\nComputer: %d",m.choice);
	printf("\nElapsed time: %ld",t2.tv_sec);
}
void human(game* a)
{
	int choice;
	do{
		printf("\nChoose to remove:");
		printf("\n1. 1 red\n2. 1 green\n3. 1 yellow\n4. %i red\n5. %i green\n6. %i yellow\nMe: ",K1,K2,K3);
		scanf(" %d",&choice);
	}
	while(!play(choice,a));
}

int main(){
	printf("Welcome to the game");
	
	game a;
	a.turn = 0;
	a.red = a.green = a.yellow = M;	
	
	while(!end(a))
	{
		printf("\n\nRound %d",a.turn+1);
		if(a.turn%2==0) computer(&a);
		else
		{
			print_(a);
			human(&a);
		}
		a.turn++;
	}
	printf("\n%s!\n",a.turn%2==0?"You W O N":"You L O S T");
}
