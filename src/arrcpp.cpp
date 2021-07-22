#include "varcpp.h"

void var::fill(size_t start, size_t end, var val)
{
    if(type != array_t)
    {   throw "Error";
    }

    if(end >= start && end > 0)
    {   
        array* arr = (array*)data;
        if(end > arr->size())
        {   arr->resize(end);
        }

        for(size_t i = start; i<end; i++)
        {   (*arr)[i] = val;
        }
    }
}

void var::fill(size_t end, var val)
{
    if(type != array_t)
    {   throw "Error";
    }

    if(end > 0)
    {   
        array* arr = (array*)data;
        if(end > arr->size())
        {   arr->resize(end);
        }

        for(size_t i = 0; i<end; i++)
        {   (*arr)[i] = val;
        }
    }
}