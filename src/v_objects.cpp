#include "varcpp.h"

#define ASSERT_SIZE(capacity, requirement) if(capacity<requirement) {throw var::BufferSizeException();}
#define ASSIGN_OPTIONAL(ptr, val) if(ptr!=NULL) {*ptr = val;}

// Construct a Tag to identify the type and size of different vars
// The Tag's first 4 bits store the internal type of the var.
// The next 4 bits tell the number of bytes holding the Byte Count of the value
// In case the var is an array or an object, the lower 4 bits of the next byte 
// tell the number of bytes holding the Element Count of the var.
// .________.________.________________.________.________.________________.
// |  type  |numBytes|Data Byte Count | RSRVD  |numBytes| Element Count  |
// '--------'--------'----------------'--------'--------'----------------'
//   4 bits   4 bits      n bytes       4 bits   4 bits       n bytes
//
size_t setTag(const varType type, char buff[32], size_t bytes)
{
    size_t dataBytes = 0;

    while(bytes > 0)
    {   dataBytes++;
        buff[dataBytes] = (char)bytes;
        bytes >>= 8;
    }

    buff[0] = (type << 4);
    buff[0] += (char)dataBytes;

    return dataBytes + 1;
}

size_t setTag(const varType type, char buff[32], size_t bytes, size_t elements)
{
    size_t baseSize = setTag(type, buff, bytes);
    size_t elemBytes = 0;

    while(elements > 0)
    {   elemBytes++;
        buff[baseSize + elemBytes] = (char)elements;
        elements >>= 8;
    }

    buff[baseSize] = (char)elemBytes;
    return baseSize + elemBytes + 1;
}

// Writes a boolean_t type var to a buffer with a size
size_t writeBoolean_t(bool val, char* buff, const size_t size)
{
    ASSERT_SIZE(size, 2);

    buff[0] = 0x01;
    buff[1] = val;
    return 2;
}

// Writes an integer_t type var to a buffer with a size
size_t writeInteger_t(int val, char* buff, const size_t size)
{
    char header[32];
    size_t headerSize = setTag(integer_t, header, val);
    ASSERT_SIZE(size, headerSize);
        
    memcpy(buff, header, headerSize);
    return headerSize;
}

// Writes a real_t type var to a buffer with a size
size_t writeReal_t(double val, char* buff, const size_t size)
{
    ASSERT_SIZE(size, 1 + sizeof(double));
    
    buff[0]= 0x28;
    *(double*)(buff+1) = val;
    return 1 + sizeof(double);
}

// Writes a text_t type var to a buffer with a size
size_t writeText_t(const str &val, char* buff, const size_t size)
{
    char header[32];
    size_t chars = val.length();
    size_t headerSize = setTag(text_t, header, chars);
    ASSERT_SIZE(size, headerSize + chars);
    
    memcpy(buff, header, headerSize);
    memcpy(buff+headerSize, val.c_str(), chars);
    return headerSize + chars;
}

// Writes an array_t type var to a buffer with a size
size_t writeArray_t(array &arr, char* buff, const size_t size)
{
    char header[32];
    size_t elements = arr.size();
    
    size_t totBytes = 0;
    size_t elmBytes = 0;

    for(size_t i = 0; i < elements; i++)
    {   elmBytes = writeBinary(arr[i], buff + totBytes, size - totBytes);
        totBytes += elmBytes;
    }

    size_t headerSize = setTag(array_t, header, totBytes, elements);
    ASSERT_SIZE(size, headerSize + totBytes);

    memcpy(buff + headerSize, buff, totBytes);
    memcpy(buff, header, headerSize);
    return headerSize + totBytes;
}

// Writes an attribute to a buffer with a size
size_t writeAttrib_t(atr &att, char* buff, const size_t size)
{
    size_t nameLength = att.name.length()+1;
    size_t valueBytes = 0;
    ASSERT_SIZE(size, nameLength);

    memcpy(buff, att.name.c_str(), nameLength);
    valueBytes = writeBinary(att.val, buff + nameLength, size - nameLength);

    return nameLength + valueBytes;
}

