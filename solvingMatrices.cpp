// SOLVING MATRICES WITH UNKNOWN VARIABLES USING MATRIXES AND STRING MANIPULATION
// Mason Gillespie

//preprocessor directives
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// function prototypes
void readInEquations(ifstream&, double*, int);
double getCoefficient(string, char);
void performRowOperations(double*, const int);
void printMenuChoices(double*, const int);
int getUserInput(int&, int&, double&, const int);
bool reducedEchelonFormat(double*, const int);
void printVariables(double*, const int);
bool rowIsSolved(double*, int);

int main()
{
    double *coefficients = nullptr;   // pointer will be a single dimension array that holds coefficients from equations
    ifstream inputFile;               // inputFile holds equations
    int size = 4;                     // size will increment in size by 4 depending on the number of equations
    string stringTemp = "";           // stringTemp is used to hold an full line form the file which represents an equation
    int numOfEquations = 0;           // numOfEquations is the number of equations

    // opening the input file and validating the file was opened correctly
    inputFile.open("matrix4.txt");

    if (inputFile.fail())
        cout << "File not found";
    else
    {
        // setting all output to console to fixed setprecision 2
        cout << fixed << setprecision(2);

        // reading in an equation from the input file and using a counter to found out how many there are
        while(!inputFile.eof())
        {
            getline(inputFile, stringTemp);
            numOfEquations++;
        }

        // clearing the ios flags and moving the pointer back the first bit
        inputFile.clear();
        inputFile.seekg(0, ios :: beg);

        // size is calculated by creating 4 memory allocations for each equation
        size *= numOfEquations;

        // dynamically allocating memory for size coefficients
        coefficients = new double[size];

        // calling readInEquations
        readInEquations(inputFile, coefficients, numOfEquations);

        // calling performRowOperations
        performRowOperations(coefficients, numOfEquations);
    }

    // deleting memory allocation and setting pointer to null
    delete [] coefficients;
    coefficients = nullptr;
    return 0;
}// end of main function

// getCoefficient uses string functions to break apart the string read in from the input and returns
// the coefficient of the argument variable
double getCoefficient(string equation, char variable)
{
    int variablePos = 0;            // position of variable in equation
    int signPos = 0;                // position of sign in equation
    string stringTemp = "";         // stringTemp will hold equation from input file
    string coefficientTemp = "";    // coefficientTemp will hold string version of coefficient
    double coefficient = 0.0;       // coefficient will be stored in proper place in memory
    char charTemp = 'A';            // charTemp will hold temporary position of sign

    // variablePos is searched for in the equation
    variablePos = equation.find(variable);

    // variable is searched for and if it is in the first position then it's coefficient has to be one
    // and if there is no occurrence of the variable then its coefficient is 0
    if (variablePos == 0)
        return 1;
    if (variablePos == -1)
        return 0;

    // signPos is initialized to variablePos for traversing the string
    signPos = variablePos;

    // string is traversed until the beginning is reached or a sign is found
    while (signPos > 0 && (charTemp != '+' && charTemp != '-'))
    {
        signPos--;
        charTemp = equation.at(signPos);
    }

    // if the sign position is one left of the variable then the coefficient must be 1 or -1
    if (signPos == variablePos - 1 && charTemp == '+')
        return 1;
    if (signPos == variablePos - 1 && charTemp == '-')
        return -1;

    // reading in the coefficient from the beginning of the string then converting to a double before returning the value
    if (signPos == 0)
    {
        coefficientTemp = equation.substr(0, variablePos);
        coefficient = stod(coefficientTemp);
        return coefficient;
    }

    // reading in the coefficient in between a sign and the variable position then returning the double value
    // if the sign at signPos is a '+' then the reading starts after the sign
    if (charTemp == '+')
    {
        coefficientTemp = equation.substr(signPos + 1, (variablePos - (signPos + 1)));
        coefficient = stod(coefficientTemp);
        return coefficient;
    }
    else
    {
        coefficientTemp = equation.substr(signPos, (variablePos - signPos));
        coefficient = stod(coefficientTemp);
        return coefficient;
    }

}// end of getCoefficient

