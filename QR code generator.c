/*
	QR Code Generator
	E/17/064
*/

#include <stdio.h>		// standard library
#include <stdlib.h>		// library for system clear function
#include <string.h>		// library for string length & string comparison function
#include <ctype.h>		// libratry for change characters to lower case function

#define BLACK    40		
#define RED      41		//defining ASCII color codes
#define GREEN    42		
#define YELLOW   43		
#define BLUE     44
#define MAGENTA  45
#define CYAN     46
#define WHITE    47

int i, j, k, n , l , count; 		//declaration of global variables used in program
int s=0, t=0;
char arr[120];
int Arr[132];
int hash[132];
int bi[9];
int encode[132][9];
char input[119];
char test;

char *line1 = "Invalid argument for color.";
char *line2 = "usage : ";
char *line3 = " -c [black|red|green|yellow|blue|magenta|cyan]";
char *line4 = " -h for the help about the program";
char *line5 = "Incorrect usage of arguments.";

//function prototypes
void hashArray(char arr[]);
void binVal(int deci);
void encode2Dbin(int hash[]);
void lowerStr(int argc, char argv[],char lower[]);
void setCurser(int x, int y);
void setColor(char lower[]);
void qrUnit(char col[]);
void genSquare(int x, int y, int length, char lower[]);
void PIS(int x,int y, int length,char lower[]);
void qrWriter(int x,int y, int col, int row, char lower[],int  encode[132][9]);
void codeVersion(int length, char lower[], int encode[132][9]);
int correctInput(char lower[]);


//QR code generate main function
int main (int argc, char **argv){
	
	char lower[9];
	lowerStr(argc,argv[2],lower);				// change the commandline color input into lower case
	
	//condition to print QR code
	if((argc == 1) || ((argc == 3) && (strcmp(argv[1],"-c") == 0) && (correctInput(lower) == 0))){	
		printf("Enter the URL : ");			
		
		int value = 0;
		
		for (int count = 0; value == 0 ; count++){
			test = getchar();			//take input character for testing
			if (test != '\n' && test != ' '){	//condition to reject space and new line before character entered
				scanf("%s", input);		//take the string from the second letter
				value = strlen(input);		
			}
		}
		
		arr[0] = test;					//type cast of first character to string as arr[0] index value
		strcat(arr,input);				//concatanation of arr[0] and input array "returns the arr array"
				
		n = strlen(arr);
		
		if (n <= 3) {printf("String is too small. Not supported by available QR versions\n");}		//URL length less than or equal 3

		else if( n >= 120) {printf("String is too long. Not supported by available QR versions\n");}     //URL length greater than or equal 120


		
		//conditions for the output of given  input URL according to length with color
		else if (argc == 3){
			system("clear");
			
			if(( n > 3 ) && ( n <= 20 )){
				codeVersion(20,lower, encode);		//URL length between 4 and 20		
			}
			else if ((n > 20) && (n < 120)){
				codeVersion(38,lower, encode);		//URL length between 21 and 119
			}
		}

		//condition for the output of given input URL using default black color
		else if(argc == 1){
			system("clear");
			// 'NO' color argument
			if((n > 3) && (n <= 20)){
				codeVersion(20,"black",encode);
			}

			else if ((n > 20) && (n < 120)){
				codeVersion(38,"black",encode);
			}
		}
	}
	
	//condition for wrong colour argument
	else if((argc == 3) && (strcmp(argv[1],"-c") == 0) && (correctInput(lower) == 1)){
		printf("%s\n%s\n%s%s\n%s%s\n",line1,line2,argv[0],line3,argv[0],line4);
	}

	//condition for print help message
	else if((argc == 2) && (strcmp(argv[1],"-h") == 0)){
		printf("%s\n%s%s\n%s%s\n",line2,argv[0],line3,argv[0],line4);
	}

	//condition for different from above conditions 
	else {
		printf("%s\n%s\n%s%s\n%s%s\n",line5,line2,argv[0],line3,argv[0],line4);
	}
	
	return 0;
}


