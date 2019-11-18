
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex> 
#include <map>
#include <stack>
#include <ctype.h>
#include <stdlib.h>


using namespace std;


string Lexor(int CurrentState, string lexeme)
{
	string token;

	switch (CurrentState)
	{
	case 0:
		cout << "KEYWORD   " << "     " << "=" << "     " << lexeme << endl;
		token = "KEYWORD   ";
		break;
	case 1:
		cout << "IDENTIFIER" << "     " << "=" << "     " << lexeme << endl;
		token = "IDENTIFIER";
		break;
	case 2:
		cout << "SEPARATOR " << "     " << "=" << "     " << lexeme << endl;
		token = "SEPARATOR ";
		break;
	case 3:
		cout << "OPERATOR  " << "     " << "=" << "     " << lexeme << endl;
		token = "OPERATOR  ";
		break;
	case 4:
		cout << "INTEGER   " << "     " << "=" << "     " << lexeme << endl;
		token = "INTEGER   ";
		break;
	case 5:
		cout << "REAL      " << "     " << "=" << "     " << lexeme << endl;
		token = "REAL      ";
		break;
	case 6:
		cout << "UNKNOWN   " << "     " << "=" << "     " << lexeme << endl;
		token = "UNKNOWN   ";
		break;
	}

	return token;
}


int checkKeyword(string lexeme)
{
	int		stateConfirm = 0;
	regex	checkValid("^[a-zA-Z]{1}[a-zA-Z0-9$]*$");
	regex	checkNum("^[0-9]*[.]?[0-9]+$");
	string	Keywords[43] = { "auto","break","case","char","const","continue","default",
							"do","double","else","enum","extern","float","for","goto",
							"if","int","long","register","return","short","signed",
							"sizeof","static","struct","switch","typedef","union",
							"unsigned","void","volatile","while","whileend","bool","then",
							"endif","doend","forend","input","output","and","or","function" };

	if (regex_match(lexeme, checkValid) == true)
	{
		for (int i = 0; i < (sizeof(Keywords) / sizeof(Keywords[0])); i++)
		{
			if (lexeme == Keywords[i])
			{
				stateConfirm = 7;
			}

		}
	}

	if (regex_match(lexeme, checkNum))
	{
		stateConfirm = 4;
	}

	if (stateConfirm == 0)
	{
		stateConfirm++;
	}

	return stateConfirm;
}

int checkIdentifier(string lexeme)
{
	int		stateConfirm = 0;
	regex	checkValid("^[a-zA-Z]{1}[a-zA-Z0-9$]*$");

	if (regex_match(lexeme, checkValid) == true)
	{
		stateConfirm = 7;
	}
	else
	{
		stateConfirm = 2;
	}

	return stateConfirm;
}