// readInEquations loops the entire dynamic memory allocation and stores the coefficients received from getCoefficient()
// into their proper places in the dynamic array
void readInEquations(ifstream &input, double *coefficients, const int numOfEquations)
{
    string stringTemp = "";     // stringTemp holds current string equation
    double doubleTemp = 0.0;    // doubleTemp holds returned value from getCoefficient()
    int equalSign = 0;          // equalSign  holds the position of the equal sign

    // for loop will iterate for currentEquation amount of times
    for (int currentEquation = 1; currentEquation <= numOfEquations; currentEquation++)
    {
        // reading in next equation from input file
        getline(input, stringTemp);

        // if statements will move coefficients pointer to the x variable position in memory
        if (currentEquation == 2)
            coefficients += 4;
        if (currentEquation == 3)
            coefficients += 8;
        if (currentEquation == 4)
            coefficients += 12;

        // calling getCoefficient() and storing the return value in the x position
        doubleTemp = getCoefficient(stringTemp, 'x');

        *coefficients = doubleTemp;

        // calling getCoefficient() and storing the return value in the y position
        doubleTemp = getCoefficient(stringTemp, 'y');

        coefficients += 1;
        *coefficients = doubleTemp;
        coefficients -= 1;

        // calling getCoefficient() and storing the return value in the z position
        doubleTemp = getCoefficient(stringTemp, 'z');

        coefficients += 2;
        *coefficients = doubleTemp;
        coefficients -= 2;

        // finding where the equal sign is and then reading in the remaining value and storing it
        // in the value position
        equalSign = stringTemp.find('=');

        stringTemp = stringTemp.substr(equalSign + 1);

        doubleTemp = stod(stringTemp);

        coefficients += 3;
        *coefficients = doubleTemp;
        coefficients -= 3;

        // returning to the first equations x variable position or the very first memory allocation
        if (currentEquation == 2)
            coefficients -= 4;
        if (currentEquation == 3)
            coefficients -= 8;
        if (currentEquation == 4)
            coefficients -= 12;
    }

}// end of main function

// printMenuChoices will print out the current equations after each row operation is used
void printMenuChoices(double* coefficients, const int numOfEquations)
{
    // for loop iterates for the total number of equations
    for (int equation = 0; equation < numOfEquations; equation++)
    {
        // printing out the x variable
        cout << *coefficients << "x";

        coefficients++;

        // '+' has to be printed to screen if the coefficient is positive
        if (*coefficients >= 0)
            cout << "+" << *coefficients << "y";
        else
            cout << *coefficients << "y";

        coefficients++;

        if (*coefficients >= 0)
            cout << "+" << *coefficients << "z";
        else
            cout << *coefficients << "z";

        coefficients++;

        cout << "=" << *coefficients << endl;

        coefficients++;
    }
    // moving back to the very first memory allocation
    coefficients -= (4 * numOfEquations);

    cout << endl;
    // printing out the menu
    cout << "1) Switch two rows." << endl;
    cout << "2) Multiply row by non-zero number." << endl;
    cout << "3) Add scalar multiple of one row to another." << endl;
    cout << "4) Quit." << endl;

}// end of menuChoice

