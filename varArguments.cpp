#pragma once
// file: printer.h
// author: George Li
// 2024/04/10

#include <set>
#include <vector>
#include <iostream>
#include <unistd.h>

#define __GCC__ 1

#if __GCC__
#define LOG(x)	(std::cout << __FILE__ << ':' << __LINE__<<" : " << __PRETTY_FUNCTION__ << ": " << (x)<< std::endl)
#else
#define LOG(x)	(std::cout << __FILE__ << ':' << __LINE__<<" : " << __FUNCSIG__ << ": " << (x) << std::endl)
#endif // __GCC__

template<typename T>
std::ostream& operator<<(std::ostream &os, const std::vector<T>& val )
{
    os << "{";
    for (auto it = val.begin(); it != val.end();++it)
    {  
        os << *it;
        if(it != val.end()-1)
        {
            os << ", ";
        }
        
    }
    os << "}";
    return os;
}

template<typename T,typename N>
std::ostream& operator<<(std::ostream &os, const std::set<T, N>& val )
{
    os << "{";
    for (auto it = val.begin(); it != val.end(); )
    {  
        os << *it;
        if(++it != val.end())
        {
            os << ", ";
        }
        
    }
    os << "}";
    return os;
}

template<typename T>
std::ostream& print(std::ostream& os, const T& t)
{
    return os << t;    
}

template<typename T, typename... Args>
std::ostream& print(std::ostream& os, const T& t, const Args& ... rest)
{
    os << "t="   <<  t << ",remain type size= "<<sizeof...(Args) << "\n";
    return print(os, rest...);
}
int main()
{

    int i = 10;
    std::string str("helloMoto");
    double d = 23.99;
    print(std::cout, 23, i, str, d);

    return 0;
}