// Writes an object_t type var to a buffer with a size
size_t writeObject_t(object &arr, char* buff, const size_t size)
{
    char header[32];
    size_t elements = arr.size();
    
    size_t totBytes = 0;
    size_t elmBytes = 0;

    for(size_t i = 0; i < elements; i++)
    {   elmBytes = writeAttrib_t(arr[i], buff + totBytes, size - totBytes);
        totBytes += elmBytes;
    }

    size_t headerSize = setTag(object_t, header, totBytes, elements);
    ASSERT_SIZE(size, headerSize + totBytes);

    memcpy(buff + headerSize, buff, totBytes);
    memcpy(buff, header, headerSize);
    return headerSize + totBytes;
}

// Writes a blob_t type var to a buffer with a size
size_t writeBlob_t(blob val, char* buff, const size_t size)
{
    char header[32];
    size_t bytes = val.size;
    size_t headerSize = setTag(blob_t, header, bytes);
    ASSERT_SIZE(size, headerSize + bytes);
    
    memcpy(buff, header, headerSize);
    memcpy(buff+headerSize, val.arr, bytes);
    return headerSize + bytes;
}

// Writes a function_t type var to a buffer with a size
// Not to be actually used. Functions can't be exported
size_t writeFunction_t(char* buff, const size_t size)
{
    ASSERT_SIZE(size, 1);
        
    buff[0]= 0x60;
    return 1;
}

// Writes an undefined_t type var to a buffer with a size
size_t writeUndefined_t(char* buff, const size_t size)
{
    ASSERT_SIZE(size, 1);
        
    buff[0]= 0x70;
    return 1;
}

// Writes a var object to a buffer with a size
// The method of converting to binary data differs per internal type
size_t writeBinary(const var& val, char* buff, const int size)
{
    switch(val.getType())
    {   case boolean_t:   return writeBoolean_t(*(bool*)val.getData(), buff, size);
        case integer_t:   return writeInteger_t(*(int*)val.getData(), buff, size);
        case real_t:      return writeReal_t(*(double*)val.getData(), buff, size);
        case text_t:      return writeText_t(*(str*)val.getData(), buff, size);
        case array_t:     return writeArray_t(*(array*)val.getData(), buff, size);
        case object_t:    return writeObject_t(*(object*)val.getData(), buff, size);
        case blob_t:      return writeBlob_t(*(blob*)val.getData(), buff, size);
        case function_t:  return writeFunction_t(buff, size);
        case undefined_t: return writeUndefined_t(buff, size);
    }

    return 0;
}

// Gets details of the var object from the buffer.
// Extracts Type, Header Size, Byte Count, Element Count
void getDetails(const char* buff, const size_t size, varType &type, size_t &headerSize, size_t &byteCount, size_t &elemCount)
{
    elemCount  = 0;
    byteCount  = 0;
    headerSize = 0;
    
    // Read the 1st byte for type and Byte Count length
    ASSERT_SIZE(size, 1);
    type  = (varType)((buff[0] & 0xF0) >> 4);
    size_t byteLength = buff[0] & 0x0F;
    headerSize += 1;

    // Read following bytes for Byte Count
    if(byteLength > 0)
    {   ASSERT_SIZE(size, headerSize + byteLength);
        for(size_t i = headerSize + byteLength - 1; i >= headerSize; i--)
        {   byteCount <<= 8;
            byteCount |= (unsigned char)buff[i];
        }
    }
    headerSize += byteLength;

    // For arrays and objects
    if(type == array_t || type == object_t)
    {   // Read 1 byte for Element Count length
        ASSERT_SIZE(size, headerSize + 1);
        size_t elemLength = buff[headerSize] & 0x0F;
        headerSize += 1;

        // Read following bytes for Element Count
        ASSERT_SIZE(size, headerSize + elemLength);
        for(size_t i = headerSize + elemLength - 1; i >= headerSize; i--)
        {   elemCount <<= 8;
            elemCount |= (unsigned char)buff[i];
        }
        headerSize += elemLength;
    }
}

// Creates and populates a var with array_t type from the buffer
var makeArray(char* buff, size_t size, size_t bytes, size_t elements)
{
    var res = array{};
    array* arr = (array*)res.getData();
    arr->resize(elements);

    size_t totUsed=0;
    size_t elmUsed=0;
    for(size_t i = 0; i < elements; i++)
    {   (*arr)[i] = readBinary(buff+totUsed, size-totUsed, &elmUsed);
        totUsed += elmUsed;
    }

    return res;
}

