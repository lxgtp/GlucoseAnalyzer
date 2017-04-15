#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define NUMDAYS 30 /* Number of days of data to pull and compute */

void openFile(char *inputFile,char *outPut,char *settingsFile); /* opens the files */
void getSettings(); /* idea for importing settings from an extra .txt file for various outputs */
void importLines(); /* calls parseLine, getLine */
char *getLine (FILE *fp); /* gets line from file */
void parseLine(char*line, int index); /* parse the lines from the input file to tokens and get dates and data */
void printData(); /* loops through dates and data and prints to output stream */
void mallocData(); /* mallocs the arrays dates and data */
void calcData(); /* calls converToInt, computeAverage, findMax, findMin */
void convertToInt(); /* convert the data to ints for dataVals*/
double computeAverage(); /* computes the average in dataVals */
int findMax(); /* find the max in dataVals */
int findMin(); /* find the min in dataVals */
void outputData(char *outputFile); /* outputs the data to a file passed in at argv[2] */
void sendEmail(); /* not working - but should send email of data */
void createPlotFile(); /* creates a file to be used for plotting purposes */

FILE *fp; /* file pointer with argv[1] */
FILE *settings; /* settings file */
char *outputFile; /* file pointer with argv[2] if passed in */
char *dates[31]; /* array of strings that stores the datas from the input file */
char *data[31]; /* array of strings that stores the data/reading from the input file */
int dataVals[30]; /* array of the data but converted to int */
double average; /* the avrg of the dataVals*/
int max; /* max of dataVals*/
int min; /* min of dataVals */
int NUMDAYS;

int main (int argc, char* argv[]){
    
    openFile(argv[1],argv[2],argv[3]);
    
    getSettings();
    
    importLines();
    
    calcData();
    
    // printData();
    
    outputData(outputFile);

    createPlotFile();
    
    // sendEmail();
    
    printf("End Main, exiting....\n");
    
    exit(EXIT_SUCCESS);
    
}//end Main

/* opens the files passed into argv[] */
void openFile(char *inputFile,char *outPut,char *settingsFile){
    
    /* open the file passed into at the start of the program
   currently only looks for argv[1] */
    settings = fopen(settingsFile,"r");
    if(settings==NULL){
        printf("Settings File Not Found...\n");
        exit(EXIT_FAILURE);
    }
    
    fp = fopen(inputFile,"r");
    if(fp==NULL){
        exit(EXIT_FAILURE);
    }
    else{
        printf("File found: %s\n",inputFile);
        printf("Pulling and computing data...\n");
    }
    
    mallocData();

    
/* get name of ouputFile from argv[2]*/   
    if(outPut==NULL){
        printf("No ouptput file given..\n");
        exit(EXIT_FAILURE);
    }
    else{
        outputFile = outPut;
    }
}//end openFile

void getSettings(){
    int numDays;
    fscanf(settings,"%d",&numDays);
    NUMDAYS=numDays;
    printf("Settings File Found numDays: %d\n",numDays);
}

/* does some loops and calls getLine and parseLine to import the data of the file */
void importLines(){

/* get lines for correct amount of desired readings */
    fseek(fp,393,SEEK_SET);//seek to first needed line of dates and data
    char c;
    int x=0;
    char *test[30];
    while(x<30){
        test[x]=malloc(sizeof(char)*26);
        test[x]=getLine(fp);
        x++;
    }
    
/* parse the lines for dates and data */
    x=0;
    int a=0;
    while(x<30){
        parseLine(test[a],a);
        a++;
        x++;
    }
}//end importLines

/* reads a line from the file of 100 chars long just to make sure you reach end of line returns this line to caller */
char *getLine (FILE *fp){
    char *line;
    line = malloc(sizeof(char)*26);
    fgets(line,45,fp);
    //fseek(fp,1,SEEK_CUR);
    //printf("Line Read: %s\n",line);
    return line;
}//end getLine

/* parse the line looking for the token ';' returns this new char to caller */
void parseLine(char*line, int index){
    char*tmp=strtok(line,";");
    dates[index]=tmp;
    tmp=strtok(NULL,";");
    tmp=strtok(NULL,";");
    data[index]=tmp;
    
}//end parseLine

/*call this if you wish to quickly print out the info stored in dates and data
    only prints to output stream not a file */
void printData(){
    int cnt=0;
    printf("Printing data...\n");
    while(cnt<30){
        printf("%d: %s",cnt+1,dates[cnt]);
        printf(" Reading: %s",data[cnt]);
        printf(" DataVal: %d\n",dataVals[cnt]);
        cnt++;
    }
    printf("Average reading for last %d days was: %f\nMinimum reading for last %d days was: %d\nMaximum reading for last %d days was: %d\n", NUMDAYS, average, NUMDAYS, min, NUMDAYS, max);
}//end printData

