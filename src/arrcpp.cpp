#include "varcpp.h"
#include <iostream>

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

bool var::every(bool(*fnc)(var e))
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data;
    for(size_t i=0; i<arr->size(); i++)
    {   if(!fnc((*arr)[i]))
        {   return false;
        }
    }

    return true;
}

bool var::every(bool(*fnc)(var e, void* params), void* params)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data;
    for(size_t i=0; i<arr->size(); i++)
    {   if(!fnc((*arr)[i], params))
        {   return false;
        }
    }

    return true;
}

bool var::some(bool(*fnc)(var e))
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data;
    for(size_t i=0; i<arr->size(); i++)
    {   if(fnc((*arr)[i]))
        {   return true;
        }
    }

    return false;
}

bool var::some(bool(*fnc)(var e, void* params), void* params)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data;
    for(size_t i=0; i<arr->size(); i++)
    {   if(fnc((*arr)[i], params))
        {   return true;
        }
    }

    return false;
}

var var::filter(bool(*fnc)(var e))
{
    if(type != array_t)
    {   throw "Error";
    }

    var result = array{};
    array* arr = (array*)data;
    ((array*)result.data)->resize(arr->size());

    size_t rIdx = 0; 
    for(size_t i=0; i<arr->size(); i++)
    {   if(fnc((*arr)[i]))
        {   result[rIdx] = (*arr)[i];
            rIdx++;
        }
    }

    ((array*)result.data)->resize(rIdx);
    return result;
}

var var::filter(bool(*fnc)(var e, void* params), void* params)
{
    if(type != array_t)
    {   throw "Error";
    }

    var result = array{};
    array* arr = (array*)data;
    ((array*)result.data)->resize(arr->size());

    size_t rIdx = 0; 
    for(size_t i=0; i<arr->size(); i++)
    {   if(fnc((*arr)[i], params))
        {   result[rIdx] = (*arr)[i];
            rIdx++;
        }
    }

    ((array*)result.data)->resize(rIdx);
    return result;
}

var& var::find(bool(*fnc)(var e))
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data; 
    for(size_t i=0; i < arr->size(); i++)
    {   if(fnc((*arr)[i]))
        {   return (*arr)[i];
        }
    }

    throw "Error";
}

var& var::find(bool(*fnc)(var e, void* params), void* params)
{
     if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data; 
    for(size_t i=0; i<arr->size(); i++)
    {   if(fnc((*arr)[i], params))
        {   return (*arr)[i];
        }
    }

    throw "Error";
}

int var::findIndex(bool(*fnc)(var e))
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data; 
    for(size_t i=0; i < arr->size(); i++)
    {   if( fnc((*arr)[i]) )
        {   return i;
        }
    }

    return -1;
}

int var::findIndex(bool(*fnc)(var e, void* params), void* params)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data; 
    for(size_t i=0; i < arr->size(); i++)
    {   if(fnc((*arr)[i], params))
        {   return i;
        }
    }

    return -1;
}

void var::forEach(void(*fnc)(var &e))
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data; 
    for(size_t i=0; i < arr->size(); i++)
    {   fnc((*arr)[i]);
    }

}

void var::forEach(void(*fnc)(var &e, void* params), void* params)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data; 
    for(size_t i=0; i < arr->size(); i++)
    {   fnc((*arr)[i], params);
    }
}