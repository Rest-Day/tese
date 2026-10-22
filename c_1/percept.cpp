#include "percept.h"
#include "tool.h"
#include<bits/stdc++.h>

using namespace std;


PerceptionSystem r_t;
std::vector<std::string> hist;


/*
init程序
不需要参数也不返回值，但是会改变主结构体一号的值
具体功能是刷新状态
*/
void init()
{
    srand(2026);//设置随机种子
    delete []main_one.image;
    main_one.image = new unsigned char[10000];//让image指向一个new数组，预留空间的同时方便释放内存
    float ave = 0;//计算平均值的变量，本质用于求和，但为了和结构体中的变量统一如此命名
    for(int i = 0; i < 10000; i ++)//挨个写入随机亮度值，同时计算亮度总和
    {
        main_one.image[i] = rand() % 256;
        ave += main_one.image[i];
    }
    main_one.ave_brightness = ave / 10000;//计算平均值
    main_one.is_initialized = 1;//刷新初始化状态
    main_one.noise_level = 0;//刷新各种状态
    main_one.target_count = 0;
    main_one.ave_brightness = update_ave(main_one.image, main_one.width * main_one.height);
    cout << "[INFO] System initialized" << endl;
    cout << "[INFO] Status: " << main_one.height << "x" << main_one.width << ", noice=" 
    << main_one.noise_level << "%, brightness="; //顶不住了换一行
    printf("%.2f", main_one.ave_brightness);//小数一定要用printf写（✋️😭✋️）
    cout << ", targrts="  << main_one.target_count << ", initialized=" << main_one.is_initialized << endl;
    print(main_one.image, main_one.width);//打印一下，他居然有自己的函数这可真奢侈
}

/*
show函数
不需要参数也不返回值
功能是输出亮度值
取用数字可能不是那么规范，但这是我上个月（两周前）写的所以就这样吧
*/
void show()
{
    int count = 0;//由于要实现换行，干脆用一个变量作为指针指向的地址
    for(int i = 0; i < main_one.height; i ++)
    {
        for(int j = 0; j < main_one.width; j ++)
        {
            count = i * 100 + j;
            if((int)main_one.image[count] < 10)//判断位数
            {
                cout << (int)main_one.image[count] << "   ";
            }
            else if((int)main_one.image[count] < 100)
            {
                cout << (int)main_one.image[count] << "  ";
            }
            else
            {
                cout << (int)main_one.image[count] << " ";
            }
        }
        cout << endl;
    }
}

/*
noise_p函数
需要一个添加噪点百分比的参数，没有返回值
功能是添加胡椒噪点
*/
void noise_percentage(int percentage)
{
    if (percentage > 100 || percentage <= 0)//看看参数有没有越界
    {
        cout << "[ERROR] Noise percentage must be between 1 and 100." << endl;
        return;
    }
    srand(time(NULL));//随机的随机种子，那很随机了
    bool visited[10000] = {0};
    int count = 0;
    int site = 0;
    while (count < percentage * 100)
    {
        site = rand() % 10000;
        if(visited[site] == 0)
        {
            int y_f = rand() % 2;//亮点和暗点的概率一半一半
            if(y_f == 0)
            {
                main_one.image[site] = 255;
            }
            else
            {
                main_one.image[site] = 0;
            }
            visited[site] = 1;
            count ++;
        }
        else
        {
            continue;
        }
    }
    main_one.noise_level += percentage * (100 - main_one.noise_level) / 100;//更新噪点值
    main_one.ave_brightness = update_ave(main_one.image, main_one.width * main_one.height);
    cout << "[INFO] Noise added: " << percentage << "%." << endl;
}

