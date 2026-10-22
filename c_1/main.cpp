#include<bits/stdc++.h>
#include "percept.h"
#include "tool.h"

using namespace std;

int main()
{
    string input;//保存输入
    string edg;//最后一个输入，本质检测有没有多的参数
    string parameter;//参数
    string his;//历史记录
    int sw_input = 0;//根据枚举实现的指令识别，储存在switch时运用到的编号

        while(getline(cin, input))
        {
            if (input.empty())
            {
                continue;
            }
            else
            {
                stringstream read(input);//获取输入流，拆开一个一个看
                read >> parameter;//第一个输入
                sw_input = (int)check(parameter);//查看是什么指令
                if (!init_check() && sw_input != 0 && sw_input != 9 && sw_input != 10)//未初始化且执行的是需要初始化的指令
                {
                    cout << "[ERROR] System not initialized! Please run 'init' first." << endl;
                }
                else
                {
                    switch (sw_input)
                    /*
                    共两种类型：含参与不含参
                    */
                    {
                        case 0:
                        if (read >> edg)//有参数就报错
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        init();
                        his = parameter;//记录指令
                        history_add(his);
                        break;
                    
                        case 1:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        show();
                        his = parameter;
                        history_add(his);
                        break;
                        
                        case 2:
                        if (read >> edg)
                        {
                            his = parameter;//记录指令
                            parameter = edg;
                            if (read >> edg)//检查多余参数
                            {
                                cout << "[ERROR] Invalid command format!" << endl;
                                break;
                            }
                            else if(typec(parameter) != 'i')//检查参数类型
                            {
                                cout << "[ERROR] Invalid argument!" << endl;
                                break;
                            }
                            else
                            {
                                his += " ";//记录参数
                                his += parameter;
                                undo_in(his);
                                noise_percentage(stoi(parameter));
                                history_add(his);
                                break;
                            }
                        
                        }
                        else 
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                        }
                        break;
                        
                        case 3:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        his = parameter;
                        undo_in(his);
                        denoice();
                        history_add(his);
                        break;

                        
                        case 4:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        edge();
                        his = parameter;
                        history_add(his);
                        break;
                        
                        case 5:
                        if (read >> edg)
                        {
                            his = parameter;
                            parameter = edg;
                            if (read >> edg)
                            {
                                cout << "[ERROR] Invalid command format!" << endl;
                                break;
                            }
                            else if(typec(parameter) != 'i')
                            {
                                cout << "[ERROR] Invalid argument!" << endl;
                                break;
                            }
                            else
                            {
                                his += " ";
                                his += parameter;
                                undo_in(his);
                                threshold_value(stoi(parameter));
                                history_add(his);
                                break;
                            }
                            
                        }
                        else 
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                        }
                        break;
                        
                        case 6:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        find();
                        his = parameter;
                        history_add(his);
                        break;
                        
                        case 7:
                        if (read >> edg)
                        {
                            his = parameter;
                            parameter = edg;
                            if (read >> edg)
                            {
                                cout << "[ERROR] Invalid command format!" << endl;
                                break;
                            }
                            else if(typec(parameter) != 'c')
                            {
                                cout << "[ERROR] Invalid argument!" << endl;
                                break;
                            }
                            else
                            {
                                his += " ";
                                his += parameter;
                                undo_in(his);
                                flip_direction(parameter);
                                history_add(his);
                                break;
                            }
                            
                        }
                        else 
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                        }
                        break;
                        
                        case 8:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        his = parameter;
                        undo_in(his);
                        reset();
                        history_add(his);
                        break;
                        
                        case 9:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        help();
                        his = parameter;
                        history_add(his);
                        break;
                        
                        case 10:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        exit_p();
                        his = parameter;
                        history_add(his);
                        break;
                        
                        case 11:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        history();
                        his = parameter;
                        history_add(his);
                        break;
                        
                        case 12:
                        if (read >> edg)
                        {
                            his = parameter;
                            parameter = edg;
                            if (read >> edg)
                            {
                                cout << "[ERROR] Invalid command format!" << endl;
                                break;
                            }
                            else if(typec(parameter) != 'i')
                            {
                                cout << "[ERROR] Invalid argument!" << endl;
                                break;
                            }
                            else
                            {
                                his += " ";
                                his += parameter;
                                undo_in(his);
                                batch_frames(stoi(parameter));
                                history_add(his);
                                break;
                            }
                            
                        }
                        else 
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                        }
                        break;
                        
                        case 13:
                        if (read >> edg)
                        {
                            cout << "[ERROR] Invalid command format!" << endl;
                            break;
                        }
                        histogram();
                        his = parameter;
                        history_add(his);
                        break;
                        
                        case 14:
                        if (read >> edg)
                        {
                            //undo涉及多次输入历史记录所以这部分写在函数里
                            parameter = edg;
                            if (read >> edg)
                            {
                                cout << "[ERROR] Invalid command format!" << endl;
                                break;
                            }
                            else if(typec(parameter) != 'i')
                            {
                                cout << "[ERROR] Invalid argument!" << endl;
                                break;
                            }
                            else
                            {
                                undo(stoi(parameter));
                                break;
                            }
                            
                        }
                        else
                        {
                            undo();
                            break;
                        }
                        break;
                        
                        case 15:
                        cout << "[ERROR] Unknown command!" << endl;
                        break;
                    }
                }
            }
        }
        
    
    exit_p();
}