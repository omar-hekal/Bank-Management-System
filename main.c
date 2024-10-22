#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include "data.h"


// Enable ANSI REG ADD HKCU\CONSOLE /f /v VirtualTerminalLevel /t REG_DWORD /d 1
User user_construct(char * line){
    User new_user;
    strcpy(new_user.username, strtok(line, " "));
    strcpy(new_user.password, strtok(NULL, "\n")); // bec the last word will contain the \n character in it
    return new_user;
}

void load_users_data(char * file_path){
    FILE * file = fopen(file_path, "r");

    //handling errors for file
    if (file == NULL){ // if not found
        printf(ANSI_COLOR_RED"There is no user account\n"ANSI_COLOR_RESET);
        exit(-1);
    }

    num_of_users = get_num_of_lines(file, file_path);
    rewind(file);
    users_arr = malloc(num_of_users * sizeof(User));

    char line[100];
    for(int i=0; i<num_of_users; i++){
        fgets(line, 100, file);
        users_arr[i] = user_construct(line);
    }
    fclose(file);
}

int get_num_of_lines(FILE *file, char * file_name){
    // check the new line character at the end
    fseek(file, -1, SEEK_END);
    char c = fgetc(file);
    if (c != '\n') {
        FILE * f= fopen(file_name, "a");
        fprintf(f, "\n");
        fclose(f);
    }
    rewind(file);
    // get the new lines
    c  = fgetc(file);
    int count=0;
    while(c != EOF){
        c = fgetc(file);
        if (c == '\n') count ++;
    }
    return count;
}

Date date_construct(char * str){
    Date d;
    d.month = atoi(strtok(str, "-"));
    d.year = atoi(strtok(NULL, "-"));
    return d;
}

Account account_construct(char * line){
    Account acc;

    acc.account_number = atoll(strtok(line, ","));

    strcpy(acc.name, strtok(NULL, ","));

    strcpy(acc.email, strtok(NULL, ","));

    acc.balance = atof(strtok(NULL, ","));

    strcpy(acc.mobile, (strtok(NULL, ",")));

    acc.date_opened = date_construct(strtok(NULL, ","));

    return acc;
}

void load_accounts_data(char * file_path){
    FILE * file = fopen(file_path, "r");

    //handling errors for file
    if (file == NULL){ // if not found
        printf("There is no accounts");
        exit(-1);
    }
    else{
        num_of_accounts = get_num_of_lines(file, file_path);
        char line[100];
        rewind(file);
        accounts_arr = malloc(num_of_accounts * sizeof(Account));
        int x = mkdir("accounts"); // create the folder if not exsist
        for(int i=0; i<num_of_accounts && !feof(file); i++){
            fgets(line, 100, file);
            accounts_arr[i] = account_construct(line);
            max_account_num = max_account_num > accounts_arr[i].account_number ? max_account_num : accounts_arr[i].account_number; // get max account num

            // check if transaction file for each account is existed or not, if not create new one
            char account_file_name[24];
            sprintf(account_file_name, "accounts/%lld.txt", accounts_arr[i].account_number);
            FILE *f = fopen(account_file_name, "a");
            fclose(f);
        }
        fclose(file);
    }
}

void print_account(Account acc){
    printf("Account Number: %lld\n", acc.account_number);
    printf("Name: %s\n", acc.name);
    printf("E-mail: %s\n", acc.email);
    printf("Balance: %.2f $\n", acc.balance);
    printf("Mobile: %s\n", acc.mobile);
    printf("Opened: %s %d\n\n", months[acc.date_opened.month-1], acc.date_opened.year);
}

int compare_by_name(const void* p1,const void* p2){
    Account * acc1 =(Account *) p1, * acc2 = (Account *)p2;
    return stricmp(acc1->name, acc2->name);
}

int compare_by_balance(const void* p1,const void* p2){
    Account * acc1 =(Account *) p1, * acc2 = (Account *)p2;
    // will be sorted in ascending order
    return acc1->balance - acc2->balance;
}

int compare_by_date(const void* p1,const void* p2){
    Account * acc1 =(Account *) p1, * acc2 = (Account *)p2;
    if (acc1->date_opened.year > acc2->date_opened.year) return 1;
    else if (acc1->date_opened.year < acc2->date_opened.year) return -1;
    else{
        if (acc1->date_opened.month > acc2->date_opened.month) return 1;
        else if (acc1->date_opened.month < acc2->date_opened.month) return -1;
        else return 0;
    }
}

