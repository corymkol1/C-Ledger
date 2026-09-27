#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TRANSACTIONS 1000
#define DESCRIPTION_LENGTH 100
#define CATEGORY_LENGTH 50
#define DATE_LENGTH 11
#define SEARCH_LENGTH 100

#define DATA_FILE "transactions.dat"
#define CSV_FILE "transactions.csv"

typedef struct
{
    int id;
    char date[DATE_LENGTH];
    char description[DESCRIPTION_LENGTH];
    char category[CATEGORY_LENGTH];
    double amount;
    char type; // I = Income, E = Expense
} Transaction;


// Function prototypes
void display_menu(void);

void add_transaction(
    Transaction transactions[],
    int *count,
    char type
);

void view_transactions(
    Transaction transactions[],
    int count
);

void view_summary(
    Transaction transactions[],
    int count
);

void search_transactions(
    Transaction transactions[],
    int count
);

void edit_transaction(
    Transaction transactions[],
    int count
);

void delete_transaction(
    Transaction transactions[],
    int *count
);

void save_transactions(
    Transaction transactions[],
    int count
);

void load_transactions(
    Transaction transactions[],
    int *count
);

void export_csv(
    Transaction transactions[],
    int count
);

void choose_category(
    char category[],
    char type
);

void clear_input_buffer(void);

void remove_newline(char string[]);

int get_transaction_index_by_id(
    Transaction transactions[],
    int count,
    int id
);

void reassign_ids(
    Transaction transactions[],
    int count
);

void to_lowercase(
    char destination[],
    const char source[]
);


// Main program
int main(void)
{
    Transaction transactions[MAX_TRANSACTIONS];

    int transaction_count = 0;
    int choice = 0;

    load_transactions(
        transactions,
        &transaction_count
    );

    do
    {
        display_menu();

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input. Please enter a number.\n");

            clear_input_buffer();

            continue;
        }

        clear_input_buffer();

        switch (choice)
        {
            case 1:
                add_transaction(
                    transactions,
                    &transaction_count,
                    'I'
                );

                save_transactions(
                    transactions,
                    transaction_count
                );

                break;

            case 2:
                add_transaction(
                    transactions,
                    &transaction_count,
                    'E'
                );

                save_transactions(
                    transactions,
                    transaction_count
                );

                break;

            case 3:
                view_transactions(
                    transactions,
                    transaction_count
                );

                break;

            case 4:
                view_summary(
                    transactions,
                    transaction_count
                );

                break;

            case 5:
                search_transactions(
                    transactions,
                    transaction_count
                );

                break;

            case 6:
                edit_transaction(
                    transactions,
                    transaction_count
                );

                save_transactions(
                    transactions,
                    transaction_count
                );

                break;

            case 7:
                delete_transaction(
                    transactions,
                    &transaction_count
                );

                save_transactions(
                    transactions,
                    transaction_count
                );

                break;

            case 8:
                export_csv(
                    transactions,
                    transaction_count
                );

                break;

            case 9:
                save_transactions(
                    transactions,
                    transaction_count
                );

                printf("\nData saved.\n");
                printf("Goodbye!\n");

                break;

            default:
                printf("\nInvalid selection.\n");
        }

    } while (choice != 9);

    return 0;
}


// Display menu
void display_menu(void)
{
    printf("\n");
    printf("=========================================\n");
    printf("        SMALL BUSINESS BOOKKEEPER\n");
    printf("=========================================\n");

    printf("1. Add Income\n");
    printf("2. Add Expense\n");
    printf("3. View Transactions\n");
    printf("4. View Financial Summary\n");
    printf("5. Search Transactions\n");
    printf("6. Edit Transaction\n");
    printf("7. Delete Transaction\n");
    printf("8. Export Transactions to CSV\n");
    printf("9. Save and Exit\n");

    printf("=========================================\n");
}


// Add transaction
void add_transaction(
    Transaction transactions[],
    int *count,
    char type
)
{
    if (*count >= MAX_TRANSACTIONS)
    {
        printf("\nMaximum transaction limit reached.\n");

        return;
    }

    Transaction transaction;

    transaction.id = *count + 1;
    transaction.type = type;

    printf("\nEnter date (MM/DD/YYYY): ");

    fgets(
        transaction.date,
        DATE_LENGTH,
        stdin
    );

    remove_newline(transaction.date);

    // If user entered more characters than buffer allowed,
    // clear anything remaining.
    if (strlen(transaction.date) == DATE_LENGTH - 1)
    {
        clear_input_buffer();
    }

    printf("Enter description: ");

    fgets(
        transaction.description,
        DESCRIPTION_LENGTH,
        stdin
    );

    remove_newline(transaction.description);

    choose_category(
        transaction.category,
        type
    );

    printf("Enter amount: $");

    while (
        scanf(
            "%lf",
            &transaction.amount
        ) != 1
    )
    {
        printf(
            "Invalid amount. Enter amount: $"
        );

        clear_input_buffer();
    }

    clear_input_buffer();

    if (transaction.amount < 0)
    {
        transaction.amount *= -1;
    }

    transactions[*count] = transaction;

    (*count)++;

    printf(
        "\nTransaction #%d added successfully.\n",
        transaction.id
    );
}