// getUserInput reads in user input used for row operations
int getUserInput(int& rowAlpha, int& rowBeta, double& multiplier, const int numOfEquations)
{
    int choice = 0;

    // menu is printed and user enters a choice from menu
    cin >> choice;
    cout << endl;

    // validating that choice is a correct choice
    while (choice < 1 || choice > 4)
    {
        cout << "Invalid option, please enter a menu choice." << endl;
        cin >> choice;
        cout << endl;
    }

    // switch statement for menu options
    switch(choice){
    case 1: // switching two rows operation
        cout << "Enter 2 rows to switch: ";
        cin >> rowAlpha >> rowBeta;
        cout << endl;

        // validating that rows are within range of how many rows there are
        while (rowAlpha < 1 || rowAlpha > numOfEquations)
        {
            cout << "Incorrect valid option: " << rowAlpha << endl;
            cout << "Enter new row: ";
            cin >> rowAlpha;
            cout << endl;
        }

        while (rowBeta < 1 || rowBeta > numOfEquations)
        {
            cout << "Incorrect valid option: " << rowBeta << endl;
            cout << "Enter new row: ";
            cin >> rowBeta;
            cout << endl;
        }
        break;
    case 2: // multiplying row operation
        cout << "Enter a row and a non-zero multiplier: ";
        cin >> rowAlpha >> multiplier;
        cout << endl;

        // validating that the row is within range
        while (rowAlpha < 1 || rowAlpha > numOfEquations)
        {
            cout << "Incorrect valid option: " << rowAlpha << endl;
            cout << "Enter new row: ";
            cin >> rowAlpha;
            cout << endl;
        }

        // validating that the multiplier is not 0
        while (multiplier == 0)
        {
            cout << "Multiplier cannot be 0" << endl;
            cout << "Enter non-zero multiplier: ";
            cin >> multiplier;
            cout << endl;
        }
        break;
    case 3: // adding scalar multiple row operation
        cout << "Enter the row to use for multiplication, the multiplier, and the row to be ";
        cout << "modified respectively: ";
        cin >> rowAlpha >> multiplier >> rowBeta;
        cout << endl;

        // validating that rows do not equal each other, the rows are within range, and the multiplier is not 0
        while (rowAlpha == rowBeta)
        {
            cout << "Rows cannot be the same. Please enter rows again: ";
            cin >> rowAlpha >> rowBeta;
        }

        while (rowAlpha < 1 || rowAlpha > numOfEquations)
        {
            cout << "Incorrect valid option: " << rowAlpha << endl;
            cout << "Enter new row: ";
            cin >> rowAlpha;
            cout << endl;
        }

        while (multiplier == 0)
        {
            cout << "Multiplier cannot be 0" << endl;
            cout << "Enter non-zero multiplier: ";
            cin >> multiplier;
            cout << endl;
        }

        while (rowBeta < 1 || rowBeta > numOfEquations)
        {
            cout << "Incorrect valid option: " << rowBeta << endl;
            cout << "Enter new row: ";
            cin >> rowBeta;
            cout << endl;
        }
        break;
    case 4: // end row operations
        return 4;
        break;
    }

    return choice;
}// end of getUserInput