void sort_accounts(int type){
    if (type == 1) qsort(accounts_arr, num_of_accounts, sizeof(Account), compare_by_name);
    else if (type == 2) qsort(accounts_arr, num_of_accounts, sizeof(Account), compare_by_balance);
    else if (type == 3) qsort(accounts_arr, num_of_accounts, sizeof(Account), compare_by_date);
}

int check_login_data(char *username, char *password){
    for (int i=0; i<num_of_users; i++){
        if (strcmp(username, users_arr[i].username) == 0){
            if (strcmp(password, users_arr[i].password) == 0){
                return 1;
            }
            else
                return 0;
        }
    }
    return 0;
}

void log_to_progarm(){
    load_accounts_data("accounts.txt");
    int command;
    while(1){
        menu_command();
        printf(">>> ");
        command = get_digit_input();
        switch(command){
            //////////////////////////////////////ADD//////////////////////////////
            case 1:
                add_command();
                break;
            //////////////////////////////////////DELETTE//////////////////////////
            case 2:
                delete_command();
                break;
            //////////////////////////////////////MODIFY///////////////////////////
            case 3:
                modify_command();
                break;
            //////////////////////////////////////SEARCH///////////////////////////
            case 4:
                search_command();
                break;
            ///////////////////////////////////ADVANCED SEARCH/////////////////////
            case 5:
                advanced_search_command();
                break;
            //////////////////////////////////////WITHDRAW/////////////////////////
            case 6:
                withdraw_command();
                break;
            //////////////////////////////////////DEPOSIT//////////////////////////
            case 7:
                deposit_command();
                break;
            //////////////////////////////////////TRANSFER/////////////////////////
            case 8:
                transfer_command();
                break;
            //////////////////////////////////////REPORT///////////////////////////
            case 9:
                report_command();
                break;
            //////////////////////////////////////PRINT////////////////////////////
            case 10:
                print_command();
                break;
            //////////////////////////////////////QUIT/////////////////////////////
            case 11:
                quit_command();
                break;
            default:
                printf(ANSI_COLOR_RED"Incorrect input; kindly select one of the available options\n\n"ANSI_COLOR_RESET);
        }
    }
}

void start_program(){
    printf(ANSI_COLOR_BLUE"\t\t\t\t\tWelcome to Bank Management System\n"ANSI_COLOR_RESET);
    load_users_data("users.txt");
    int not_loggedin = 1, command = 0;
    char command_str[20];
    char username[50];
    char password[50];

    while(not_loggedin){
        menu_command();
        printf(">>> ");

        command = get_digit_input();
        switch(command){
            //////////////////////////////////////LOGIN///////////////////////////
            case 1:
                printf(ANSI_COLOR_YELLOW"Enter your username: "ANSI_COLOR_RESET);
                scanf("%s", username);
                printf(ANSI_COLOR_YELLOW"Enter your password: "ANSI_COLOR_RESET);
                scanf("%s", password);

                if (check_login_data(username, password)){
                    is_logged = 1;
                    not_loggedin = 0;
                    printf(ANSI_COLOR_BLUE"\n\t\t\t\tHello %s\n"ANSI_COLOR_RESET, username);
                    log_to_progarm();
                }
                else{
                    printf(ANSI_COLOR_RED"Incorrect username & password\n\n"ANSI_COLOR_RESET);
                }
                break;

            ////////////////////////////////////////QUIT//////////////////////////
            case 2:
                quit_command();
                break;

            default:
                printf(ANSI_COLOR_RED"Incorrect input; kindly select one of the available options\n\n"ANSI_COLOR_RESET);
        }
    }
}

int get_digit_input(){
    char choice_str[5];
    fflush(stdin);
    fgets(choice_str, 5, stdin);
    choice_str[strlen(choice_str)-1] = '\0';

    if (strlen(choice_str) > 2) return -1;
    else{
        if (isdigit(choice_str[0]) && isdigit(choice_str[1])) return atoi(choice_str);
        else if (isdigit(choice_str[0]) && strlen(choice_str) == 1) return atoi(choice_str);
        else return -1;
    }
}

int check_keyword(char *name, char *keyword){
    char namecopy[50];
    strcpy(namecopy, name);
    char * token = strtok(namecopy, " ");
    do{
        if (stricmp(token, keyword) == 0) return 1;
        token = strtok(NULL, " ");
    }while(token);
    return 0;
}

int validate_name(char *name){
    if(isspace(name[0])) return 0;
    int char_count = 0;
    for (int i=0; i<strlen(name); i++){
        if (!((isalpha(name[i]) || isspace(name[i]))))
            return 0;
        else{
            if (isalpha(name[i])) char_count++;
        }
    }

    if(char_count == 0) return 0;
    return 1;
}

