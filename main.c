#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//nama stuctnya product aja ya biar sama.... ty
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
            scanf("%[^\n]", name);

            searchByName(root, name);
        }

        else{
            printf("Invalid Menu!\n");
        }

    } while(choice < 1 || choice > 2);
}// ini buat yang di int main

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

    int choice;

    do {

        printf("\n=== PRODUCT LIST ===\n");

        if (root == NULL) {
            printf("No Product Available!\n");
        }
        else {
            displayProduct(root);
        }

        printf("\n--- TOKO MABOK ---\n");
        printf("1. Insert Product\n");
        printf("2. Search Product\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("5. Checkout\n");
        printf("6. Exit Program\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch(choice){

            case 1:
                //nnti
                break;

            case 2:
                seach();
                break;

            case 3:
                //nnti
                break;

            case 4:
                //nnti
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