/*
denoice函数
不需要参数也不返回值
功能是平滑降噪
用了三个妙妙小工具 edge_test putin ave
*/
void denoice()
{
    unsigned char * temp = new unsigned char[10000];
    int mid[25] = {0};
    for (int i = 0; i < 10000; i ++)//每个数据都当做中心数据处理一遍
    {
        temp[i] = main_one.image[i];
    }
    for (int i = 0; i < 10000; i ++)//覆盖主图像
    {
        if(!edge_test(i))//忽略边缘数据👿
        {
            for(int j = 0; j < 25; j ++)
            {
                mid[j] = temp[put_in(j, i)];
            }
            //int q = ave(mid, 25);
            main_one.image[i] = ave(mid, 25);
        }
    }
    main_one.noise_level *= 0.3;//更新噪点值
    main_one.ave_brightness = update_ave(main_one.image, main_one.width * main_one.height);
    delete []temp;//进行一个释放动态数组的运动
    cout << "[INFO] Denoised with 5x5 median filter." << endl;
} 

/*
edge函数
需要参数也不返回值
功能是边缘检测
*/
void edge()
{
    int Gx = 0;
    int Gy = 0;
    int g = 0;
    int count = 0;
    for(int y = 0; y < main_one.height; y ++)
    {
        for(int x = 0; x < main_one.width; x ++)
        {
            count = y * 100 + x;//检索图像中元素
            /*
            检测边界
            */
            if (x == 99)
            {
                Gx = 0;
                Gy = main_one.image[count + 100] - main_one.image[count];
            }
            else if (y == 99)
            {
                Gx = main_one.image[count + 1] - main_one.image[count];
                Gy = 0;
            }
            else
            {
                Gx = main_one.image[count + 1] - main_one.image[count];
                Gy = main_one.image[count + 100] - main_one.image[count];
            }
                //取绝对值
            if (Gx < 0)
            {
                Gx *= -1;
            }
            if(Gy < 0)
            {
                Gy *= -1;
            }
            g = Gx + Gy;
            cout << ((g > 100) ? "#": ".");//判断并输出
        }
        cout << endl;
    }
}

/*
threshold函数
需要一个整型参数，不返回值
功能是阈值分割
*/
void threshold_value(int value)
{
    if (value > 255 || value < 0)//检测非法输入
    {
        cout << "[ERROR] Threshold must be between 0 and 255." << endl;
        return;
    }
    int count = 0;
    for(int y = 0; y < main_one.height; y ++)//逐个修改亮度值
    {
        for(int x = 0; x < main_one.width; x ++)
        {
            count = x + y * 100;
            main_one.image[count] = (main_one.image[count] < value) ? 0: 255;
        }
    }
    main_one.ave_brightness = update_ave(main_one.image, main_one.width * main_one.height);//更新平均亮度
    cout << "[INFO] Threshold applied (value=" << value << ")." << endl;//输出结果
    print(main_one.image, main_one.width);
}

/*
find函数
不返回值也不需要参数
功能是查找目标数
*/
void find()
{
    int count = 0;//count变量用于计算坐标
    int tg_n = 0;//传入mmove的tg变量，表示目标的面积
    int tg_c = 0;//统计目标数的变量
    int area_x = 0;//计算中心坐标的xy
    int area_y = 0;
    int cent[4] = {0};//记录最边界的xy
    bool visited[10000] = {0};//遍历visited数组
    for (int y = 0; y < main_one.height; y ++)
    {
        for (int x = 0; x < main_one.width; x ++)
        {
            count = x + (y * 100);
            if(main_one.image[count] == 255 && !visited[count])
            {
                //录入边界数值
                cent[0] = x;
                cent[1] = x;
                cent[2] = y;
                cent[3] = y;
                tg_n = 0;
                tg_n = mmove(count, visited, cent);
                if (tg_n > FIND_MIN_AREA)//若目标面积大于宏，记为正式目标并更新参数
                {
                    tg_c ++;
                    area_x = (cent[0] + cent[1]) / 2;//计算中心点
                    area_y = (cent[2] + cent[3]) / 2;
                    cout << "[INFO] Target " << tg_c << ": center=(" << area_x << "," << area_y << "), area=" << tg_n << endl;//输出结果
                }
            }
        }
    }
    cout << "[INFO] Found " << tg_c << " target(s)." << endl;//输出总结果
    main_one.target_count = tg_c;//更新target
}