//function to check the colour argument
int correctInput(char lower[]){
	if((strcmp(lower,"black")==0)||(strcmp(lower,"red")==0)||(strcmp(lower,"green")==0)||(strcmp(lower,"yellow")==0)||
  		(strcmp(lower,"blue")==0)||(strcmp(lower, "magenta")==0)||(strcmp(lower,"cyan")==0) )	{
	
		return 0;			//condition to return zero for correct color argument input
	
	}else{
		return 1;			//condition to return one to wrong color argument input
	}
}

//function to print the QR code 
void codeVersion(int length, char lower[],int encode[132][9]){

	if(length == 20){					//condition to print small version of QR
		PIS(2,2,6,lower);
		PIS(14,2,6 , lower);
		PIS(2,14,6,lower);				//printing position identification squares 
		hashArray(arr);					//convert hashed ASCII array
		encode2Dbin(hash); 				//convert 2D binary array
		qrWriter(8,8,4,4,lower,encode);
		qrWriter(8,2,2,2,lower,encode);
		qrWriter(2,8,2,2,lower,encode);			//printing encoded pattern of characters
		genSquare(1,1,20,"white");			//printing white border
		printf("\e[0m");	
		printf("\n");}

	else if(length == 38){					//condition to print large version of QR
		PIS(2,2,6,lower);
		PIS(32,2,6 , lower);
		PIS(2,32,6,lower);				//printing position identification squares
		hashArray(arr);					//convert hashed ASCII array
		encode2Dbin(hash);				//convert 2D binary array
		qrWriter(8,8,10,10,lower,encode);	
		qrWriter(8,2,8 ,2,lower,encode);
		qrWriter(2,8,2,8,lower,encode);			//printing encoded pattern of characters
		genSquare(1,1,38,"white");			//printing white border
		printf("\e[0m");
		printf("\n");}
}


//function to get hash string ASCII value  array
void hashArray(char arr[]){
	n = strlen(arr);					//length of the input URL
	for(j=0; (arr[j] != '\0') ; j++){
		Arr[j+1]  = (int) arr[j];
		hash[j+1] = Arr [j+1];				//1 to n index hashed string ASCII int value
	}

	hash [0] = 50 + j;					//0th index value of hased string

	if((3 < n)&&(n<=20))  {l=23;}				//condition for short URL loop
	if((20<n)&& (n<120))  {l=131;}				//condition for long URL loop

	for(i=0; i<(l-j); i++){
		Arr[i+j+1] = Arr[i+1] + 1;
		hash[l-i] = Arr[i+j+1];				//(n+1)th index to (l)th index hased string int values
	}

	hash[l+1]='\0';						//end of array
}

//function to get the binary of decimal in 9 bits
int bi[9];							//declare array size
void binVal(int deci){
	for(k=8; k>=0; k=k-1){
		bi[k] = deci % 2 ;				//moduler division return 1 | 0
		deci = deci / 2 ;				//integer division return quotient
	}
	bi[9] = '\0';						
}


//function to make 9 bit binary value array of hash strings 
int encode[132][9];						//declare 2D array size
void encode2Dbin(int hash[]){
	for(i=0;hash[i]!='\0'; i++ ){
		binVal(hash[i]);				//return 9 bit binary value of integer
		for(k=0; k<9; k++){
			encode[i][k] = bi[k];			//(i)th row (k)th column value of encode array
		}
	}
}

//function to lower case the color argument
void lowerStr(int argc, char argv[], char lower[]){
	if(argc==3){						//condition to make lower case
	long int a = strlen(argv);				//length of argv array
	for (i=0; i < a ; i++){
		lower[i] = tolower(argv[i]);			//make each character into lower case & assign to new array "lower"
		//printf("%c\n",lower[i]);
	}
	lower[a] = '\0';
	}

}

//function to set the curser position on console
void setCurser(int x, int y){
	printf("\033[%d;%dH", y+1, x+1);			//set the cursor point on screen by adding 1 to x, y (if 0,0 >>> 1, 1)
}

