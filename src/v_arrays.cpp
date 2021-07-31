#include "varcpp.h"

#define ASSERT_ARRAY(a)    if((a)->type!=array_t) {throw var::TypeMismatchException();}
#define ASSERT_NOTEMPTY if(length()==0) {throw var::ArrayElementNotFoundException();}

#define ASSERT_RANGE(b, e, len) if(b<0 || e<0 || e<b) {throw var::ArrayIncorrectRangeException();}
#define ASSERT_INDEX(i) if(i<0) {throw var::ArrayIncorrectIndexException();}
#define ASSERT_RANGE_STRICT(b, e, len) if(b<0 || e<0 || e<b || b>=len || e >len) {throw var::ArrayIncorrectRangeException();}
#define ASSERT_INDEX_STRICT(i, len) if(i<0 || i>=len) {throw var::ArrayIncorrectIndexException();}

// Generates the correct index range for array functions
// Negative beg/end indicate index from the end of the array
// end == 0 indicates end of the array.
void getRange(int beg, int end, size_t len, size_t &bIdx, size_t &eIdx)
{
    bIdx = beg >= 0 ? beg : len + beg;
    eIdx = end == 0 ? len : end > 0 ? end : len + end;
}

// Generates the correct index for array functions
// Negative value indicate index from the end of the array
void getIndex(int val, size_t len, size_t &idx)
{
    idx = val >= 0 ? val : len + val;
}

// Fills the array between 2 indexes with a specific value
// Negative indexes are counted from the end of the array
// If the array is too small, it gets increased to fit.
// Asserts that indexes must be within the valid range and begin is before end.
void var::fill(int beg, int end, const var &val)
{   
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    size_t bIdx, eIdx;
    getRange(beg, end, arr->size(), bIdx, eIdx);
    ASSERT_RANGE(bIdx, eIdx, len);

    if(eIdx > arr->size())
    {   arr->resize(eIdx);
    }

    for(size_t i = bIdx; i < eIdx; i++)
    {   (*arr)[i] = val;
    }
}

// Fills the array till a given indexe with a specific value
// Negative indexes are counted from the end of the array
// If the array is too small, it gets increased to fit
// Asserts that indexes must be within the valid range and begin is before end.
void var::fill(int end, const var &val)
{
    if(end>0)
    {   fill(0, end, val);
    }
    else
    {   fill(end, 0, val);
    }
}

// Checks if every element returns true for a function call
bool var::every(bool(*fnc)(var e)) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i = 0; i < arr->size(); i++)
    {   if(!fnc((*arr)[i]))
        {   return false;
        }
    }

    return true;
}

// Checks if every element returns true for a function call
// Additional parameter package supplied to the function as a void* pointer
bool var::every(bool(*fnc)(var e, void* params), void* params) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i = 0; i < arr->size(); i++)
    {   if(!fnc((*arr)[i], params))
        {   return false;
        }
    }

    return true;
}


// Checks if some elements in the array return true for a function call
bool var::some(bool(*fnc)(var e)) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i = 0; i < arr->size(); i++)
    {   if(fnc((*arr)[i]))
        {   return true;
        }
    }

    return false;
}

// Checks if some elements in the array return true for a function call
// Additional parameter package supplied to the function as a void* pointer
bool var::some(bool(*fnc)(var e, void* params), void* params) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i = 0; i < arr->size(); i++)
    {   if(fnc((*arr)[i], params))
        {   return true;
        }
    }

    return false;
}

// Filters the array and returns a new array that only includes elements
// that resolve to true for some given function call
var var::filter(bool(*fnc)(var e)) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    var result = array{};
    ((array*)result.data)->resize(arr->size());

    size_t count = 0;
    for(size_t i = 0; i < arr->size(); i++)
    {   if(fnc((*arr)[i]))
        {   result[count] = (*arr)[i];
            count++;
        }
    }

    ((array*)result.data)->resize(count);
    return result;
}

