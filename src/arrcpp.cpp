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

size_t var::indexOf(const var &key)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data; 
    for(size_t i=0; i < arr->size(); i++)
    {   if((*arr)[i] == key)
        {   return i;
        }
    }

    return -1;
}

bool var::includes(const var &key)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = (array*)data; 
    for(size_t i=0; i < arr->size(); i++)
    {   if((*arr)[i] == key)
        {   return true;
        }
    }

    return false;
}

size_t var::length()
{
    return type == array_t ? ((array*)data)->size() : -1;
}

int var::push(const var &e)
{
    if(type != array_t)
    {   throw "Error";
    }

    ((array*)data)->push_back(e);
    return ((array*)data)->size();
}

int var::push_back(const var &e)
{
    if(type != array_t)
    {   throw "Error";
    }

    ((array*)data)->push_back(e);
    return ((array*)data)->size();
}

var var::pop()
{
    if(type != array_t)
    {   throw "Error";
    }

    if(((array*)data)->size() == 0)
    {    throw "Error";
    }

    var ret = ((array*)data)->back();
    ((array*)data)->pop_back();

    return ret;
}

var var::pop_back()
{
    if(type != array_t)
    {   throw "Error";
    }

    if(((array*)data)->size() == 0)
    {    throw "Error";
    }

    var ret = ((array*)data)->back();
    ((array*)data)->pop_back();

    return ret;
}

int var::unshift(const var &e)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = ((array*)data);
    arr->insert(arr->begin(), e);
    return arr->size();
}

int var::push_front(const var &e)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = ((array*)data);
    arr->insert(arr->begin(), e);
    return arr->size();
}

var var::shift()
{
    if(type != array_t)
    {   throw "Error";
    }

    if(((array*)data)->size() == 0)
    {    throw "Error";
    }

    array* arr = ((array*)data);
    var ret = arr->front();
    arr->erase(arr->begin());

    return ret;
}

var var::pop_front()
{
    if(type != array_t)
    {   throw "Error";
    }

    if(((array*)data)->size() == 0)
    {    throw "Error";
    }

    array* arr = ((array*)data);
    var ret = arr->front();
    arr->erase(arr->begin());

    return ret;
}

int var::insert(size_t pos, const var &e)
{
    if(type != array_t)
    {   throw "Error";
    }

    if(pos<0)
    {   throw "Error";
    }

    array* arr = ((array*)data);
    if(arr->size() < pos)
    {   arr->resize(pos+1);
        (*arr)[pos]=e;
    }
    else
    {    arr->insert(arr->begin()+pos, e);
    }

    return arr->size();
}

int var::erase(size_t pos)
{
    if(type != array_t)
    {   throw "Error";
    }

    if(pos<0)
    {   throw "Error";
    }

    array* arr = ((array*)data);
    if(arr->size() >= pos)
    {   arr->erase(arr->begin()+pos);
    }

    return arr->size();
}

int var::erase(size_t start, size_t end)
{
    if(type != array_t)
    {   throw "Error";
    }

    if(start > end || start < 0 || end < 0)
    {   throw "Error";
    }

    array* arr = ((array*)data);
    arr->erase(arr->begin() + start,  end < arr->size() ? arr->begin() + end : arr->end());
    
    return arr->size();
}

var var::slice(int beg, int end)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = ((array*)data);
    size_t len = arr->size();
    size_t bIdx = beg >= 0 ? beg : len + beg;
    size_t eIdx = end == 0 ? len : end > 0 ? end : len + end;

    var res = array{};
    array* rArr = ((array*)res.data);

    std::cout<< bIdx << " " << eIdx << "\n";

    if(bIdx < 0 || eIdx < 0 || eIdx <= bIdx || bIdx > len)
    {   return res;
    }

    if(eIdx > len)
    {   eIdx = len;
    }

    rArr->resize(eIdx-bIdx);
    for(size_t e = 0, i = bIdx; i < eIdx; i++, e++)
    {   (*rArr)[e] = (*arr)[i];
    }

    return res;
}

void var::splice(int beg, int count )
{
    splice(beg, count, var());
}

void var::splice(int beg, const var &elements)
{
    splice(beg, 0, elements);
}

void var::splice(int beg, int count, const var &elements)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = ((array*)data);
    array* eArr = ((array*)elements.data);
    size_t len = arr->size();
    size_t bIdx = beg >= 0 ? beg : len + beg + 1;
    size_t num = bIdx + count > len ? len - bIdx : count;

    if(bIdx < 0 || bIdx > len)
    {   throw "Error";
    }

    if(num > 0)
    {   arr->erase(arr->begin()+bIdx, arr->begin()+bIdx+num);
    }

    if(elements.type == array_t)
    {   arr->insert(arr->begin()+bIdx, eArr-> begin(), eArr->end());
    }
    return;
}

str var::join(str separator)
{
    if(type != array_t)
    {   throw "Error";
    }

    array* arr = ((array*)data);
    
    if(arr->size() == 0)
    {   return "";
    }

    str res = (*arr)[0].toString();
    for(size_t i = 0; i<arr->size(); i++)
    {   res += separator + (*arr)[i].toString();
    }

    return res;
}

void var::reverse()
{
    if(type != array_t)
    {   throw "Error";
    }

    var temp;
    array* arr = ((array*)data);

    for(size_t beg = 0, end = arr->size()-1; beg < end; beg++, end--)
    {   temp = (*arr)[beg];
        (*arr)[beg] = (*arr)[end];
        (*arr)[end] = temp;
    }
}
