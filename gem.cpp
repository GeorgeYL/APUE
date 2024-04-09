#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>

template<typename T>
std::ostream& operator<<(std::ostream &os, const std::vector<T>& val )
{
    os << "{";
    for (auto it = val.begin(); it != val.end(); ++it)
    {    os << *it;
        if(it != val.end() -1)
        {
            os << ", ";
        }
    }
    os << "}";
    return os;
}

std::vector<int> Dec2SevenBase(int num)
{
    int result =0;
    std::vector<int> vec7th;
    if(num <= 0) return vec7th;
    int div = num;
    int mod = 0;
    while(div / 7 >=1) {
        mod =  div % 7;        
        div = div/7;
        vec7th.insert(vec7th.begin(), mod); // get the low bit of the 7th number first.
    }
    vec7th.insert(vec7th.begin(), div); // the last step should save the division    
    std::cout << "Decimal("<< num << ")converted 7-based number is=>:" << vec7th << std::endl;
    return vec7th;
}

int anyBasetoDecimalVal(const std::vector<int>& vec, int base)
{
    int result = 0;    
    if(vec.empty()) return 0;

    for(std::size_t i =0; i < vec.size(); i++)
    {
        result += std::pow(base, vec.size()- 1 -i)* vec[i];
    }
    std::cout << vec<< " To decimal number(power value) = " << result << std::endl; 
    return result;
}

struct Gem
{
    Gem(int value): number(value){};
    int number;
    int merge(const Gem& other) // will release power if merge with another gem.
    {
        std::vector<int> retBinary;
        //1. convert decimal number to 7 based value. 
        std::vector<int> this7vec = Dec2SevenBase(number);
        std::vector<int> other7vec = Dec2SevenBase(other.number);
        //2. align the vector size.
        int sizeDiff = this7vec.size() - other7vec.size();
        if(sizeDiff !=0)
        {
            if(sizeDiff > 0)
            {
                // insert many 0 values(count = sizeDiff) to the start point of vector.
                other7vec.insert(other7vec.begin(), (std::size_t)sizeDiff, 0);
            }else{
                sizeDiff =  other7vec.size() - this7vec.size();
                this7vec.insert(this7vec.begin(), (std::size_t)sizeDiff, 0);
            }
        }
    
        std::cout << ">>>>>>Afert vector size algined<<<<<" << std::endl;
        std::cout << this7vec << std::endl;
        std::cout << other7vec << std::endl;
        // 3. merging the two 7 based vector and generated a binary vector.
        for(int i =0; i < this7vec.size(); i++)
        {
            if(this7vec[i]==other7vec[i]) retBinary.push_back(0);
            else {
                retBinary.push_back(1);
            }
        }
        // return the power value in decimal value
        // 4. convert the binary value to decimal number as the released power
        return anyBasetoDecimalVal(retBinary, 2);
    }
};

std::ostream& operator<<(std::ostream& os, const Gem &s){
    return os << s.number;
}

int main()
{
    auto start = std::chrono::steady_clock::now();
    std::vector<Gem> vecGem={18, 370, 45, 36, 78};
    std::cout << vecGem << std::endl;
    int distantVal = 123; 
    std::pair<int, int> elePair;
    for(auto i=0; i < vecGem.size(); i++)
    {
        for(auto j = i+1; j < vecGem.size(); j++)
        {
            int value = vecGem[i].merge(vecGem[j]);
            std::cout << i << ":" << j<<", released power=>" << value << std::endl;
            int tmp = std::abs(value-123);
            if(tmp < distantVal) // we need to find the pair which have the smallest distant value 
            {
                distantVal = tmp;
                elePair.first = i; 
                elePair.second =j;
            }
        }
    }
    
    std::cout << "\n==============================" << std::endl;
    std::cout << "abs(123 - power)=> distant value= " << distantVal << "; comes from elements: "<< elePair.first << ":" << elePair.second<< std::endl;
    std::cout << vecGem[elePair.first] << "," << vecGem[elePair.second] << std::endl;
    std::cout << vecGem[elePair.first].merge(vecGem[elePair.second]) << " is the release power value!!!!" << std::endl;
    auto end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds diff = end - start;
    std::cout << "Time used:" << diff.count() << " nanoseconds(1second = 1000 000 000nanoseconds)" << std::endl;
    return 0;
}

/* 聊城一中信息学奥赛学习训练
令牌能量
【 问题描述】
Lyoier 在某次探险时进入一个石屋， 石屋内的桌子上有 N 张金属令牌， 每个金
属令牌都有一个数字， 且令牌上都有一个卡扣， 该卡扣可以将两张令牌合二为一。
两张金属令牌合并后会产生奇异的能量， 但这些奇异可能激发危险的机关而且是
致命的， 也有可能是成为打开前进之路的钥匙。 Lyoier 看到石屋内的说明： 需要
找出两张特定令牌令牌， 使这两张令牌合并后产生的能量值最接近 123， 这样才
能打开前进的门。
令牌合并后产生能量值的计算方法是： 将两个令牌所代表的数字转换为 7 进制
的数后， 一一对照这两个七进制数的每一位， 若相同， 则结果为 0 否则为 1。
如： 两张令牌所代表的数为18和370， 将这两个数转化为7进制后是24和1036，
对于高位不足的数， 采取高位添‘0’ 的方法， 即两个数为 0024， 1036。 最后得
到的能量值 C 为 1011， 再将 C 当作二进制数转换为十进制数。 那么转换后的 C
就为令牌合并以后所产生的能量值。
【 输入文件】
第一行一个数 N， 表示金属令牌的数量。（2<=N<=900）
第二行 N 个数， 每个数用空格隔开， 每个数表示第 I 个金属令牌所代表的数字
(0<=每个数<=11111)
【 输出文件】
一个数， 代表你所找到的最接近 123 的能量值
【 样例输入】
5 1
8 370 45 36 78
【 样例输出】
15
【 样例说明】
370 和 78 这两颗金属令牌所产生的能量值 15 最接近 123
*/
