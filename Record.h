#pragma once
#include <fstream>

//¥ÊµµΩ·ππÃÂ
struct Save
{


};

class Record
{
public:
	Record();
	void save_level();		//¥¢¥Ê¥Êµµ
	void load_level();		//¥Êµµ∂¡»°

private:
	Save save_file;


};