// Category menu
void choose_category(
    char category[],
    char type
)
{
    int choice = 0;

    if (type == 'I')
    {
        printf("\nIncome Categories\n");
        printf("-----------------\n");
        printf("1. Service Revenue\n");
        printf("2. Sales Revenue\n");
        printf("3. Interest Income\n");
        printf("4. Other Income\n");
        printf("5. Custom Category\n");

        printf("Choose category: ");

        scanf("%d", &choice);

        clear_input_buffer();

        switch (choice)
        {
            case 1:
                strcpy(
                    category,
                    "Service Revenue"
                );
                break;

            case 2:
                strcpy(
                    category,
                    "Sales Revenue"
                );
                break;

            case 3:
                strcpy(
                    category,
                    "Interest Income"
                );
                break;

            case 4:
                strcpy(
                    category,
                    "Other Income"
                );
                break;

            case 5:
                printf(
                    "Enter custom category: "
                );

                fgets(
                    category,
                    CATEGORY_LENGTH,
                    stdin
                );

                remove_newline(category);

                break;

            default:
                strcpy(
                    category,
                    "Other Income"
                );
        }
    }
    else
    {
        printf("\nExpense Categories\n");
        printf("------------------\n");
        printf("1. Advertising\n");
        printf("2. Office Supplies\n");
        printf("3. Software\n");
        printf("4. Rent\n");
        printf("5. Utilities\n");
        printf("6. Insurance\n");
        printf("7. Professional Fees\n");
        printf("8. Travel\n");
        printf("9. Meals\n");
        printf("10. Vehicle Expense\n");
        printf("11. Bank Fees\n");
        printf("12. Other Expense\n");
        printf("13. Custom Category\n");

        printf("Choose category: ");

        scanf("%d", &choice);

        clear_input_buffer();

        switch (choice)
        {
            case 1:
                strcpy(
                    category,
                    "Advertising"
                );
                break;

            case 2:
                strcpy(
                    category,
                    "Office Supplies"
                );
                break;

            case 3:
                strcpy(
                    category,
                    "Software"
                );
                break;

            case 4:
                strcpy(
                    category,
                    "Rent"
                );
                break;

            case 5:
                strcpy(
                    category,
                    "Utilities"
                );
                break;

            case 6:
                strcpy(
                    category,
                    "Insurance"
                );
                break;

            case 7:
                strcpy(
                    category,
                    "Professional Fees"
                );
                break;

            case 8:
                strcpy(
                    category,
                    "Travel"
                );
                break;

            case 9:
                strcpy(
                    category,
                    "Meals"
                );
                break;

            case 10:
                strcpy(
                    category,
                    "Vehicle Expense"
                );
                break;

            case 11:
                strcpy(
                    category,
                    "Bank Fees"
                );
                break;

            case 12:
                strcpy(
                    category,
                    "Other Expense"
                );
                break;

            case 13:
                printf(
                    "Enter custom category: "
                );

                fgets(
                    category,
                    CATEGORY_LENGTH,
                    stdin
                );

                remove_newline(category);

                break;

            default:
                strcpy(
                    category,
                    "Other Expense"
                );
        }
    }
}


// View all transactions
void view_transactions(
    Transaction transactions[],
    int count
)
{
    if (count == 0)
    {
        printf("\nNo transactions available.\n");

        return;
    }

    printf("\n");
    printf(
        "====================================================================================\n"
    );

    printf(
        "%-4s %-11s %-9s %-20s %-12s %s\n",
        "ID",
        "Date",
        "Type",
        "Category",
        "Amount",
        "Description"
    );

    printf(
        "====================================================================================\n"
    );

    for (int i = 0; i < count; i++)
    {
        char *type_name;

        if (transactions[i].type == 'I')
        {
            type_name = "Income";
        }
        else
        {
            type_name = "Expense";
        }

        printf(
            "%-4d %-11s %-9s %-20s $%-11.2f %s\n",
            transactions[i].id,
            transactions[i].date,
            type_name,
            transactions[i].category,
            transactions[i].amount,
            transactions[i].description
        );
    }

    printf(
        "====================================================================================\n"
    );
}


