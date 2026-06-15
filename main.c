#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif
#define MAX_PRODUCT 1000
#define FILE_NAME "liquor.txt"

#define White "\x1b[38;5;15m"
#define Blue "\x1b[38;5;33m"
#define Green "\x1b[38;5;42m"
#define GreenMoney "\x1b[38;5;46m"
#define Red "\x1b[38;5;160m"
#define Gold "\x1b[38;5;220m"
#define WineRed "\x1b[38;5;88m"
#define Reset "\x1b[0m"

#define BANNER_WIDTH 53
#define TABLE_WIDTH 51

#define BANNER_WIDTH 53
#define TABLE_WIDTH 51

int getTerminalWidth()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
        return w.ws_col;
#endif
    return 80; // fallback
}

void printPadding(int contentWidth)
{
    int pad = (getTerminalWidth() - contentWidth) / 2;
    if (pad < 0)
        pad = 0;
    for (int i = 0; i < pad; i++)
        printf(" ");
}

#define PB() printPadding(BANNER_WIDTH) 
#define PT() printPadding(TABLE_WIDTH)  

struct product
{
    int id;
    char name[100];
    int price;
    int height;
    struct product *left;
    struct product *right;
};

struct product *root = NULL;

void saveData(struct product *curr, FILE *fp);
void writeFile();
void readFile();

struct product *createproduct(int id, char name[], int price)
{

    struct product *node = (struct product *)malloc(sizeof(struct product));