int validate_mobile(char *mobile){
    for (int i=0; i<strlen(mobile); i++)
        if (!((isdigit(mobile[i]))))
            return 0;
    return 1;
}

int validate_email(char *email) {
    int at_num=0, dot_num=0, at_index = -1, dot_index = -1, len=strlen(email);

    if (!isalpha(email[0])) return 0;   //check the first char is alphabetic character
    if (!isalpha(email[len-1])) return 0;   //check the last char is alphabetic character

    for(int i=0; i<strlen(email); i++){
        if (!((isalnum(email[i]) || email[i] == '@' || email[i] == '.' || email[i] == '_' || email[i] == '-')))
            return 0;

        if(email[i] == '@'){
            if (!isalnum(email[i-1]) || !isalnum(email[i+1])) return 0;
            at_num++;
            at_index = i;
        }

        else if(email[i] == '.'){
            if (!isalnum(email[i-1]) || !isalnum(email[i+1])) return 0;
            dot_num++;
            dot_index = i;
        }
        else if(email[i] == '_' || email[i] == '-')
            if (!isalnum(email[i-1]) || !isalnum(email[i+1])) return 0;
    }
    //    no @              no dot           not dot after @
    if (at_index == -1 || dot_index == -1 || dot_index <= at_index) {
        return 0;
    }

    //    check domain size         check top level domain size
    if (dot_index - at_index <= 1 || len - dot_index - 1 < 2) {
        return 0;  // Invalid email structure
    }

    if(at_num>1) return 0;

    return 1;
}

int validate_account_number(char * x){
    if(strlen(x)!=10) return 0;

    for(int i=0;i<10;i++){
        if (!isdigit(x[i])) return 0;
    }
    return 1;
}

int validate_amount(char *amount){
    //check all characters are digits and at most 1 dot
    int dotCount = 0;
    while (*amount){
        if (!isdigit(*amount)){
            if (*amount == '.' && dotCount == 0)
                dotCount++;

            else
                return 0;  // Not a digit or dot

        }
        amount++;
    }
    return 1;
}

void r_strip(char *str){
    // Remove spaces at the end of string
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])){
        str[--len] = '\0';
    }
}

void advanced_search_command(){
    char keyword[20];
    int flag = 1;

    printf(ANSI_COLOR_YELLOW"Enter your keyword: "ANSI_COLOR_RESET);
    scanf("%s", keyword);
    for(int i=0; i<num_of_accounts; i++){
        if (check_keyword(accounts_arr[i].name, keyword)){
            print_account(accounts_arr[i]);
            printf("\n");
            flag = 0;
        }
    }

    if (flag) printf(ANSI_COLOR_RED"No accounts found"ANSI_COLOR_RESET);
}

void search_command(){
    char acc_num_str[11];
    printf(ANSI_COLOR_YELLOW"Please enter the account number for the account: "ANSI_COLOR_RESET);
    scanf("%s",&acc_num_str);

    if (validate_account_number(acc_num_str)==0){
        printf(ANSI_COLOR_RED"error invalid account number\n\n"ANSI_COLOR_RESET);
        return;
    }

    unsigned long long acc_num = atoll(acc_num_str);
    int not_found = 1;

    for(int i=0; i<num_of_accounts; i++){
        if (accounts_arr[i].account_number == acc_num){
            print_account(accounts_arr[i]);
            not_found = 0;
        }
    }

    if (not_found){
        printf(ANSI_COLOR_RED"Account number not found. Please double-check the account number and try again.\n\n"ANSI_COLOR_RESET);
    }
}

