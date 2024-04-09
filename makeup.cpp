// file: makeup.cpp
#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <memory>
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

class EquipElem
{
    int cost;
    int rewarding;
    int resonate;

public: 
    EquipElem(){}
    EquipElem(int phyCost, int reward, int reson)
        :cost(phyCost), rewarding(reward), resonate(reson)
    {}
    virtual ~EquipElem(){}
    int getCost() const {return cost;}
    int getRewarding() const  {return rewarding;}
    int getResonate() const {return resonate;};

    bool operator==(const EquipElem& other) const 
    {
        return this->cost == other.cost
        && this->rewarding == other.rewarding 
        && this->resonate == other.resonate;
    }
};


class Equips : public EquipElem
{
    int count;
public:
    friend std::ostream& operator<<(std::ostream&os, const Equips& e);
    Equips():EquipElem(0, 0, 0), count(0){}

    Equips(int phyCost, int reward, int reson, int number = 0) 
        : EquipElem(phyCost, reward, reson), count(number){}
    
    void add(int val) { count += val;}
    void add() { ++count;}
    int getCount() const {return count;}
    
    int getResonateRewarding() const 
    {
        if(count >=5)
            return getResonate()* count;
        else return 0;
    }
};
std::ostream& operator<<(std::ostream &os, const Equips& e )
{
    os << "[";
    os << "count = "<<e.getCount() 
    << ", cost="<<e.getCost() << ",rewarding=" << e.getRewarding()
    << ", resonatePoint=" << e.getResonate()
    << ", resonateRewarding="<< e.getResonateRewarding();
    os << "]";

    return os;
}

struct SoliderLyoier 
{
    const int equipMax = 330; // 可购买装备最大数
    std::vector<Equips> vecEquips;
    int physical;
    int maxpower;
    SoliderLyoier(int equipNum, int phyPoints):physical(phyPoints), maxpower(0)
    {
        //vecEquips.resize(equipNum);
    }
    ~SoliderLyoier()
    {
    }

    SoliderLyoier& prepareEquips(int cost, int reward, int resonatePoint)
    {
        vecEquips.push_back({cost,reward,resonatePoint,0});
        return *this;
    }

    void buyEquip()
    {
        int buyMaxPower = 0;
        int count =0;
        std::shared_ptr<Equips> pShare;
        
        for(auto& i : vecEquips)
        { 
            int cnt = physical/i.getCost();
            int power = cnt * i.getRewarding();
            if(cnt >= 5)
            {
                power += cnt * i.getResonate();
            }
            if(power > buyMaxPower)
            {
                buyMaxPower = power;
                count = cnt;
                pShare = std::make_shared<Equips>(i);                    
            }                
        }

        // refresh the vector item and update the count.
        auto it = find(vecEquips.begin(), vecEquips.end(), *pShare.get());
        if(it != vecEquips.end())
        {
            it->add(count);
            physical -= (it->getCost() * count);
            maxpower = it->getRewarding() * count; // buy power without resonating value
        }       

        std::cout << "\n===>can buy count =" << count << ",buy maxpower=" << maxpower << std::endl;
        std::cout << "===>totalpower(buyMaxPower+resonating)=" << maxpower + it->getResonateRewarding() << std::endl;;
        std::cout << *it << std::endl;

    }
    void print()
    {
        std::cout << vecEquips << std::endl;
        std::cout << " ==>> The max power is: " << maxpower << ", physical=" << physical << std::endl; 
    }

};


int main()
{    
    auto start = std::chrono::steady_clock::now();

    SoliderLyoier lyoier(5,87);
    lyoier.prepareEquips(35,48,7)
        .prepareEquips(3,57,6)
        .prepareEquips(45,9,24)
        .prepareEquips(3,4,0)
        .prepareEquips(5,7,7);

    lyoier.print();
    lyoier.buyEquip();
    
    std::cout << ">>>>Printe the equip vector again..." << std::endl;
    lyoier.print();

    auto end = std::chrono::steady_clock::now();
    std::chrono::nanoseconds diff = end - start;
    std::cout << "Time used:" << diff.count() << " nanoseconds(1second = 1000 000 000nanoseconds)" << std::endl;
    return 0;
}

/*聊城一中信息学奥赛学习训练 13561201696
装备选购
【 问题描述】
“工欲善其事,必先利其器”,Lyoier 探险前需要准备部分装备。 购买每件装备都需
要付出一定的体力值， 同时获得一定的能力值（每种装备可以无限次购买， 但体
力值是有限的）。 选购的这些装备还很神奇： 同种装备数量>=5 时， 它们将产生
共鸣， 产生更强大的力量， 即除了每件装备正常的能力值之外还会获得额外的
Q*K（K 表示挑选的数量） 的能力值。 但是， Lyoier 也会付出更多的体力， 即体
力值还将多付出 Q*K（K 表示挑选的数量）。 显然， 挑选的物品越多， 产生的力
量就越大， 体力点也会付出越多。 不过， 这额外获得的能力值、 付出的体力值是
可以不要的。 为了能获得最大的能力值， 同时也不至于使自己过分劳累（体力恰
好为 0 不算过分劳累）， Lyoier 应该如何选购装备呢？
【 输入文件】
第一行， 两个数 N 和 M， N 表示有多少种装备,0<=N<=330， 每种装备可以买无
限次。 M 表示 Lyoier 所拥有的体力点数,0<=M<=400。
以下 N 行， 第 I 行三个数， WI， QI,Q。 WI 表示买第 I 个装备所付出的体力点，
QI 表示买第 I 个装备能获得的能力点， Q 表示当第 I 个装备的数量>=5 时所产生
的共鸣点。 1<=WI<=9000,0<=QI<=9000,0<=Q<=9000。
【 输出文件】
一个数， 表示在保证 Lyoier 不会过分劳累的情况下所能买到的最大能力值
【 样例输入】
5 87
35 48 7
3 57 6
45 9 24
3 4 0
5 7 7
【 样例输出】
1653
*/
