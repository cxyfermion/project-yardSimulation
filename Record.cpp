#include "Record.h"

constexpr auto RECORDER_FILE = "recorder.txt";

Record::Record()
{



}

void Record::save_level()
{
    //this->save_file.save_coins = this->total_coin;
    //for (int i = 0; i < 4; i++)
    //{
    //    for (int j = 0; j < 5; j++)
    //    {
    //        this->save_file.save_level[i][j] = this->cur_level[i][j];
    //    }
    //}
    //std::ofstream file(RECORDER_FILE);
    //file.write((char*)&save_file, sizeof(save));
    //file.close();



}

void Record::load_level()
{

    //std::ifstream file;
    //file.open(RECORDER_FILE, std::ios::in);
    //if (!file.is_open())
    //{
    //    //第一次会打开失败，则初始化
    //    save_file.save_coins = 0;
    //    for (int i = 0; i < 4; i++)
    //    {
    //        for (int j = 0; j < 5; j++)
    //        {
    //            this->save_file.save_level[i][j] = 0;
    //        }
    //    }
    //}
    //else
    //{
    //    file.read((char*)&save_file, sizeof(save));
    //}
    //file.close();
    //this->total_coin = this->save_file.save_coins;
    ////注意删除
    ////this->total_coin = 999999;
    //for (int i = 0; i < 4; i++)
    //{
    //    for (int j = 0; j < 5; j++)
    //    {
    //        this->cost_rate += this->save_file.save_level[i][j] * 0.1;
    //        this->cur_level[i][j] = this->save_file.save_level[i][j];
    //        //注意删除
    //        //this->cur_level[i][j] = 0;
    //    }
    //}


}