// Financial summary
void view_summary(
    Transaction transactions[],
    int count
)
{
    double total_income = 0.0;
    double total_expenses = 0.0;

    for (int i = 0; i < count; i++)
    {
        if (transactions[i].type == 'I')
        {
            total_income +=
                transactions[i].amount;
        }

        else if (
            transactions[i].type == 'E'
        )
        {
            total_expenses +=
                transactions[i].amount;
        }
    }

    double net_income =
        total_income - total_expenses;

    printf("\n");
    printf("=========================================\n");
    printf("            FINANCIAL SUMMARY\n");
    printf("=========================================\n");

    printf(
        "Total Income:             $%10.2f\n",
        total_income
    );

    printf(
        "Total Expenses:           $%10.2f\n",
        total_expenses
    );

    printf("-----------------------------------------\n");

    printf(
        "Net Income:               $%10.2f\n",
        net_income
    );

    printf("-----------------------------------------\n");

    if (net_income > 0)
    {
        printf("Business Status: PROFIT\n");
    }

    else if (net_income < 0)
    {
        printf("Business Status: LOSS\n");
    }

    else
    {
        printf("Business Status: BREAK EVEN\n");
    }

    printf("=========================================\n");
}


// Search transactions
void search_transactions(
    Transaction transactions[],
    int count
)
{
    if (count == 0)
    {
        printf("\nNo transactions to search.\n");

        return;
    }

    char search[SEARCH_LENGTH];
    char search_lower[SEARCH_LENGTH];

    printf(
        "\nEnter description or category to search: "
    );

    fgets(
        search,
        SEARCH_LENGTH,
        stdin
    );

    remove_newline(search);

    to_lowercase(
        search_lower,
        search
    );

    int found = 0;

    printf("\nSearch Results\n");
    printf(
        "====================================================================================\n"
    );

    for (int i = 0; i < count; i++)
    {
        char description_lower[
            DESCRIPTION_LENGTH
        ];

        char category_lower[
            CATEGORY_LENGTH
        ];

        to_lowercase(
            description_lower,
            transactions[i].description
        );

        to_lowercase(
            category_lower,
            transactions[i].category
        );

        if (
            strstr(
                description_lower,
                search_lower
            ) != NULL
            ||
            strstr(
                category_lower,
                search_lower
            ) != NULL
        )
        {
            char *type_name =
                transactions[i].type == 'I'
                ? "Income"
                : "Expense";

            printf(
                "#%d | %s | %s | %s | $%.2f | %s\n",
                transactions[i].id,
                transactions[i].date,
                type_name,
                transactions[i].category,
                transactions[i].amount,
                transactions[i].description
            );

            found = 1;
        }
    }

    if (!found)
    {
        printf(
            "No matching transactions found.\n"
        );
    }

    printf(
        "====================================================================================\n"
    );
}


// Edit transaction
void edit_transaction(
    Transaction transactions[],
    int count
)
{
    if (count == 0)
    {
        printf(
            "\nNo transactions available to edit.\n"
        );

        return;
    }

    view_transactions(
        transactions,
        count
    );

    int id;

    printf(
        "\nEnter transaction ID to edit: "
    );

    scanf("%d", &id);

    clear_input_buffer();

    int index =
        get_transaction_index_by_id(
            transactions,
            count,
            id
        );

    if (index == -1)
    {
        printf(
            "\nTransaction not found.\n"
        );

        return;
    }

    Transaction *transaction =
        &transactions[index];

    printf(
        "\nEditing Transaction #%d\n",
        transaction->id
    );

    printf(
        "Current date: %s\n",
        transaction->date
    );

    printf(
        "Enter new date: "
    );

    fgets(
        transaction->date,
        DATE_LENGTH,
        stdin
    );

    remove_newline(
        transaction->date
    );

    if (
        strlen(transaction->date)
        == DATE_LENGTH - 1
    )
    {
        clear_input_buffer();
    }

    printf(
        "Current description: %s\n",
        transaction->description
    );

    printf(
        "Enter new description: "
    );

    fgets(
        transaction->description,
        DESCRIPTION_LENGTH,
        stdin
    );

    remove_newline(
        transaction->description
    );

    printf(
        "Current category: %s\n",
        transaction->category
    );

    choose_category(
        transaction->category,
        transaction->type
    );

    printf(
        "Current amount: $%.2f\n",
        transaction->amount
    );

    printf(
        "Enter new amount: $"
    );

    scanf(
        "%lf",
        &transaction->amount
    );

    clear_input_buffer();

    if (
        transaction->amount < 0
    )
    {
        transaction->amount *= -1;
    }

    printf(
        "\nTransaction updated successfully.\n"
    );
}