/*
flip函数
需要一个string数组(字符串)作为参数，不返回值
功能是根据输入翻转图像
*/
void flip_direction(string & direction)
{
    char dir = 0;//把direction改成char
    unsigned char prc = 0;//翻转时储存零时值的变量
    int count = 0;//计算坐标与翻转坐标
    int anticount = 0;
    if (direction.size() != 1)//若输入多个字符
    {
        cout << "[ERROR] Invalid direction! Use h, v, or b." << endl;
        return;
    }
    dir = direction[0];//string 转 char
    if (dir == 'h')//h 情况
    {
        for(int y = 0; y < 100; y ++)
        {
            for(int x = 0; x < 50; x ++)//只走右半面  
            {
                count = x + (y * 100);
                anticount = (99 - x) + (y * 100);
                prc = main_one.image[count];
                main_one.image[count] = main_one.image[anticount];
                main_one.image[anticount] = prc;
            }
        }
        cout << "[INFO] Image flipped horizontally." << endl;
    } 
    else if (dir == 'v')//v 情况
    {
        for(int y = 0; y < 50; y ++)//只走上半
        {
            for(int x = 0; x < 100; x ++)
            {
                count = x + (y * 100);
                anticount = (99 - y) * 100 + x;
                prc = main_one.image[count];
                main_one.image[count] = main_one.image[anticount];
                main_one.image[anticount] = prc;
            }
        }
        cout << "[INFO] Image flipped vertically." << endl;
    }
    else if (dir == 'b')//b 情况
    {
         for(int y = 0; y < 50; y ++)//只走任意一半
        {
            for(int x = 0; x < 100; x ++)
            {
                count = x + (y * 100);
                anticount = (99 - y) * 100 + (99 - x);
                prc = main_one.image[count];
                main_one.image[count] = main_one.image[anticount];
                main_one.image[anticount] = prc;
            }
        }
        cout << "[INFO] Image flipped both." << endl;
    }
    else//非hbv 即非法输入
    {
        cout << "[ERROR] Invalid direction! Use h, v, or b." << endl;
        return;
    }
    
}

/*
reset函数
不需要参数也不返回值
功能是充值图像，套用了init的部分代码
*/
void reset()
{
    if (main_one.image != nullptr)
    {
        delete[] main_one.image;
        main_one.image = nullptr;
    }
    srand(2024);//设置随机种子
    main_one = r_t;
    main_one.image = nullptr;
    main_one.image = new unsigned char[10000];
    for(int i = 0; i < 10000; i ++)//挨个写入随机亮度值，同时计算亮度总和
    {
        main_one.image[i] = rand() % 256;
    }
    main_one.is_initialized = true;//刷新初始化状态
    main_one.ave_brightness = update_ave(main_one.image, main_one.width * main_one.height);
    cout << "[INFO] System reset." << endl;
    cout << "[INFO] Status: " << main_one.height << "x" << main_one.width << ", noice=" 
    << main_one.noise_level << "%, brightness="; //顶不住了换一行
    printf("%.2f", main_one.ave_brightness);//小数一定要用printf写（✋️😭✋️）
    cout << ", targrts="  << main_one.target_count << ", initialized=" << main_one.is_initialized << endl;
    print(main_one.image, main_one.width);//打印一下，他居然有自己的函数这可真奢侈
}