void report_command(){
    char acc_num_str[20];
    printf(ANSI_COLOR_YELLOW"Enter the account number: "ANSI_COLOR_RESET);
    scanf("%s", acc_num_str);

    if (!validate_account_number(acc_num_str)){ // check the length of the account number
        printf(ANSI_COLOR_RED"Wrong Account number\n\n"ANSI_COLOR_RESET);
        return;
    }

    unsigned long long acc_num = atoll(acc_num_str);
    int accountIndex = -1;
    for (int i = 0; i < num_of_accounts; i++){
        if (accounts_arr[i].account_number == acc_num){
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1){
        printf(ANSI_COLOR_RED"Account not found\n\n"ANSI_COLOR_RESET);
        return;
    }

    char account_file_name[24];
    sprintf(account_file_name, "accounts/%s.txt", acc_num_str);

    FILE * f=fopen(account_file_name,"r");
    if(f == NULL){
        printf(ANSI_COLOR_RED"Error for opening the file.\n"ANSI_COLOR_RESET);
        return;
    }

    int count = get_num_of_lines(f, account_file_name);
    rewind(f);

    if(count == 0){ // check if the file is empty
        printf(ANSI_COLOR_RED"No transactions found.\n\n"ANSI_COLOR_RESET);
        return;
    }


    char str[100];
    int i;
    if(count<5){
        for(i=0; i<count; i++)
        {
            fgets(str,100,f);
            printf("%s",str);
        }
    }
    else{
        for(i=0; i<(count-5); i++) // get to fifth line from the end
            fgets(str,100,f);

        for(i=0; i<5; i++){
            fgets(str,100,f);
            printf("%s",str);
        }
    }
    fclose(f);
    printf("\n");
}

void delete_command(){
    int i,accountIndex=-1;
    char acc_num_str[100];
    printf(ANSI_COLOR_YELLOW"please enter account number for account to be deleted: "ANSI_COLOR_RESET);
    scanf("%s",&acc_num_str);

    if(validate_account_number(acc_num_str)==0){
        printf(ANSI_COLOR_RED"error invalid account number\n\n"ANSI_COLOR_RESET);
        return;
    }

    unsigned long long acc_num = atoll(acc_num_str);
    for(i=0; i<num_of_accounts; i++)
    {
        if(accounts_arr[i].account_number==acc_num)
        {
            accountIndex=i;
            break;
        }
    }
    if(accountIndex==-1)
    {
        printf(ANSI_COLOR_RED"error account number does not exist\n\n"ANSI_COLOR_RESET);
        return;
    }

    if(accounts_arr[accountIndex].balance >= 0.01)
    {
        printf(ANSI_COLOR_RED"error account cant be deleted because balance is greater than zero\n\n"ANSI_COLOR_RESET);
        return ;
    }

    // exchange the account required to be deleted by the last account
    accounts_arr[accountIndex].account_number=accounts_arr[num_of_accounts-1].account_number;
    accounts_arr[accountIndex].balance=accounts_arr[num_of_accounts-1].balance;
    accounts_arr[accountIndex].date_opened.month=accounts_arr[num_of_accounts-1].date_opened.month;
    accounts_arr[accountIndex].date_opened.year=accounts_arr[num_of_accounts-1].date_opened.year;
    strcpy(accounts_arr[accountIndex].name,accounts_arr[num_of_accounts-1].name);
    strcpy(accounts_arr[accountIndex].email,accounts_arr[num_of_accounts-1].email);
    strcpy(accounts_arr[accountIndex].mobile,accounts_arr[num_of_accounts-1].mobile);

    --num_of_accounts;
    if (save_command()){
        char account_file_name[24];
        sprintf(account_file_name, "accounts/%lld.txt", acc_num);
        remove(account_file_name);
    }
}

void modify_command(){
    int i,accountIndex=-1,modify=0;
    char acc_num_str[11];
    int wrong_input = 0;
    char new_name[50], new_mobile[20], new_email[50];
    printf(ANSI_COLOR_YELLOW"please enter the account number for the account: "ANSI_COLOR_RESET);
    scanf("%s",&acc_num_str);

    if (validate_account_number(acc_num_str)==0){
        printf(ANSI_COLOR_RED"error invalid account number\n\n"ANSI_COLOR_RESET);
        return;
    }

    unsigned long long acc_num = atoll(acc_num_str);
    for(i=0; i<num_of_accounts; i++)
    {
        if(acc_num==accounts_arr[i].account_number)
        {
            accountIndex=i;
            break;
        }
    }
    if(accountIndex==-1)
    {
        printf(ANSI_COLOR_RED"error account number doesnt exist\n\n"ANSI_COLOR_RESET);
        return;
    }

    while (1){
    printf(ANSI_COLOR_YELLOW"\nType 1 to modify name, 2 to modify mobile, 3 to modify email address and 0 to exit\n>>> "ANSI_COLOR_RESET);
    modify = get_digit_input();

    if(modify==1){
        printf(ANSI_COLOR_YELLOW"enter the new name: "ANSI_COLOR_RESET);
        fflush(stdin);
        gets(new_name);
        if(!validate_name(new_name)){
            printf(ANSI_COLOR_RED"Invalid name format\n"ANSI_COLOR_RESET);
            wrong_input = 1;
        }
        if (wrong_input){
            wrong_input = 0;
        }
        else strcpy(accounts_arr[accountIndex].name, new_name);
    }

    else if(modify==2){
        printf(ANSI_COLOR_YELLOW"enter the new mobile number: "ANSI_COLOR_RESET);
        fflush(stdin);
        gets(new_mobile);
        if(!validate_mobile(new_mobile)){
            printf(ANSI_COLOR_RED"Invalid mobile format\n"ANSI_COLOR_RESET);
            wrong_input = 1;
        }
        if (wrong_input){
            wrong_input = 0;
        }
        else strcpy(accounts_arr[accountIndex].mobile, new_mobile);
    }

    else if(modify==3){
        printf(ANSI_COLOR_YELLOW"enter the new email address: "ANSI_COLOR_RESET);
        fflush(stdin);
        gets(new_email);
        if(!validate_email(new_email)){
            printf(ANSI_COLOR_RED"Invalid email format\n"ANSI_COLOR_RESET);
            wrong_input = 1;
        }
        if (wrong_input){
            wrong_input = 0;
        }
        else strcpy(accounts_arr[accountIndex].email, new_email);
    }

    else if(modify==0) break;
    else printf(ANSI_COLOR_RED"Invalid input; please choose one of the options\n\n"ANSI_COLOR_RESET);
    }

    save_command();
}

void add_command(){
    char acc_num_str[11];
    char wrong_input = 0;
    char new_name[50], new_mobile[20], new_email[50];
    Account newAccount;
    newAccount.account_number = max_account_num + 1;
    printf("Enter new account details: \n\n");


    ///////////////////////getting name/////////////////////////////////////////
    printf(ANSI_COLOR_YELLOW"Enter name: "ANSI_COLOR_RESET);
    fflush(stdin);
    fgets(new_name, sizeof(new_name), stdin);
    new_name[strcspn(new_name, "\n")] = '\0'; // Remove newline character
    r_strip(new_name); // Remove spaces at the end

    while(!validate_name(new_name)){  // check if the name is valid or not
        printf(ANSI_COLOR_RED"Invalid name format\n\n"ANSI_COLOR_RESET);

        int choice = -1;
        while(choice != 0 && choice != 1){
            printf(ANSI_COLOR_YELLOW"Do you want to continue? yes:1 no:0 "ANSI_COLOR_RESET);
            choice = get_digit_input();
            if (choice == 1){
                printf(ANSI_COLOR_YELLOW"Enter name: "ANSI_COLOR_RESET);
                fflush(stdin);
                fgets(new_name, sizeof(new_name), stdin);
                new_name[strcspn(new_name, "\n")] = '\0'; // Remove newline character
                r_strip(new_name); // Remove spaces at the end
            }

            else if (choice == 0){
                return; // return back to main menu
            }
        }

    }

    strcpy(newAccount.name, new_name);


    ///////////////////////////////getting mobile///////////////////////////
    printf(ANSI_COLOR_YELLOW"Enter mobile: "ANSI_COLOR_RESET);
    fgets(new_mobile, sizeof(new_mobile), stdin);
    new_mobile[strcspn(new_mobile, "\n")] = '\0'; // Remove newline character
    r_strip(new_mobile); // Remove spaces at the end

    while(!validate_mobile(new_mobile)){  // check if the name is valid or not
        printf(ANSI_COLOR_RED"Invalid mobile format\n\n"ANSI_COLOR_RESET);

        int choice = -1;
        while(choice != 0 && choice != 1){
            printf(ANSI_COLOR_YELLOW"Do you want to continue? yes:1 no:0 "ANSI_COLOR_RESET);
            choice = get_digit_input();
            if (choice == 1){
                printf(ANSI_COLOR_YELLOW"Enter mobile: "ANSI_COLOR_RESET);
                fflush(stdin);
                fgets(new_mobile, sizeof(new_mobile), stdin);
                new_mobile[strcspn(new_mobile, "\n")] = '\0'; // Remove newline character
                r_strip(new_mobile); // Remove spaces at the end
            }

            else if (choice == 0){
                return; // return back to main menu
            }
        }

    }

    strcpy(newAccount.mobile, new_mobile);


    ///////////////////////////////////getting email//////////////////////////
    printf(ANSI_COLOR_YELLOW"Enter email: "ANSI_COLOR_RESET);
    fgets(new_email, sizeof(new_email), stdin);
    new_email[strcspn(new_email, "\n")] = '\0'; // Remove newline character
    r_strip(new_email); // Remove spaces at the end

    while(!validate_email(new_email)){  // check if the name is valid or not
        printf(ANSI_COLOR_RED"Invalid email format\n\n"ANSI_COLOR_RESET);

        int choice = -1;
        while(choice != 0 && choice != 1){
            printf(ANSI_COLOR_YELLOW"Do you want to continue? yes:1 no:0 "ANSI_COLOR_RESET);
            choice = get_digit_input();
            if (choice == 1){
                printf(ANSI_COLOR_YELLOW"Enter email: "ANSI_COLOR_RESET);
                fflush(stdin);
                fgets(new_email, sizeof(new_email), stdin);
                new_email[strcspn(new_email, "\n")] = '\0'; // Remove newline character
                r_strip(new_email); // Remove spaces at the end
            }

            else if (choice == 0){
                return; // return back to main menu
            }
        }

    }
    strcpy(newAccount.email, new_email);

    newAccount.balance=0;

    // Date Opened (current system date)
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    newAccount.date_opened.month = now->tm_mon + 1;
    newAccount.date_opened.year = now->tm_year + 1900;

    // Add the new account to the array
    num_of_accounts++;
    accounts_arr = realloc(accounts_arr, num_of_accounts * sizeof(Account));
    accounts_arr[num_of_accounts - 1] = newAccount;

    if (save_command())
        printf(ANSI_COLOR_GREEN"New Account is added successfully.\n"ANSI_COLOR_RESET);
    else
        return;
}

void withdraw_command(){
    char acc_num_str[11],amount_str[20];
    float amount;

    printf(ANSI_COLOR_YELLOW"Enter account number for withdrawal: "ANSI_COLOR_RESET);
    scanf("%s",&acc_num_str);
    if (validate_account_number(acc_num_str)==0)
    {
        printf(ANSI_COLOR_RED"Error invalid account number\n\n"ANSI_COLOR_RESET);
        return;
    }

    unsigned long long acc_num = atoll(acc_num_str);
    int accountIndex = -1;
    for (int i = 0; i < num_of_accounts; i++)
    {
        if (accounts_arr[i].account_number == acc_num)
        {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1)
    {
        printf(ANSI_COLOR_RED"Account not found\n\n"ANSI_COLOR_RESET);
        return;
    }

    printf(ANSI_COLOR_YELLOW"Enter withdrawal amount (max 10,000 $): "ANSI_COLOR_RESET);
    scanf("%s", amount_str);

    if(!validate_amount(amount_str)){
        printf(ANSI_COLOR_RED"Invalid amount format\n\n"ANSI_COLOR_RESET);
        return;
    }
    else amount = atof(amount_str);

    if (amount <= 0 || amount > 10000)
    {
        printf(ANSI_COLOR_RED"Invalid withdrawal amount\n\n"ANSI_COLOR_RESET);
        return;
    }

    if (amount > accounts_arr[accountIndex].balance)
    {
        printf(ANSI_COLOR_RED"Insufficient balance, Current Balance = %.2f\n\n"ANSI_COLOR_RESET, accounts_arr[accountIndex].balance);
        return;
    }

    // Perform withdrawal
    accounts_arr[accountIndex].balance -= amount;

    if (save_command()){
        char account_file_name[24];
        sprintf(account_file_name, "accounts/%lld.txt", accounts_arr[accountIndex].account_number);
        FILE *ptr= fopen(account_file_name,"a");
        fprintf(ptr,"Withdraw:%.2f, Balance:%.2f $\n",amount,accounts_arr[accountIndex].balance);
        fclose(ptr);

        printf(ANSI_COLOR_GREEN"Withdrawal successful. New balance: %.2f $\n"ANSI_COLOR_RESET, accounts_arr[accountIndex].balance);
    }
    else
        return;
}

void deposit_command(){
    char acc_num_str[11],amount_str[20];
    char wrong_input = 0;
    float amount;

    printf(ANSI_COLOR_YELLOW"Enter account number for deposit: "ANSI_COLOR_RESET);
    scanf("%s",&acc_num_str);
    if (validate_account_number(acc_num_str)==0)
    {
        printf(ANSI_COLOR_RED"Error invalid account number\n\n"ANSI_COLOR_RESET);
        return;
    }

    unsigned long long acc_num = atoll(acc_num_str);
    int accountIndex = -1;
    for (int i = 0; i < num_of_accounts; i++)
    {
        if (accounts_arr[i].account_number == acc_num)
        {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1){
        printf(ANSI_COLOR_RED"Account not found\n\n"ANSI_COLOR_RESET);
        return;
    }

    printf(ANSI_COLOR_YELLOW"Enter deposit amount (max 10,000 $): "ANSI_COLOR_RESET);
    scanf("%s", amount_str);

    if(!validate_amount(amount_str)){
        printf(ANSI_COLOR_RED"Invalid amount format\n\n"ANSI_COLOR_RESET);
        return;
    }
    else amount = atof(amount_str);

    if (amount <= 0 || amount > 10000){
        printf(ANSI_COLOR_RED"Invalid deposit amount\n\n"ANSI_COLOR_RESET);
        return;
    }

    // Perform deposit
    accounts_arr[accountIndex].balance += amount;

    if (save_command()){
        char account_file_name[24];
        sprintf(account_file_name, "accounts/%lld.txt", accounts_arr[accountIndex].account_number);
        FILE *ptr= fopen(account_file_name,"a");
        fprintf(ptr,"Deposit:%.2f, Balance:%.2f $\n",amount,accounts_arr[accountIndex].balance);
        fclose(ptr);

        printf(ANSI_COLOR_GREEN"Deposit successful. New balance: %.2f $\n"ANSI_COLOR_RESET, accounts_arr[accountIndex].balance);
    }
    else
        return;
}

void transfer_command(){
    char sender_acc_num_str[11],receiver_acc_num_str[11],amount_str[20];
    char wrong_input = 0;
    float amount;

    printf(ANSI_COLOR_YELLOW"Enter sender account number: "ANSI_COLOR_RESET);
    scanf("%s",&sender_acc_num_str);
    if (validate_account_number(sender_acc_num_str)==0)
    {
        printf(ANSI_COLOR_RED"Error invalid account number\n\n"ANSI_COLOR_RESET);
        return;
    }

    unsigned long long sender_acc_num = atoll(sender_acc_num_str);
    // Find the sender account
    int senderIndex = -1;
    for (int i = 0; i < num_of_accounts; i++)
    {
        if (accounts_arr[i].account_number == sender_acc_num)
        {
            senderIndex = i;
            break;
        }
    }

    if (senderIndex == -1)
    {
        printf(ANSI_COLOR_RED"Sender account not found\n\n"ANSI_COLOR_RESET);
        return;
    }


    printf(ANSI_COLOR_YELLOW"Enter receiver account number: "ANSI_COLOR_RESET);
    scanf("%s",&receiver_acc_num_str);
    if (validate_account_number(receiver_acc_num_str)==0)
    {
        printf(ANSI_COLOR_RED"Error invalid account number\n\n"ANSI_COLOR_RESET);
        return;
    }

    unsigned long long receiver_acc_num = atoll(receiver_acc_num_str);
    // Find the receiver account
    int receiverIndex = -1;
    for (int i = 0; i < num_of_accounts; i++)
    {
        if (accounts_arr[i].account_number == receiver_acc_num)
        {
            receiverIndex = i;
            break;
        }
    }

    if (receiverIndex == -1)
    {
        printf(ANSI_COLOR_RED"Receiver account not found\n\n"ANSI_COLOR_RESET);
        return;
    }

    if(receiver_acc_num == sender_acc_num){
        printf(ANSI_COLOR_RED"Receiver and Sender are the same\n\n"ANSI_COLOR_RESET);
        return;
    }

    printf(ANSI_COLOR_YELLOW"Enter transfer amount: "ANSI_COLOR_RESET);
    scanf("%s", amount_str);

    if(!validate_amount(amount_str))
    {
        printf(ANSI_COLOR_RED"Invalid amount format\n\n"ANSI_COLOR_RESET);
        return;
    }
    else amount = atof(amount_str);

    if (amount <= 0)
    {
        printf(ANSI_COLOR_RED"Invalid transfer amount\n\n"ANSI_COLOR_RESET);
        return;
    }

    if (amount > accounts_arr[senderIndex].balance)
    {
        printf(ANSI_COLOR_RED"Insufficient balance in the sender account\n\n"ANSI_COLOR_RESET);
        return;
    }

    // Perform transfer
    accounts_arr[senderIndex].balance -= amount;
    accounts_arr[receiverIndex].balance += amount;

    if(save_command())
    {
        FILE * ptr;
        // Record transactions for sender
        char account_file_name[24];
        sprintf(account_file_name, "accounts/%lld.txt", accounts_arr[senderIndex].account_number);
        ptr= fopen(account_file_name,"a");
        fprintf(ptr,"Transfer (Outgoing):%.2f, Balance:%.2f $\n",amount,accounts_arr[senderIndex].balance);
        fclose(ptr);

        // Record transactions for receiver
        sprintf(account_file_name, "accounts/%lld.txt", accounts_arr[receiverIndex].account_number);
        ptr= fopen(account_file_name,"a");
        fprintf(ptr,"Transfer (Incoming):%.2f, Balance:%.2f $\n",amount,accounts_arr[receiverIndex].balance);
        fclose(ptr);

        printf(ANSI_COLOR_GREEN"Transfer successful. New balance in sender's account: %.2f $\n"ANSI_COLOR_RESET, accounts_arr[senderIndex].balance);
        printf(ANSI_COLOR_GREEN"Transfer successful. New balance in receiver's account: %.2f $\n\n"ANSI_COLOR_RESET, accounts_arr[receiverIndex].balance);
    }
    else
        return;
}

void print_command(){
    int type;
    printf(ANSI_COLOR_YELLOW"Could you please specify the sorting order you prefer for the data to be printed?\n"ANSI_COLOR_RESET);
    printf("1 for sorted by name / 2 for sorted by balance / 3 for sorted by open date\n");
    printf(">>> ");
    type = get_digit_input();
    while(type<1 || type>3){
        printf(ANSI_COLOR_RED"Incorrect input: kindly select one of the available options\n"ANSI_COLOR_RESET);
        printf(">>> ");
        type = get_digit_input();
    }
    sort_accounts(type);

    int table = -1;
    while(table !=0 && table != 1){
        printf(ANSI_COLOR_YELLOW"Do you want to print it in table format? yes:1 no:0 "ANSI_COLOR_RESET);
        table = get_digit_input();
    }

    if(type == 1) printf(ANSI_COLOR_GREEN"Sorted by names(a-z)\n"ANSI_COLOR_RESET);
    else if(type == 2) printf(ANSI_COLOR_GREEN"Sorted by Balance (ascendingly)\n"ANSI_COLOR_RESET);
    else if(type == 3) printf(ANSI_COLOR_GREEN"Sorted by Date (old-new)\n"ANSI_COLOR_RESET);


    if (table == 1){
        print_table();
        printf("\n");
    }
    else if (table == 0){
        for(int i=0; i<num_of_accounts; i++){
            print_account(accounts_arr[i]);
        }
        printf("\n");
    }
}

void print_table(){
    printf(ANSI_COLOR_GREEN"%-10s | %-20s | %-25s | %-12s | %-15s | %-10s | %-4s\n"ANSI_COLOR_RESET, "Number", "Name", "Email", "Balance", "Mobile", "Month", "Year");
    printf(ANSI_COLOR_GREEN"--------------------------------------------------------------------------------------------------------------------\n"ANSI_COLOR_RESET);
    for (int i = 0; i<num_of_accounts; i++){
        Account acc = accounts_arr[i];
        printf("%lld | %-20s | %-25s | %-12.2f | %-15s | %-10s | %-4d\n", acc.account_number, acc.name, acc.email, acc.balance, acc.mobile, months[acc.date_opened.month-1], acc.date_opened.year);
    }

}

void quit_command(){
    printf(ANSI_COLOR_GREEN"\nThank you for using our program.\n"ANSI_COLOR_RESET);
    exit(0);
}

void menu_command(){
    if (is_logged){
        printf("==================================================================================================================\n");
        printf(ANSI_COLOR_YELLOW"Commands:\n"ANSI_COLOR_RESET);
        printf("\t1. Add          2. Delete              3. Modify\n");
        printf("\t4. Search       5. Advanced Search     6. Withdraw\n");
        printf("\t7. Deposit      8. Transfer            9. Report\n");
        printf("\t10. Print       11. Quit\n");
    }
    else{
        printf(ANSI_COLOR_YELLOW"Commands:\n"ANSI_COLOR_RESET);
        printf("\t1. Login\n");
        printf("\t2. Quit\n");
    }
}

int save_command(){
    int x;
    printf(ANSI_COLOR_YELLOW"type 1 to save date or type zero to discard changes: "ANSI_COLOR_RESET);
    x = get_digit_input();
    if(x==0){
        free(accounts_arr);
        load_accounts_data("accounts.txt");
        return 0;
    }
    else if(x==1){
        FILE *ptr=fopen("accounts.txt","w");
        for(int i=0; i<num_of_accounts; i++)
            fprintf(ptr,"%lld,%s,%s,%.2f,%s,%d-%d\n",accounts_arr[i].account_number,accounts_arr[i].name,accounts_arr[i].email,accounts_arr[i].balance,accounts_arr[i].mobile,accounts_arr[i].date_opened.month,accounts_arr[i].date_opened.year);
        fclose(ptr);
        free(accounts_arr);
        load_accounts_data("accounts.txt");
        printf(ANSI_COLOR_GREEN"Data Saved\n\n"ANSI_COLOR_RESET);
        return 1;
    }
    else{
        printf(ANSI_COLOR_RED"invalid input\n\n"ANSI_COLOR_RESET);
        save_command();
    }

}


void main(){
    start_program();
}
