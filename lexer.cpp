// --------------------------------------- //
//				Asssignment 1              //
//		Tokens and Lexemes with FSM        //
// Created by Chang (Gavin) Shu on 9/11/19 //
//                                         //
//				Team Members:              //
//				David Huang                //
//				Kevin Peralta              //
// --------------------------------------- //



/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Define Libraries                                                                                                                            */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fstream>
#include <regex>
#include <map>
#include <queue>
#include "Header.h"

using namespace std;


/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Function Definitions                                                                                                                        */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

void lexer(string);						/* Main operating function which includes a FSM to determine tokens and lexemes */
int Keyword(char iden[]);				/* Function to determine keyword or identifier ---------------------------------*/
int OpOrSep(char ch);					/* Function to determine if the current character is an operator or a separator */
string readInFile();					/* Function to read an input file and output the text into strings -------------*/
string Extract(string line);			/* Function to extract and remove comment blocks from the input string ---------*/
void writeFile(string, string);			/* Function to output results into a text file ---------------------------------*/
string outfileName;						/* Global variable for output file name ----------------------------------------*/
char decision;							/* Global variable for save text file decision ---------------------------------*/

queue<Pair> tkn;

/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/*                                                   FUNCTIONS AND CODES START HERE                                                            */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/


/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Keyword Function: Function to determine keyword or identifier                                                                               */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

int Keyword(char iden[])
{
	char keywords[43][10] = { "auto","break","case","char","const","continue","default",
							"do","double","else","enum","extern","float","for","goto",
							"if","int","long","register","return","short","signed",
							"sizeof","static","struct","switch","typedef","union",
							"unsigned","void","volatile","while","whileend","bool","then",
							"endif","doend","forend","input","output","and","or","function" };
	int i, flag = 0;

	/* State selection by comparing input string with existing table -----
	   If any acceptance state other than 0, will transition to next state */

	for (i = 0; i < 43; ++i) {
		if (strcmp(keywords[i], iden) == 0)
		{
			flag = 1;
			break;
		}
	}
	return flag;
}


/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Operator Or Separator Function: Function to determine if the current character is an operator or a separator                                */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

int OpOrSep(char ch)
{
	/* Define operators and separators -----------------------*/
	char operators[] = "+-*/%=<>^";
	char separators[] = "'(){}[],.:;!";
	int i = 0;
	string lexeme;

	/* For loop through all possible operators and separators.
	   Compare input character and break if a match is found - */

	for (i = 0; i < 12; ++i)
	{
		if (ch == operators[i])
		{
			lexeme = ch;
			writeFile("Operator  ", lexeme);
			break;
		}
		else if (ch == separators[i])
		{
			lexeme = ch;
			writeFile("Separator ", lexeme);
			break;
		}
	}
	return 0;
}


/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Read From File Function: Funtion to read an input file and output the text into strings                                                     */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

string readInFile()
{
	string fileName;
	cout << "Enter the input filename (include .txt) to be read: ";
	cin >> fileName;
	cout << "\nToken        |  Lexeme\n_____________|________\n             |\n";		/* Output file header ---------------------------------*/

	ifstream file;
	file.open(fileName);

	if (!file)
	{
		cout << "Cannot find file name with " << fileName << endl;
		exit(1);
	}

	stringstream ss;
	ss << file.rdbuf();
	file.close();
	return ss.str();
}


/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Output File Function: Function to output results into a text file                                                                           */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

void writeFile(string token, string lexeme)
{
	ofstream outFile;
	Pair tmp;
	tmp.token = token;
	tmp.lexeme = lexeme;
	tkn.push(tmp);

	cout << token << "   |  " << lexeme << "\n";										/* Console output of each token and lexeme ------------*/
	outFile.open(outfileName, ios::app);												/* Open output text file and append to existing data --*/
	outFile << token << "   |  " << lexeme << "\n";										/* Append the proper data to the output text file -----*/
	outFile.close();																	/* Save and close output text file --------------------*/
}


/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Extract Function: Function to extract and remove comment blocks from the input string                                                       */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

string Extract(string line)
{
	string result;																		/* Holds the new string without comment block ---------*/

	regex matcher("!.*!");																/* Regex to match "!any comment!" in the string -------*/
	regex_replace(back_inserter(result), line.begin(), line.end(), matcher, "$2");		/* Extracts & places non-commented section into result */
	return result;																		/* Returns the extracted string to the main function --*/
}


/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Main Function: Main function and output                                                                                                     */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