/*
help函数
不需要参数也不返回值
功能是输出指令列表
*/
void help()
{
    cout << "<init>[void]系统初始化与图像显示指令" << endl;
    cout << "<show>[void]图像数据查看指令" << endl;
    cout << "<noise>[int]添加噪声" << endl;
    cout << "<denoise>[void]图像去噪" << endl;
    cout << "<edge>[void]边缘检测" << endl;
    cout << "<threshold>[int]转换二值图像" << endl;
    cout << "<find>[void]寻找连续目标" << endl;
    cout << "<flip>[void]图像翻转" << endl;
    cout << "<reset>[void]重置系统" << endl;
    cout << "<help>[void]查看指令" << endl;
    cout << "exit>[void]退出系统" << endl;
    cout << "<history>[void]操作日志" << endl;
    cout << "<bath>[int]批量处理" << endl;
    cout << "<histogram>[void]灰度直方图" << endl;
    cout << "<undo>[int]撤销操作" << endl;
}

/*
exit函数
不需要参数也不返回值
功能是释放内存，清除输入流，安全退出
*/
void exit_p()
{
    cout << "[INFO] Exit achieved! Perception simulator closed." << endl;
    delete []main_one.image;
    cin.clear();
    exit(0);    
}


/*
history函数
不需要参数也不返回值
功能是查询历史记录
*/
void history()
{
    for (int i = (int)hist.size() - 1; i >= 0; --i)//挨个输出历史记录
        cout << hist[i] << endl;
}

/*
bath函数
需要一个整型做参数，不返回值
功能是批量处理
*/
void batch_frames(int input)
{
    if (input > BATCH_MAX_FRAMES || input <= 0)//检测非法输入
    {
        cout << "[ERROR] Frame count must be a positive integer (1-99)." << endl;
        return;
    }
    double add = 0;//在这里定义add，使其不随着循环清零
    for (int n = 0; n < input ; n ++)
    {
        srand( time(NULL) + n);//还是借用了init里的代码
        for(int i = 0; i < 10000; i ++)//挨个写入随机亮度值，同时计算亮度总和
        {
            main_one.image[i] = rand() % 256;
        }
        
        srand(time(NULL));//随机的随机种子，那很随机了
        set<int> num;//一个不允许重复参数的set数组
        while((int)num.size() < 10 * 10)//只要没roll出足够数量的随机数就一直roll下去
        {
            num.insert(rand() % 10000 + 1);
        }
        for(int i : num)//用i读num
        {
            int y_f = rand() % 2;//亮点和暗点的概率一半一半
            if(y_f == 0)
            {
                main_one.image[i] = 255;
            }
            else
            {
                main_one.image[i] = 0;
            }
        }
        main_one.noise_level += 10 * (100 - main_one.noise_level) / 100;//更新噪点值
        main_one.ave_brightness = update_ave(main_one.image, main_one.width * main_one.height);

        unsigned char * temp = new unsigned char[10000];
        int mid[25];
        for (int i = 0; i < 10000; i ++)//每个数据都当做中心数据处理一遍
        {
            temp[i] = main_one.image[i];
            if(edge_test(i))//忽略边缘数据👿
            {
                for(int j = 0; j < 25; j ++)
                {
                    mid[j] = temp[put_in(j, i)];
                }
                temp[i] = ave(mid, 25);
            }
        }
        for (int i = 0; i < 10000; i ++)//覆盖主图像
        {
            main_one.image[i] = temp[i]; 
        }

        main_one.noise_level *= 0.3;//更新噪点值
        main_one.ave_brightness = update_ave(main_one.image, main_one.width * main_one.height);
        delete []temp;//进行一个释放动态数组的运动

        /*
        以下是edge函数，由于输出部分不同决定把代码都搬过来
        */
        int Gx = 0;
        int Gy = 0;
        int g = 0;
        int count = 0;
        int numm = 0;
        for(int y = 0; y < main_one.height; y ++)
        {
            for(int x = 0; x < main_one.width; x ++)
            {
                count = y * 100 + x;//检索图像中元素
                /*
                检测边界
                */
                if (x == 99)
                {
                    Gx = 0;
                    Gy = main_one.image[count + 100] - main_one.image[count];
                }
                else if (y == 99)
                {
                    Gx = main_one.image[count + 1] - main_one.image[count];
                    Gy = 0;
                }
                else if(x == 99 && y == 99)
                {
                    Gx = 0;
                    Gy = 0;
                }
                else
                {
                    Gx = main_one.image[count + 1] - main_one.image[count];
                    Gy = main_one.image[count + 100] - main_one.image[count];
                }
                //取绝对值
                if (Gx < 0)
                {
                    Gx *= -1;
                }
                if(Gy < 0)
                {
                    Gy *= -1;
                }
                g = Gx + Gy;
                numm += (g > 100) ? 1: 0;//判断并赋值num
            }
        }
        add += numm;
        cout << "[INFO] Processing frame " << n + 1 << "/" << input << "... edge pixels: " << numm << endl;
        
    }
    cout << "[INFO] Average edge pixels: ";//
    printf("%.1f", add / input);
    cout << endl;
}

