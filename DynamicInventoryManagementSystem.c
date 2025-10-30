#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 50

typedef struct
{
    char name[NAME_LENGTH];
    int product_id;
    int quantity;
    float price;
} Product;

Product *productList = NULL;
int totalProducts = 0;

void displayMenu()
{
    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
    printf("==================================\n");
    printf("Enter your choice: ");
}

void showProducts()
{
    printf("\n========= PRODUCT LIST =========\n");
    if (totalProducts == 0)
    {
        printf("Inventory is empty.\n");
        return;
    }

    int i = 0;
    while (i < totalProducts)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               productList[i].product_id,
               productList[i].name,
               productList[i].price,
               productList[i].quantity);
        i++;
    }
}
void addProduct()
{
    Product *temp = (Product *)realloc(productList, (totalProducts + 1) * sizeof(Product));

    if (temp == NULL)
    {
        printf("\nMemory allocation failed, cannot add new product.\n");
        return;
    }

    productList = temp;

    printf("\nEnter Details for new Product:\n");
    printf("Product ID: ");
    scanf("%d", &productList[totalProducts].product_id);
    if (productList[totalProducts].product_id < 1 || productList[totalProducts].product_id > 10000)
    {
        printf("Invalid Product ID. Must be 1-10000. Exiting.\n");
        exit(1);
    }

    printf("Product Name: ");
    scanf(" %49[^\n]", productList[totalProducts].name);

    printf("Product Price: ");
    scanf("%f", &productList[totalProducts].price);
    if (productList[totalProducts].price < 0 || productList[totalProducts].price > 100000)
    {
        printf("Invalid Price. Must be 0-100000. Exiting.\n");
        exit(1);
    }

    printf("Product Quantity: ");
    scanf("%d", &productList[totalProducts].quantity);
    if (productList[totalProducts].quantity < 0 || productList[totalProducts].quantity > 1000000)
    {
        printf("Invalid Quantity. Must be 0-1000000. Exiting.\n");
        exit(1);
    }

    totalProducts++;
    printf("\nProduct Added Successfully!\n");
}

void updateQuantity()
{
    int id, newQuantity;
    int foundIndex = -1;

    printf("\nEnter Product ID to update quantity: ");
    scanf("%d", &id);
    if (id < 1 || id > 10000)
    {
        printf("Invalid Product ID format. Exiting.\n");
        exit(1);
    }

    int i = 0;
    while (i < totalProducts)
    {
        if (productList[i].product_id == id)
        {
            foundIndex = i;
            break;
        }
        i++;
    }

    if (foundIndex != -1)
    {
        printf("Enter new Quantity: ");
        scanf("%d", &newQuantity);
        if (newQuantity < 0 || newQuantity > 1000000)
        {
            printf("Invalid Quantity. Must be 0-1000000. Exiting.\n");
            exit(1);
        }

        productList[foundIndex].quantity = newQuantity;
        printf("Quantity updated successfully!\n");
    }
    else
    {
        printf("Product not found.\n");
    }
}

void searchProductById()
{
    int id;
    int found = 0;

    printf("\nEnter Product ID to Search: ");
    scanf("%d", &id);
    if (id < 1 || id > 10000)
    {
        printf("Invalid Product ID format. Exiting.\n");
        exit(1);
    }

    int i = 0;
    while (i < totalProducts)
    {
        if (productList[i].product_id == id)
        {
            printf("Product Found:\n");
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   productList[i].product_id,
                   productList[i].name,
                   productList[i].price,
                   productList[i].quantity);
            found = 1;
            break;
        }
        i++;
    }

    if (!found)
    {
        printf("Product not found.\n");
    }
}

void searchProductByName()
{
    char name[NAME_LENGTH];
    int found = 0;

    printf("\nEnter name to search (partial allowed): ");
    scanf(" %49[^\n]", name);

    int i = 0;
    while (i < totalProducts)
    {
        if (strstr(productList[i].name, name) != NULL)
        {
            if (found == 0)
            {
                printf("Products Found:\n");
            }
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   productList[i].product_id,
                   productList[i].name,
                   productList[i].price,
                   productList[i].quantity);
            found = 1;
        }
        i++;
    }

    if (!found)
    {
        printf("No products found with that name.\n");
    }
}

