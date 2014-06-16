#ifndef SINGLETON_INCLUDED
#define SINGLETON_INCLUDED

#pragma once

template<typename T>
class CSingleton
{
private:
    static T            obj;

protected:
    CSingleton()    {}
    CSingleton(const CSingleton&)   {}
    CSingleton&         operator=(const CSingleton&)    {}

    virtual             ~CSingleton()   {}

public:
    static T&           Instance()
    {
        return obj;
    }
};

template<typename T>
T CSingleton<T>::obj;



#endif // SINGLETON_INCLUDED
