#pragma once
#include <fstream>

//�浵�ṹ��
struct Save
{


};

class Record
{
public:
	Record();
	void save_level();		//����浵
	void load_level();		//�浵��ȡ

private:
	Save save_file;


};