//function to set the background color of screen according to colour input
void setColor(char lower[]){
	if (strcmp(lower,"black")==0){
		printf("\e[%im", BLACK);			//condition to set the bg color to black
	}
	else if (strcmp(lower,"red")==0){
		printf("\e[%im", RED);				//condition to set the bg color to red
	}
	else if (strcmp(lower,"green")==0){
		printf("\e[%im", GREEN);			//  "       "  "    "  "       "   green
	}
	else if (strcmp(lower, "yellow")==0){				
		printf("\e[%im", YELLOW);			//  "       "  "    "  "       "   yellow
	}
	else if (strcmp(lower,"blue")==0){
		printf("\e[%im", BLUE);				//  "       "  "    "  "       "   blue
	}
	else if (strcmp(lower,"magenta")==0){
		printf("\e[%im", MAGENTA);			//  "       "  "    "  "       "   magenta
	}
	else if (strcmp(lower,"cyan")==0){
		printf("\e[%im",CYAN);				//  "       "  "    "  "       "   cyan
	}
	else if (strcmp(lower,"white")==0){
		printf("\e[%im", WHITE);			//  "       "  "    "  "       "   white
	}
	else {printf("\e[%im", BLACK);}				// condition for set bg to black as default (no color argument)

}

//function to print one QR unit
void qrUnit(char lower[]){
	setColor(lower);					//set the bg color
	printf("  ");						//print one QR unit square by two space
	printf("\e[%im", WHITE);				//reset bg to white
}

//function to print square border in given length
void genSquare(int x, int y, int length, char lower[]){

	x = x-1;						//reset x to keep the curser position to given initial x
	y = y-1;						// "    y   "           "                    "         y

	for (int h =2*x ; h < 2*x + (length*2)-2; h= h+2 ){
		setCurser(h,y);					//horzontal top line
		qrUnit(lower);					//print unit square in given lower color
		setCurser(h,y+length-1);			//horizontal bottom line
		qrUnit(lower);					//print unit square in given lower color
	}

	for (int v = y; v < y+ length; v=v+1){
		setCurser(2*x, v);				//vertical left line
		qrUnit(lower);					//print unit square in given lower color
		setCurser(2*x+(length*2)-2 ,v);			//vertical right line
		qrUnit(lower);					//print unit square in given lower color
	}
	printf("\n");						//printing new line
}

//function to print position identification squares
void PIS(int x, int y, int length, char lower[]) {
	genSquare( x,  y,  length ,  lower);			//printing middle squares of PIS in given lower color
	x=x+1; y=y+1;length = length-2;				//redefine x, y, length of the square to be printed
	genSquare( x , y, length ,"white");			//printing white void square border
	x=x+1; y=y+1;length = length-2;				//redefine x,y, length of the square to be printed
	genSquare( x ,  y,  length,  lower);			//printing outer border of PIS square in given lower color
}

//function to print the encode 2D array of input URL
void qrWriter(int x,int y, int col, int row, char lower[], int encode[132][9]){
	int r, c, p, q;						//declare local variable for count row , column, 3*3 square row, 3*3 square column respectively
	int resetX = x;						//declare variable resetX to the x value to reuse in the loop
	//int resetY = y;						//declare variable resetY to the y value to reuse in the loop

	for(r=0; r < row ; r++){
		for(c=0; c < col ; c++){
			for(p=0; p < 3; p++){
				for(q=0; q < 3; q++){
					if((encode[s][t]==1)){				//condition to  print unit square in  color of lower 
					       	genSquare(x+q, y+p, 1, lower);    
					}
					else { genSquare(x+q, y+p, 1,"white"); 		//condition to print white unit square
				       	} t++;						//increase column index encode 2D array
				}
			
			}			
			x = x + 3;							//move to next 3*3 square in a row
		       	s++; t=0;							//increase row index and reset column index to zero of encode 2D array
		}
		x = resetX;								//rest the x value to the initial x value
		y=y+3;									//move to next row
	} 
}

