#pragma once

template <class T>
inline void Delete(T*& p){
    if (p) delete p;
    p = nullptr;
}

template <class T>
inline void DeleteArray(T*& p){
    if (p) delete[] p;
    p = nullptr;
}

template <class T>
inline void Release(T*& p){
    if (p) p->Release();
    p = nullptr;
}

#include <list>

template <class T>
void DeleteListElem(std::list<T*>& arr, T* elem){
    // 削除。
	typedef typename std::list<T*>::iterator iterator;

	iterator it;

    for ( it = arr.begin(); it != arr.end();){
        if ((*it) == elem){
            Delete(*it);
            it = arr.erase(it);
        }
        else{
            ++it;
        }
    }
}

template <class T>
void DeleteList(std::list<T*>& arr){
	typedef typename std::list<T*>::iterator iterator;

    for ( iterator it = arr.begin(); it != arr.end(); it++) Delete(*it);
    arr.clear();
}

// 16バイトにアラインメントする。
#define ALIGN16 _declspec(align(16))