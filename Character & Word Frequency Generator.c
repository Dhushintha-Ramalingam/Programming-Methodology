		/*
				E/17/064 Project 2 (25/06/2020)
		*/

/* c libraries */
#include <stdio.h>
#include <stdlib.h>		//malloc(), exit()		
#include <string.h>		//strcpy(), strcmp(), strlen()
#include <ctype.h>		//isalnum(), tolower(), isdigit()

/* character structure */
typedef struct _{
	char CH[2];
	int count ;
	struct _*next;
}llc_t;

/* string structure */
typedef struct __{
	char str[100];
	int count;
	struct __ * next;
}lls_t;

/* declaration & referencing struct type pointers */
llc_t *head1 = NULL, *current1;
lls_t *head = NULL, *current;

/* global variables */
int total = 0;
int elements=10, IsNum = 1;
float maxPercent = 0.0;
int maxCount, scaled;


/* function prototype */
void charFunc( char *file);
void charSort();
void wordFunc(char *file);
void wordSort();

char * charToStr(char ch);
char * prePros(char str[100]);

int maxLen(int );
void numCheck(char arr[]);
void maxFloat(int );

void printRow(int , char data[], int );
void printSpace(int );
void printBar(int );
void printBase(int );


int main(int argc, char **argv){
	/* local variables declaration & initialization */
	int countFlag = 0, i;
	char option ;
	char fileName[1000][50];
	int x = 0;
	
	/* loop for checking command line arguments and suitable output assignment */
	for(i = 1; i < argc; i++){
	
		/* error message for wrong use of '-' in an argument */
		if(argv[i][0]=='-'){
			if((strcmp(argv[i],"-l")!=0) && (strcmp(argv[i],"-c")!=0) && (strcmp(argv[i],"-w")!=0) && (strcmp(argv[i],"--scaled")!=0)){
				printf("Invalid option [%s]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n", argv[i] ,argv[0]);
				return 0;
			}
		}
	
		/* condition for '-l' argument */
		if(strcmp(argv[i],"-l")==0){
			/* if '-l' is the last element of the argument list*/
			if(i == argc - 1){
				printf("Not enough options for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
				return 0;
			}
			
			numCheck(argv[i+1]);		//function call to convert the next argument to number
				
			/* if next element of '-l' has non numeric characters */
			if((elements == 0) && (IsNum == 0)){
				printf("Invalid options for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
				return 0;
			}
			/* if next element of '-l' is negetive number */
			else if((elements < 0) && (IsNum == 1)){
				printf("Invalid option(negative) for [-l]\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
				return 0;
			}
			/* if next element of '-l' is '0' or '-0' */
			else if ((elements == 0) && (IsNum == 1)){
				return 0;
			}
			
		}
		
		/* condition for execute the program in character mode */
		if(strcmp(argv[i],"-c")==0){
			countFlag += 1;
			option = 'c';
		}
		
		/* condition for execute the program in word mode */
		if(strcmp(argv[i],"-w")==0){
			countFlag += 1;
			option = 'w';
		}
		
		/* default option to execute in word mode */
		if(countFlag == 0){
			option = 'w';
		}
		
		/* condition to print the chart in full scale*/
		if(strcmp(argv[i],"--scaled")==0){
			scaled = 1;
		}
		
		/* making text filename arguments array */
		if(argv[i][0] != '-'){
			if(strcmp(argv[i-1],"-l") != 0){
				strcpy(fileName[x] , argv[i]);
				x++;
			}
		}	
			
	}
	
	/* condition : no file arguments */
	if(x == 0){
		printf("No input files were given\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
		return 0;
	}
	
	/* condition : both '-c' , '-w' flags given */
	if(countFlag > 1){
		printf("[-c] and [-w] cannot use together\nusage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
		return 0;		
	}
	
	/* printing character bar chart */
	if(option == 'c'){
	
		//creating character linked list
		for(int i = 0; i < x; i++){
			charFunc(fileName[i]);	
		}
		//condition : Null linked list / only empty files 
		if(head1 == NULL){
			printf("No data to process\n");
			return 0;
		}
		
		charSort();		//sort the character linked list in decending order of count			
		maxCount = head1->count;	//count of the maximum occured character 
		printf("\n");
		maxFloat(elements);			//percentage of maximum occured character 
		
		//printing the bar chart
		int z = 0;
		for(current1 = head1; (z < elements) && (current1 != NULL) ; current1 = current1->next,z++){
			printRow(1,current1->CH,current1->count);
		}
		printBase(1);	
	}
	
	/* printing word bar chart */
	if(option == 'w'){
	
		//creating word linked list
		for(int i = 0; i < x; i++){
			wordFunc(fileName[i]);	
		}
		
		//condition : Null linked list / only empty files 
		if(head == NULL){
			printf("No data to process\n");
			return 0;
		}
		
		wordSort();		//sort the word linked list in decending order of count	
		maxCount = head->count;		//count of the maximum occured word
		int N = maxLen(elements);	//length of the maximum length word
		printf("\n");
		
		//printing the bar chart
		int y =0;
		for(current = head ; y < elements && current != NULL; current = current->next,y++){
			printRow(N,current->str,current->count);
		}
		printBase(N);
	}
				
	return 0;
}

/* function to take character from file and include in linked list  */
void charFunc( char *file){
	FILE *fp1;			//declare the file pointer
	char ch;
	fp1 = fopen(file, "r");							//open the file in read mode
	if(fp1 == NULL){								//check if the pointer is null
		printf("Cannot open one or more given files\n");
		exit(0);
	}
	
	//get the characters from file and preprocess and add to the list
	while(fscanf(fp1, "%c", &ch) != EOF){
		if(isalnum(ch)){							//condition only take number and alphabet
			ch = tolower(ch);						//convert to lower case character
			int Flag = 0;
			for(current1 = head1; current1 != NULL; current1= current1->next){
				if(strcmp(current1->CH,charToStr(ch))==0){
					current1->count += 1;			//increase the count of character according to the occurance in file
					Flag = 1;
					total +=1;						//total charcter count				
					break;
				}
			}
			if(Flag == 0){
				current1 = (llc_t *)malloc(sizeof(llc_t));		//dynamic memory allocation for current1 node
				strcpy(current1->CH , charToStr(ch));	//copy the character to the list
				current1->count = 1;					//assign count of the character to 1
				current1->next = head1;					//refer the next node to head1 
				head1 = current1;						//refer the head1 node to current1 (new head address)
				total += 1;								//total charcter count
			}
		}
	}
	
	fclose(fp1);			//closing the file
}

/* function to take words from file and include in linked list  */
void wordFunc(char *file){
	FILE *fp2;						//declare the file pointer
	char string[100];
	fp2 = fopen(file, "r");			//open the file in read mode
	if(fp2 == NULL){				//check if the pointer is null
		printf("Cannot open one or more given files\n");
		exit(0);
	}
	
	//get the words from file and preprocess and add to the list
	while(fscanf(fp2, "%s", string) != EOF){	
		int Flag = 0;
		for(current = head; current != NULL; current= current->next){
			if(strcmp(current->str,prePros(string))==0){
				current->count += 1;					//increase the count of word according to the occurance in file
				Flag = 1;
				total += 1;								//total word count
				break;
			}
		}
		
		if((Flag == 0) && (strlen(prePros(string))!= 0)){
			current = (lls_t *)malloc(sizeof(lls_t));	//dynamic memory allocation for current node
			strcpy(current->str , prePros(string));		//copy the word to the linked list
			current->count = 1;							//assign count of word to 1
			current->next = head;						//refer the next node to head1 
			head = current;								//refer the head1 node to current1 (new head address)
			total += 1;									//total word count
		}
	}
	fclose(fp2);			//closing the file
}

/* function to sort the linked list in decending order of the char occurence  */
void charSort(){
	llc_t *temp1, *pivot1;								//declare char struct type poiters
	pivot1 = (llc_t *)malloc(sizeof(llc_t));			//dynamic memory allocation
	
	for(current1 = head1; current1 != NULL; current1 = current1->next){
		temp1 = current1;			
		for(temp1 =current1; temp1 != NULL; temp1 = temp1->next){
			if(temp1->count >= current1->count){		//condition to update highest count character
				strcpy(pivot1->CH , current1->CH);		//copy the data to temprary pivot pointer
				pivot1->count = current1->count;
				
				strcpy(current1->CH , temp1->CH);		//change the current1 to temp1 node values
				current1->count = temp1->count;
				
				strcpy(temp1->CH , pivot1->CH);			//update the temp1 pointer values to pivot1 values
				temp1->count = pivot1->count; 
			}	
		}
		
	}
}

/* function to sort the linked list in decending order of the word occurence  */
void wordSort(){
	lls_t *temp, *pivot;								//declare word struct type pointers
	pivot = (lls_t *)malloc(sizeof(lls_t));				//dynamic memory allocation
	
	for(current = head; current != NULL; current = current->next){
		temp = current;
		for(temp =current; temp != NULL; temp = temp->next){
			if(temp->count >= current->count){			//condition to update highest count character
				strcpy(pivot->str , current->str);		//copy the data to temprary pivot pointer
				pivot->count = current->count;
				
				strcpy(current->str , temp->str);		//change the current to temp node values
				current->count = temp->count;
				
				strcpy(temp->str , pivot->str);			//update the temp pointer values to pivot values
				temp->count = pivot->count; 
			}	
		}
		
	}
}

/* function to change character to string : used in char linked list*/ 
char * charToStr(char ch){
	static char arr[2];
	arr[0] = ch;
	arr[1] = '\0';
	return arr;
}

/* function to return only alphanumeric lower case string */
char * prePros(char str[100]){
	static char proStr[100];
	int j=0;
	for(int i = 0; str[i] != '\0'; i++){
		if(isalnum(str[i])){				//condition to take only number and alphabet
			proStr[j] = tolower(str[i]);	//change to lower case
			j++;
		}
	}
	proStr[j]='\0';
	return proStr;
}

/* function to return the maximum length of the selected words from linked list*/
int maxLen(int elements){
	int i = 0;
	//loop to get the maximum length 
	int j = 0;
	for(current = head; (j < elements) && (current != NULL) ; current = current->next, j++){
		if (i <= strlen(current->str)){
			i = strlen(current->str);		//update maximum length
		}
		float val = (float)current->count/total*100.0; 
		if(val >= maxPercent){
			maxPercent = val;				//update the global variable maximum percentage of selected list of words
		}
	}
	return i;
}

/* function to return the maximum percentage of one charcter in selected list of charcters */
void maxFloat(int elements){
	int j = 0;
	for(current1 = head1; (j < elements) && (current1 != NULL); current1 = current1->next){
		float val = (float)current1->count/total*100.0; 
		if(val >= maxPercent){				//condition to update global variable maximum percentage 
			maxPercent = val;				//percentage of maximum occured character of selected list of chars
		}
	}
}

/* function to change string to number */
void numCheck(char arr[]){
	int num = 0, i, j = 0,sign = 1;
	if (arr[0] == '-'){
		sign = -1;						//condition for negative number
		j = 1;
	} 
	
	for(i = j; arr[i] != '\0' ; i++){
		 if(isdigit(arr[i])==0){
		 	IsNum = 0;					//update global variable that string is not a number
		 	sign = 0;					//condition to non digit string 
		 	break;
		 }
		 num = (num + arr[i]-'0')*10; 
	}
	elements = num*sign/10;				//update the global variable elements from 10 and return the user input	
}

/* function to print one bar of the bar chart */
void printRow(int maxWlen, char data[], int count){
	float percentage = (float)count * 100 / (float)total;		//percentage of the char or word
	int frameLen;
	int m,n;													//local variables
	
	//defining the float number length												
	if(maxPercent < 10){				//condition : maximum percentage is less than 10.00% 
		m = 5;
	}
	else if(maxPercent < 100){			//condition : maximum percentage is grater than 9.99% and less than 99.99%
		m = 6;
	}
	else if(maxPercent == 100){			//condition : maximum percentage is 100.00%
		m = 7;
	}
	
	frameLen = 77 - m - maxWlen;		//define the length of the frame 
	
	if (scaled==1){						//condition to print the bar in full scale in frame
		n = count * frameLen / maxCount;
	}
	else{
		n = count * frameLen / total;
	}
	
	/* printing the pattern */
	printSpace(maxWlen + 2);
	printBar(n);
	printf("\n %s ", data);
	printSpace(maxWlen - strlen(data));		
	printBar(n);
	printf("%.2f%%\n", percentage);		//printing corrosponding percentage of char or word
	printSpace(maxWlen + 2);
	printBar(n);
	printf("\n");
	printSpace(maxWlen + 2);
	printf("\u2502\n");
}

/* function to print the spaces in y axis label*/
void printSpace(int nofSpaces){
	for(int i = 0; i < nofSpaces; i++){
		printf(" ");
	}
}

/* function to print the bar */
void printBar(int n){
	printf("\u2502");					//print vertical line
	for (int i = 0; i < n; i++){
		printf("\u2591");				//print horizontal bar
	}
}

/* function to print the base*/ 
void printBase(int maxlenword){
	printSpace(maxlenword + 2);
	printf("\u2514");					//print axis origin
	int baselen = 80 - 2 - 1 - maxlenword;		//define the x axis length
	for (int i = 0; i < baselen; i++){
		printf("\u2500");				//print horizontal line
	}
	printf("\n");
}


