#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PRODUCT 1000

struct product {
    int id;
    char name[100];
    int price;
    int height;
    struct product *left;
    struct product *right;
};

struct product *root = NULL;

struct product *createproduct(int id, char name[], int price) {

    struct product *node = (struct product *)malloc(sizeof(struct product));

    node->id = id;
    strcpy(node->name, name);
    node->price = price;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

int getheight(struct product *node) {

    if (node == NULL)
    {
        return 0;
    }

    return node->height;
}

int max(int a, int b) {

    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int getbalance(struct product *node) {

    if (node == NULL)
    {
        return 0;
    }

    return getheight(node->left) - getheight(node->right);
}

struct product *leftrotate(struct product *x) {

    struct product *y = x->right;
    struct product *temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = 1 + max(getheight(x->left), getheight(x->right));
    y->height = 1 + max(getheight(y->left), getheight(y->right));

    return y;
}

struct product *rightrotate(struct product *y) {

    struct product *x = y->left;
    struct product *temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = 1 + max(getheight(y->left), getheight(y->right));
    x->height = 1 + max(getheight(x->left), getheight(x->right));

    return x;
}

struct product *insertproduct(struct product *node, int id, char name[], int price) {

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

    node->height = 1 + max(getheight(node->left), getheight(node->right));

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

struct product *findmin(struct product *node) {

    struct product *curr = node;

    while (curr->left != NULL)
    {
        curr = curr->left;
    }

    return curr;
}

struct product *deleteproduct(struct product *node, int id) {

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

    node->height = 1 + max(getheight(node->left), getheight(node->right));

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

void freeTree(struct product *node) {
    if (node == NULL)
        return;

    freeTree(node->left);
    freeTree(node->right);

    free(node);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnter() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("\nPress Enter to continue...");
    getchar();
}

int getIDByName(struct product *curr, char name[]) {

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

void storeProducts(struct product *node) {
    if (node == NULL)
    {
        return;
    }

    storeProducts(node->left);

    if (productCount < MAX_PRODUCT) {
        tempProducts[productCount++] = *node;
    }

    storeProducts(node->right);
}

int compareNameAsc(const void *a, const void *b) {
    struct product *p1 = (struct product *)a;
    struct product *p2 = (struct product *)b;

    return strcmp(p1->name, p2->name);
}

int compareNameDesc(const void *a, const void *b) {
    struct product *p1 = (struct product *)a;
    struct product *p2 = (struct product *)b;

    return strcmp(p2->name, p1->name);
}

int comparePriceAsc(const void *a, const void *b) {
    struct product *p1 = (struct product *)a;
    struct product *p2 = (struct product *)b;

    if (p1->price < p2->price)
        return -1;
    if (p1->price > p2->price)
        return 1;
    return 0;
}

int comparePriceDesc(const void *a, const void *b) {
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

void printRupiah(int price) {
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

void formatRupiah(int price, char result[]) {
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

void displayproduct() {

    productCount = 0;

    storeProducts(root);

    if (sortType == 1) {
        if (sortOrder == 1) {
            qsort(tempProducts,
                  productCount,
                  sizeof(struct product),
                  compareNameAsc);
        } else {
            qsort(tempProducts,
                  productCount,
                  sizeof(struct product),
                  compareNameDesc);
        }
    } else {
        if (sortOrder == 1) {
            qsort(tempProducts,
                  productCount,
                  sizeof(struct product),
                  comparePriceAsc);
        } else {
            qsort(tempProducts,
                  productCount,
                  sizeof(struct product),
                  comparePriceDesc);
        }
    }

    char rupiah[50];

    for (int i = 0; i < productCount; i++) {
        formatRupiah(tempProducts[i].price, rupiah);

        printf("| %-5d | %-19s | Rp%-12s |\n",
               tempProducts[i].id,
               tempProducts[i].name,
               rupiah);
    }
}

int role = 0;

void login() {

    int choice;
    char username[100];
    char password[100];

    do
    {

        printf("\n=== LOGIN ===\n");
        printf("1. Admin\n");
        printf("2. Customer\n");
        do {
            printf("Choose: ");
            scanf("%d", &choice);

            if (choice < 1 || choice > 2) {
                printf("Invalid Menu!\n");
            }

        } while (choice < 1 || choice > 2);

        switch (choice)
        {

        case 1:

            printf("Username : ");
            scanf("%s", username);

            printf("Password : ");
            scanf("%s", password);

            if (strcmp(username, "admin") == 0 && strcmp(password, "12345") == 0)
            { 
                role = 1;
                printf("Login Success as Admin!\n");

                pressEnter();
                clearScreen();
            }
            else
            {
                printf("Wrong Username or Password!\n");

                pressEnter();
                clearScreen();
            }
            break;

        case 2:
            role = 2;
            printf("Login Success as Customer!\n");

            pressEnter();
            clearScreen();
            break;

        default:
            printf("Invalid Menu!\n");
        }

    } while (role == 0);
}

int checkName(struct product *curr, char name[]) {

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

int checkID(struct product *curr, int id) {

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

int isInvalidIDLength(int id) {
    if (id < 10000 || id > 99999)
    {
        return 1;
    }

    return 0;
}

void insert() {
    clearScreen();

    int id, price, usedID, invalidIdLength;
    char name[100];

    printf("\n================= PRODUCT LIST =================\n");
    printf("+-------+---------------------+----------------+\n");
    printf("| ID    | Name                | Price          |\n");
    printf("+-------+---------------------+----------------+\n");
    displayproduct();
    printf("+-------+---------------------+----------------+\n");

    printf("\n=== INSERT PRODUCT ===\n");
    do
    {
        printf("Product Name: ");
        scanf(" %[^\n]", name);

        if (checkName(root, name))
        {
            printf("Product is already exists!\n");
        }
    } while (checkName(root, name));

    do
    {
        printf("Product ID [5 digits]: ");
        scanf("%d", &id);

        usedID = checkID(root, id);
        invalidIdLength = isInvalidIDLength(id);

        if (usedID)
        {
            printf("Product Id is already used!\n");
        }

        if (invalidIdLength)
        {
            printf("Product Id must be 5 digits!\n");
        }
    } while (usedID || invalidIdLength);

    do
    {
        printf("Product Price: ");
        scanf("%d", &price);

        if (price <= 0)
        {
            printf("Price cannot be negative or zero!\n");
        }
    } while (price <= 0);

    root = insertproduct(root, id, name, price);
    printf("\nProduct Added Successfully!\n");

    pressEnter();
    clearScreen();
}

void searchID(struct product *curr, int id) {

    if (curr == NULL)
    {
        printf("Product Not Found!\n");
        return;
    }

    if (curr->id == id)
    {
        printf("\n--- PRODUCT FOUND ---\n");
        printf("ID    : %d\n", curr->id);
        printf("Name  : %s\n", curr->name);
        printf("Price : Rp");
        printRupiah(curr->price);
        printf("\n");
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

void searchName(struct product *curr, char name[]) {

    if (curr == NULL)
    {
        return;
    }

    searchName(curr->left, name);

    if (strcmp(curr->name, name) == 0)
    {

        printf("\n=== PRODUCT FOUND ===\n");
        printf("ID    : %d\n", curr->id);
        printf("Name  : %s\n", curr->name);
        printf("Price : Rp");
        printRupiah(curr->price);
        printf("\n");
    }

    searchName(curr->right, name);
} 

void search() {
    clearScreen();

    int choice;

    do
    {

        printf("\n=== SEARCH PRODUCT ===\n");
        printf("1. Search by ID\n");
        printf("2. Search by Name\n");
        printf("3. Back to Main Menu\n");
        printf("Choose: ");
        scanf("%d", &choice);

        if (choice == 1)
        {

            int id;

            printf("Input Product ID: ");
            scanf("%d", &id);

            searchID(root, id);

            pressEnter();
            clearScreen();
        }

        else if (choice == 2)
        {

            char name[100];

            printf("Input Product Name: ");
            scanf(" %[^\n]", name);

            int found = checkName(root, name);

            if (found)
            {
                searchName(root, name);
            }
            else
            {
                printf("Product Not Found!\n");
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
            printf("Invalid Menu!\n");
        }

    } while (choice < 1 || choice > 3);
} 

void delete() {
    clearScreen();

    int choice, id;
    char name[100], confirm;

    printf("\n=== DELETE PRODUCT ===\n");
    printf("1. Delete by ID\n");
    printf("2. Delete by Name\n");
    printf("3. Back to Main Menu\n");
    do
    {
        printf("Choose: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 3)
        {
            printf("Invalid Menu!\n");
        }
    } while (choice < 1 || choice > 3);

    switch (choice)
    {
    case 1:
        do
        {
            printf("Input Product ID: ");
            scanf("%d", &id);

            searchID(root, id);

            if (checkID(root, id))
            {
                printf("Are you sure you want to delete this product? [y/n]: ");
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y')
                {
                    root = deleteproduct(root, id);
                    printf("Product deleted successfully!\n");

                    pressEnter();
                    clearScreen();
                    return;
                }
                else
                {
                    printf("Deletion cancelled.\n");

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
            printf("Input Product Name: ");
            scanf(" %[^\n]", name);

            if (!checkName(root, name))
            {
                printf("Product Not Found!\n");
            }
            else
            {
                searchName(root, name);
            }

            if (checkName(root, name))
            {
                printf("Are you sure you want to delete this product? [y/n]: ");
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y')
                {
                    int id = getIDByName(root, name);

                    if (id != -1)
                    {
                        root = deleteproduct(root, id);
                        printf("Product deleted successfully!\n");

                        pressEnter();
                        clearScreen();
                        return;
                    }
                }
                else
                {
                    printf("Deletion cancelled.\n");
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

void updatePriceByID(struct product *curr, int id, int newPrice) {
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

void updatePriceByName(struct product *curr, char name[], int newPrice) {
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

void update() {
    clearScreen();

    int choice, id, newPrice;
    char name[100];

    printf("\n=== UPDATE PRODUCT ===\n");
    printf("1. Update by ID\n");
    printf("2. Update by Name\n");
    printf("3. Back to Main Menu\n");
    do
    {
        printf("Choose: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 3)
        {
            printf("Invalid Menu!\n");
        }
    } while (choice < 1 || choice > 3);

    if (choice == 1)
    {
        do
        {
            printf("Input Product ID: ");
            scanf("%d", &id);

            searchID(root, id);

            if (checkID(root, id))
            {
                do
                {
                    printf("Input New Price: ");
                    scanf("%d", &newPrice);

                    if (newPrice <= 0)
                    {
                        printf("Price cannot be negative or zero!\n");
                    }
                } while (newPrice <= 0);

                updatePriceByID(root, id, newPrice);

                printf("Product price updated successfully!\n");

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
            printf("Input Product Name: ");
            scanf(" %[^\n]", name);

            if (!checkName(root, name))
            {
                printf("Product Not Found!\n");
            }
            else
            {
                searchName(root, name);
            }

            if (checkName(root, name))
            {
                do
                {
                    printf("Input New Price: ");
                    scanf("%d", &newPrice);

                    if (newPrice <= 0)
                    {
                        printf("Price cannot be negative or zero!\n");
                    }
                } while (newPrice <= 0);

                updatePriceByName(root, name, newPrice);

                printf("Product price updated successfully!\n");

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

void findProduct(struct product *curr, int id, struct product **result) {

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

void checkout() {
    clearScreen();

    if (root == NULL)
    {
        printf("No Product Available!\n");

        pressEnter();
        clearScreen();
        return;
    }
    else
    {
        printf("\n================= PRODUCT LIST =================\n");
        printf("+-------+---------------------+----------------+\n");
        printf("| ID    | Name                | Price          |\n");
        printf("+-------+---------------------+----------------+\n");
        displayproduct();
        printf("+-------+---------------------+----------------+\n");
    }

    int totalall = 0;
    char choice;

    do
    {

        int id, qty;

        printf("\n--- CHECKOUT ---\n");

        struct product *item = NULL;

        do {
            printf("Input Product ID : ");
            scanf("%d", &id);

            item = NULL;
            findProduct(root, id, &item);

            if (item == NULL) {
                printf("Product Not Found!\n");
            }

        } while (item == NULL);

        do {
            printf("Input Quantity   : ");
            scanf("%d", &qty);

            if (qty <= 0) {
                printf("Quantity must be greater than 0!\n");
            }
        } while (qty <= 0);

        int total = item->price * qty;
        totalall += total;

        printf("\nItem Added!\n");
        printf("Product : %s\n", item->name);
        printf("Subtotal: Rp");
        printRupiah(total);
        printf("\n");

        do {
            printf("\nAdd More Item? [y/n]: ");
            scanf(" %c", &choice);

            if (choice != 'y' && choice != 'Y' &&
                choice != 'n' && choice != 'N')
            {
                printf("Invalid Choice!\n");
            }

        } while (choice != 'y' && choice != 'Y' &&
                 choice != 'n' && choice != 'N');

    } while (choice == 'y' || choice == 'Y');

    printf("\n=== FINAL TOTAL ===\n");
    printf("Grand Total : Rp");
    printRupiah(totalall);
    printf("\n");

    pressEnter();
    clearScreen();
}

void sortingMenu() {
    clearScreen();

    printf("=== SORTING MENU ===\n");

    do {
        printf("1. Sort by Name\n");
        printf("2. Sort by Price\n");
        printf("Choose: ");
        scanf("%d", &sortType);

        if (sortType < 1 || sortType > 2) {
            printf("Invalid Choice!\n");
        }
    } while (sortType < 1 || sortType > 2);

    printf("\n");

    do {
        printf("1. Ascending\n");
        printf("2. Descending\n");
        printf("Choose: ");
        scanf("%d", &sortOrder);

        if (sortOrder < 1 || sortOrder > 2) {
            printf("Invalid Choice!\n");
        }
    } while (sortOrder < 1 || sortOrder > 2);

    pressEnter();
    clearScreen();
}

void logout() {
    char confirm;

    do {
        printf("Are you sure you want to logout? [y/n]: ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            role = 0;
            printf("Logout Success!\n");

            pressEnter();
            clearScreen();

            login();
            return;
        } else if (confirm == 'n' || confirm == 'N') {
            printf("Logout Cancelled!\n");

            pressEnter();
            clearScreen();
            return;
        } else {
            printf("Invalid Choice!\n");
        }

    } while (1);
}

int main() {
    root = insertproduct(root, 12345, "Beer", 50000);
    root = insertproduct(root, 12346, "Wine", 750000);
    root = insertproduct(root, 12347, "Vodka", 350000);
    root = insertproduct(root, 12348, "Tequila", 650000);
    root = insertproduct(root, 12349, "Gin", 400000);
    root = insertproduct(root, 12350, "Sake", 350000);

    login();

    int choice;

    do
    {

        printf("\n================= PRODUCT LIST =================\n");

        if (root == NULL)
        {
            printf("No Product Available!\n");
        }
        else
        {
            printf("+-------+---------------------+----------------+\n");
            printf("| ID    | Name                | Price          |\n");
            printf("+-------+---------------------+----------------+\n");
            displayproduct();
            printf("+-------+---------------------+----------------+\n");
        }

        printf("\n+--------------------------------+\n");
        printf("|          LIQUOR STORE          |\n");
        printf("+--------------------------------+\n");
        printf("| 1. Insert Product [Admin Only] |\n");
        printf("| 2. Search Product              |\n");
        printf("| 3. Update Product [Admin Only] |\n");
        printf("| 4. Delete Product [Admin Only] |\n");
        printf("| 5. Checkout Product            |\n");
        printf("| 6. Sorting Menu                |\n");
        printf("| 7. Logout                      |\n");
        printf("| 8. Exit Program                |\n");
        printf("+--------------------------------+\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice)
        {

        case 1:

            if (role != 1)
            {
                printf("Access Denied! Admin Only!\n");
                pressEnter();
                clearScreen();
            }
            else
            {
                insert();
            }

            break;

        case 2:
            search();
            break;

        case 3:

            if (role != 1)
            {
                printf("Access Denied! Admin Only!\n");
                pressEnter();
                clearScreen();
            }
            else
            {
                update();
            }

            break;

        case 4:

            if (role != 1)
            {
                printf("Access Denied! Admin Only!\n");
                pressEnter();
                clearScreen();
            }
            else
            {
                delete();
            }

            break;

        case 5:
            checkout();
            break;

        case 6:
            sortingMenu();
            break;

        case 7:
            logout();
            break;

        case 8:
            printf("\nThank You!\n");
            break;

        default:
            printf("Invalid Menu!\n");
            pressEnter();
            clearScreen();
        }

    } while (choice != 8);

    freeTree(root);
    return 0;
}
