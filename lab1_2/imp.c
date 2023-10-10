#include "imp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
char* a ;
char* op = "";
char* b;
// const char * ans = "ans";
const char* ANS = "ANS";
float num1 = 0, num2 = 0;
int aIsAns = 0, bIsAns = 0;
FILE *fptr;
void saveAns(float result){
    fptr = fopen("ans_val.txt","w");
    if(fptr == NULL){
        printf("cannot opend ans");
        return;
    }
    fprintf(fptr,"%f",result);
    fclose(fptr);
    return;
}
float getAns(){
    float ansNum = 0;
    fptr = fopen("ans_val.txt","r");
    if(fptr == NULL){
        printf("cannot opend ans");
        return -1;
    }
    fscanf(fptr,"%f",&ansNum);  
    fclose(fptr);
    return ansNum;
}
int checkValidNumber(char* number){
    const char *pattern = "^[+-]?[0-9]+\\.?[0-9]*$";
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    if(result){
        printf("Compiler failed");
        return 0;
    }
    return (regexec(&regex,number, 0,NULL,0) == 0);
}
int checkValidInput(char* input, int n){
    char *word = strtok(input, " ");
    int count = 0;
    int isValidA = 1, isValidB = 1;
    aIsAns = 0, bIsAns = 0;
    // 1. get each word -----------------
    while(word != NULL){
        if(count == 0 ) a = word;
        if(count == 1) op = word;
        if(count == 2) b =  word;
        count++;
        word=strtok(NULL, " ");
    } 
    if(count != 3) return 0;
    // 2. check have ANS -----------------
    if(strcmp(a, ANS) == 0){
        num1 = getAns();
        aIsAns = 1;
    }
    if(strcmp(b, ANS) == 0){
        num2 = getAns();
        bIsAns = 1;
    }
    // 3. check valid number -------------
    if(aIsAns == 0)
        isValidA = checkValidNumber(a);
    if(bIsAns == 0)
        isValidB = checkValidNumber(b);
    // return -------------------
    return isValidA && isValidB;
}
void calc (char*input,int n){
    if(checkValidInput(input, n) == 0 || strlen(op) > 1){
        printf("SYNTAX ERROR\n");
        return;
    }
    if(aIsAns == 0){
        num1 = strtof(a, NULL);
    }
    if(bIsAns == 0){
        num2 = strtof(b, NULL);
    }
    float result;
    if(op[0] == '+'){
        result = num1 + num2;
    }
    else if(op[0] == '-'){
        result = num1 - num2;
    }
    else if(op[0] == '*'){
        result = num1 * num2;

    }
    else if(op[0] == '/'){
        if(num2 == 0){
            printf("MATH ERROR\n");
            return;
        }   
        else{
            result = num1 / num2;
        }
    }
    else if(op[0] == '%'){
        if(num2 == 0){
            printf("MATH ERROR\n");
            return;
        }   
        else{
            result = (int)(num1) % (int)(num2);
        }
    }
    else{
        printf("SYNTAX ERROR\n");
        return;
    }
    printf("%.2f", result);
    printf("\n");
    // save ans
    saveAns(result);
}