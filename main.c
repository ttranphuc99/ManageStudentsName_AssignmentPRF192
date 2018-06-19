/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Thien Phuc
 *
 * Created on June 14, 2018, 9:16 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

/*
 * 
 */
//declare a struct
struct data {
    char name[256];
    char nameSorted[256];
};

//set a struct name
struct data info[50];


//to import data from file, return index of final data
int importData() {
    int i = -1;
    
    FILE* f = fopen("Name.txt", "r");
    
    while (!feof(f)) {
        i++;
        fscanf(f, "%[^\n]%*c", info[i].name); 
    }
    fclose(f);
    return i;
}

//to export data to file
void exportData(int index) {
    FILE* f = fopen("Name.txt", "a");
    
    fprintf(f, "\n%s", info[index].name);
    fclose(f);
}

//to export search history
void exportHistory(char str[]) {
    FILE* f = fopen("History.txt", "a");
    
    fprintf(f, "\n%s", str);
    fclose(f);
}

//to get string in form of name
void getName(char* ch) {
    int count = 1;
    do {
        if (count > 255) {
            printf("\n\t\t*This string is too long! You cannot enter more!\n");
            printf("\n\t\tPress anykey to continue!");
            getchar();
            break;
        }
        else {
            fflush(stdin);
            do {
                *ch = getche();
                //validate input
                if (!((*ch >= 'a' && *ch <= 'z') || (*ch >= 'A' && *ch <= 'Z') 
                    || *ch == ' ' || *ch == 13 || *ch == 8))
                {
                    printf("\b \b");               
                }
            }
            while (!((*ch >= 'a' && *ch <= 'z') || (*ch >= 'A' && *ch <= 'Z') 
                  || *ch == ' ' || *ch == 13 || *ch == 8));

            //switch to next element
            if (*ch != 13 && *ch != 8) ch++;

            //delete previous element if input backspace
            if (*ch == 8) {
                *ch = ' ';
                printf(" \b");
                ch--;	
            }
            count++;
        }
    } while (*ch != 13);
    *ch = '\0';
}

//to delete a char in a string
void deleteChar(char str[], int pos) {
    int temp = pos + 1;
    while (str[pos] != '\0') {
        str[pos] = str[temp];
        temp++;
        pos++;
    }
}

//to format input
void formatName(char str[]) {
    int count = 0;
    
    //check 1st character
    while (str[count] == ' ') {
        deleteChar(str, count);
    }
    if (str[count] >= 'a' && str[count] <= 'z') str[count] -= 32;
    
    count++;
    //check others characters
    while (str[count+1] != '\0') {
        //delete spare space
        while (str[count] == ' ' && str[count+1] == ' ') {
            deleteChar(str, count+1);
        }
        //capitalize
        if (str[count] == ' ' && str[count+1] <= 'z' && str[count+1] >= 'a') {
            str[count+1] -= 32;
        }
        if (str[count-1] != ' ' && str[count] <= 'Z' && str[count] >= 'A') {
            str[count] += 32;
        }
        count++;
    }
}

//to add new name to file, return the index of final data
int addNewName(int index) {
    //check number of infomation in file
    if (index >= 49) printf("Error. No more space to add more infomation!\n");
    else {
        index++;
        getName(info[index].name);
        formatName(info[index].name);
        exportData(index);
    }
}

void search(int index, char key[]) {
    int i;
    char* rel;
    for (i = 0; i <= index; i++) {
        rel = strstr(info[i].name, key);
        if (rel != NULL) printf("\n%s", info[i].name);
    }
}

void printMenu() {
    system("cls");
    printf("\n\n");
    printf("\t\t1) Input student name.\n");
    printf("\t\t2) Search student name.\n");
    printf("\t\t3) View search history.\n");
    printf("\t\t4) List all the name in ascending order.\n");
    printf("\t\t5) Exit.\n");
    printf("\t\tYour choice: ");
}

int getChoice(int max) {
    int temp, choice;
    char term;
    
    do {
        fflush(stdin);
        temp = scanf("%d%c", &choice, &term);
        
        if (temp != 2 || term != '\n' || choice > max) printf("Error! Enter again!\t");
    } while (temp != 2 || term != '\n' || choice > max);
    
    return choice;
}

//get yes no
char getYesNo() {
    char choice;
    
    do {
        fflush(stdin);
        scanf("%c", &choice);
        
        if (choice != 'Y' && choice != 'n' && choice != 'y' && choice != 'N') printf("Error. Again!\t");
    } while (choice != 'Y' && choice != 'n' && choice != 'y' && choice != 'N');
    
    return choice;
}


//view search history
void viewSearchHistory() {
    FILE* f = fopen("History.txt", "r");
    char str[256];
    
    system("cls");
    printf("\n\n\t\tSearch history:\n");
    
    while (!feof(f)) {
        fscanf(f, "%[^\n]%*c", str);
        printf("\n\t\t%s", str);
    }
    fclose(f);
    getchar();
}

//sort name
void sortName(int numOfStudents) {
    //set a new array to sort
    int i;
    for (i = 0; i <= numOfStudents; i++) {
        strcpy(info[i].nameSorted, info[i].name);
    }
    
    char term[256];
    //two for to sort
    int j;
    for (i = 0; i <= numOfStudents-1; i++) {
        for (j = i+1; j <= numOfStudents; j++) {
            if (info[i].nameSorted[0] > info[j].nameSorted[0]) {
                strcpy(term, info[i].nameSorted);
                strcpy(info[i].nameSorted, info[j].nameSorted);
                strcpy(info[j].nameSorted, term);
            }
        }
    }
    
    system("cls");
    printf("\n\n");
    for (i = 0; i <= numOfStudents; i++) {
        printf("\t\t%s\n", info[i].nameSorted);
    }
    getchar();
}

int main(int argc, char** argv) {
    int choice;
    
    //get data from file
    int numOfStudent = importData();
    
    do {
        printMenu();
        choice = getChoice(5);

        switch (choice) {
            //input student name
            case 1: {
                //check quanlity
                if (numOfStudent >= 50) printf("Full disk! You cannot add more data.\n");
                else {
                    char str[256];
                    
                    system("cls");
                    printf("\n\n");
                    printf("\t\tInput: ");
                    getName(str);
                    formatName(str);
                    
                    printf("\n\t\tDo you want to add %s? [y/n]\t", str);
                    char ans = getYesNo();
                    
                    if (ans == 'Y' || ans == 'y') {
                        numOfStudent++;
                        strcpy(info[numOfStudent].name, str);
                        exportData(numOfStudent);
                        printf("\t\tSuccess!");
                        fflush(stdin);
                        getchar();
                    } 
                }
                break;
            }
            //search
            case 2: {
                char str[256];
                system("cls");
                printf("\n\n\t\tEnter key words: ");
                getName(str);
                formatName(str);
                
                //export to file
                exportHistory(str);
                
                //search
                int i, flag = 0;
                for (i = 0; i <= numOfStudent; i++) {
                    char* rel = strstr(info[i].name, str);
                    
                    if (rel != NULL) {
                        if (!flag) {
                            printf("\n\n\t\tResult:\n");
                            flag = 1;
                        }
                        printf("\t\t%s\n", info[i].name);
                    }
                }
                
                if (!flag) printf("\n\t\tNo result found!\n");
                getchar();
                break;
            }
            //view search history
            case 3: {
                viewSearchHistory();
                break;
            }
            //list all
            case 4: {
                sortName(numOfStudent);
                break;
            }
            //exit
            case 5: {
                
            }
        }
    } while (choice != 5);
    
    return (EXIT_SUCCESS);
}