// Filters the array and returns a new array that only includes elements
// that resolve to true for some given function call
// Additional parameter package supplied to the function as a void* pointer
var var::filter(bool(*fnc)(var e, void* params), void* params) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    var result = array{};
    ((array*)result.data)->resize(arr->size());

    size_t count = 0; 
    for(size_t i=0; i<arr->size(); i++)
    {   if(fnc((*arr)[i], params))
        {   result[count] = (*arr)[i];
            count++;
        }
    }

    ((array*)result.data)->resize(count);
    return result;
}

// Returns the first element that resolves to true for some given function call
// If no element resolves to true, an exception is thrown
var& var::find(bool(*fnc)(var e)) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   if(fnc((*arr)[i]))
        {   return (*arr)[i];
        }
    }

    throw var::ArrayElementNotFoundException();
}

// Returns the first element that resolves to true for some given function call
// If no element resolves to true, an exception is thrown
// Additional parameter package supplied to the function as a void* pointer
var& var::find(bool(*fnc)(var e, void* params), void* params) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i<arr->size(); i++)
    {   if(fnc((*arr)[i], params))
        {   return (*arr)[i];
        }
    }

    throw var::ArrayElementNotFoundException();
}

// Returns the index of the first element that resolves to true for some given function call
// If no element resolves to true, -1 is returned
size_t var::findIndex(bool(*fnc)(var e)) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   if( fnc((*arr)[i]) )
        {   return i;
        }
    }

    return -1;
}

// Returns the index of the first element that resolves to true for some given function call
// If no element resolves to true, -1 is returned
// Additional parameter package supplied to the function as a void* pointer
size_t var::findIndex(bool(*fnc)(var e, void* params), void* params) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   if(fnc((*arr)[i], params))
        {   return i;
        }
    }

    return -1;
}

// Calls a function for each element in the array
void var::forEach(void(*fnc)(var &e))
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   fnc((*arr)[i]);
    }
}

void var::forEach(void(*fnc)(const var &e)) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   fnc((*arr)[i]);
    }
}

// Calls a function for each element in the array
// Additional parameter package supplied to the function as a void* pointer
void var::forEach(void(*fnc)(var &e, void* params), void* params)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   fnc((*arr)[i], params);
    }
}

void var::forEach(void(*fnc)(const var &e, void* params), void* params) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   fnc((*arr)[i], params);
    }
}

// Finds the index of a specific key element
// If the element is not found, -1 is returned
size_t var::indexOf(const var &key) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   if((*arr)[i] == key)
        {   return i;
        }
    }

    return -1;
}

// Finds if the array includes a specific key element
bool var::includes(const var &key) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    for(size_t i=0; i < arr->size(); i++)
    {   if((*arr)[i] == key)
        {   return true;
        }
    }

    return false;
}

// Returns the length of the array
size_t var::length() const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;
    return arr->size();
}

// Adds an element to the end of the array
int var::push(const var &e)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    arr->push_back(e);
    return arr->size();
}

// Adds an element to the end of the array
int var::push_back(const var &e)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    arr->push_back(e);
    return arr->size();
}

// Removes and returns the last element of an array
// If the array is empty, an exception is thrown
var var::pop()
{
    ASSERT_NOTEMPTY
    array* arr = (array*)data;

    var ret = arr->back();
    arr->pop_back();

    return ret;
}

// Removes and returns the last element of an array
// If the array is empty, an exception is thrown
var var::pop_back()
{
    ASSERT_NOTEMPTY
    array* arr = (array*)data;

    var ret = arr->back();
    arr->pop_back();

    return ret;
}

// Adds an element to the beginning of the array
int var::unshift(const var &e)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    arr->insert(arr->begin(), e);
    return arr->size();
}

// Adds an element to the beginning of the array
int var::push_front(const var &e)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    arr->insert(arr->begin(), e);
    return arr->size();
}

// Removes and returns the first element of an array
// If the array is empty, an exception is thrown
var var::shift()
{
    ASSERT_NOTEMPTY
    array* arr = (array*)data;

    var ret = arr->front();
    arr->erase(arr->begin());

    return ret;
}

