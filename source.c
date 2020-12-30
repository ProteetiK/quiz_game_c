#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//all details pertaining to the current player will be handled through this structure
struct player
{
	char name[100];
	int score;
	int high_score;
	
} P, temp;
//informations about this version of the game
void about()
{
	printf("\nABOUT THE GAME\n\nThis is a merit-based quiz. 4 cherries are awarded for every correct answer and 1 cherry is deducted for every wrong answer. This version of the game has been developed by Proteeti Kushari.\n");
}
//help regarding playing the game
void help()
{
	printf("\nHELP\n\nEnter choice as per menu, and then enter only the option letter for each question.\nthere is only 1 correct option.\nYour end score will be displayed at the end of each game, and the highest score will be saved for you whenever you play the game.\n");
}
//sets current game score of player to zero, this is invoked before every game
void reset_score()
{
	P.score=0;
}
//resets the highest score of the current player to 0, invoked only via the menu
void reset_high_score ()
{
	P.high_score=0;
}
//shows the name and score details of current player
void show_record()
{
	printf("\nHi %s!\nYour highest score is: %d cherries", P.name, P.high_score);
}
//shows score in the last game played in current session and the all-time high score
void show_score()
{
	printf("\nScore: %d cherries\nHigh Score: %d cherries", P.score, P.high_score);
}
//presents the menu and details the options to be chosen by player
void menu()
{
	printf("\n\n\nMENU\n\n1. New Game\n2. View Scores\n3. Reset High Score\n4. View Profile \n5. Help\n6. About the game\n7. Exit\n\nEnter choice: ");
}
void sub_menu ()
{
	printf("\n\n\nSUBJECT MENU\n\n1. Maths\n2. History\n3. Geography\n4. Science \n5. Main Menu\n\nEnter choice: ");
}
//updates current high score to the log file
void update_record()
{
	FILE *fp, *f;
    char c;
    //first we copy the contents of log file to a temp file
    fp = fopen("players.txt", "r");
    f = fopen ("temp.txt", "w");
    c=fgetc(fp);
    while (c!=EOF)
    {
    	fputc(c, f);
      	c=fgetc(fp);
    }
    fclose(fp);
    fclose(f);
    //we copy back the contents from temp file to the log file until we reach the current player
    //for the current player we simply insert the latest details from the players structure
    fp = fopen("players.txt", "w");
    f = fopen ("temp.txt", "r");
    while (!feof(f))
    {
        fscanf(f, "%s", temp.name);
        fscanf(f, "%d\n", &temp.high_score);
        if (strcmp(P.name, temp.name)==0)
            temp.high_score=P.high_score;
        fputs(temp.name, fp);
		fputs(" \0", fp);
		fprintf(fp, "%d", temp.high_score);
		fputs("\n\0", fp);
    }
    fclose(fp);
    fclose (f);
}
//heart of the program
//controls the quiz game
void game()
{
	reset_score();
	char q_buf[500], ans_user, ans, hold, op1[100], op2[100];
	int j, ch;
	FILE *q, *a;
	while (ch!=5)
	{
		sub_menu();
		scanf("%d", &ch);
		switch(ch)
		{
			case 1:
				q=fopen("questions_m.txt", "r");
				a=fopen("answers_m.txt", "r");
				break;
			case 2:
				q=fopen("questions_h.txt", "r");
				a=fopen("answers_h.txt", "r");
				break;
			case 3:
				q=fopen("questions_g.txt", "r");
				a=fopen("answers_g.txt", "r");
				break;
			case 4:
				q=fopen("questions_s.txt", "r");
				a=fopen("answers_s.txt", "r");
				break;
			case 5:
				return;
			default:
				printf("\nInvalid choice.");
				return;
		}
	//we read 3 lines from questions file containing the question and the 2 lines of options
	//then we read one character from the answers file and match with the user-entered character
	//we update the score likewise
	for (j=0; j<10; j++)
	{
		fgets(q_buf, 500, q);
		fgets(op1, 100, q);
		fgets(op2, 100, q);
		printf("\n%s%s%s\nAns: ", q_buf, op1, op2);
		scanf(" %c", &ans_user);
		ans=fgetc(a);
		if (ans==ans_user)
		{
			
			printf("\nCorrect answer!\n+4 cherries!\n");
			P.score+=4;
		}
		else
		{
			printf("\nWrong answer!\nCorrect Ans: %c\n-1 cherry!\n", ans);
			P.score-=1;
		}
		printf("*******************");
	}
	fclose(a);
	fclose(q);
	//we print the current score and high score after updating the high score if necessary
	printf("\nCongratulations!\n");
	P.high_score+=P.score;
	show_score();
	//we update the details of this player in the log file
	update_record();
	}
}
void main ()
{
	char buffer[100];
	int sflag=0, choice=-1;
	//we take the name of the current player as input and reset the current score for them
	printf("\nHello! Welcome to Quiz Game!\nEnter player name: ");
	scanf("%[^\n]%*c", P.name);
	reset_score(P);
	//we check if this player is an old player or a new one
	//we return the last high score of the player if he/she is an old one
	//otherwise we initialize the high score to zero and enter the records of the new player into the log
	FILE *old_players=fopen("players.txt", "r");
	while(!feof(old_players))
	{
		fscanf(old_players, "%s", buffer);
		if (strcmp(P.name, buffer)==0)
		{
			flag=1;
			fscanf(old_players, "%d", &P.high_score);
			printf("\nWelcome back %s!\nYour highest score is: %d", P.name, P.high_score);
			break;
		}
	}	
	fclose(old_players);
	if (flag==0)
	{
		FILE *new_player=fopen("players.txt", "a");
		printf("\nWelcome %s!\nStart a new game today!", P.name);
		reset_high_score();
		fputs(P.name, new_player);
		fputs(" \0", new_player);
		fprintf(new_player, "%d", P.high_score);
		fputs("\n\0", new_player);
		fclose(new_player);
	}
	//we present the choice of options to the player and keep iterating until the user escapes
	while (choice!=7)
	{
		menu();
		scanf("%d", &choice);
		switch(choice)
		{
			case 1:
				game();
				break;
			case 2:
				show_score();
				break;
			case 3:
				reset_high_score();
				break;
			case 4:
				show_record();
				break;
			case 5:
				help();
				break;
			case 6:
				about();
				break;
			case 7:
				exit(0);
			default:
				printf("\nInvalid choice entered.\n");
		}
	}
}
