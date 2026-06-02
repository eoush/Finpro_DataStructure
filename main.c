#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct product {
    int id;
    char name[100];
    int price;
    int height;
    struct product *left;
    struct product *right;
};

struct product *root = NULL;
struct product *curr = NULL;

struct product* createproduct(int id, char name[], int price) {

    struct product *node = (struct product*)malloc(sizeof(struct product));

    node->id = id;
    strcpy(node->name, name);
    node->price = price;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

int getheight(struct product *node) {

    if (node == NULL) {
        return 0;
    }

    return node->height;
}

int max(int a, int b) {

    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}

int getbalance(struct product *node) {

    if (node == NULL) {
        return 0;
    }

    return getheight(node->left) - getheight(node->right);
}

struct product* leftrotate(struct product *x) {

    struct product *y = x->right;
    struct product *temp = y->left;

    y->left = x;
    x->right = temp;

    // update height
    x->height = 1 + max(getheight(x->left), getheight(x->right));
    y->height = 1 + max(getheight(y->left), getheight(y->right));

    return y;
}

struct product* rightrotate(struct product *y) {

    struct product *x = y->left;
    struct product *temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = 1 + max(getheight(y->left), getheight(y->right));
    x->height = 1 + max(getheight(x->left), getheight(x->right));

    return x;
}

struct product* insertproduct(struct product *node, int id, char name[], int price) {

    if (node == NULL) {
        return createproduct(id, name, price);
    }

    if (id < node->id) {
        node->left = insertproduct(node->left, id, name, price);
    }
    else if (id > node->id) {
        node->right = insertproduct(node->right, id, name, price);
    }
    else {
        return node;
    }

    node->height = 1 + max(getheight(node->left), getheight(node->right));

    int balance = getbalance(node);

    //ll
    if (balance > 1 && id < node->left->id) {
        return rightrotate(node);
    }

    //rr
    if (balance < -1 && id > node->right->id) {
        return leftrotate(node);
    }

    //lr
    if (balance > 1 && id > node->left->id) {
        node->left = leftrotate(node->left);
        return rightrotate(node);
    }

    //rl
    if (balance < -1 && id < node->right->id) {
        node->right = rightrotate(node->right);
        return leftrotate(node);
    }

    return node;
}

struct product* findmin(struct product *node) {

    struct product *curr = node;

    while (curr->left != NULL) {
        curr = curr->left;
    }

    return curr;
}

struct product* deleteproduct(struct product *node, int id) {

    if (node == NULL) {
        return NULL;
    }

    if (id < node->id) {
        node->left = deleteproduct(node->left, id);
    }
    else if (id > node->id) {
        node->right = deleteproduct(node->right, id);
    }
    else {
        if (node->left == NULL || node->right == NULL) {

            struct product *temp = NULL;

            if (node->left != NULL) {
                temp = node->left;
            }
            else {
                temp = node->right;
            }

            if (temp == NULL) {
                temp = node;
                node = NULL;
            }
            else {
                *node = *temp;
            }

            free(temp);
        }
        else {

            struct product *temp = findmin(node->right);

            node->id = temp->id;
            strcpy(node->name, temp->name);
            node->price = temp->price;

            node->right = deleteproduct(node->right, temp->id);
        }
    }

    if (node == NULL) {
        return NULL;
    }

    node->height = 1 + max(getheight(node->left), getheight(node->right));

    int balance = getbalance(node);

    //ll
    if (balance > 1 && getbalance(node->left) >= 0) {
        return rightrotate(node);
    }

    //lr
    if (balance > 1 && getbalance(node->left) < 0) {
        node->left = leftrotate(node->left);
        return rightrotate(node);
    }

    //rr
    if (balance < -1 && getbalance(node->right) <= 0) {
        return leftrotate(node);
    }

    //rl
    if (balance < -1 && getbalance(node->right) > 0) {
        node->right = rightrotate(node->right);
        return leftrotate(node);
    }

    return node;
}

void displayproduct(struct product *node) {

    if (node == NULL) {
        return;
    }

    displayproduct(node->left);

    printf("| %-5d | %-20s | %-10d |\n", node->id, node->name, node->price);

    displayproduct(node->right);
}

int role = 0;

void login(){

    int choice;
    char username[100];
    char password[100];

    do{

        printf("\n=== LOGIN ===\n");
        printf("1. Admin\n");
        printf("2. Customer\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch(choice){

            case 1:

                printf("Username : ");
                scanf("%s", username);

                printf("Password : ");
                scanf("%s", password);

                if(strcmp(username, "admin") == 0 && strcmp(password, "12345") == 0);// kalo login admin pake usn "admin" sama pass "12345"
                    role = 1;
                    printf("Login Success as Admin!\n");
                }
                else{
                    printf("Wrong Username or Password!\n");
                }
                break;

            case 2:
                role = 2;
                printf("Login Success as Customer!\n");
                break;

            default:
                printf("Invalid Menu!\n");
        }

    }while(role == 0);
}