/* ensures there is proper space for all the data put into these arrays */
void mallocData(){
    int cnt=0;
    while(cnt<30){
        dates[cnt]=malloc(sizeof(char)*10);
        data[cnt]=malloc(sizeof(char)*5);
//	dataVals[cnt] = malloc(sizeof(int)*3);
        cnt++;
    }
}//end mallocData

/*calls methods to calc the data in dataVals just used to clean up the main*/
void calcData(){
    convertToInt();
    average = computeAverage();
    min = findMin();
    max = findMax();
}

/* Converts data string array to double array */
void convertToInt(){
	int i=0;
	for(i; i<NUMDAYS; i++){
		dataVals[i] = atoi(data[i]);
	}
}//end convertToInt

/* computes the average of dataVals, aka the average of the readings */
double computeAverage(){
	int i=0;
	double sum=0.0;
	for(i; i<NUMDAYS; i++){
		sum += dataVals[i];
	}
	return sum/NUMDAYS;
}//end computeAverage

/* finds the highest data value in dataVals aka, highest reading */
int findMax(){
	max = dataVals[0];
	int i=1;
	for(i; i<NUMDAYS; i++){
		if(dataVals[i]>max){
			max = dataVals[i];
		}
	}
	return max;
}//end findMax

/* finds min data value in dataVals aka, lowest reading */
int findMin(){
	min = dataVals[0];
	int i=1;
	for(i; i<NUMDAYS; i++){
		if(dataVals[i]<min){
			min = dataVals[i];
		}
	}
	return min;
}//end findMin

/* output the data to the txt file passed in after the .csv (argv[2])
    assuming this is not empty */
void outputData(char *outputFile){
    
    FILE *tmp;
    tmp = fopen(outputFile,"w");
    if(tmp==NULL){
        exit(EXIT_FAILURE);
    }
    else{
        printf("Output File Opened, writing data...\n");
    }
    fputs("Glucose Meter Reading Report: Your Last 30 Days\n\n", tmp);
    fputs("Your Glucose Levels Were:\n\n", tmp);
    int x = 30;
    while(x>0){
	switch(x){
		case 30:
			fputs("Days 1 - 5\t\t", tmp);
			break;
		case 25:
			fputs("Days 6 - 10\t\t", tmp);
			break;
		case 20:
			fputs("Days 11 - 15\t", tmp);
			break;
		case 15:
			fputs("Days 16 - 20\t", tmp);
			break;
		case 10:
			fputs("Days 21 - 25\t", tmp);
			break;
		case 5:
			fputs("Days 26 - 30\t", tmp);
			break;
	}
        fputs("Date: ",tmp);
        fputs(dates[x-1],tmp);
        fputs(" -- ",tmp);
        fputs(data[x-1],tmp);
        fputs(" mg/dl",tmp);
	if((x-1)%5==0){
        	fputs("\n",tmp);
	}else{
		fputs("\t", tmp);
	}
        x--;
    }
	
	fputs("\nYour Average Glucose Level Was: ", tmp);
	fprintf(tmp, "%f\n", average);
	fputs("\nYour Lowest Glucose Level Was: ", tmp);
	fprintf(tmp, "%d\n", min);
	fputs("\nYour Highest Glucose Level Was: ", tmp);
	fprintf(tmp, "%d", max);

    
    printf("Finished, closing file.\n");
    
    //fclose(tmp); /*if you clsoe the file you get an error, trying to work this out */
    
}//end outputData

/* suppose to send data to email */
void sendEmail(){
        char cmd[100];  // to hold the command.
        char to[] = "tug14646@temple.edu"; // email id of the recepient.
        char body[] = "This is a test email sent from a BeagleBone Black.";    // email body.
        char tempFile[100];     // name of tempfile.

        strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

        FILE *fp = fopen(tempFile,"w"); // open it for writing.
        fprintf(fp,"%s\n",body);        // write body to it.
//        fclose(fp);             // close it.

        sprintf(cmd,"sendmail %s < %s",to,tempFile); // prepare command.
        system(cmd);     // execute it.


}//end sendEmail

/* creates a file that is formatted to use with plotter */
void createPlotFile(){
	FILE *fp = fopen("plotdata.dat", "w");
	int i=0;
	for (i; i<NUMDAYS; i++){
		fprintf(fp, "%d", i+1);
		fputs("\t", fp);
		fputs(data[i], fp);
		fputs("\n", fp);
	}
}