    node->id = id;
    strcpy(node->name, name);
    node->price = price;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

int getheight(struct product *node)
{

    if (node == NULL)
    {
        return 0;
    }

    return node->height;
}

int maxx(int a, int b)
{

    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int getbalance(struct product *node)
{

    if (node == NULL)
    {
        return 0;
    }

    return getheight(node->left) - getheight(node->right);
}

struct product *leftrotate(struct product *x)
{

    struct product *y = x->right;
    struct product *temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = 1 + maxx(getheight(x->left), getheight(x->right));
    y->height = 1 + maxx(getheight(y->left), getheight(y->right));

    return y;
}

struct product *rightrotate(struct product *y)
{

    struct product *x = y->left;
    struct product *temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = 1 + maxx(getheight(y->left), getheight(y->right));
    x->height = 1 + maxx(getheight(x->left), getheight(x->right));

    return x;
}

struct product *insertproduct(struct product *node, int id, char name[], int price)
{

    if (node == NULL)
    {
        return createproduct(id, name, price);
    }

    if (id < node->id)
    {
        node->left = insertproduct(node->left, id, name, price);
    }
    else if (id > node->id)
    {
        node->right = insertproduct(node->right, id, name, price);
    }
    else
    {
        return node;
    }

    node->height = 1 + maxx(getheight(node->left), getheight(node->right));

    int balance = getbalance(node);

    // ll
    if (balance > 1 && id < node->left->id)
    {
        return rightrotate(node);
    }

    // rr
    if (balance < -1 && id > node->right->id)
    {
        return leftrotate(node);
    }

    // lr
    if (balance > 1 && id > node->left->id)
    {
        node->left = leftrotate(node->left);
        return rightrotate(node);
    }

    // rl
    if (balance < -1 && id < node->right->id)
    {
        node->right = rightrotate(node->right);
        return leftrotate(node);
    }

    return node;
}

struct product *findmin(struct product *node)
{

    struct product *curr = node;

    while (curr->left != NULL)
    {
        curr = curr->left;
    }

    return curr;
}

struct product *deleteproduct(struct product *node, int id)
{

    if (node == NULL)
    {
        return NULL;
    }

    if (id < node->id)
    {
        node->left = deleteproduct(node->left, id);
    }
    else if (id > node->id)
    {
        node->right = deleteproduct(node->right, id);
    }
    else
    {
        if (node->left == NULL || node->right == NULL)
        {

            struct product *temp = NULL;

            if (node->left != NULL)
            {
                temp = node->left;
            }
            else
            {
                temp = node->right;
            }

            if (temp == NULL)
            {
                temp = node;
                node = NULL;
            }
            else
            {
                *node = *temp;
            }

            free(temp);
        }
        else
        {

            struct product *temp = findmin(node->right);

            node->id = temp->id;
            strcpy(node->name, temp->name);
            node->price = temp->price;

            node->right = deleteproduct(node->right, temp->id);
        }
    }

    if (node == NULL)
    {
        return NULL;
    }

    node->height = 1 + maxx(getheight(node->left), getheight(node->right));

    int balance = getbalance(node);

    // ll
    if (balance > 1 && getbalance(node->left) >= 0)
    {
        return rightrotate(node);
    }

    // lr
    if (balance > 1 && getbalance(node->left) < 0)
    {
        node->left = leftrotate(node->left);
        return rightrotate(node);
    }

    // rr
    if (balance < -1 && getbalance(node->right) <= 0)
    {
        return leftrotate(node);
    }

    // rl
    if (balance < -1 && getbalance(node->right) > 0)
    {
        node->right = rightrotate(node->right);
        return leftrotate(node);
    }

    return node;
}

void freeTree(struct product *node)
{
    if (node == NULL)
        return;

    freeTree(node->left);
    freeTree(node->right);

    free(node);
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnter()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
    printf("\n");
    PB(); printf("      Press Enter to continue...");
    getchar();
}

int getIDByName(struct product *curr, char name[])
{

    if (curr == NULL)
    {
        return -1;
    }

    if (strcmp(curr->name, name) == 0)
    {
        return curr->id;
    }

    int left = getIDByName(curr->left, name);

    if (left != -1)
    {
        return left;
    }

    return getIDByName(curr->right, name);
}

struct product tempProducts[MAX_PRODUCT];
int productCount = 0;

void storeProducts(struct product *node)
{
    if (node == NULL)
    {
        return;
    }

    storeProducts(node->left);

    if (productCount < MAX_PRODUCT)
    {
        tempProducts[productCount++] = *node;
    }

    storeProducts(node->right);
}

int compareNameAsc(const void *a, const void *b)
{
    struct product *p1 = (struct product *)a;
    struct product *p2 = (struct product *)b;

    return strcmp(p1->name, p2->name);
}

int compareNameDesc(const void *a, const void *b)
{
    struct product *p1 = (struct product *)a;
    struct product *p2 = (struct product *)b;

    return strcmp(p2->name, p1->name);
}

int comparePriceAsc(const void *a, const void *b)
{
    struct product *p1 = (struct product *)a;
    struct product *p2 = (struct product *)b;

    if (p1->price < p2->price)
        return -1;
    if (p1->price > p2->price)
        return 1;
    return 0;
}

int comparePriceDesc(const void *a, const void *b)
{
    struct product *p1 = (struct product *)a;
    struct product *p2 = (struct product *)b;

    if (p1->price > p2->price)
        return -1;
    if (p1->price < p2->price)
        return 1;
    return 0;
}

int sortType = 2;
int sortOrder = 2;

void printRupiah(int price)
{
    if (price >= 1000)
    {
        printRupiah(price / 1000);
        printf(".%03d", price % 1000);
    }
    else
    {
        printf("%d", price);
    }
}

void formatRupiah(int price, char result[])
{
    char temp[50];
    sprintf(temp, "%d", price);

    int len = strlen(temp);
    int count = 0;
    int j = 0;

    for (int i = len - 1; i >= 0; i--)
    {
        result[j++] = temp[i];
        count++;

        if (count % 3 == 0 && i != 0)
        {
            result[j++] = '.';
        }
    }

    result[j] = '\0';

    len = strlen(result);
    for (int i = 0; i < len / 2; i++)
    {
        char t = result[i];
        result[i] = result[len - 1 - i];
        result[len - 1 - i] = t;
    }
}

void displayproduct()
{

    productCount = 0;

    storeProducts(root);

    if (sortType == 1)
    {
        if (sortOrder == 1)
        {
            qsort(tempProducts,
                  productCount,
                  sizeof(struct product),
                  compareNameAsc);
        }
        else
        {
            qsort(tempProducts,
                  productCount,
                  sizeof(struct product),
                  compareNameDesc);
        }
    }
    else
    {
        if (sortOrder == 1)
        {
            qsort(tempProducts,
                  productCount,
                  sizeof(struct product),
                  comparePriceAsc);
        }
        else
        {
            qsort(tempProducts,
                  productCount,
                  sizeof(struct product),
                  comparePriceDesc);
        }
    }

    char rupiah[50];

    for (int i = 0; i < productCount; i++)
    {
        formatRupiah(tempProducts[i].price, rupiah);

        PT(); printf(WineRed "     | " White "%-5d " WineRed "| " White "%-19s " WineRed "| " GreenMoney "Rp%-16s " WineRed "|\n",
               tempProducts[i].id,
               tempProducts[i].name,
               rupiah);
    }
}

void productList()
{
    PT(); printf(WineRed "     =================== " Gold "PRODUCT LIST " WineRed "===================\n");

    if (root == NULL)
    {
        PT(); printf(Red "No Product Available!\n" Reset);
    }
    else
    {
        PT(); printf("     +-------+---------------------+--------------------+\n");
        PT(); printf("     |" Gold " ID    " WineRed "| " Gold "Name                " WineRed "| " Gold "Price (per bottle) " WineRed "|\n");
        PT(); printf("     +-------+---------------------+--------------------+\n");
        displayproduct();
        PT(); printf("     +-------+---------------------+--------------------+\n" Reset);
    }
}

void printBanner()
{
    PB(); printf(WineRed "        .~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.\n");
    PB(); printf(WineRed "       /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\\n");
    PB(); printf(WineRed "      =====================================================\n");
    PB(); printf("      ||" Gold "       _      ___   ___  _   _  ___  ____        " WineRed "||\n");
    PB(); printf("      ||" Gold "      | |    |_ _| / _ \\| | | |/ _ \\|  _ \\       " WineRed "||\n");
    PB(); printf("      ||" Gold "      | |     | | | | | | | | | | | | |_) |      " WineRed "||\n");
    PB(); printf("   ===||" Gold "      | |___  | | | |_| | |_| | |_| |  _ <       " WineRed "||\n");
    PB(); printf(" //   ||" Gold "      |_____||___| \\__\\_\\\\___/ \\___/|_| \\_\\\      " WineRed "||\n");
    PB(); printf("||    ||" Gold "                                                 " WineRed "||\n");
    PB(); printf("||    ||" Gold "           ____ _____ ___  ____  _____           " WineRed "||\n");
    PB(); printf("||    ||" Gold "          / ___|_   _/ _ \\|  _ \\| ____|          " WineRed "||\n");
    PB(); printf("||    ||" Gold "          \\___ \\ | || | | | |_) |  _|            " WineRed "||\n");
    PB(); printf(" \\\\   ||" Gold "           ___) || || |_| |  _ <| |___           " WineRed "||\n");
    PB(); printf("   ===||" Gold "          |____/ |_| \\___/|_| \\_\\\\____|          " WineRed "||\n");
    PB(); printf("      ||" Gold "                                                 " WineRed "||\n");
    PB(); printf("      ||" Gold "              Luxury in Every Pour.              " WineRed "||\n");
    PB(); printf("      ||" Gold "                                                 " WineRed "||\n");
    PB(); printf(WineRed "      =====================================================\n" Reset);
}

void printWelcomeMessage()
{
    PB(); printf(WineRed "        .~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.\n");
    PB(); printf(WineRed "       /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\\n");
    PB(); printf(WineRed "      =====================================================\n");
    PB(); printf("      ||" Gold "       _      ___   ___  _   _  ___  ____        " WineRed "||\n");
    PB(); printf("      ||" Gold "      | |    |_ _| / _ \\| | | |/ _ \\|  _ \\       " WineRed "||\n");
    PB(); printf("      ||" Gold "      | |     | | | | | | | | | | | | |_) |      " WineRed "||\n");
    PB(); printf("   ===||" Gold "      | |___  | | | |_| | |_| | |_| |  _ <       " WineRed "||\n");
    PB(); printf(" //   ||" Gold "      |_____||___| \\__\\_\\\\___/ \\___/|_| \\_\\\      " WineRed "||\n");
    PB(); printf("||    ||" Gold "                                                 " WineRed "||\n");
    PB(); printf("||    ||" Gold "           ____ _____ ___  ____  _____           " WineRed "||\n");
    PB(); printf("||    ||" Gold "          / ___|_   _/ _ \\|  _ \\| ____|          " WineRed "||\n");
    PB(); printf("||    ||" Gold "          \\___ \\ | || | | | |_) |  _|            " WineRed "||\n");
    PB(); printf(" \\\\   ||" Gold "           ___) || || |_| |  _ <| |___           " WineRed "||\n");
    PB(); printf("   ===||" Gold "          |____/ |_| \\___/|_| \\_\\\\____|          " WineRed "||\n");
    PB(); printf("      ||" Gold "                                                 " WineRed "||\n");
    PB(); printf("      ||" Gold "           Welcome to the Liquor Store           " WineRed "||\n");
    PB(); printf("      ||" Gold "              Luxury in Every Pour.              " WineRed "||\n");
    PB(); printf("      ||" Gold "                                                 " WineRed "||\n");
    PB(); printf(WineRed "      =====================================================\n" Reset);
}

int role = 0;

void login()
{

    int choice;
    char username[100];
    char password[100];

    do
    {
        printWelcomeMessage();

        PB(); printf(WineRed "      ||" Gold "                      LOGIN                      " WineRed "||\n");
        PB(); printf("      =====================================================\n");
        PB(); printf("      ||" Gold "                   1. Admin                      " WineRed "||\n");
        PB(); printf("      ||" Blue "                   2. Customer                   " WineRed "||\n");
        PB(); printf("      =====================================================\n");
        do
        {
            PB(); printf(Blue "      Choose[1/2]: ");
            scanf("%d", &choice);

            if (choice < 1 || choice > 2)
            {
                PB(); printf(Red "      Invalid Menu!\n" Reset);
            }

        } while (choice < 1 || choice > 2);

        switch (choice)
        {

        case 1:

            PB(); printf(Gold "      Username : ");
            scanf("%s", username);

            PB(); printf("      Password : ");
            scanf("%s", password);

            if (strcmp(username, "admin") == 0 && strcmp(password, "12345") == 0)
            {
                role = 1;
                PB(); printf(Green "      Login Success as Admin!\n" Reset);

                pressEnter();
                clearScreen();
            }
            else
            {
                PB(); printf(Red "      Wrong Username or Password!\n" Reset);

                pressEnter();
                clearScreen();
            }
            break;

        case 2:
            role = 2;
            PB(); printf(Green "      Login Success as Customer!\n" Reset);

            pressEnter();
            clearScreen();
            break;

        default:
            PB(); printf(Red "      Invalid Menu!\n" Reset);
        }

    } while (role == 0);
}

int checkName(struct product *curr, char name[])
{

    if (curr == NULL)
    {
        return 0;
    }

    if (strcmp(curr->name, name) == 0)
    {
        return 1;
    }

    int leftCheck = checkName(curr->left, name);
    if (leftCheck)
    {
        return 1;
    }

    return checkName(curr->right, name);
}

int checkID(struct product *curr, int id)
{

    if (curr == NULL)
    {
        return 0;
    }

    if (curr->id == id)
    {
        return 1;
    }

    if (id < curr->id)
    {
        return checkID(curr->left, id);
    }
    else
    {
        return checkID(curr->right, id);
    }
}

int generateID()
{
    int id;

    do
    {
        id = rand() % 90000 + 10000;
    } while (checkID(root, id));

    return id;
}

void insert()
{
    clearScreen();

    int id, price;
    char name[100];

    productList();

    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      |" White "  Insert Product                                  " WineRed "|\n");
    PB(); printf(WineRed "      +--------------------------------------------------+\n" Reset);
    do
    {
        PB(); printf("      Product Name: ");
        scanf(" %[^\n]", name);

        if (checkName(root, name))
        {
            PB(); printf(Green "      Product is already exists!\n" Reset);
        }
    } while (checkName(root, name));

    id = generateID();

    do
    {
        PB(); printf("      Product Price: ");
        scanf("%d", &price);

        if (price <= 0)
        {
            PB(); printf(Red "      Price cannot be negative or zero!\n" Reset);
        }
        else if (price > 100000000)
        {
            PB(); printf(Red "      Price cannot be greater than 100 million!\n" Reset);
        }
    } while (price <= 0 || price > 100000000);

    root = insertproduct(root, id, name, price);
    writeFile();
    PB(); printf(Green "      Product Added Successfully!\n" Reset);

    pressEnter();
    clearScreen();
}

void searchID(struct product *curr, int id)
{

    if (curr == NULL)
    {
        PB(); printf("      Product Not Found!\n");
        return;
    }

    if (curr->id == id)
    {
        char rupiah[50];
        formatRupiah(curr->price, rupiah);

        printf("\n");
        PB(); printf(WineRed "      =================== " Gold "PRODUCT FOUND " WineRed "===================\n");
        PB(); printf("      || " White "Id    : %-5d                                   " WineRed "||\n", curr->id);
        PB(); printf("      || " White "Name  : %-19s                     " WineRed "||\n", curr->name);
        PB(); printf("      || " White "Price : " GreenMoney "Rp%-16s                      " WineRed "||\n", rupiah);
        PB(); printf("      =====================================================\n" Reset);
        return;
    }

    if (id < curr->id)
    {
        searchID(curr->left, id);
    }
    else
    {
        searchID(curr->right, id);
    }
}

void searchName(struct product *curr, char name[])
{

    if (curr == NULL)
    {
        return;
    }

    searchName(curr->left, name);

    if (strcmp(curr->name, name) == 0)
    {

        char rupiah[50];
        formatRupiah(curr->price, rupiah);

        printf("\n");
        PB(); printf(WineRed "      =================== " Gold "PRODUCT FOUND " WineRed "===================\n");
        PB(); printf("      || " White "Id    : %-5d                                   " WineRed "||\n", curr->id);
        PB(); printf("      || " White "Name  : %-19s                     " WineRed "||\n", curr->name);
        PB(); printf("      || " White "Price : " GreenMoney "Rp%-16s                      " WineRed "||\n", rupiah);
        PB(); printf("      =====================================================\n" Reset);
    }

    searchName(curr->right, name);
}

void search()
{
    clearScreen();

    int choice;

    do
    {
        productList();
        PB(); printf(WineRed "      +--------------------------------------------------+\n");
        PB(); printf("      |" White "  Search Product                                  " WineRed "|\n");
        PB(); printf(WineRed "      +--------------------------------------------------+\n");
        PB(); printf("      |" White " 1. Search by ID                                  " WineRed "|\n");
        PB(); printf("      |" White " 2. Search by Name                                " WineRed "|\n");
        PB(); printf("      |" White " 3. Back to Main Menu                             " WineRed "|\n");
        PB(); printf(WineRed "      +--------------------------------------------------+\n" Reset);
        PB(); printf("      Choose: ");
        scanf("%d", &choice);

        if (choice == 1)
        {

            int id;

            PB(); printf("      Input Product ID: ");
            scanf("%d", &id);

            searchID(root, id);

            pressEnter();
            clearScreen();
        }

        else if (choice == 2)
        {

            char name[100];

            PB(); printf("      Input Product Name: ");
            scanf(" %[^\n]", name);

            int found = checkName(root, name);

            if (found)
            {
                searchName(root, name);
            }
            else
            {
                PB(); printf("      Product Not Found!\n");
            }

            pressEnter();
            clearScreen();
        }

        else if (choice == 3)
        {
            clearScreen();
            return;
        }

        else
        {
            PB(); printf("      Invalid Menu!\n");
        }

    } while (choice < 1 || choice > 3);
}

void delete()
{
    clearScreen();

    int choice, id;
    char name[100], confirm;

    productList();
    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      |" White "  Delete Product                                  " WineRed "|\n");
    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      |" White "  1. Delete by ID                                  " WineRed "|\n");
    PB(); printf("      |" White "  2. Delete by Name                                " WineRed "|\n");
    PB(); printf("      |" White "  3. Back to Main Menu                             " WineRed "|\n");
    PB(); printf(WineRed "      +--------------------------------------------------+\n" Reset);
    do
    {
        PB(); printf("      Choose: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 3)
        {
            PB(); printf("      Invalid Menu!\n");
        }
    } while (choice < 1 || choice > 3);

    switch (choice)
    {
    case 1:
        do
        {
            PB(); printf("      Input Product ID: ");
            scanf("%d", &id);

            searchID(root, id);

            if (checkID(root, id))
            {
                PB(); printf("      Are you sure you want to delete this product? [y/n]: ");
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y')
                {
                    root = deleteproduct(root, id);
                    writeFile();
                    PB(); printf("      Product deleted successfully!\n");

                    pressEnter();
                    clearScreen();
                    return;
                }
                else
                {
                    PB(); printf("      Deletion cancelled.\n");

                    pressEnter();
                    clearScreen();
                    break;
                }
            }
        } while (!checkID(root, id));
        break;

    case 2:
        do
        {
            PB(); printf("      Input Product Name: ");
            scanf(" %[^\n]", name);

            if (!checkName(root, name))
            {
                PB(); printf("      Product Not Found!\n");
            }
            else
            {
                searchName(root, name);
            }

            if (checkName(root, name))
            {
                PB(); printf("      Are you sure you want to delete this product? [y/n]: ");
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y')
                {
                    int id = getIDByName(root, name);

                    if (id != -1)
                    {
                        root = deleteproduct(root, id);
                        writeFile();
                        PB(); printf("      Product deleted successfully!\n");

                        pressEnter();
                        clearScreen();

                        return;
                    }
                }
                else
                {
                    PB(); printf("      Deletion cancelled.\n");
                    pressEnter();
                    clearScreen();
                    break;
                }
            }
        } while (!checkName(root, name));
        break;

    case 3:
        clearScreen();
        return;
    }
}

void updatePriceByID(struct product *curr, int id, int newPrice)
{
    if (curr == NULL)
    {
        return;
    }

    if (curr->id == id)
    {
        curr->price = newPrice;
        return;
    }

    if (id < curr->id)
    {
        updatePriceByID(curr->left, id, newPrice);
    }
    else
    {
        updatePriceByID(curr->right, id, newPrice);
    }
}

void updatePriceByName(struct product *curr, char name[], int newPrice)
{
    if (curr == NULL)
    {
        return;
    }

    updatePriceByName(curr->left, name, newPrice);

    if (strcmp(curr->name, name) == 0)
    {
        curr->price = newPrice;
        return;
    }

    updatePriceByName(curr->right, name, newPrice);
}

void updateProductNamebyID(struct product *curr, int id, char newName[])
{
    if (curr == NULL)
    {
        return;
    }

    if (curr->id == id)
    {
        strcpy(curr->name, newName);
        return;
    }

    if (id < curr->id)
    {
        updateProductNamebyID(curr->left, id, newName);
    }
    else
    {
        updateProductNamebyID(curr->right, id, newName);
    }
}

void updateProductNamebyName(struct product *curr, char name[], char newName[])
{
    if (curr == NULL)
    {
        return;
    }

    updateProductNamebyName(curr->left, name, newName);

    if (strcmp(curr->name, name) == 0)
    {
        strcpy(curr->name, newName);
        return;
    }

    updateProductNamebyName(curr->right, name, newName);
}

void updateProductName(int id, int choice, char name[])
{
    char newName[100];

    do
    {
        PB(); printf("      Input New Name: ");
        scanf(" %[^\n]", newName);

        if (checkName(root, newName))
        {
            PB(); printf("      Product is already exists!\n");
        }
    } while (checkName(root, newName));

    if (choice == 1)
    {
        updateProductNamebyID(root, id, newName);
    }
    else
    {
        updateProductNamebyName(root, name, newName);
    }
}

void updateProductPrice(int id, int choice, char name[])
{
    int newPrice;

    do
    {
        PB(); printf("      Input New Price: ");
        scanf("%d", &newPrice);

        if (newPrice <= 0)
        {
            PB(); printf("      Price cannot be negative or zero!\n");
        }
        else if (newPrice > 100000000)
        {
            PB(); printf("      Price cannot be greater than 100 million!\n");
        }
    } while (newPrice <= 0 || newPrice > 100000000);

    if (choice == 1)
    {
        updatePriceByID(root, id, newPrice);
    }
    else
    {
        updatePriceByName(root, name, newPrice);
    }
}

void selectUpdateNameBasedOn()
{
    int choice, id;
    char name[100];

    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      |" White "  Update Based On                                 " WineRed "|\n");
    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      | " White "1. ID                                            " WineRed "|\n");
    PB(); printf("      | " White "2. Name                                          " WineRed "|\n");
    PB(); printf("      +--------------------------------------------------+\n" Reset);
    do
    {
        PB(); printf("      Choose: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 2)
        {
            PB(); printf(Red "      Invalid Choice!\n" Reset);
        }
    } while (choice < 1 || choice > 2);

    if (choice == 1)
    {
        do
        {
            PB(); printf("      Input Product ID: ");
            scanf("%d", &id);

            searchID(root, id);

            if (checkID(root, id))
            {
                updateProductName(id, choice, name);
                writeFile();
                PB(); printf(Green "      Product name updated successfully!\n" Reset);

                pressEnter();
                clearScreen();
                return;
            }
        } while (!checkID(root, id));
    }
    else if (choice == 2)
    {
        do
        {
            PB(); printf("      Input Product Name: ");
            scanf(" %[^\n]", name);

            if (!checkName(root, name))
            {
                PB(); printf(Red "      Product Not Found!\n" Reset);
            }
            else
            {
                searchName(root, name);
            }

            if (checkName(root, name))
            {
                updateProductName(id, choice, name);
                writeFile();
                PB(); printf(Green "      Product name updated successfully!\n" Reset);

                pressEnter();
                clearScreen();
                return;
            }
        } while (!checkName(root, name));
    }
    else
    {
        clearScreen();
        return;
    }
}

void selectUpdatePriceBasedOn()
{
    int choice, id;
    char name[100];

    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      |" White "  Update Based On                                 " WineRed "|\n");
    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      | " White "1. ID                                            " WineRed "|\n");
    PB(); printf("      | " White "2. Name                                          " WineRed "|\n");
    PB(); printf("      +--------------------------------------------------+\n" Reset);
    do
    {
        PB(); printf("      Choose: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 2)
        {
            PB(); printf(Red "      Invalid Choice!\n" Reset);
        }
    } while (choice < 1 || choice > 2);

    if (choice == 1)
    {
        do
        {
            PB(); printf("      Input Product ID: ");
            scanf("%d", &id);

            searchID(root, id);

            if (checkID(root, id))
            {
                updateProductPrice(id, choice, name);
                writeFile();
                PB(); printf(Green "      Product Price updated successfully!\n" Reset);

                pressEnter();
                clearScreen();
                return;
            }
        } while (!checkID(root, id));
    }
    else if (choice == 2)
    {
        do
        {
            PB(); printf("      Input Product Name: ");
            scanf(" %[^\n]", name);

            if (!checkName(root, name))
            {
                PB(); printf(Red "      Product Not Found!\n" Reset);
            }
            else
            {
                searchName(root, name);
            }

            if (checkName(root, name))
            {
                updateProductPrice(id, choice, name);
                writeFile();
                PB(); printf(Green "Product Price updated successfully!\n" Reset);

                pressEnter();
                clearScreen();
                return;
            }
        } while (!checkName(root, name));
    }
    else
    {
        clearScreen();
        return;
    }
}

void update()
{
    clearScreen();

    int choice;

    productList();
    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      |" White "  Update Product                                  " WineRed "|\n");
    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      | " White "1. Update Product Name                           " WineRed "|\n");
    PB(); printf("      | " White "2. Update Product Price                          " WineRed "|\n");
    PB(); printf("      | " White "3. Back to Main Menu                             " WineRed "|\n");
    PB(); printf("      +--------------------------------------------------+\n" Reset);
    do
    {
        PB(); printf("      Choose: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 3)
        {
            PB(); printf(Red "      Invalid Menu!\n" Reset);
        }
    } while (choice < 1 || choice > 3);

    if (choice == 1)
    {
        selectUpdateNameBasedOn();
    }
    else if (choice == 2)
    {
        selectUpdatePriceBasedOn();
    }
    else
    {
        clearScreen();
        return;
    }
}

void findProduct(struct product *curr, int id, struct product **result)
{

    if (curr == NULL)
    {
        return;
    }

    if (curr->id == id)
    {
        *result = curr;
        return;
    }

    if (id < curr->id)
    {
        findProduct(curr->left, id, result);
    }
    else
    {
        findProduct(curr->right, id, result);
    }
}

void checkout()
{
    clearScreen();

    productList();

    int totalall = 0;
    char choice;

    do
    {

        int id, qty;

        PB(); printf(WineRed "      +-------------------- " Gold "CHECKOUT " WineRed "--------------------+\n" Reset);

        struct product *item = NULL;

        do
        {
            PB(); printf("      Input Product ID : ");
            scanf("%d", &id);

            item = NULL;
            findProduct(root, id, &item);

            if (item == NULL)
            {
                PB(); printf(Red "      Product Not Found!\n" Reset);
            }

        } while (item == NULL);

        do
        {
            PB(); printf("      Input Quantity   : ");
            scanf("%d", &qty);

            if (qty <= 0)
            {
                PB(); printf(Red "      Quantity must be greater than 0!\n" Reset);
            }
        } while (qty <= 0);

        int total = item->price * qty;
        totalall += total;

        printf("\n");
        PB(); printf("      Item Added!\n");
        PB(); printf("      Product : %s\n", item->name);
        PB(); printf("      Subtotal: " GreenMoney "Rp");
        printRupiah(total);
        printf(Reset "\n");

        do
        {
            PB(); printf("      Add More Item? [y/n]: ");
            scanf(" %c", &choice);

            if (choice != 'y' && choice != 'Y' &&
                choice != 'n' && choice != 'N')
            {
                PB(); printf(Red "      Invalid Choice!\n" Reset);
            }

        } while (choice != 'y' && choice != 'Y' &&
                 choice != 'n' && choice != 'N');

    } while (choice == 'y' || choice == 'Y');

    printf("\n");
    PB(); printf(WineRed "      ==================== " Gold "FINAL TOTAL " WineRed "====================\n");
    PB(); printf(White "      Grand Total : " GreenMoney "Rp");
    printRupiah(totalall);
    printf(Reset "\n");

    pressEnter();
    clearScreen();
}

void sortingMenu()
{
    clearScreen();

    PB(); printf(WineRed "      +--------------------------------------------------+\n");
    PB(); printf("      |" White "  Sorting Menu                                    " WineRed "|\n");
    PB(); printf(WineRed "      +--------------------------------------------------+\n" Reset);

    do
    {
        PB(); printf("      1. Sort by Name\n");
        PB(); printf("      2. Sort by Price\n");
        PB(); printf("      Choose: ");
        scanf("%d", &sortType);

        if (sortType < 1 || sortType > 2)
        {
            PB(); printf(Red "      Invalid Choice!\n" Reset);
        }
    } while (sortType < 1 || sortType > 2);

    PB(); printf("\n");

    do
    {
        PB(); printf("      1. Ascending\n");
        PB(); printf("      2. Descending\n");
        PB(); printf("      Choose: ");
        scanf("%d", &sortOrder);

        if (sortOrder < 1 || sortOrder > 2)
        {
            PB(); printf(Red "      Invalid Choice!\n" Reset);
        }
    } while (sortOrder < 1 || sortOrder > 2);

    pressEnter();
    clearScreen();
}

void logout()
{
    char confirm;

    do
    {
        PB(); printf("      Are you sure you want to logout? [y/n]: ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y')
        {
            role = 0;
            PB(); printf("      Logged out successfully!\n");
            pressEnter();
            clearScreen();
            return;
        }
        else if (confirm == 'n' || confirm == 'N')
        {
            PB(); printf("      Logout cancelled.\n");
            pressEnter();
            clearScreen();
            return;
        }
        else
        {
            PB(); printf(Red "      Invalid Choice!\n" Reset);
        }

    } while (1);
}

void saveData(struct product *curr, FILE *fp)
{

    if (curr == NULL)
    {
        return;
    }

    saveData(curr->left, fp);

    fprintf(fp, "%d|%s|%d\n",
            curr->id,
            curr->name,
            curr->price);

    saveData(curr->right, fp);
}

void writeFile()
{

    FILE *fp = fopen(FILE_NAME, "w");

    if (fp == NULL)
    {
        return;
    }

    saveData(root, fp);

    fclose(fp);
}

void readFile()
{

    FILE *fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
    {
        return;
    }

    int id;
    int price;
    char name[100];

    while (fscanf(fp,
                  "%d|%[^|]|%d\n",
                  &id,
                  name,
                  &price) == 3)
    {

        root = insertproduct(
            root,
            id,
            name,
            price);
    }

    fclose(fp);
}

void adminMenu()
{
    int choice;

    do
    {
        printBanner();
        productList();
        PB(); printf(WineRed "      +--------------------------------------------------+\n");
        PB(); printf("      |" White " Status: " Gold "Admin                                    " WineRed "|\n");
        PB(); printf("      +--------------------------------------------------+\n");
        PB(); printf("      |" White " 1. Insert Product                                " WineRed "|\n");
        PB(); printf("      |" White " 2. Search Product                                " WineRed "|\n");
        PB(); printf("      |" White " 3. Update Product                                " WineRed "|\n");
        PB(); printf("      |" White " 4. Delete Product                                " WineRed "|\n");
        PB(); printf("      |" White " 5. Sorting Menu                                  " WineRed "|\n");
        PB(); printf("      |" White " 6. Logout                                        " WineRed "|\n");
        PB(); printf("      |" White " 7. Exit Program                                  " WineRed "|\n");
        PB(); printf("      +--------------------------------------------------+\n" Reset);
        do
        {
            PB(); printf("      Choose: ");
            scanf("%d", &choice);

            if (choice < 1 || choice > 7)
            {
                PB(); printf(Red "      Invalid Menu!\n" Reset);
            }
        } while (choice < 1 || choice > 7);

        switch (choice)
        {
        case 1:
            insert();
            break;
        case 2:
            search();
            break;
        case 3:
            update();
            break;
        case 4:
            delete();
            break;
        case 5:
            sortingMenu();
            break;
        case 6:
            logout();

            if (role == 0)
            {
                return;
            }
            break;
        case 7:
            printf("\n");
            PB(); printf(WineRed "      ====================================================\n");
            PB(); printf(Gold "              Thank you for visiting Liquor Store.        \n");
            PB(); printf("                     Have a wonderful day!                \n" Reset);
            PB(); printf(WineRed "      ====================================================\n" Reset);
            break;
        }
    } while (choice != 7);
}

void customerMenu()
{
    int choice;

    do
    {
        printBanner();
        productList();
        PB(); printf(WineRed "      +--------------------------------------------------+\n");
        PB(); printf("      |" White " Status: " Blue "Customer                                 " WineRed "|\n");
        PB(); printf("      +--------------------------------------------------+\n");
        PB(); printf("      |" White " 1. Search Product                                " WineRed "|\n");
        PB(); printf("      |" White " 2. Sorting Menu                                  " WineRed "|\n");
        PB(); printf("      |" White " 3. Checkout Product                              " WineRed "|\n");
        PB(); printf("      |" White " 4. Logout                                        " WineRed "|\n");
        PB(); printf("      |" White " 5. Exit Program                                  " WineRed "|\n");
        PB(); printf("      +--------------------------------------------------+\n" Reset);
        do
        {
            PB(); printf("      Choose: ");
            scanf("%d", &choice);

            if (choice < 1 || choice > 5)
            {
                PB(); printf(Red "      Invalid Menu!\n" Reset);
            }
        } while (choice < 1 || choice > 5);

        switch (choice)
        {
        case 1:
            search();
            break;
        case 2:
            sortingMenu();
            break;
        case 3:
            checkout();
            break;
        case 4:
            logout();

            if (role == 0)
            {
                return;
            }
            break;
        case 5:
            printf("\n");
            PB(); printf(WineRed "      ====================================================\n");
            PB(); printf(Gold "              Thank you for visiting Liquor Store.        \n");
            PB(); printf("                     Have a wonderful day!                \n" Reset);
            PB(); printf(WineRed "      ====================================================\n" Reset);
            break;
        }
    } while (choice != 5);
}

int main()
{
    srand(time(NULL));

    readFile();

    while (1)
    {
        login();

        if (role == 1)
        {
            adminMenu();
        }
        else if (role == 2)
        {
            customerMenu();
        }

        if (role == 0)
        {
            continue;
        }

        break;
    }

    freeTree(root);
    return 0;
}

// NOTE :
// Pada saat pembuatan video presentasi belum menambahkan sistem file processing.
// File Processing yang ada pada source code ditambahkan agar perubahan yang dilakukan dapat tersimpan untuk penggunaan berikutnya.
// Mekanismenya yaitu perubahan data yang terjadi akan tercatat ke dalam file "liquor.txt", yang akan dibaca program setiap kali program dirun.
// Terima kasih....
