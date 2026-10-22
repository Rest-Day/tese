#ifndef PERCEPT_H
#define PERCEPT_H
#include <bits/stdc++.h>

extern std::vector<std::string> hist;//历史记录用到的数组
void init();
void show();
void noise_percentage(int percentage);
void denoice();
void edge();
void threshold_value(int value);
void find();
void flip_direction(std::string & direction);
void reset();
void help();
void exit_p();
void history();
void batch_frames(int input);
void histogram();
void undo(int times = 1);


#endif