void searchProductByPriceRange()
{
    float minPrice, maxPrice;
    int found = 0;

    printf("\nEnter minimum price: ");
    scanf("%f", &minPrice);
    if (minPrice < 0 || minPrice > 100000)
    {
        printf("Invalid Price. Must be 0-100000. Exiting.\n");
        exit(1);
    }

    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);
    if (maxPrice < 0 || maxPrice > 100000)
    {
        printf("Invalid Price. Must be 0-100000. Exiting.\n");
        exit(1);
    }

    printf("\nProducts in price range:\n");

    int i = 0;
    while (i < totalProducts)
    {
        if (productList[i].price >= minPrice && productList[i].price <= maxPrice)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   productList[i].product_id,
                   productList[i].name,
                   productList[i].price,
                   productList[i].quantity);
            found = 1;
        }
        i++;
    }

    if (!found)
    {
        printf("No products found in that price range.\n");
    }
}

void deleteProductById()
{
    int id;
    int foundIndex = -1;

    printf("\nEnter Product ID to delete: ");
    scanf("%d", &id);

    if (id < 1 || id > 10000)
    {
        printf("Invalid Product ID format. Exiting.\n");
        exit(1);
    }

    for (int i = 0; i < totalProducts; i++)
    {
        if (productList[i].product_id == id)
        {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1)
    {
        printf("Product not found.\n");
    }
    else
    {
        for (int i = foundIndex; i < totalProducts - 1; i++)
        {
            productList[i] = productList[i + 1];
        }
        totalProducts--;
        if (totalProducts == 0)
        {
            free(productList);
            productList = NULL;
        }
        else
        {
            Product *temp = (Product *)realloc(productList, totalProducts * sizeof(Product));
            if (temp != NULL)
            {
                productList = temp;
            }
        }

        printf("Product deleted successfully!\n");
    }
}

void runMenu()
{
    int choice = 0;
    do
    {
        displayMenu();
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            showProducts();
            break;
        case 3:
            updateQuantity();
            break;
        case 4:
            searchProductById();
            break;
        case 5:
            searchProductByName();
            break;
        case 6:
            searchProductByPriceRange();
            break;
        case 7:
            deleteProductById();
            break;
        case 8:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1-8.\n");
        }
    } while (choice != 8);
}

int main()
{
    printf("Enter the initial number of products: ");
    scanf("%d", &totalProducts);
    if (totalProducts < 1 || totalProducts > 100)
    {
        printf("Invalid initial product count. Must be 1-100. Exiting.\n");
        exit(1);
    }

    productList = (Product *)calloc(totalProducts, sizeof(Product));

    if (productList == NULL)
    {
        printf("Initial memory allocation failed. Exiting.\n");
        return 1;
    }
    int i = 0;
    while (i < totalProducts)
    {
        printf("\nEnter Details for Product %d\n", i + 1);
        printf("Product ID: ");
        scanf("%d", &productList[i].product_id);

        if (productList[i].product_id < 1 || productList[i].product_id > 10000)
        {
            printf("Invalid Product ID. Must be 1-10000. Exiting.\n");
            exit(1);
        }

        printf("Product Name: ");
        scanf(" %49[^\n]", productList[i].name);

        printf("Product Price: ");
        scanf("%f", &productList[i].price);

        if (productList[i].price < 0 || productList[i].price > 100000)
        {
            printf("Invalid Price. Must be 0-100000. Exiting.\n");
            exit(1);
        }

        printf("Product Quantity: ");
        scanf("%d", &productList[i].quantity);

        if (productList[i].quantity < 0 || productList[i].quantity > 1000000)
        {
            printf("Invalid Quantity. Must be 0-1000000. Exiting.\n");
            exit(1);
        }
        i++;
    }

    runMenu();

    free(productList);
    printf("Memory released successfully.\n");

    return 0;
}