// Removes and returns the first element of an array
// If the array is empty, an exception is thrown
var var::pop_front()
{
    ASSERT_NOTEMPTY
    array* arr = (array*)data;

    var ret = arr->front();
    arr->erase(arr->begin());

    return ret;
}

// Inserts a new element into the array at some index
// Negative indexes are counted from the end of the array
// If the index is larger than the array, the array is resized
int var::insert(int pos, const var &e)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    size_t idx;
    getIndex(pos, arr->size(), idx);
    ASSERT_INDEX(idx);

    if(arr->size() < idx)
    {   arr->resize(idx+1);
        (*arr)[idx] = e;
    }
    else
    {    arr->insert(arr->begin()+idx, e);
    }

    return arr->size();
}

// Erases an element from the array at some index
// Negative indexes are counted from the end of the array
// Asserts that the index must be valid
int var::erase(int pos)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    size_t idx;
    getIndex(pos, arr->size(), idx);

    ASSERT_INDEX_STRICT(idx, arr->size());
    arr->erase(arr->begin()+idx);

    return arr->size();
}

// Erases a range of element from the array between some indexes
// Negative indexes are counted from the end of the array
// Asserts that the range must be valid
int var::erase(int beg, int end)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    size_t bIdx, eIdx;
    getRange(beg, end, arr->size(), bIdx, eIdx);

    ASSERT_RANGE_STRICT(bIdx, eIdx, arr->size());
    arr->erase(arr->begin()+bIdx, arr->begin()+eIdx);
    
    return arr->size();
}

// Returns a copy of a sub section of the array between specified indexes
// Negative indexes are counted from the end of the array
// Asserts that the range must be valid
var var::slice(int beg, int end) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    size_t bIdx, eIdx;
    getRange(beg, end, arr->size(), bIdx, eIdx);

    if(eIdx > arr->size())
    {   eIdx = arr->size();
    }

    ASSERT_RANGE_STRICT(bIdx, eIdx, arr->size());
    var res = array(eIdx-bIdx);
    array* rArr = ((array*)res.data);

    for(size_t e = 0, i = bIdx; i < eIdx; i++, e++)
    {   (*rArr)[e] = (*arr)[i];
    }

    return res;
}

// Removes 'count' number of elements from starting index 'beg' in the array
// Negative indexes are counted from the end of the array
// Inserts the elements of another array in the place of the removed elements
void var::splice(int beg, int count, const var &elements)
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    size_t bIdx, eIdx;
    getRange(beg, beg+count, arr->size(), bIdx, eIdx);
    ASSERT_INDEX_STRICT(bIdx, arr->size());

    if(bIdx != eIdx)
    {   arr->erase(arr->begin()+bIdx, arr->begin()+eIdx);
    }

    if(elements.type == array_t)
    {   array* eArr = ((array*)elements.data);
        arr->insert(arr->begin()+bIdx, eArr-> begin(), eArr->end());
    }

    return;
}

// Removes 'count' number of elements from starting index 'beg' in the array
// Negative indexes are counted from the end of the array
void var::splice(int beg, int count )
{
    splice(beg, count, var());
}

// Inserts the elements of another array at starting index 'beg' in the array
// Negative indexes are counted from the end of the array
void var::splice(int beg, const var &elements)
{
    splice(beg, 0, elements);
}

// Joins the elements in the array with a separator string
str var::join(str separator) const
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;
    
    if(arr->size() == 0)
    {   return "";
    }

    str res = (*arr)[0].toString();
    for(size_t i = 1; i<arr->size(); i++)
    {   res += separator + (*arr)[i].toString();
    }

    return res;
}

// Reverses the contents of an array
void var::reverse()
{
    ASSERT_ARRAY(this);
    array* arr = (array*)data;

    var temp;
    for(size_t beg = 0, end = arr->size()-1; beg < end; beg++, end--)
    {   temp = (*arr)[beg];
        (*arr)[beg] = (*arr)[end];
        (*arr)[end] = temp;
    }
}