int checkName(struct product* curr, char name[]){

    if (curr == NULL){
        return 0;
    }

    if (strcmp(curr->name, name) == 0){
        return 1;
    }

    int leftCheck = checkName(curr->left, name);
    if (leftCheck){
        return 1;
    }

    return checkName(curr->right, name);
}

int checkID(struct product* curr, int id){

    if (curr == NULL){
        return 0;
    }

    if (curr->id == id){
        return 1;
    }

    if (id < curr->id){
        return checkID(curr->left, id);
    } else {
        return checkID(curr->right, id);
    }
}

int isInvalidIDLength(int id){
    if(id < 10000 || id > 99999) {
        return 1;
    }

    return 0;
}

void insert(){
    int id, price, usedID, invalidIdLength;
    char name[100];

    curr = (struct product*)malloc(sizeof(struct product));
    curr = root;

    printf("\n=== INSERT PRODUCT ===\n");
    do {
        printf("Product Name : ");
        scanf(" %[^\n]", name);

        if(checkName(curr, name)){
            printf("Product is already exists!\n");
        }
    } while (checkName(curr, name));

    do {
        printf("Product ID   : ");
        scanf("%d", &id);

        usedID = checkID(curr, id);
        invalidIdLength = isInvalidIDLength(id);

        if(usedID) {
            printf("Product Id is already used!\n");
        }

        if (invalidIdLength) {
            printf("Product Id must be 5 digits!\n");
        }
    } while (usedID || invalidIdLength);

    do {
        printf("Product Price: ");
        scanf("%d", &price);

        if (price <= 0){
            printf("Price cannot be negative or zero!\n");
        }
    } while (price <= 0);

    printf("\nProduct Added Successfully!\n");
}

//ini pny DAVED
void searchID(struct product* curr, int id){

    if (curr == NULL){
        printf("Product Not Found!\n");
        return;
    }

    if (curr->id == id){
        printf("\n--- PRODUCT FOUND ---\n");
        printf("ID    : %d\n", curr->id);
        printf("Name  : %s\n", curr->name);
        printf("Price : %d\n", curr->price);
        return;
    }

    if (id < curr->id){
        searchByID(curr->left, id);
    }
    else {
        searchByID(curr->right, id);
    }
}// cek subtree kiri abis gitu cek subtree kanan sampe nemu

void searchName(struct product* curr, char name[]){

    if (curr == NULL){
        printf("Product Not Found!\n");
        return;
    }

    searchByName(curr->left, name);

    if (strcmp(curr->name, name) == 0){

        printf("\n=== PRODUCT FOUND ===\n");
        printf("ID    : %d\n", curr->id);
        printf("Name  : %s\n", curr->name);
        printf("Price : %d\n", curr->price);
    }

    searchByName(curr->right, name);
}// sama ini juga cek kiri baru kanan

void search(){

    int choice;

    do{

        printf("\n=== SEARCH PRODUCT ===\n");
        printf("1. Search by ID\n");
        printf("2. Search by Name\n");
        printf("Choose: ");
        scanf("%d", &choice);

        if (choice == 1){

            int id;

            printf("Input Product ID: ");
            scanf("%d", &id);

            searchByID(root, id);
        }

        else if (choice == 2){

            char name[100];

            printf("Input Product Name: ");
            scanf(" %[^\n]", name);

            searchByName(root, name);
        }

        else{
            printf("Invalid Menu!\n");
        }

    } while(choice < 1 || choice > 2);
}// ini buat yang di int main

// belum kelar
void delete(){
    int choice, id;
    char name[100], confirm;

    printf("\n=== DELETE PRODUCT ===\n");
    printf("1. Delete by ID\n");
    printf("2. Delete by Name\n");
    do {
        printf("Choose: ");
        scanf("%d", &choice);

        if(choice < 1 || choice > 2){
            printf("Invalid Menu!\n");
        }
    } while(choice < 1 || choice > 2);

    switch (choice){
        case 1:
            do {
                printf("Input Product ID: ");
                scanf("%d", &id);

                searchByID(root, id);

                if(checkID(root, id)) {
                    printf("Are you sure you want to delete this product? [y/n]: ");
                    scanf(" %c", &confirm);
                    if(confirm == 'y' || confirm == 'Y') {
                        // Call delete function here
                        printf("Product deleted successfully!\n");
                    } else {
                        printf("Deletion cancelled.\n");
                        break;
                    }
                }
            } while(!checkID(root, id));
            break;
        case 2:
            do {
                printf("Input Product Name: ");
                scanf(" %[^\n]", name);

                searchByName(root, name);

                if(checkName(root, name)) {
                    printf("Are you sure you want to delete this product? [y/n]: ");
                    scanf(" %c", &confirm);
                    if(confirm == 'y' || confirm == 'Y') {
                        // Call delete function here
                        printf("Product deleted successfully!\n");
                    } else {
                        printf("Deletion cancelled.\n");
                        break;
                    }
                }
            } while(!checkName(root, name));
            break;
    }
}

