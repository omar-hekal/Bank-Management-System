#ifndef DATA

#define DATA

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"



typedef struct{
    int month, year;
} Date;

typedef struct{
    unsigned long long account_number;
    char name[50];
    char mobile[20];
    float balance;
    char email[50];
    Date date_opened;
} Account;

char months [][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

typedef struct{
    char username[50];
    char password[50];
} User;


Account * accounts_arr;
User * users_arr;
int num_of_accounts;
unsigned long long max_account_num=0;
int num_of_users;
int is_logged = 0;

User user_construct(char *line);
void load_users_data();
int get_num_of_lines();
Date date_construct();
Account account_construct();
void load_accounts_data();
void print_account();
int compare_by_name();
int compare_by_balance();
int compare_by_date();
void sort_accounts();
int check_login_data();
void log_to_progarm();
void start_program();
int get_digit_input();
int check_keyword();
int validate_name();
int validate_mobile();
int validate_email();
int validate_account_number();
int validate_amount();
void r_strip();
void advanced_search_command();
void search_command();
void report_command();
void delete_command();
void modify_command();
void add_command();
void withdraw_command();
void deposit_command();
void transfer_command();
void print_command();
void print_table();
void quit_command();
void menu_command();
int save_command();


#endif
