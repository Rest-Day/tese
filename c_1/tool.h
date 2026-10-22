#ifndef TOOL_H
#define TOOL_H
#include <bits/stdc++.h>

#define IMAGE_SIZE          100
#define IMAGE_PIXELS        10000
#define PREVIEW_COLS        20
#define PREVIEW_ROWS        10
#define NOISE_MIN           1
#define NOISE_MAX           100
#define DENOISE_WINDOW_SIZE 5
#define DENOISE_WINDOW_HALF 2
#define EDGE_THRESHOLD      100
#define THRESHOLD_MIN       0
#define THRESHOLD_MAX       255
#define FIND_MIN_AREA       2
#define HISTORY_MAX         20
#define BATCH_MAX_FRAMES    99
#define UNDO_MAX_RECORDS    10
#define MAGIC_BYTE_0        0x50  // 'P'
#define MAGIC_BYTE_1        0x53  // 'S'

struct PerceptionSystem //定义一个结构体
{
    unsigned char * image;
    int width = 100;
    int height = 100;
    int noise_level = 0;
    float ave_brightness;
    int target_count = 0;
    bool is_initialized = 0;
    std::string nname;
};
extern PerceptionSystem main_one;//主结构体一号，程序中主要数据的储存调用都在这里进行

extern std::deque<PerceptionSystem> kj;

enum class inp
{
    INIT, 
    SHOW, 
    NOISE, //
    DENOISE, //
    EDGE, 
    THRESHOLD, //
    FIND, 
    FLIP, //
    RESET, //
    HELP, 
    EXIT, 
    HISTORY, 
    BATCH, //
    HISTOGRAM, 
    UNDO, 
    UNKNOWN
};

void print (unsigned char * main, int wid);
float update_ave(unsigned char * main, int wid);
int ave(int * wind, int lon);
bool edge_test(int i);
int put_in(int num, int group);
int mmove(int cou, bool vis[10000], int cen[4]);
void undo_in(std::string & n);
inp check(const std::string & s);
char typec(std::string & in);
bool init_check();
void history_add(std::string & in);

#endif