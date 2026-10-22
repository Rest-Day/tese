#include "tool.h"
#include "percept.h"
#include <bits/stdc++.h>

using namespace std;

PerceptionSystem main_one;//主结构体一号，程序中主要数据的储存调用都在这里进行
std::deque<PerceptionSystem> kj;

//用于打印图像的工具
void print (unsigned char * main, int wid)
{
    int add = 0;
    for(int r = 0; r < 10; r ++)
    {
        for(int c = 0; c < 20; c ++)
        {
            add = 0;
            for (int x = 5 * c; x < 5 * c + 5; x++)
            {
                for (int y = 10 * r; y < 10 * r + 10; y++)
                {
                    add += main[x + y * wid];
                }
            }
            add /= 50;
            cout << ((add > 127) ? "#": ".");
        }
        cout << endl;
    }
}

float update_ave(unsigned char * main, int wid)//记得更新平均亮度值
{
    float bright_sum = 0;//传统求平均，先计算总和
    for(int i = 0; i < wid; i ++)
    {
        bright_sum += main[i];
    }
    bright_sum /= wid;//再计算平均
    return bright_sum;//平均值扔回程序里
}

int ave(int * wind, int lon)/*计算什么来着
                            哦对了中位数 */ 
{
    int num = lon / 2;//只需要按大小排列一半的数
    int count = 0;
    int n = 0;
    for(int i = 0; i <= num; i ++)//传统原始排列方法，一个个比大小
        {
            count = i;
            while(count < lon)
            {
                count ++;
                if(wind[i] < wind[count])
                {
                    n = wind[i];
                    wind[i] = wind[count];
                    wind[count] = n;
                }
            }
        }
        return wind[num];
}

bool edge_test(int i)//一个检测在不在图像边缘的妙妙小工具
{
    bool end = 0;
    if(i < 200 || i > 9800)//是不是最上/下两行
    {
        end = 1;
    }
    i %= 100;
    if(i == 0 || i == 1 || i == 98 || i == 99)//是不是最左/右两列
    {
        end = 1;
    }
    return end;
}

int put_in(int num, int group)//用来写入那个5x5窗口
{
    int change = 0;
    int num_plus = num % 5;
    if(num < 5)//窗口第一行，也就是图像中距离窗口中心点往上的两行
    {
        change = group - 203 + num_plus;
    }
    else if(num < 10)//第二行
    {
        change = group - 103 + num_plus;
    }
    else if(num < 15)//第三行
    {
        change = group - 2 + num_plus; 
    }
    else if(num < 20)//第四行
    {
        change = group + 99 + num_plus;
    }
    else if(num < 25)//第五行
    {
        change = group + 199 + num_plus;
    }
    return change;
}

//检测目标面积的工具，大致思路是走迷宫一样的递归
int mmove(int cou, bool vis[10000], int cen[4])
{
    int tg = 0;
    int way[4] = {100, -100, 1, -1};
    int x = cou % 100;
    int y = cou / 100;
    int ncou = 0;
    if (!vis[cou] && main_one.image[cou] == 255)
    {
        tg ++;
        vis[cou] = true;
        if (x > cen[0])
        {
            cen[0] = x;
        }
        else if (x < cen[1])
        {
            cen[1] = x;
        }
        if (y < cen[2])
        {
            cen[2] = y;
        }
        else if(y > cen[3])
        {
            cen[3] = y;
        }
        for (int i = 0; i < 4; i ++)
        {
            ncou = cou + way[i];//八个方向（三个）随心配，走得通就走走不通就返回
            
            if ((x == 0 && i == 3) || (x == 99 && i == 2) || 
            (y == 0 && i == 1) || (y == 99 && i == 0))//在边缘也返回
            {
                continue;
            }
            else 
            {
                tg += mmove(ncou, vis, cen);
            }
            
        }
    }
    return tg;
}

/*
输入撤销栈的工具
*/
void undo_in(string & n)
{
    PerceptionSystem snap;
    snap = main_one;
    snap.image = new unsigned char[10000];
    memcpy(snap.image, main_one.image, 10000);
    snap.nname = n;
    kj.push_back(snap);
    if (kj.size() > UNDO_MAX_RECORDS)//爆了就丢最底层的一个
    {
        kj.pop_front();
    }
}

//根据输入指令决定调用哪个函数的工具
inp check(const string & s)
{
    if (s == "init")      return inp::INIT;
    if (s == "show")      return inp::SHOW;
    if (s == "noise")     return inp::NOISE;
    if (s == "denoise")   return inp::DENOISE;
    if (s == "edge")      return inp::EDGE;
    if (s == "threshold") return inp::THRESHOLD;
    if (s == "find")      return inp::FIND;
    if (s == "flip")      return inp::FLIP;
    if (s == "reset")     return inp::RESET;
    if (s == "help")      return inp::HELP;
    if (s == "exit")      return inp::EXIT;
    if (s == "history")   return inp::HISTORY;
    if (s == "batch")     return inp::BATCH;
    if (s == "histogram") return inp::HISTOGRAM;
    if (s == "undo")      return inp::UNDO;
    return inp::UNKNOWN;
}

//检查输入参数是什么类型的工具(int / char / other)
char typec(string & in)
{
    bool i = 0;
    bool c = 0;
    char ans = '0';
    for (int n = 0; n < (int)in.size(); n ++)
    {
        if (in[n] >= '0' && in[n] <= '9')
        {
            i = 1;
        }
        else
        {
            c = 1;
        }
    }
    if (i == 1 && c == 1)
    {
        ans = 'x';
    }
    else if (i == 1)
    {
        ans = 'i';
    }
    else if (c == 1)
    {
        ans = 'c';
    }
    return ans;
}

/*
检查是否初始化
*/
bool init_check()
{
    if(!main_one.is_initialized)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
history函数附带的工具
功能是把指令压入hist
*/
void history_add(string & in)
{
    hist.push_back(in);
    while(hist.size() > 20)
    {
        hist.erase(hist.begin());
    }
}