// reducedEchelonFormat tests to see if the rows are in reduced echelon form
bool reducedEchelonFormat(double* coefficients, const int numOfEquations)
{
    int count = 0;
    int x = 0,
        y = 0,
        z = 0;

    // if there are 2 rows then only two variables are being solved for
    if (numOfEquations == 2)
    {
          // for loop will run through the dynamic array and test that there is only 1 one and every other
          // coefficient is 0
          for(int currEquation = 0; currEquation < numOfEquations; currEquation++)
          {
              coefficients += (currEquation * 4);

              for(int currNum = 0; currNum < 3; currNum++)
              {
                  if(*coefficients == 1.0)
                  {
                      if (currNum == 0)
                        x++;
                      else if (currNum == 1)
                        y++;
                      else
                        z++;

                      count++;
                  }
                  // if there is any other coefficient other than 1 or 0 then return false
                  else if (*coefficients != 0)
                  {
                      coefficients -= (currNum + (currEquation * 4));
                      return false;
                  }

                  coefficients++;
              }
              // moving pointer back to the beginning
              coefficients -= (3 + (currEquation * 4));

              // if count is equal to 0 then there is only one one
              if(count != 1)
                return false;

              count = 0;
           }
           // testing if the variables with the coefficient of one are not the same variable
           if (x > 1 || y > 1 || z > 1)
               return false;
    }
    // if there are 3 equations then variables follow echelon form and if there are 4 equations then the last row
    // will be all zeros
    else if (numOfEquations == 3 || numOfEquations == 4)
    {
        // testing that the the first row is 1 0 0
        if (*coefficients != 1.0)
            return false;

        coefficients++;

        for(int currNum = 0; currNum < 2; currNum++)
        {
            if(*coefficients != 0.0)
                return false;
            coefficients++;
        }

        coefficients++;

        // testing that the second row is 0 1 0
        if (*coefficients != 0.0)
            return false;

        coefficients++;

        if (*coefficients != 1.0)
            return false;

        coefficients++;

        if (*coefficients != 0.0)
            return false;

        coefficients += 2;

        // testing that the third row is 0 0 1
        for (int currNum = 0; currNum < 2; currNum++)
         {
            if (*coefficients != 0.0)
                return false;

            coefficients++;
        }

        if (*coefficients != 1.0)
            return false;

        coefficients -= 10;

        // if there are 4 equations then testing that the last row is 0 0 0 0
        if (numOfEquations == 4)
        {
            coefficients += 12;

            for (int currNum = 0; currNum < 4; currNum++)
            {
            if (*coefficients != 0.0)
                return false;

            coefficients++;
            }

            coefficients -= 16;
        }
    }

    return true;
}// end of reducedEchelonFormat

// reduced echelon has been achieved and the variables are being printed to the console
void printVariables(double* coefficients, const int numOfEquations)
{
    for (int currEquation = 0; currEquation < numOfEquations; currEquation++)
    {
        coefficients += (currEquation * 4);

        for (int currNum = 0; currNum < 3; currNum++)
        {
            // for loop will iterate through each variable in the equations and will find the 1 coefficient
            // then the pointer is moved to the proper location and the variable is printed out
            if (*coefficients == 1.0)
            {
                // x variable
                if (currNum == 0)
                {
                    coefficients += 3;

                    cout << "x: " << *coefficients << " ";

                    coefficients -= 3;
                }

                // y variable
                else if (currNum == 1)
                {
                    coefficients += 2;

                    cout << "y: " << *coefficients << " ";

                    coefficients -= 2;
                }

                // z variable
                else if (currNum == 2)
                {
                    coefficients += 1;

                    cout << "z: " << *coefficients << " ";

                    coefficients -= 1;
                }
            }
            coefficients++;
        }
        // moving back the beginning
        coefficients -= (3 + (currEquation * 4));
    }
}// end of printVariable

// rowIsSolved tests to see if the user is trying to perform multiplication or add a scalar multiple
// to a row that is already solved
bool rowIsSolved(double* coefficients, int rowPosition)
{
    // moving the pointer to the beginning of the indicated row
    coefficients += rowPosition;
    int count = 0;

    // testing that the variables are either 1 or 0 and that they add up to 1
    for (int currNum = 0; currNum < 3; currNum++)
    {
        if (*coefficients == 1.0)
            count++;
        else if (*coefficients != 0.0)
        {
            coefficients -= (currNum + rowPosition);
            return false;
        }

        coefficients++;
    }

    // moving back to the beginning
    coefficients -= (3 + rowPosition);

    // if the rowPosition is pointing to the fourth equation then there needs to be all zeros
    // and every other row needs to have at least one one in the variables
    if (count == 1 && rowPosition != 12)
        return true;
    else if (count == 0 && rowPosition == 12)
        return true;
    else
        return false;
}// end of rowIsSolved

