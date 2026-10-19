// src/lcdb/lcdb_c_api.hpp
#pragma once

extern "C"
{
    int lcdbOpen(const char* root, int shotno, const char* category);

    int lcdbClose(const char* root, int shotno, const char* category);

    int lcdbCategoryCount(const char* root, int shotno, int* count, int* maxlen);

    int lcdbCategoryList(const char* root, int shotno, int* count, int* maxlen, void* arr);

    int lcdbValueType(const char* root, int shotno, const char* categeory, const char* dataname, int* datatype,
                      int* dimension, int* shape, int* maxlen);

    int lcdbValueDim(const char* root, int shotno, const char* categeory, const char* dataname, int* dimension);

    int lcdbValue(const char* root, int shotno, const char* categeory, const char* dataname, int datatype,
                  int dimension, int* shape, int maxlen, void* values);
}