int main(int argc, const char* argv[])
{
	char decision;
	cout << "Do you want to save the results in a text file? Y or N" << endl;			/* Prompts user to save to text file or not -----------*/
	cin >> decision;																	/* Stores the user response to variable "decision" ----*/

	if (decision == 'Y' || decision == 'y')												/* Only generates an output text file if user wants to */
	{
		ofstream outFile;

		cout << "Enter the filename (include .txt) you would like to save to: ";		/* Prompts the user to input the output text file name */
		cin >> outfileName;																/* Stores the user defined file name to outfileName ---*/

		outFile.open(outfileName, ios::app);											/* Open output text file and append to existing data --*/
		outFile << "Token        |  Lexeme\n_____________|________\n             |\n";	/* Stores header to the output file -------------------*/
		outFile.close();
	}
	else {}

	string holder = readInFile();

	lexer(Extract(holder)); 															/* Calling FSM function and starting selection process */
	
	Pair eos;
	eos.token = "$";
	eos.lexeme = '\0';
	tkn.push(eos);
	parser(tkn);

	cout << "\nPlease press \"Enter\" to exit the program.\nThank you.\n";				/* Output an exiting program instruction --------------*/
	cin.get(), cin.get();																/* Waiting for a keystroke (Enter) to exit the program */
	return 0;
}

/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/* Lexer Function: Main operating function which includes a FSM to determine tokens and lexemes                                                */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/

void lexer(string result)
{
	/* Define variables and identifiers */
	size_t count = 1;
	char ch = result.at(0);
	char iden[20];
	char real[20];
	int i = 0, j = 0, k = 0;
	string lexeme;

	/* Defining the starting state q0 ---------------*/
	/* There will be 5 states: Q = {A,B,C,D,E} ------*/
	/* Final State Set depends on what is looked for */
	/* If keyword or identifier, FSS = {B} ----------*/
	/* If numbers, FSS = {C,D,E} --------------------*/
	/* State A (q0) is never part of the final state */
	char state = 'A';


	/* FSM STARTS HERE! ---------------------------- */

	while (count <= result.size())														/* While ch is not the last character in the file --------------------*/
	{
		ch = result.at(count - 1);

		if (state == 'A')
		{
			if (isalpha(ch))
			{
				iden[i++] = ch;
				state = 'B';
			}
			else if (isdigit(ch))
			{
				real[j++] = ch;
				state = 'C';
			}
			else if (ch == '.')
			{
				lexeme = ch;
				writeFile("Separator ", lexeme);
			}
			else
			{
				OpOrSep(ch);															/* Send ch to Operator or Separator function -------------------------*/
				state = 'A';															/* Return state to A to begin at q0 ----------------------------------*/
			}
		}
		else if (state == 'B')
		{
			if (isalpha(ch) || isdigit(ch) || ch == '$')
			{
				iden[i++] = ch;
				state = 'B';
			}
			else
			{
				iden[i] = '\0';
				i = 0;															/* Send ch to Operator or Separator function -------------------------*/
				state = 'A';															/* Return state to A to begin at q0 ----------------------------------*/

				if (Keyword(iden) == 1)													/* Send string alp to FSM function and if accepted, output as Keyword */
				{
					lexeme = iden;
					writeFile("Keyword   ", lexeme);
				}
				else																	/* Otherwise, will output as an identifier ---------------------------*/
				{
					lexeme = iden;
					writeFile("Identifier", lexeme);
				}
				OpOrSep(ch);
			}
		}
		else if (state == 'C')
		{
			if (isdigit(ch))
			{
				real[j++] = ch;
				state = 'C';
			}
			else if (ch == '.')
			{
				real[j++] = ch;
				state = 'D';
			}
			else
			{
				real[j] = '\0';
				j = 0;
				state = 'A';															/* Return state to A to begin at q0 ----------------------------------*/
				lexeme = real;
				writeFile("Integer   ", lexeme);
				OpOrSep(ch);															/* Send ch to Operator or Separator function -------------------------*/
			}
		}
		else if (state == 'D')
		{
			if (isdigit(ch))
			{
				real[j++] = ch;
				state = 'E';
			}
			else
			{
				real[j - 1] = '\0';														/* Set real to equal the integer number and NOT the ending period ----*/
				j = 0;
				state = 'A';															/* Return state to A to begin at q0 ----------------------------------*/
				lexeme = real;
				writeFile("Integer   ", lexeme);
				ch = result.at(count - 2);												/* Doing "count - 2" to properly process the ending period -----------*/
				OpOrSep(ch);															/* Send ch (the period) to Operator or Separator function ------------*/
			}
		}
		else if (state == 'E')
		{
			if (isdigit(ch))
			{
				real[j++] = ch;
				state = 'E';
			}
			else
			{
				real[j] = '\0';
				j = 0;
				state = 'A';															/* Return state to A to begin at q0 ----------------------------------*/
				lexeme = real;
				writeFile("Real      ", lexeme);
				OpOrSep(ch);															/* Send ch to Operator or Separator function -------------------------*/
			}
		}
		count++;																		/* Increment count to proceed down the string ------------------------*/
	}
}


/* --------------------------------------------------------------------------------------------------------------------------------------------*/
/*                                                              END OF ASSIGNMENT 1                                                            */
/* --------------------------------------------------------------------------------------------------------------------------------------------*/
