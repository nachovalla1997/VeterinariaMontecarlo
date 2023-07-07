#ifndef RESOURCE_H
#define RESOURCE_H

typedef struct Resource
{
    int id;
    char name[50];
    char description[100];
    int quantity;
    int price;
} Resource;

Resource *createResource(int id, char *name, char *description, int quantity, int price);

#endif // RESOURCE_H