// belum kelar
void update(){
    int choice, id, newPrice;
    char name[100];

    printf("\n=== UPDATE PRODUCT ===\n");
    printf("1. Update by ID\n");
    printf("2. Update by Name\n");
    do {
        printf("Choose: ");
        scanf("%d", &choice);

        if(choice < 1 || choice > 2){
            printf("Invalid Menu!\n");
        }
    } while(choice < 1 || choice > 2);

    if(choice == 1) {
        do {
            printf("Input Product ID: ");
            scanf("%d", &id);

            searchByID(root, id);

            if(checkID(root, id)) {
                printf("Input New Price: ");
                scanf("%d", &newPrice);
                // Call update function here
                printf("Product price updated successfully!\n");
            }
        } while(!checkID(root, id));
    } else {      
        do {
            printf("Input Product Name: ");
            scanf(" %[^\n]", name);

            searchByName(root, name);

            if(checkName(root, name)) {
                printf("Input New Price: ");
                scanf("%d", &newPrice);
                // Call update function here
                printf("Product price updated successfully!\n");
            }
        } while(!checkName(root, name));
    }            
}

void findProduct(struct product* curr, int id, struct product** result){

    if (curr == NULL) {
        return;
    }

    if (curr->id == id){
        *result = curr;
        return;
    }

    if (id < curr->id){
        findProduct(curr->left, id, result);
    }
    else {
        findProduct(curr->right, id, result);
    }
}// ini buat menu menu kalo mau cari (buat menu c/o, update, delete ya isa)

void checkout(){

    if (root == NULL){
        printf("No Product Available!\n");
        return;
    }

    int totalall = 0;
    char choice;

    do {

        int id, qty;

        printf("\n--- CHECKOUT ---\n");

        printf("Input Product ID : ");
        scanf("%d", &id);

        struct Product* item = NULL;

        findProduct(root, id, &item);

        if (item == NULL){
            printf("Product Not Found!\n");
        }
        else{

            printf("Input Quantity   : ");
            scanf("%d", &qty);

            int total = item->price * qty;
            totalall += total;

            printf("\nItem Added!\n");
            printf("Product : %s\n", item->name);
            printf("Subtotal: %d\n", total);
        }

        printf("\nAdd More Item? [y/n]: ");
        scanf("%c", &choice);

    } while(choice == 'y' || choice == 'Y');

    printf("\n=== FINAL TOTAL ===\n");
    printf("Grand Total : %d\n", totalall);
}//ini isa beli item, terus kalo udah ditanyain mau beli lagi apa nda, sampe udah mau berentu

int main(){

    login();

    int choice;

    do {

        printf("\n=== PRODUCT LIST ===\n");

        if (root == NULL) {
            printf("No Product Available!\n");
        }
        else {
            displayProduct(root);
        }

        printf("\n+--------------------------+\n");
        printf("|        TOKO MABOK        |\n");
        printf("+--------------------------+\n");
        printf("| 1. Insert Product        |\n");
        printf("| 2. Search Product        |\n");
        printf("| 3. Update Product        |\n");
        printf("| 4. Delete Product        |\n");
        printf("| 5. Checkout Product      |\n");
        printf("| 6. Exit Program          |\n");
        printf("+--------------------------+\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch(choice){

            case 1:

                if(role != 1){
                    printf("Access Denied! Admin Only!\n");
                }
                else{
                    insert();
                }

                break;

            case 2:
                search();
                break;

            case 3:

                if(role != 1){
                    printf("Access Denied! Admin Only!\n");
                }
                else{
                    update();
                }

                break;

            case 4:

                if(role != 1){
                    printf("Access Denied! Admin Only!\n");
                }
                else{
                    delete();
                }

                break;

            case 5:
                checkout();
                break;

            case 6:
                printf("Thank You!\n");
                break;

            default:
                printf("Invalid Menu!\n");
        }

    } while(choice != 6);

    return 0;
}