int checkSeparator(string lexeme)
{
	int		stateConfirm = 0;
	string	Keywords[11] = { "'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";" };

	for (int i = 0; i < (sizeof(Keywords) / sizeof(Keywords[0])); i++)
	{
		if (lexeme == Keywords[i])
		{
			stateConfirm = 7;
		}
	}

	if (stateConfirm == 0)
	{
		stateConfirm = 3;
	}

	return stateConfirm;
}

int checkOperator(string lexeme)
{
	int		stateConfirm = 0;
	string	Keywords[8] = { "*", "+", "-", "=", "/", ">", "<", "%" };

	for (int i = 0; i < (sizeof(Keywords) / sizeof(Keywords[0])); i++)
	{
		if (lexeme == Keywords[i])
		{
			stateConfirm = 7;
		}
	}

	if (stateConfirm == 0)
	{
		stateConfirm = 4;
	}

	return stateConfirm;
}

int checkInteger(string lexeme)
{
	int		stateConfirm = 0;
	regex	checkInt("^\\d+$");

	if (regex_match(lexeme, checkInt) == true)
	{
		stateConfirm = 7;
	}
	else
	{
		stateConfirm = 5;
	}

	return stateConfirm;
}

int checkReal(string lexeme)
{
	int		stateConfirm = 0;
	regex	checkReal("^\\d*[.]{1}\\d+$");

	if (regex_match(lexeme, checkReal) == true)
	{
		stateConfirm = 7;
	}
	else
	{
		stateConfirm = 6;
	}

	return stateConfirm;
}

int main()
{
	int		i = 0, j = 0, sandwhichGate = 0, combinationGate = 0, statePosition = 0, r = 0, c = 0, stateFinal = 0;
	string	lexemeList[100], tokenList[100];
	int		stateTable[1][8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	smatch	match;
	regex	commentStart("^[!]{1}");
	regex	commentEnd("[!]{1}$");
	regex	legalWord("^[a-zA-Z]{1}[a-zA-Z0-9$]*$");
	regex	legalWordL("[a-zA-Z]{1}[a-zA-Z0-9$]*$");
	regex	legalWordR("^[a-zA-Z]{1}[a-zA-Z0-9$]*");
	regex	legalSepL("^[(\\{\\[,:;]{1}");
	regex	legalSepR("[)\\}\\],:;]{1}$");
	regex	legalOp("^[*+\\-=/><%]{1}$");
	regex	legalSepOp("[(\\{\\[)\\}\\],:;*+\\-=/><%]{1}");
	regex	legalNum("^[0-9]*[.]?[0-9]+$");
	regex	legalNumL("[0-9]*[.]?[0-9]+$");
	regex	legalNumR("^[0-9]*[.]?[0-9]+");
	regex	legalSandwich("[a-zA-Z]{1}[a-zA-Z0-9$]*");

	fstream		file;
	string		word, filename, inputtext;
	int			choice;

	cout << endl << "Please select one of the two input options below to start the syntax analysis!" << endl << endl;
	cout << "1: Read From an Existing File" << endl;
	cout << "2: Input Custom Free Text" << endl;
	cout << "Please Select From Above: ";
	cin >> choice;
	cout << endl << endl;

	if (choice == 1)
	{
		string filename;
		cout << "Enter the input filename (include .txt) to be read: ";
		cin >> filename;

		file.open(filename);

		if (!file)
		{
			cout << "Cannot find file name with " << filename << endl;
			exit(1);
		}
	}
	else if (choice == 2)
	{
		cout << "Please Input Your Custom Free Text (NOTE: be sure to have a ';' at the end of a line of code): ";
		file.open("Option 2.txt", ios::out | ios::trunc);
		getline(cin, inputtext);
		getline(cin, inputtext);
		file << inputtext;
		cout << endl << endl;
		file.close();
		filename = "Option 2.txt";
		file.open(filename.c_str());
	}
	else
	{
		cout << "Selected choice is not VALID...exiting program. Press Enter.";
		cin.get();
		return 0;
	}

	while (file >> word)
	{
		if (regex_match(word, commentStart) == true)
		{
			file >> word;
			while (regex_search(word, match, commentEnd) == false)
			{
				file >> word;
			}
			file >> word;
		}


		if (regex_search(word, match, legalWord) == true)
		{
			for (auto x : match)
			{
				lexemeList[i] = x;
				i++;
				j++;
				combinationGate++;
			}
		}

		if (regex_search(word, match, legalSepL) == true)
		{
			for (auto x : match)
			{
				lexemeList[i] = x;
				i++;
				j++;
				sandwhichGate++;
				combinationGate++;
			}

			if (regex_search(word, match, legalWordL) == true)
			{
				for (auto x : match)
				{
					lexemeList[i] = x;
					i++;
					j++;
				}
			}
		}



		if (regex_search(word, match, legalSepR) == true)
		{
			for (auto x : match)
			{
				lexemeList[i] = x;
				i++;
				j++;
				sandwhichGate++;
				combinationGate++;

			}


			if (regex_search(word, match, legalWordR) == true)
			{
				for (auto x : match)
				{
					lexemeList[i] = lexemeList[i - 1];
					lexemeList[i - 1] = x;
					i++;
					j++;
				}
			}
		}

		if (regex_search(word, match, legalOp) == true)
		{
			for (auto x : match)
			{
				lexemeList[i] = x;
				i++;
				j++;
				combinationGate++;
			}
		}

		if (regex_search(word, match, legalNum) == true)
		{
			for (auto x : match)
			{
				lexemeList[i] = x;
				i++;
				j++;
				combinationGate++;
			}
		}

		if (sandwhichGate == 2)
		{
			if (regex_search(word, match, legalSandwich) == true)
			{
				for (auto x : match)
				{
					lexemeList[i] = lexemeList[i - 1];
					lexemeList[i - 1] = x;
					i++;
					j++;
				}
			}
		}

		if (combinationGate == 0)
		{
			if (regex_search(word, match, legalWordR) == true)
			{
				for (auto x : match)
				{
					lexemeList[i] = x;
					i++;
					j++;
				}
			}
			else if (regex_search(word, match, legalNumR) == true)
			{
				for (auto x : match)
				{
					lexemeList[i] = x;
					i++;
					j++;
				}
			}

			if (regex_search(word, match, legalSepOp) == true)
			{
				for (auto x : match)
				{
					lexemeList[i] = x;
					i++;
					j++;
				}
			}

			if (regex_search(word, match, legalWordL) == true)
			{
				for (auto x : match)
				{
					lexemeList[i] = x;
					i++;
					j++;
				}
			}
			else if (regex_search(word, match, legalNumL) == true)
			{
				for (auto x : match)
				{
					lexemeList[i] = x;
					i++;
					j++;
				}
			}
		}

		sandwhichGate = 0;
		combinationGate = 0;
	}
	file.close();

	for (i = 0; i < j; i++)
	{
		c = 0;

		while (statePosition != 7)
		{
			statePosition = stateTable[r][c];

			switch (statePosition)
			{
			case 0:
				c = checkKeyword(lexemeList[i]);
				if (c == 7)
				{
					stateFinal = 0;
				}
				break;
			case 1:
				c = checkIdentifier(lexemeList[i]);
				if (c == 7)
				{
					stateFinal = 1;
				}
				break;
			case 2:
				c = checkSeparator(lexemeList[i]);
				if (c == 7)
				{
					stateFinal = 2;
				}
				break;
			case 3:
				c = checkOperator(lexemeList[i]);
				if (c == 7)
				{
					stateFinal = 3;
				}
				break;
			case 4:
				c = checkInteger(lexemeList[i]);
				if (c == 7)
				{
					stateFinal = 4;
				}
				break;
			case 5:
				c = checkReal(lexemeList[i]);
				if (c == 7)
				{
					stateFinal = 5;
				}
				break;
			case 6: 
				c = 7;
				stateFinal = 6;
				break;
			case 7:
				tokenList[i] = Lexor(stateFinal, lexemeList[i]);
				break;
			}
		}
		statePosition = 0;
	}
	cout << endl << endl;

	lexemeList[j] = "$";
	tokenList[j] = "FINISH    ";

	file.open("Lexical Analyzer Output.txt", ios::out | ios::trunc);
	file << "TOKEN	" << "          " << "		        LEXEME" << endl;
	for (i = 0; i < j; i++)
	{
		file << tokenList[i] << "	     " << "	=" << "		" << lexemeList[i] << endl;
	}
	file.close();

	map< string, map<string, int> > parseTable;
	stack<string> parser;

	i = 0;
	parser.push("$");
	parser.push("S");

	parseTable["S"]["ID"] = 1;
	parseTable["A"]["ID"] = 2;
	parseTable["E"]["ID"] = 3;
	parseTable["E"]["("] = 3;
	parseTable["E'"]["+"] = 4;
	parseTable["E'"]["-"] = 5;
	parseTable["E'"][")"] = 6;
	parseTable["E'"]["$"] = 6;
	parseTable["T"]["ID"] = 7;
	parseTable["T"]["("] = 7;
	parseTable["T'"]["+"] = 6;
	parseTable["T'"]["-"] = 6;
	parseTable["T'"]["*"] = 8;
	parseTable["T'"]["/"] = 9;
	parseTable["T'"][")"] = 6;
	parseTable["T'"]["$"] = 6;
	parseTable["F"]["ID"] = 10;
	parseTable["F"]["("] = 11;

	file.open("Syntax Analyzer Output.txt", ios::out | ios::trunc);
	cout << "Token: " << tokenList[i] << "   " << "Lexeme: " << lexemeList[i] << endl;
	file << "Token: " << tokenList[i] << "   " << "Lexeme: " << lexemeList[i] << endl;

	while (parser.size() > 0)
	{
		if (lexemeList[i] == ";")
		{
			i++;
			parser.pop();
			parser.push("$");
			parser.push("S");
			cout << "\nToken: " << tokenList[i] << "   " << "Lexeme: " << lexemeList[i] << endl;
			file << "\nToken: " << tokenList[i] << "   " << "Lexeme: " << lexemeList[i] << endl;
		}
		if (tokenList[i] == "IDENTIFIER" || tokenList[i] == "INTEGER   " || tokenList[i] == "REAL      ")
		{
			lexemeList[i] = "ID";
		}
		if (lexemeList[i] == parser.top())
		{
			i++;
			parser.pop();
			cout << "\nToken: " << tokenList[i] << "   " << "Lexeme: " << lexemeList[i] << endl;
			file << "\nToken: " << tokenList[i] << "   " << "Lexeme: " << lexemeList[i] << endl;
		}
		else
		{
			switch (parseTable[parser.top()][lexemeList[i]])
			{
			case 1:
				parser.pop();
				parser.push("A");
				cout << "<Statement> -> <Assign>" << endl;
				file << "<Statement> -> <Assign>" << endl;
				break;

			case 2:
				parser.pop();
				parser.push("E");
				parser.push("=");
				parser.push("ID");
				cout << "<Assign> -> ID = <Expression>" << endl;
				file << "<Assign> -> ID = <Expression>" << endl;
				break;

			case 3:
				parser.pop();
				parser.push("E'");
				parser.push("T");
				cout << "<Expression> -> <Term> <Expression>'" << endl;
				file << "<Expression> -> <Term> <Expression>'" << endl;
				break;

			case 4:
				parser.pop();
				parser.push("E'");
				parser.push("T");
				parser.push("+");
				cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
				file << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
				break;

			case 5:
				parser.pop();
				parser.push("E'");
				parser.push("T");
				parser.push("-");
				cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
				file << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
				break;

			case 6:
				parser.pop();
				cout << "<Expression Prime> -> epsilon" << endl;
				file << "<Expression Prime> -> epsilon" << endl;
				cout << "<Term Prime> -> epsilon" << endl;
				file << "<Term Prime> -> epsilon" << endl;
				break;

			case 7:
				parser.pop();
				parser.push("T'");
				parser.push("F");
				cout << "<Term> -> <Factor> <Term Prime>" << endl;
				file << "<Term> -> <Factor> <Term Prime>" << endl;
				break;

			case 8:
				parser.pop();
				parser.push("T'");
				parser.push("F");
				parser.push("*");
				cout << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
				file << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
				break;

			case 9:
				parser.pop();
				parser.push("T'");
				parser.push("F");
				parser.push("/");
				cout << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
				file << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
				break;

			case 10:
				parser.pop();
				parser.push("ID");
				cout << "<Factor> -> ID | Num" << endl;
				file << "<Factor> -> ID | Num" << endl;
				break;

			case 11:
				parser.pop();
				parser.push(")");
				parser.push("E");
				parser.push("(");
				cout << "<Factor> -> ( <Expression> )" << endl;
				file << "<Factor> -> ( <Expression> )" << endl;
				break;

			default:
				cout << "Parsing Table Defaulted" << endl;
				file << "Parsing Table Defaulted" << endl;
				system("pause");
				return 0;
				break;
			}
		}
	}
	file.close();

	system("pause");
	return 0;

}