/*
histogram函数
不需要参数也不返回值
功能是输出灰度直方图，#代表50个像素，向下取整
*/
void histogram()
{
    int level = 0;//计算像素所属亮度区间
    int img[16] = {0};//统计亮度区间内像素量
    for(int i = 0; i < 10000; i ++)
    {
        if (main_one.image[i] == 0)//0单拎出来算
        {
            level = 0;
            img[level] ++;
        }
        else
        {
            level = (main_one.image[i] - 1) / 15;
            img[level - 1] ++;
        }
        
    }
    cout << "  0- 15: ";//15和255单独输出
    for(int n = 0; n < img[0] / 50; n ++)
    {
        cout << "#";
    } 
    cout << endl;
    for(int i = 1; i < 15; i ++)
    {
        cout << setw(3) << i * 15 + 1 << "-" << setw(3) << (i + 1) * 15 << ": ";
        for(int n = 0; n < img[i] / 50; n ++)
        {
            cout << "#";
        } 
        cout << endl;
    }
    cout << "240-255: ";
    for(int n = 0; n < img[15] / 50; n ++)
    {
        cout << "#";
    } 
    cout << endl;
}

/*
现代化改造的undu函数
需要一个整型参数（默认1），不返回值
功能是撤销操作，支持多个撤销，搭配tool里的undo_in食用
*/
void undo(int times)
{

    if (times > UNDO_MAX_RECORDS)//超过20条
    {
        cout << "[ERROR] Stupid move." << endl;
        return;
    }
    //记录undo
    string undo_hist;
    cout << "[INFO] Undo: load" << endl;
    for (int i = 0; i < times; i ++)
    {
        if (kj.empty())//空栈报错
        {
            cout << "[ERROR] Nothing to undo." << endl;
            return;
        }
        PerceptionSystem jk = kj.back();//栈顶

        kj.pop_back(); //弹出栈顶
        delete []main_one.image;//释放现图像内存
        main_one = jk;//拷贝
        main_one.image = new unsigned char[10000];
        memcpy(main_one.image, jk.image, 10000);//深拷贝图像
        delete []jk.image;//释放快照内存
        undo_hist = "undo :<";
        undo_hist += jk.nname;
        undo_hist += ">";
        //记录每次undo的指令
        history_add(undo_hist);
        cout << "[INFO] Status: " << main_one.height << "x" << main_one.width << ", noice=" 
        << main_one.noise_level << "%, brightness="; //顶不住了换一行
        printf("%.2f", main_one.ave_brightness);//小数一定要用printf写（✋️😭✋️）
        cout << ", targrts="  << main_one.target_count << ", initialized=" << main_one.is_initialized << endl;
        print(main_one.image, main_one.width);//打印一下，他居然有自己的函数这可真奢侈
    }
    undo_hist = "undo";
    undo_hist += " ";
    undo_hist += times;
    history_add(undo_hist);
}

