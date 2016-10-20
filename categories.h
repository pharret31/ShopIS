#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <cstring>
#include <deque>
#include <cstdlib>
#include <cstdio>

#include <QDebug>

#define PRODUCTS_AMOUNT 100

using namespace std;

struct product {
    int weight;
    char name[20];
    char providerName[20];
};

struct categories {
    int maxWeight;
    char categoryName[20];
    int countProducts;
    product products[PRODUCTS_AMOUNT];
};

class Categories
{
public:
    Categories(char *fileName);
    ~Categories();

    deque<categories> buffer;
    deque<categories>::iterator iter;

    void updateBuffer();
    void saveBuffer();

private:
    char *m_fileName;
    FILE *m_file;
    categories *m_prod;
};

#endif // CATEGORIES_H
