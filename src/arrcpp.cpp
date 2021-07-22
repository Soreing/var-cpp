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