// Reads an attribute from the buffer with a size
size_t readAttrib_t(atr &att, char* buff, const size_t size)
{
    size_t nameLen = strlen(buff)+1;
    size_t dataSize;

    att.name = str(buff, nameLen-1);
    att.val = readBinary(buff+nameLen, size-nameLen, &dataSize);

    return dataSize + nameLen;
}

// Creates and populates a var with object_t type from the buffer
var makeObject(char* buff, size_t size, size_t bytes, size_t elements)
{
    var res = object{ };
    object* arr = (object*)res.getData();
    arr->resize(elements);

    size_t totUsed=0;
    size_t elmUsed=0;

    for(size_t i = 0; i < elements; i++)
    {   elmUsed = readAttrib_t((*arr)[i], buff+totUsed, size-totUsed);
        totUsed += elmUsed;
        ASSERT_SIZE(bytes, totUsed);
    }

    return res;
}

// Reads a var object from a buffer with a size
var readBinary(char* buff, const size_t size, size_t* bytes)
{
    varType type;
    size_t byteCount;
    size_t elemCount;
    size_t headerSize;

    getDetails(buff, size, type, headerSize, byteCount, elemCount);

    if(type <= real_t)
    {   ASSERT_SIZE(size, headerSize);
        ASSIGN_OPTIONAL(bytes, headerSize);

        switch(type)
        {   case boolean_t: return var(*(bool*)&byteCount);
            case integer_t: return var((int)byteCount);
            case real_t:    return var(*(double*)(buff+1));
        }
    }
    else
    {   ASSERT_SIZE(size, headerSize + byteCount);
        ASSIGN_OPTIONAL(bytes, headerSize + byteCount);

        switch(type)
        {   case text_t:    return var(str(buff+headerSize, byteCount));
            case array_t:   return makeArray(buff+headerSize, size, byteCount, elemCount);
            case object_t:  return makeObject(buff+headerSize, size, byteCount, elemCount);
            case blob_t:    return var(blob{buff+headerSize, byteCount});
        }
    }


    return var();
}

str var::toJSON(str indent) const
{
    if(type != array_t && type != object_t)
    {    return toString();
    }

    str JSON = "";
    str lvlIndent = indent + "    ";

    if(type == array_t)
    {   array* arr = (array*)data;
        size_t len = arr->size();
        size_t end = len -1;

        if(len == 0)
        {   return "[ ]";
        }
    
        JSON += "[";

        for(size_t i = 0; i< len-1; i++)
        {   JSON += "\n" + lvlIndent;
            switch((*arr)[i].type)
            {   case text_t: JSON += "\"" + (*arr)[i].toString() + "\""; break;
                case object_t:
                case array_t: JSON += (*arr)[i].toJSON(lvlIndent); break;
                default: JSON += (*arr)[i].toString();
            }
            JSON += ",";
        }

        JSON += "\n" + lvlIndent;
        switch((*arr)[end].type)
        {   case text_t: JSON += "\"" + (*arr)[end].toString() + "\""; break;
            case object_t:
            case array_t: JSON += (*arr)[end].toJSON(lvlIndent); break;
            default: JSON += (*arr)[end].toString();
        }
        JSON += "\n" + indent + "]";
    }

    if(type == object_t)
    {   object* arr = (object*)data;
        size_t len = arr->size();
        size_t end = len -1;

        if(len == 0)
        {   return "{ }";
        }
    
        JSON += "{";

        for(size_t i = 0; i< len-1; i++)
        {   JSON += "\n" + lvlIndent + "\"" + (*arr)[i].name + "\": ";
            switch((*arr)[i].val.type)
            {   case text_t: JSON += "\"" + (*arr)[i].val.toString() + "\""; break;
                case object_t:
                case array_t: JSON += (*arr)[i].val.toJSON(lvlIndent); break;
                default: JSON += (*arr)[i].val.toString();
            }
            JSON += ",";
        }

        JSON += "\n" + lvlIndent + "\"" + (*arr)[end].name + "\": ";
        switch((*arr)[end].val.type)
        {   case text_t: JSON += "\"" + (*arr)[end].val.toString() + "\""; break;
            case object_t:
            case array_t: JSON += (*arr)[end].val.toJSON(lvlIndent); break;
            default: JSON += (*arr)[end].val.toString();
        }
        JSON += "\n" + indent + "}";
    }

    return JSON;
}