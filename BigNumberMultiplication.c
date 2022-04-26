#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"

/*
    This program solves the problem of not being able to make operations
    on bigger numbers than integers or longs by implementing basic multiplication 
    and addition algorithms,however it doesn't still provide other more complex operations or 
    number types.

    Written by Mustafa Tunahan BAŞ (No:150119055)
*/

void GetInputs(char** number1,char** number2);//Gets the inputs from the input file
void WriteOutputs(Node* number1,Node* number2,Node* result,Node** nodes,int count);//Writes outputs into the output file
void WriteList(Node* node,FILE* file);//Writes a single list into the given file
Node* SplitDigits(char* number);
/*
    Splits the string into integer digit nodes(This function doesn't handle errors,
    so inputting a non-integer character will break the execution of the program or result 
    in error)
*/
int GetDigit(char** number,int* digit);
/*
    Gets the digit from a char array and updates the pointer to the next char
*/
Node* MultiplyBigNumbers(Node* first,Node* second,Node***,int* subResultCount);//Multiplies big numbers
Node* AddBigNumbers(Node* first,Node* second);//Adds big numbers

int main()
{
    char* number1;
    char* number2;
    GetInputs(&number1,&number2);

    Node* nm1 = SplitDigits(number1);
    Node* nm2 = SplitDigits(number2);
    
    Node** nodes;
    int count;
    Node* result = MultiplyBigNumbers(nm1,nm2,&nodes,&count);

    WriteOutputs(ReverseList(nm1,NULL),ReverseList(nm2,NULL),result,nodes,count);
}

void GetInputs(char** number1,char** number2)
{
    int size=1;
    char *line;
    int ch;
    FILE* file = fopen("Input.txt","r");
    
    size_t len = 0;
    line = realloc(NULL, sizeof(*line)*size);

    while((ch=fgetc(file))!=EOF && ch != '\n')
    {
        line[len++]=ch;
        if(len==size)
            line = realloc(line, sizeof(*line)*(size+=4));
    }

    fclose(file);

    char* reader = line;

    while (*reader>='0' && *reader<='9')
        reader++;

    *reader = '\0';

    char* nm1 = line;
    char* nm2 = (++reader);

    while (*reader>='0' && *reader<='9')
        reader++;

    *reader='\0';
    *number1 = nm1;
    *number2=nm2;
}

void WriteOutputs(Node* number1,Node* number2,Node* result,Node** nodes,int count)
{
    FILE* file = fopen("Output.txt","w");

    fprintf(file,"\n\n\n\n");
    for (size_t i = 0; i < count+7; i++)
        fprintf(file," ");
    WriteList(number1,file);
    fprintf(file,"\n");
    Node* temp = number1;
    int counter=0;
    do
    {
        counter++;
    } while ((temp=temp->next)!=NULL);

    for (size_t i = 0; i < counter+7; i++)
        fprintf(file," ");
    WriteList(number2,file);
    fprintf(file,"\n");

    for (size_t i = 0; i < count+4; i++)
        fprintf(file," ");
    fprintf(file,"x\n");

    for (size_t i = 0; i < count+4; i++)
        fprintf(file," ");
    for (size_t i = 0; i < counter+3; i++)
        fprintf(file,"-");
    fprintf(file,"\n");

    
    for (size_t i = count; i > 0; i--)
    {
        for (size_t j = 0; j < i+7; j++)
            fprintf(file," ");

        WriteList(ReverseList(*(nodes++),NULL),file);
        fprintf(file,"\n");                   
    }
    fprintf(file,"      +\n");

    for (size_t i = 0; i < 6; i++)
        fprintf(file," ");
    for (size_t i = 0; i < count+counter+1; i++)
        fprintf(file,"-");
    fprintf(file,"\n");

    for (size_t i = 0; i < 8; i++)
        fprintf(file," ");
    WriteList(result,file);

    fprintf(file,"\n\n\n\n");
    fprintf(file,"Multiplicand : ");
    WriteList(number1,file);
    fprintf(file,"\nMultiplier : ");
    WriteList(number2,file);
    fprintf(file,"\nProduct : ");
    WriteList(result,file);

    fclose(file);

    // fprintf(file,"First Number : ");
    // WriteList(number1,file);
    // fprintf(file,"\n");
    // fprintf(file,"Second Number : ");
    // WriteList(number2,file);
    // fprintf(file,"\n");
    // fprintf(file,"Result : ");
    // WriteList(result,file);
    // fclose(file);
}

void WriteList(Node* node,FILE* file)
{
    do
    {
        putc(node->data + '0',file);
    } while ((node = node->next)!=NULL);
}

Node* SplitDigits(char* number)
{
    Node* node;
    int digit=0;

    if(GetDigit(&number,&digit))
    {
        node = CreateNode(digit);
    }
    else return NULL;

    Node* digits=node;

    while(GetDigit(&number,&digit))
    {
        node = AddNode(node,digit,0);
    }

    return ReverseList(digits,NULL);
}

int GetDigit(char** number,int* digit)
{
    int dg = **number - '0';
    if(dg<0 || dg>9)
        return 0;
    
    (*number)++;
    *digit=dg;
    return 1;
}

Node* MultiplyBigNumbers(Node* first,Node* second,Node*** subResults,int* subResultCount)
{
    int nm1,nm2,res,index=0,carry=0,secondNumberDigits=1;
    if(first==NULL || second==NULL)
        return NULL;

    Node* node,*header,*firstNumber,*temp=second,*result = CreateNode(0);

    while (temp->next!=NULL)
    {
        temp=temp->next;
        secondNumberDigits++;
    }
    Node **nodes = malloc(sizeof(Node)*secondNumberDigits);

    do
    {
        GetData(second,&nm2);
        firstNumber=first;
        node = CreateNode(0);
        header=node;

        for (size_t i = 0; i < index; i++)
            node = AddNode(node,0,0);

        do
        {
            nm1 = GetData(firstNumber,&nm1) ? nm1 : 0;

            res = nm1*nm2 + carry;
            carry = res/10;

            node->data=res%10;
            if(GetNext(&firstNumber) || carry)
            {
                node = AddNode(node,0,0);
            }
            else break;    

        } while (1);

        nodes[index] = header;
        
        GetNext(&second);

    } while (++index<secondNumberDigits);
    
    for (size_t i = 0; i < secondNumberDigits; i++)
    {
        result=ReverseList(result,NULL);
        result = AddBigNumbers(result,nodes[i]);
    }

    //free(firstNumber);
    //free(node);
    *subResults=nodes;
    *subResultCount = secondNumberDigits;
    //free(header);

    /*
        I don't know how or why but freeing these nodes resulted in error in visualising the multiplication
        So I had to remove them till I found the cause
    */
    return result;
}

Node* AddBigNumbers(Node* first,Node* second)
{
    int nm1,nm2,res,terminator,carry=0;
    Node* result = CreateNode(0);
    Node* header = result;
    do
    {
        nm1 = GetData(first,&nm1) ? nm1 : 0;
        nm2 = GetData(second,&nm2) ? nm2 : 0;
        
        res = carry+nm1+nm2;
        carry = (res/10);

        result->data=res%10;

        terminator = GetNext(&first);
        terminator = GetNext(&second) || terminator;
        terminator = carry || terminator;

        if(terminator)
            result = AddNode(result,0,0);
        else break;

    }while (1);
    
    header = ReverseList(header,NULL);
    return header;
}