// Delete transaction
void delete_transaction(
    Transaction transactions[],
    int *count
)
{
    if (*count == 0)
    {
        printf(
            "\nNo transactions available to delete.\n"
        );

        return;
    }

    view_transactions(
        transactions,
        *count
    );

    int id;

    printf(
        "\nEnter transaction ID to delete: "
    );

    scanf("%d", &id);

    clear_input_buffer();

    int index =
        get_transaction_index_by_id(
            transactions,
            *count,
            id
        );

    if (index == -1)
    {
        printf(
            "\nTransaction not found.\n"
        );

        return;
    }

    printf(
        "\nDeleting:\n"
    );

    printf(
        "#%d | %s | %s | $%.2f\n",
        transactions[index].id,
        transactions[index].description,
        transactions[index].category,
        transactions[index].amount
    );

    char confirmation;

    printf(
        "Are you sure? (y/n): "
    );

    scanf(
        " %c",
        &confirmation
    );

    clear_input_buffer();

    if (
        confirmation != 'y'
        &&
        confirmation != 'Y'
    )
    {
        printf(
            "\nDeletion canceled.\n"
        );

        return;
    }

    for (
        int i = index;
        i < *count - 1;
        i++
    )
    {
        transactions[i] =
            transactions[i + 1];
    }

    (*count)--;

    reassign_ids(
        transactions,
        *count
    );

    printf(
        "\nTransaction deleted successfully.\n"
    );
}


// Find transaction by ID
int get_transaction_index_by_id(
    Transaction transactions[],
    int count,
    int id
)
{
    for (int i = 0; i < count; i++)
    {
        if (
            transactions[i].id == id
        )
        {
            return i;
        }
    }

    return -1;
}


// Reassign IDs after deletion
void reassign_ids(
    Transaction transactions[],
    int count
)
{
    for (int i = 0; i < count; i++)
    {
        transactions[i].id = i + 1;
    }
}


// Export CSV
void export_csv(
    Transaction transactions[],
    int count
)
{
    if (count == 0)
    {
        printf(
            "\nNo transactions to export.\n"
        );

        return;
    }

    FILE *file =
        fopen(
            CSV_FILE,
            "w"
        );

    if (file == NULL)
    {
        printf(
            "\nUnable to create CSV file.\n"
        );

        return;
    }

    fprintf(
        file,
        "ID,Date,Type,Category,Amount,Description\n"
    );

    for (int i = 0; i < count; i++)
    {
        char *type_name =
            transactions[i].type == 'I'
            ? "Income"
            : "Expense";

        fprintf(
            file,
            "%d,\"%s\",\"%s\",\"%s\",%.2f,\"%s\"\n",
            transactions[i].id,
            transactions[i].date,
            type_name,
            transactions[i].category,
            transactions[i].amount,
            transactions[i].description
        );
    }

    fclose(file);

    printf(
        "\nTransactions exported to %s\n",
        CSV_FILE
    );
}


// Save transaction database
void save_transactions(
    Transaction transactions[],
    int count
)
{
    FILE *file =
        fopen(
            DATA_FILE,
            "wb"
        );

    if (file == NULL)
    {
        printf(
            "\nError opening data file.\n"
        );

        return;
    }

    fwrite(
        &count,
        sizeof(int),
        1,
        file
    );

    fwrite(
        transactions,
        sizeof(Transaction),
        count,
        file
    );

    fclose(file);
}


// Load saved transaction database
void load_transactions(
    Transaction transactions[],
    int *count
)
{
    FILE *file =
        fopen(
            DATA_FILE,
            "rb"
        );

    if (file == NULL)
    {
        *count = 0;

        return;
    }

    if (
        fread(
            count,
            sizeof(int),
            1,
            file
        ) != 1
    )
    {
        *count = 0;

        fclose(file);

        return;
    }

    if (
        *count < 0
        ||
        *count > MAX_TRANSACTIONS
    )
    {
        *count = 0;

        fclose(file);

        return;
    }

    fread(
        transactions,
        sizeof(Transaction),
        *count,
        file
    );

    fclose(file);
}


// Remove newline from fgets
void remove_newline(
    char string[]
)
{
    string[
        strcspn(
            string,
            "\n"
        )
    ] = '\0';
}


// Clear remaining characters
void clear_input_buffer(void)
{
    int c;

    while (
        (c = getchar()) != '\n'
        &&
        c != EOF
    )
    {
        // Discard input
    }
}


// Convert string to lowercase
void to_lowercase(
    char destination[],
    const char source[]
)
{
    int i = 0;

    while (
        source[i] != '\0'
    )
    {
        destination[i] =
            tolower(
                (unsigned char)
                source[i]
            );

        i++;
    }

    destination[i] = '\0';
}