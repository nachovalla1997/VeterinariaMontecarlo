#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource.h"

Resource *create_resource(int id, const char *name, const char *description)
{
    Resource *resource = malloc(sizeof(Resource));
    resource->id = id;
    strncpy(resource->name, name, sizeof(resource->name));
    return resource;
}