// performRowOperations contains the logic to move, multiply, and add coefficients
void performRowOperations(double* coefficients, const int numOfEquations)
{
    int choice = 0;
    int rowAlpha = 0;
    int rowBeta = 0;
    double multiplier = 0.0;
    bool done = false;

    // while loop will iterate until the user enters 4 into choice or the equations are in reduced
    // echelon format
    while (!done)
    {
        // printing the menu
        printMenuChoices(coefficients, numOfEquations);

        // getting the choice and proper variables from the user
        choice = getUserInput(rowAlpha, rowBeta, multiplier, numOfEquations);

        // breaking the loop is the user quits
        if (choice == 4)
            break;

        // variables are initialized for next row operation but may not be used for some of the iterations
        double doubleTempAlpha = 0.0;
        double doubleTempBeta = 0.0;
        int rowAlphaPosition = (rowAlpha - 1) * 4;
        int rowBetaPosition = (rowBeta - 1) * 4;

        // switching the choice
        switch(choice){
        case 1: //switching two rows operation
            for (int currNum = 0; currNum < 4; currNum++)
            {
                // moving the pointer to the alpha row then storing the coefficient in temp variable
                coefficients += rowAlphaPosition;

                doubleTempAlpha = *coefficients;

                coefficients -= rowAlphaPosition;

                // moving the pointer to the beta row and storing coefficient in temp variable and storing
                // coefficient from alpha row in beta row
                coefficients += rowBetaPosition;

                doubleTempBeta = *coefficients;
                *coefficients = doubleTempAlpha;

                coefficients -= rowBetaPosition;

                // moving back to alpha row and storing coefficient from beta row
                coefficients += rowAlphaPosition;

                *coefficients = doubleTempBeta;

                coefficients -= rowAlphaPosition;

                // moving both position up one for next variable
                rowAlphaPosition++;
                rowBetaPosition++;
            }
            break;
        case 2: // multiplying row operation
            // if the row is not solved
            if (!rowIsSolved(coefficients, rowAlphaPosition))
            {
                // moving to the alpha row
                coefficients += rowAlphaPosition;

                // for loop will iterate through each coefficient on that row and multiply by user given constant
                for (int currNum = 0; currNum < 4; currNum++)
                {
                    if (*coefficients != 0)   // strange error with multiplying 0 and a negative number
                        *coefficients *= multiplier;

                    if (*coefficients == 0)
                        *coefficients = 0.0;
                    cout << *coefficients << "& ";
                    coefficients++;
                }
                cout << endl;

                // moving pointer back to beginning
                coefficients -= (rowAlphaPosition + 4);
            }
            else
                cout << "Operation can't be used on solved row\n" << endl;
            break;
        case 3: // adding scalar multiple to row operation
            // if the row is not solved
            if (!rowIsSolved(coefficients, rowBetaPosition))
            {
                for (int currNum = 0; currNum < 4; currNum++)
                {
                    // moving pointer to alpha row
                    coefficients += rowAlphaPosition;

                    // if coefficient is not zero (weird error with 0 possibly having a negative sign) then
                    // alpha row's coefficient is multiplied be user scalar
                    if (*coefficients != 0)
                        doubleTempAlpha = *coefficients * multiplier;

                    // product is added to beta row's coefficient
                    coefficients -= rowAlphaPosition;

                    coefficients += rowBetaPosition;

                    *coefficients += doubleTempAlpha;

                    // making sure that 0 does not have a negative sign
                    if (*coefficients == -0)
                        *coefficients = abs(*coefficients);

                    coefficients -= rowBetaPosition;

                    // moving both position up one for next variable
                    rowAlphaPosition++;
                    rowBetaPosition++;
                    doubleTempAlpha = 0.0;
                }
                cout << endl;
            }
            else
                cout << "Operation can't be used on solved row\n" << endl;
            break;
        }
        // if the equations are in reduced echelon form then printVariables() is called and while loop is exited
        // with boolean flag being set to true
        if (reducedEchelonFormat(coefficients, numOfEquations))
            {
                printVariables(coefficients, numOfEquations);

                done = true;
            }
    }
}// end of performRowOperations
