#include "Record.h"

#define NUM_SLOTS 10        //存档槽的数量

/*
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
    //for (int i = 0; i < 4; i++)
    //{
    //    for (int j = 0; j < 5; j++)
    //    {
    //        this->cost_rate += this->save_file.save_level[i][j] * 0.1;
    //        this->cur_level[i][j] = this->save_file.save_level[i][j];
    //    }
    //}
*/

Record::Record()
{

}

void Record::initGuiStyle()
{
    this->style = &ImGui::GetStyle();
}

void Record::save_level(int mark, Message& message, bool& random_initiating, int& compitence, SimuCore& core, Energy& energy, Environment& environment, Web& web, Flow& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo)
{
    //主控
    this->save_file.consoleFile.random_initiating = random_initiating;
    this->save_file.consoleFile.compitence = compitence;
    //仿真
    this->save_file.coreFile.time = core.time;
    this->save_file.coreFile.simuTime = core.simuTime;
    this->save_file.coreFile.simu_pause = core.simu_pause;
    this->save_file.coreFile.run_rate = core.run_rate;
    this->save_file.coreFile.stepping = core.stepping;
    this->save_file.coreFile.reset_zero = core.reset_zero;
    this->save_file.coreFile.reset_rand = core.reset_rand;
    this->save_file.coreFile.runtime_minutes = core.runtime_minutes;
    this->save_file.coreFile.runtime_hours = core.runtime_hours;
    this->save_file.coreFile.simu_deltaTime = core.simu_deltaTime;
    this->save_file.coreFile.freshRequire = core.freshRequire;
    this->save_file.coreFile.freshGapTime = core.freshGapTime;
    this->save_file.coreFile.freshGapSwitch = core.freshGapSwitch;
    this->save_file.coreFile.curGapTime = core.curGapTime;
    this->save_file.coreFile.simu_lastFrame = core.simu_lastFrame;
    this->save_file.coreFile.terminate_time = core.terminate_time;
    this->save_file.coreFile.terminate_countdown = core.terminate_countdown;
    this->save_file.coreFile.runtime_seconds = core.runtime_seconds;
    this->save_file.coreFile.runtime_days = core.runtime_days;
    //能源
    this->save_file.energyFile.trip = energy.trip;
    this->save_file.energyFile.total_energy = energy.total_energy;
    int i1 = 0;
    int i2 = 0;
    for (std::vector<Transformer>::const_iterator it1 = energy.transformers.begin(); it1 != energy.transformers.end(); it1++)
    {
        this->save_file.energyFile.cur_pow[i1] = it1->cur_pow;
        this->save_file.energyFile.cur_energy[i1] = it1->cur_energy;
        for (std::vector<switchGear>::const_iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
        {
            this->save_file.energyFile.state[i2] = it2->state;
            this->save_file.energyFile.time[i2] = it2->time;
            this->save_file.energyFile.pow_rate[i2] = it2->pow_rate;
            i2++;
        }
        i1++;
    }
    //环境
    this->save_file.environmentFile.weather = environment.weather;
    this->save_file.environmentFile.lightPos[0] = environment.lightPos.x;
    this->save_file.environmentFile.lightPos[1] = environment.lightPos.y;
    this->save_file.environmentFile.lightPos[2] = environment.lightPos.z;
    this->save_file.environmentFile.deltaTime = environment.deltaTime;
    this->save_file.environmentFile.lastFrame = environment.lastFrame;
    this->save_file.environmentFile.circle_radius = environment.circle_radius;
    this->save_file.environmentFile.skybox = environment.skybox;
    this->save_file.environmentFile.background = environment.background;
    this->save_file.environmentFile.texture_act = environment.texture_act;
    this->save_file.environmentFile.time_mode = environment.time_mode;
    this->save_file.environmentFile.minute_local = environment.minute_local;
    this->save_file.environmentFile.hour_local = environment.hour_local;
    this->save_file.environmentFile.light = environment.light;
    this->save_file.environmentFile.spotlight = environment.spotlight;
    //片段
    this->save_file.sizeofFragments = (int)web.fragments.size();
    this->save_file.fragmentFiles.clear();
    for (std::vector<Fragment>::const_iterator it1 = web.fragments.begin(); it1 != web.fragments.end(); it1++)
    {
        this->fragmentFile.froms_code = web.find_to(*it1->froms.begin());
        this->fragmentFile.to_code = web.find_to(it1->to);
        this->fragmentFile.total_length = it1->total_length;
        this->fragmentFile.begin = it1->begin;
        this->fragmentFile.end = it1->end;
        this->fragmentFile.disconnected = it1->disconnected;
        this->fragmentFile.amount = it1->amount;
        this->fragmentFile.cargo_type = it1->cargo_type;
        this->fragmentFile.cargo_index = it1->cargo_index;
        this->save_file.fragmentFiles.push_back(this->fragmentFile);
    }
    //物流网
    for (int i = 0; i < 51; i++)
    {
        this->save_file.webFile.convAmount[i] = web.convAmount[i];
    }
    i1 = 0;
    for (std::vector<Node>::const_iterator it1 = web.nodes.begin(); it1 != web.nodes.end(); it1++)
    {
        this->save_file.webFile.convPos[i1] = it1->convPos;
        this->save_file.webFile.wheelMode[i1] = it1->wheelMode;
        this->save_file.webFile.out[i1] = it1->out;
        this->save_file.webFile.out_flux[i1] = it1->out_flux;
        this->save_file.webFile.out_type[i1] = it1->out_type;
        this->save_file.webFile.out_index[i1] = it1->out_index;
        i1++;
    }
    //流程
    this->save_file.flowFile.window_confirm = flow.window_confirm;
    this->save_file.flowFile.windowGroundChoose = flow.windowGroundChoose;
    this->save_file.flowFile.groundSelected = flow.groundSelected;
    this->save_file.flowFile.chooseType = flow.chooseType;
    this->save_file.flowFile.chooseIndex = flow.chooseIndex;
    this->save_file.flowFile.onfocus = flow.onfocus;
    this->save_file.flowFile.emergency_stop = flow.emergency_stop;
    i1 = 0;
    for (std::vector<FlowAttrib>::const_iterator it1 = flow.flows.begin(); it1 != flow.flows.end(); it1++)
    {
        this->save_file.flowFile.flow_state[i1] = it1->flow_state;
        this->save_file.flowFile.flow_time[i1] = it1->flow_time;
        this->save_file.flowFile.scene_ready[i1] = it1->scene_ready;
        i1++;
    }
    //皮带
    i1 = 0;
    for (std::vector<ConvAttrib>::const_iterator it1 = conv.belts.begin(); it1 != conv.belts.end(); it1++)
    {
        this->save_file.convFile.conv_state[i1] = it1->conv_state;
        this->save_file.convFile.conv_cur_storage[i1] = it1->conv_cur_storage;
        this->save_file.convFile.flow_name[i1] = it1->flow_name;
        this->save_file.convFile.flow_idx[i1] = it1->flow_idx;
        this->save_file.convFile.conv_type[i1] = it1->conv_type;
        this->save_file.convFile.conv_index[i1] = it1->conv_index;
        i1++;
    }
    for (int i = 0; i < 459; i++)
    {
        this->save_file.convFile.conv_coords[i] = conv.conv_coords[i];
    }
    this->save_file.sizeofAct_flows = (int)conv.act_flows.size();
    this->save_file.act_flows.clear();
    for (std::vector<FlowState>::const_iterator it1 = conv.act_flows.begin(); it1 != conv.act_flows.end(); it1++)
    {
        this->flowState.flow_num = it1->flow_num;
        this->flowState.begin = it1->begin;
        this->save_file.act_flows.push_back(this->flowState);
    }
    //斗轮机
    i1 = 0;
    for (std::vector<WheelAttrib>::const_iterator it1 = wheel.wheels.begin(); it1 != wheel.wheels.end(); it1++)
    {
        this->save_file.wheelFile.wheel_state[i1] = it1->wheel_state;
        this->save_file.wheelFile.wheel_mode[i1] = it1->wheel_mode;
        i1++;
    }
    for (int i = 0; i < 56; i++)
    {
        this->save_file.wheelFile.wheel_coords[i] = wheel.wheel_coords[i];
    }
    //泊位
    this->save_file.berthFile.loader_state = berth.loader.loader_state;
    i1 = 0;
    for (std::vector<ShipUnloader>::const_iterator it1 = berth.unloaders.begin(); it1 != berth.unloaders.end(); it1++)
    {
        this->save_file.berthFile.unloader_state[i1] = it1->unloader_state;
        this->save_file.berthFile.crab_bucket_hor[i1] = it1->crab_bucket_hor;
        this->save_file.berthFile.crab_bucket_ver[i1] = it1->crab_bucket_ver;
        this->save_file.berthFile.crab_bucket_switch[i1] = it1->crab_bucket_switch;
        this->save_file.berthFile.crab_bucket_loaded[i1] = it1->crab_bucket_loaded;
        i1++;
    }
    this->save_file.berthFile.berth_finished = berth.berth_finished;
    for (int i = 0; i < 4; i++)
    {
        this->save_file.berthFile.webUnloaders[i] = berth.webUnloaders[i];
    }
    for (int i = 0; i < 34; i++)
    {
        this->save_file.berthFile.info_ships_occupied[i] = berth.info_ships_occupied[i];
    }
    for (int i = 0; i < 5; i++)
    {
        this->save_file.berthFile.berth_occupied[i] = berth.berth_occupied[i];
    }
    this->save_file.berthFile.unload_mode_gen = berth.unload_mode_gen;
    for (int i = 0; i < 40; i++)
    {
        this->save_file.berthFile.unloader_coords[i] = berth.unloader_coords[i];
    }
    for (int i = 0; i < 40; i++)
    {
        this->save_file.berthFile.bucket_coords[i] = berth.bucket_coords[i];
    }
    for (int i = 0; i < 5; i++)
    {
        this->save_file.berthFile.loader_coords[i] = berth.loader_coords[i];
    }
    this->save_file.sizeofShips = (int)berth.ships.size();
    this->save_file.ships.clear();
    for (std::vector<Ship>::const_iterator it1 = berth.ships.begin(); it1 != berth.ships.end(); it1++)
    {
        strcpy_s(this->shipFile.ship_name, it1->ship_name.c_str());
        this->shipFile.ship_state = it1->ship_state;
        this->shipFile.wait_pos[0] = it1->wait_pos[0];
        this->shipFile.wait_pos[1] = it1->wait_pos[1];
        this->shipFile.angle = it1->angle;
        this->shipFile.ship_total_storage = it1->ship_total_storage;
        this->shipFile.ship_current_storage = it1->ship_current_storage;
        this->shipFile.ship_index = it1->ship_index;
        this->shipFile.ship_type = it1->ship_type;
        for (int i = 0; i < 8; i++)
        {
            this->shipFile.ship_coords[i] = it1->ship_coords[i];
        }
        this->shipFile.ship_dist = it1->ship_dist;
        this->save_file.ships.push_back(this->shipFile);
    }
    //火车
    this->save_file.trainFile.building_state[0] = train.building_A.building_state;
    this->save_file.trainFile.building_state[1] = train.building_B.building_state;
    this->save_file.trainFile.building_mode[0] = train.building_A.building_mode;
    this->save_file.trainFile.building_mode[1] = train.building_B.building_mode;
    for (int i = 0; i < 6; i++)
    {
        this->save_file.trainFile.building_coords_A[i] = train.building_coords_A[i];
        this->save_file.trainFile.building_coords_B[i] = train.building_coords_B[i];
    }
    this->save_file.sizeofTrains = (int)train.trains.size();
    this->save_file.trains.clear();
    for (std::vector<TrainAttrib>::const_iterator it1 = train.trains.begin(); it1 != train.trains.end(); it1++)
    {
        this->train.train_idx = it1->train_idx;
        this->train.speed_train = it1->speed_train;
        this->train.train_type = it1->train_type;
        this->train.train_index = it1->train_index;
        this->train.train_lane = it1->train_lane;
        this->train.train_state = it1->train_state;
        this->train.current_num = it1->current_num;
        this->train.first_pos = it1->first_pos;
        this->train.max_per_wagon = it1->max_per_wagon;
        for (int i = 0; i < 7; i++)
        {
            this->train.train_coords[i] = it1->train_coords[i];
        }     
        this->train.total_num = it1->total_num;
        this->train.wagons.clear();
        for (std::vector<One_wagon>::const_iterator it2 = it1->wagons.begin(); it2 != it1->wagons.end(); it2++)
        {
            this->wagon.wagon_idx = it2->wagon_idx;
            this->wagon.amount = it2->amount;
            this->wagon.wagon_type = it2->wagon_type;
            this->wagon.wagon_index = it2->wagon_index;
            this->train.wagons.push_back(this->wagon);
        }
        this->save_file.trains.push_back(this->train);
    }
    //堆场
    this->save_file.yardFile.yard_choosed = yard.yard_choosed;
    this->save_file.yardFile.child_choosed = yard.child_choosed;
    this->save_file.yardFile.type_choosed = yard.type_choosed;
    this->save_file.yardFile.index_choosed = yard.index_choosed;
    this->save_file.yardFile.amount_choosed = yard.amount_choosed;
    this->save_file.yardFile.data_send = yard.data_send;
    this->save_file.yardFile.yard_choosing = yard.yard_choosing;
    this->save_file.yardFile.flow_yard_choosed_1 = yard.flow_yard_choosed_1;
    this->save_file.yardFile.flow_yard_choosed_2 = yard.flow_yard_choosed_2;
    this->save_file.yardFile.flow_num_choose = yard.flow_num_choose;
    this->save_file.yards.clear();
    i1 = 0;
    for (std::vector<Yard_father>::const_iterator it1 = yard.yards.begin(); it1 != yard.yards.end(); it1++)
    {
        this->save_file.yardFile.num_children[i1] = (int)it1->children.size();
        this->save_file.yardFile.yard_focus[i1] = it1->yard_focus;
        for (std::vector<Yard_child>::const_iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
        {
            this->child.child_name = it2->child_name;
            this->child.child_max_amount = it2->child_max_amount;
            this->child.child_cur_amount = it2->child_cur_amount;
            this->child.child_flux_left = it2->child_flux_left;
            this->child.child_flux_right = it2->child_flux_right;
            this->child.child_focus = it2->child_focus;
            this->child.child_type = it2->child_type;
            this->child.child_index = it2->child_index;
            this->child.child_state_left = it2->child_state_left;
            this->child.child_state_right = it2->child_state_right;
            this->save_file.yards.push_back(this->child);
        }
        i1++;
    }
    //筒仓
    this->save_file.siloFile.data_send = silo.data_send;
    this->save_file.siloFile.select_window = silo.select_window;
    this->save_file.siloFile.amount_apply = silo.amount_apply;
    this->save_file.siloFile.selected_index = silo.selected_index;
    for (int i = 0; i < 14; i++)
    {
        this->save_file.siloFile.silo_occupied[i] = silo.silo_occupied[i];
    }
    this->save_file.siloFile.storage_targets = silo.storage_targets;
    this->save_file.siloFile.storage_process = silo.storage_process;
    this->save_file.siloFile.straight_state = silo.straight_state;
    this->save_file.siloFile.straight_flux = silo.straight_flux;
    this->save_file.siloFile.out_total_0 = silo.out_total_0;
    this->save_file.siloFile.out_total_1 = silo.out_total_1;
    i1 = 0;
    for (std::vector<Silo_unit>::const_iterator it1 = silo.silos.begin(); it1 != silo.silos.end(); it1++)
    {
        this->save_file.siloFile.cur_storage[i1] = it1->cur_storage;
        this->save_file.siloFile.silo_type[i1] = it1->silo_type;
        this->save_file.siloFile.silo_flux[i1] = it1->silo_flux;
        this->save_file.siloFile.silo_focus[i1] = it1->silo_focus;
        this->save_file.siloFile.silo_state_up[i1] = it1->silo_state_up;
        this->save_file.siloFile.silo_state_down[i1] = it1->silo_state_down;
        for (int i = 0; i < 8; i++)
        {
            this->save_file.siloFile.silo_coords[i][i1] = it1->silo_coords[i];
        }
        i1++;
    }
    std::string route = "recorder_" + std::to_string(mark) + ".txt";
    std::ofstream file(route.c_str());
    //主控
    file.write((char*)&this->save_file.consoleFile.random_initiating, sizeof(this->save_file.consoleFile.random_initiating));
    file.write((char*)&this->save_file.consoleFile.compitence, sizeof(this->save_file.consoleFile.compitence));
    //仿真
    file.write((char*)&this->save_file.coreFile.time, sizeof(this->save_file.coreFile.time));
    file.write((char*)&this->save_file.coreFile.simuTime, sizeof(this->save_file.coreFile.simuTime));
    file.write((char*)&this->save_file.coreFile.simu_pause, sizeof(this->save_file.coreFile.simu_pause));
    file.write((char*)&this->save_file.coreFile.run_rate, sizeof(this->save_file.coreFile.run_rate));
    file.write((char*)&this->save_file.coreFile.stepping, sizeof(this->save_file.coreFile.stepping));
    file.write((char*)&this->save_file.coreFile.reset_zero, sizeof(this->save_file.coreFile.reset_zero));
    file.write((char*)&this->save_file.coreFile.reset_rand, sizeof(this->save_file.coreFile.reset_rand));
    file.write((char*)&this->save_file.coreFile.runtime_minutes, sizeof(this->save_file.coreFile.runtime_minutes));
    file.write((char*)&this->save_file.coreFile.runtime_hours, sizeof(this->save_file.coreFile.runtime_hours));
    file.write((char*)&this->save_file.coreFile.simu_deltaTime, sizeof(this->save_file.coreFile.simu_deltaTime));
    file.write((char*)&this->save_file.coreFile.freshRequire, sizeof(this->save_file.coreFile.freshRequire));
    file.write((char*)&this->save_file.coreFile.freshGapTime, sizeof(this->save_file.coreFile.freshGapTime));
    file.write((char*)&this->save_file.coreFile.freshGapSwitch, sizeof(this->save_file.coreFile.freshGapSwitch));
    file.write((char*)&this->save_file.coreFile.curGapTime, sizeof(this->save_file.coreFile.curGapTime));
    file.write((char*)&this->save_file.coreFile.simu_lastFrame, sizeof(this->save_file.coreFile.simu_lastFrame));
    file.write((char*)&this->save_file.coreFile.terminate_time, sizeof(this->save_file.coreFile.terminate_time));
    file.write((char*)&this->save_file.coreFile.terminate_countdown, sizeof(this->save_file.coreFile.terminate_countdown));
    file.write((char*)&this->save_file.coreFile.runtime_seconds, sizeof(this->save_file.coreFile.runtime_seconds));
    file.write((char*)&this->save_file.coreFile.runtime_days, sizeof(this->save_file.coreFile.runtime_days));
    //能源
    file.write((char*)&this->save_file.energyFile.trip, sizeof(this->save_file.energyFile.trip));
    file.write((char*)&this->save_file.energyFile.total_energy, sizeof(this->save_file.energyFile.total_energy));
    file.write((char*)&this->save_file.energyFile.cur_pow, sizeof(this->save_file.energyFile.cur_pow));
    file.write((char*)&this->save_file.energyFile.cur_energy, sizeof(this->save_file.energyFile.cur_energy));
    file.write((char*)&this->save_file.energyFile.state, sizeof(this->save_file.energyFile.state));
    file.write((char*)&this->save_file.energyFile.time, sizeof(this->save_file.energyFile.time));
    file.write((char*)&this->save_file.energyFile.pow_rate, sizeof(this->save_file.energyFile.pow_rate));
    //环境
    file.write((char*)&this->save_file.environmentFile.weather, sizeof(this->save_file.environmentFile.weather));
    file.write((char*)&this->save_file.environmentFile.lightPos, sizeof(this->save_file.environmentFile.lightPos));
    file.write((char*)&this->save_file.environmentFile.deltaTime, sizeof(this->save_file.environmentFile.deltaTime));
    file.write((char*)&this->save_file.environmentFile.lastFrame, sizeof(this->save_file.environmentFile.lastFrame));
    file.write((char*)&this->save_file.environmentFile.circle_radius, sizeof(this->save_file.environmentFile.circle_radius));
    file.write((char*)&this->save_file.environmentFile.skybox, sizeof(this->save_file.environmentFile.skybox));
    file.write((char*)&this->save_file.environmentFile.background, sizeof(this->save_file.environmentFile.background));
    file.write((char*)&this->save_file.environmentFile.texture_act, sizeof(this->save_file.environmentFile.texture_act));
    file.write((char*)&this->save_file.environmentFile.time_mode, sizeof(this->save_file.environmentFile.time_mode));
    file.write((char*)&this->save_file.environmentFile.minute_local, sizeof(this->save_file.environmentFile.minute_local));
    file.write((char*)&this->save_file.environmentFile.hour_local, sizeof(this->save_file.environmentFile.hour_local));
    file.write((char*)&this->save_file.environmentFile.light, sizeof(this->save_file.environmentFile.light));
    file.write((char*)&this->save_file.environmentFile.spotlight, sizeof(this->save_file.environmentFile.spotlight));
    //片段
    file.write((char*)&this->save_file.sizeofFragments, sizeof(this->save_file.sizeofFragments));
    for (std::vector<FragmentFile>::const_iterator it1 = this->save_file.fragmentFiles.begin(); it1 != this->save_file.fragmentFiles.end(); it1++)
    {
        file.write((char*)&it1->froms_code, sizeof(it1->froms_code));
        file.write((char*)&it1->to_code, sizeof(it1->to_code));
        file.write((char*)&it1->total_length, sizeof(it1->total_length));
        file.write((char*)&it1->begin, sizeof(it1->begin));
        file.write((char*)&it1->end, sizeof(it1->end));
        file.write((char*)&it1->disconnected, sizeof(it1->disconnected));
        file.write((char*)&it1->amount, sizeof(it1->amount));
        file.write((char*)&it1->cargo_type, sizeof(it1->cargo_type));
        file.write((char*)&it1->cargo_index, sizeof(it1->cargo_index));
    }
    //物流网
    file.write((char*)&this->save_file.webFile.convAmount, sizeof(this->save_file.webFile.convAmount));
    file.write((char*)&this->save_file.webFile.convPos, sizeof(this->save_file.webFile.convPos));
    file.write((char*)&this->save_file.webFile.wheelMode, sizeof(this->save_file.webFile.wheelMode));
    file.write((char*)&this->save_file.webFile.out, sizeof(this->save_file.webFile.out));
    file.write((char*)&this->save_file.webFile.out_flux, sizeof(this->save_file.webFile.out_flux));
    file.write((char*)&this->save_file.webFile.out_type, sizeof(this->save_file.webFile.out_type));
    file.write((char*)&this->save_file.webFile.out_index, sizeof(this->save_file.webFile.out_index));
    //流程
    file.write((char*)&this->save_file.flowFile.window_confirm, sizeof(this->save_file.flowFile.window_confirm));
    file.write((char*)&this->save_file.flowFile.windowGroundChoose, sizeof(this->save_file.flowFile.windowGroundChoose));
    file.write((char*)&this->save_file.flowFile.groundSelected, sizeof(this->save_file.flowFile.groundSelected));
    file.write((char*)&this->save_file.flowFile.chooseType, sizeof(this->save_file.flowFile.chooseType));
    file.write((char*)&this->save_file.flowFile.chooseIndex, sizeof(this->save_file.flowFile.chooseIndex));
    file.write((char*)&this->save_file.flowFile.onfocus, sizeof(this->save_file.flowFile.onfocus));
    file.write((char*)&this->save_file.flowFile.emergency_stop, sizeof(this->save_file.flowFile.emergency_stop));
    file.write((char*)&this->save_file.flowFile.flow_state, sizeof(this->save_file.flowFile.flow_state));
    file.write((char*)&this->save_file.flowFile.flow_time, sizeof(this->save_file.flowFile.flow_time));
    file.write((char*)&this->save_file.flowFile.scene_ready, sizeof(this->save_file.flowFile.scene_ready));
    //皮带
    file.write((char*)&this->save_file.convFile.conv_state, sizeof(this->save_file.convFile.conv_state));
    file.write((char*)&this->save_file.convFile.conv_cur_storage, sizeof(this->save_file.convFile.conv_cur_storage));
    file.write((char*)&this->save_file.convFile.flow_name, sizeof(this->save_file.convFile.flow_name));
    file.write((char*)&this->save_file.convFile.flow_idx, sizeof(this->save_file.convFile.flow_idx));
    file.write((char*)&this->save_file.convFile.conv_type, sizeof(this->save_file.convFile.conv_type));
    file.write((char*)&this->save_file.convFile.conv_index, sizeof(this->save_file.convFile.conv_index));
    file.write((char*)&this->save_file.convFile.conv_coords, sizeof(this->save_file.convFile.conv_coords));
    file.write((char*)&this->save_file.sizeofAct_flows, sizeof(this->save_file.sizeofAct_flows));
    for (std::vector<FlowState>::const_iterator it1 = this->save_file.act_flows.begin(); it1 != this->save_file.act_flows.end(); it1++)
    {
        file.write((char*)&it1->flow_num, sizeof(it1->flow_num));
        file.write((char*)&it1->begin, sizeof(it1->begin));
    }
    //斗轮机
    file.write((char*)&this->save_file.wheelFile.wheel_state, sizeof(this->save_file.wheelFile.wheel_state));
    file.write((char*)&this->save_file.wheelFile.wheel_mode, sizeof(this->save_file.wheelFile.wheel_mode));
    file.write((char*)&this->save_file.wheelFile.wheel_coords, sizeof(this->save_file.wheelFile.wheel_coords));
    //泊位
    file.write((char*)&this->save_file.berthFile.loader_state, sizeof(this->save_file.berthFile.loader_state));
    file.write((char*)&this->save_file.berthFile.unloader_state, sizeof(this->save_file.berthFile.unloader_state));
    file.write((char*)&this->save_file.berthFile.crab_bucket_hor, sizeof(this->save_file.berthFile.crab_bucket_hor));
    file.write((char*)&this->save_file.berthFile.crab_bucket_ver, sizeof(this->save_file.berthFile.crab_bucket_ver));
    file.write((char*)&this->save_file.berthFile.crab_bucket_switch, sizeof(this->save_file.berthFile.crab_bucket_switch));
    file.write((char*)&this->save_file.berthFile.crab_bucket_loaded, sizeof(this->save_file.berthFile.crab_bucket_loaded));
    file.write((char*)&this->save_file.berthFile.berth_finished, sizeof(this->save_file.berthFile.berth_finished));
    file.write((char*)&this->save_file.berthFile.webUnloaders, sizeof(this->save_file.berthFile.webUnloaders));
    file.write((char*)&this->save_file.berthFile.info_ships_occupied, sizeof(this->save_file.berthFile.info_ships_occupied));
    file.write((char*)&this->save_file.berthFile.berth_occupied, sizeof(this->save_file.berthFile.berth_occupied));
    file.write((char*)&this->save_file.berthFile.unload_mode_gen, sizeof(this->save_file.berthFile.unload_mode_gen));
    file.write((char*)&this->save_file.berthFile.unloader_coords, sizeof(this->save_file.berthFile.unloader_coords));
    file.write((char*)&this->save_file.berthFile.bucket_coords, sizeof(this->save_file.berthFile.bucket_coords));
    file.write((char*)&this->save_file.berthFile.loader_coords, sizeof(this->save_file.berthFile.loader_coords));
    file.write((char*)&this->save_file.sizeofShips, sizeof(this->save_file.sizeofShips));
    for (std::vector<ShipFile>::const_iterator it1 = this->save_file.ships.begin(); it1 != this->save_file.ships.end(); it1++)
    {
        file.write((char*)&it1->ship_name, sizeof(it1->ship_name));
        file.write((char*)&it1->ship_state, sizeof(it1->ship_state));
        file.write((char*)&it1->wait_pos, sizeof(it1->wait_pos));
        file.write((char*)&it1->angle, sizeof(it1->angle));
        file.write((char*)&it1->berth_idx, sizeof(it1->berth_idx));
        file.write((char*)&it1->ship_total_storage, sizeof(it1->ship_total_storage));
        file.write((char*)&it1->ship_current_storage, sizeof(it1->ship_current_storage));
        file.write((char*)&it1->ship_index, sizeof(it1->ship_index));
        file.write((char*)&it1->ship_type, sizeof(it1->ship_type));
        file.write((char*)&it1->ship_coords, sizeof(it1->ship_coords));
        file.write((char*)&it1->ship_dist, sizeof(it1->ship_dist));
    }
    //火车
    file.write((char*)&this->save_file.trainFile.building_state, sizeof(this->save_file.trainFile.building_state));
    file.write((char*)&this->save_file.trainFile.building_mode, sizeof(this->save_file.trainFile.building_mode));
    file.write((char*)&this->save_file.trainFile.building_coords_A, sizeof(this->save_file.trainFile.building_coords_A));
    file.write((char*)&this->save_file.trainFile.building_coords_B, sizeof(this->save_file.trainFile.building_coords_B));
    file.write((char*)&this->save_file.sizeofTrains, sizeof(this->save_file.sizeofTrains));
    for (std::vector<TrainAttrib>::const_iterator it1 = this->save_file.trains.begin(); it1 != this->save_file.trains.end(); it1++)
    {
        file.write((char*)&it1->train_idx, sizeof(it1->train_idx));
        file.write((char*)&it1->speed_train, sizeof(it1->speed_train));
        file.write((char*)&it1->train_type, sizeof(it1->train_type));
        file.write((char*)&it1->train_index, sizeof(it1->train_index));
        file.write((char*)&it1->train_lane, sizeof(it1->train_lane));
        file.write((char*)&it1->train_state, sizeof(it1->train_state));
        file.write((char*)&it1->current_num, sizeof(it1->current_num));
        file.write((char*)&it1->first_pos, sizeof(it1->first_pos));
        file.write((char*)&it1->max_per_wagon, sizeof(it1->max_per_wagon));
        file.write((char*)&it1->train_coords, sizeof(it1->train_coords));
        file.write((char*)&it1->total_num, sizeof(it1->total_num));
        for (std::vector<One_wagon>::const_iterator it2 = it1->wagons.begin(); it2 != it1->wagons.end(); it2++)
        {
            file.write((char*)&it2->wagon_idx, sizeof(it2->wagon_idx));
            file.write((char*)&it2->amount, sizeof(it2->amount));
            file.write((char*)&it2->wagon_type, sizeof(it2->wagon_type));
            file.write((char*)&it2->wagon_index, sizeof(it2->wagon_index));
        }
    }
    //堆场
    file.write((char*)&this->save_file.yardFile.yard_choosed, sizeof(this->save_file.yardFile.yard_choosed));
    file.write((char*)&this->save_file.yardFile.child_choosed, sizeof(this->save_file.yardFile.child_choosed));
    file.write((char*)&this->save_file.yardFile.type_choosed, sizeof(this->save_file.yardFile.type_choosed));
    file.write((char*)&this->save_file.yardFile.index_choosed, sizeof(this->save_file.yardFile.index_choosed));
    file.write((char*)&this->save_file.yardFile.amount_choosed, sizeof(this->save_file.yardFile.amount_choosed));
    file.write((char*)&this->save_file.yardFile.data_send, sizeof(this->save_file.yardFile.data_send));
    file.write((char*)&this->save_file.yardFile.yard_choosing, sizeof(this->save_file.yardFile.yard_choosing));
    file.write((char*)&this->save_file.yardFile.flow_yard_choosed_1, sizeof(this->save_file.yardFile.flow_yard_choosed_1));
    file.write((char*)&this->save_file.yardFile.flow_yard_choosed_2, sizeof(this->save_file.yardFile.flow_yard_choosed_2));
    file.write((char*)&this->save_file.yardFile.flow_num_choose, sizeof(this->save_file.yardFile.flow_num_choose));
    file.write((char*)&this->save_file.yardFile.num_children, sizeof(this->save_file.yardFile.num_children));
    file.write((char*)&this->save_file.yardFile.yard_focus, sizeof(this->save_file.yardFile.yard_focus));
    for (std::vector<Yard_child>::const_iterator it1 = this->save_file.yards.begin(); it1 != this->save_file.yards.end(); it1++)
    {
        file.write((char*)&it1->child_name, sizeof(it1->child_name));
        file.write((char*)&it1->child_max_amount, sizeof(it1->child_max_amount));
        file.write((char*)&it1->child_cur_amount, sizeof(it1->child_cur_amount));
        file.write((char*)&it1->child_flux_left, sizeof(it1->child_flux_left));
        file.write((char*)&it1->child_flux_right, sizeof(it1->child_flux_right));
        file.write((char*)&it1->child_focus, sizeof(it1->child_focus));
        file.write((char*)&it1->child_type, sizeof(it1->child_type));
        file.write((char*)&it1->child_index, sizeof(it1->child_index));
        file.write((char*)&it1->child_state_left, sizeof(it1->child_state_left));
        file.write((char*)&it1->child_state_right, sizeof(it1->child_state_right));
    }
    //筒仓
    file.write((char*)&this->save_file.siloFile.data_send, sizeof(this->save_file.siloFile.data_send));
    file.write((char*)&this->save_file.siloFile.select_window, sizeof(this->save_file.siloFile.select_window));
    file.write((char*)&this->save_file.siloFile.amount_apply, sizeof(this->save_file.siloFile.amount_apply));
    file.write((char*)&this->save_file.siloFile.selected_index, sizeof(this->save_file.siloFile.selected_index));
    file.write((char*)&this->save_file.siloFile.silo_occupied, sizeof(this->save_file.siloFile.silo_occupied));
    file.write((char*)&this->save_file.siloFile.storage_targets, sizeof(this->save_file.siloFile.storage_targets));
    file.write((char*)&this->save_file.siloFile.storage_process, sizeof(this->save_file.siloFile.storage_process));
    file.write((char*)&this->save_file.siloFile.straight_state, sizeof(this->save_file.siloFile.straight_state));
    file.write((char*)&this->save_file.siloFile.straight_flux, sizeof(this->save_file.siloFile.straight_flux));
    file.write((char*)&this->save_file.siloFile.out_total_0, sizeof(this->save_file.siloFile.out_total_0));
    file.write((char*)&this->save_file.siloFile.out_total_1, sizeof(this->save_file.siloFile.out_total_1));
    file.write((char*)&this->save_file.siloFile.cur_storage, sizeof(this->save_file.siloFile.cur_storage));
    file.write((char*)&this->save_file.siloFile.silo_type, sizeof(this->save_file.siloFile.silo_type));
    file.write((char*)&this->save_file.siloFile.silo_flux, sizeof(this->save_file.siloFile.silo_flux));
    file.write((char*)&this->save_file.siloFile.silo_focus, sizeof(this->save_file.siloFile.silo_focus));
    file.write((char*)&this->save_file.siloFile.silo_state_up, sizeof(this->save_file.siloFile.silo_state_up));
    file.write((char*)&this->save_file.siloFile.silo_state_down, sizeof(this->save_file.siloFile.silo_state_down));
    file.write((char*)&this->save_file.siloFile.silo_coords, sizeof(this->save_file.siloFile.silo_coords));
    file.close();
    message.push_message(u8"游戏保存成功！");
    message.push_sound("res/audio/saved.ogg", false);
}

void Record::load_level(int mark, Message& message, bool& random_initiating, int& compitence, SimuCore& core, Energy& energy, Environment& environment, Web& web, Flow& flow, Conveyor& conv, SlewingWheel& wheel, Berth& berth, TrainLoader& train, Yard& yard, Silo& silo)
{
    std::ifstream file;
    std::string route = "recorder_" + std::to_string(mark) + ".txt";
    file.open(route.c_str(), std::ios::in);
    if (!file.is_open())
    {
        //第一次会打开失败，则初始化
        message.push_message(u8"游戏读取失败！");
        return;
    }
    else
    {
        //主控
        file.read((char*)&this->save_file.consoleFile.random_initiating, sizeof(this->save_file.consoleFile.random_initiating));
        file.read((char*)&this->save_file.consoleFile.compitence, sizeof(this->save_file.consoleFile.compitence));
        //仿真
        file.read((char*)&this->save_file.coreFile.time, sizeof(this->save_file.coreFile.time));
        file.read((char*)&this->save_file.coreFile.simuTime, sizeof(this->save_file.coreFile.simuTime));
        file.read((char*)&this->save_file.coreFile.simu_pause, sizeof(this->save_file.coreFile.simu_pause));
        file.read((char*)&this->save_file.coreFile.run_rate, sizeof(this->save_file.coreFile.run_rate));
        file.read((char*)&this->save_file.coreFile.stepping, sizeof(this->save_file.coreFile.stepping));
        file.read((char*)&this->save_file.coreFile.reset_zero, sizeof(this->save_file.coreFile.reset_zero));
        file.read((char*)&this->save_file.coreFile.reset_rand, sizeof(this->save_file.coreFile.reset_rand));
        file.read((char*)&this->save_file.coreFile.runtime_minutes, sizeof(this->save_file.coreFile.runtime_minutes));
        file.read((char*)&this->save_file.coreFile.runtime_hours, sizeof(this->save_file.coreFile.runtime_hours));
        file.read((char*)&this->save_file.coreFile.simu_deltaTime, sizeof(this->save_file.coreFile.simu_deltaTime));
        file.read((char*)&this->save_file.coreFile.freshRequire, sizeof(this->save_file.coreFile.freshRequire));
        file.read((char*)&this->save_file.coreFile.freshGapTime, sizeof(this->save_file.coreFile.freshGapTime));
        file.read((char*)&this->save_file.coreFile.freshGapSwitch, sizeof(this->save_file.coreFile.freshGapSwitch));
        file.read((char*)&this->save_file.coreFile.curGapTime, sizeof(this->save_file.coreFile.curGapTime));
        file.read((char*)&this->save_file.coreFile.simu_lastFrame, sizeof(this->save_file.coreFile.simu_lastFrame));
        file.read((char*)&this->save_file.coreFile.terminate_time, sizeof(this->save_file.coreFile.terminate_time));
        file.read((char*)&this->save_file.coreFile.terminate_countdown, sizeof(this->save_file.coreFile.terminate_countdown));
        file.read((char*)&this->save_file.coreFile.runtime_seconds, sizeof(this->save_file.coreFile.runtime_seconds));
        file.read((char*)&this->save_file.coreFile.runtime_days, sizeof(this->save_file.coreFile.runtime_days));
        //能源
        file.read((char*)&this->save_file.energyFile.trip, sizeof(this->save_file.energyFile.trip));
        file.read((char*)&this->save_file.energyFile.total_energy, sizeof(this->save_file.energyFile.total_energy));
        file.read((char*)&this->save_file.energyFile.cur_pow, sizeof(this->save_file.energyFile.cur_pow));
        file.read((char*)&this->save_file.energyFile.cur_energy, sizeof(this->save_file.energyFile.cur_energy));
        file.read((char*)&this->save_file.energyFile.state, sizeof(this->save_file.energyFile.state));
        file.read((char*)&this->save_file.energyFile.time, sizeof(this->save_file.energyFile.time));
        file.read((char*)&this->save_file.energyFile.pow_rate, sizeof(this->save_file.energyFile.pow_rate));
        //环境
        file.read((char*)&this->save_file.environmentFile.weather, sizeof(this->save_file.environmentFile.weather));
        file.read((char*)&this->save_file.environmentFile.lightPos, sizeof(this->save_file.environmentFile.lightPos));
        file.read((char*)&this->save_file.environmentFile.deltaTime, sizeof(this->save_file.environmentFile.deltaTime));
        file.read((char*)&this->save_file.environmentFile.lastFrame, sizeof(this->save_file.environmentFile.lastFrame));
        file.read((char*)&this->save_file.environmentFile.circle_radius, sizeof(this->save_file.environmentFile.circle_radius));
        file.read((char*)&this->save_file.environmentFile.skybox, sizeof(this->save_file.environmentFile.skybox));
        file.read((char*)&this->save_file.environmentFile.background, sizeof(this->save_file.environmentFile.background));
        file.read((char*)&this->save_file.environmentFile.texture_act, sizeof(this->save_file.environmentFile.texture_act));
        file.read((char*)&this->save_file.environmentFile.time_mode, sizeof(this->save_file.environmentFile.time_mode));
        file.read((char*)&this->save_file.environmentFile.minute_local, sizeof(this->save_file.environmentFile.minute_local));
        file.read((char*)&this->save_file.environmentFile.hour_local, sizeof(this->save_file.environmentFile.hour_local));
        file.read((char*)&this->save_file.environmentFile.light, sizeof(this->save_file.environmentFile.light));
        file.read((char*)&this->save_file.environmentFile.spotlight, sizeof(this->save_file.environmentFile.spotlight));
        //片段
        file.read((char*)&this->save_file.sizeofFragments, sizeof(this->save_file.sizeofFragments));
        this->save_file.fragmentFiles.clear();
        for (int i = 0; i < this->save_file.sizeofFragments; i++)
        {
            file.read((char*)&this->fragmentFile.froms_code, sizeof(this->fragmentFile.froms_code));
            file.read((char*)&this->fragmentFile.to_code, sizeof(this->fragmentFile.to_code));
            file.read((char*)&this->fragmentFile.total_length, sizeof(this->fragmentFile.total_length));
            file.read((char*)&this->fragmentFile.begin, sizeof(this->fragmentFile.begin));
            file.read((char*)&this->fragmentFile.end, sizeof(this->fragmentFile.end));
            file.read((char*)&this->fragmentFile.disconnected, sizeof(this->fragmentFile.disconnected));
            file.read((char*)&this->fragmentFile.amount, sizeof(this->fragmentFile.amount));
            file.read((char*)&this->fragmentFile.cargo_type, sizeof(this->fragmentFile.cargo_type));
            file.read((char*)&this->fragmentFile.cargo_index, sizeof(this->fragmentFile.cargo_index));
            this->save_file.fragmentFiles.push_back(this->fragmentFile);
        }
        //物流网
        file.read((char*)&this->save_file.webFile.convAmount, sizeof(this->save_file.webFile.convAmount));
        file.read((char*)&this->save_file.webFile.convPos, sizeof(this->save_file.webFile.convPos));
        file.read((char*)&this->save_file.webFile.wheelMode, sizeof(this->save_file.webFile.wheelMode));
        file.read((char*)&this->save_file.webFile.out, sizeof(this->save_file.webFile.out));
        file.read((char*)&this->save_file.webFile.out_flux, sizeof(this->save_file.webFile.out_flux));
        file.read((char*)&this->save_file.webFile.out_type, sizeof(this->save_file.webFile.out_type));
        file.read((char*)&this->save_file.webFile.out_index, sizeof(this->save_file.webFile.out_index));
        //流程
        file.read((char*)&this->save_file.flowFile.window_confirm, sizeof(this->save_file.flowFile.window_confirm));
        file.read((char*)&this->save_file.flowFile.windowGroundChoose, sizeof(this->save_file.flowFile.windowGroundChoose));
        file.read((char*)&this->save_file.flowFile.groundSelected, sizeof(this->save_file.flowFile.groundSelected));
        file.read((char*)&this->save_file.flowFile.chooseType, sizeof(this->save_file.flowFile.chooseType));
        file.read((char*)&this->save_file.flowFile.chooseIndex, sizeof(this->save_file.flowFile.chooseIndex));
        file.read((char*)&this->save_file.flowFile.onfocus, sizeof(this->save_file.flowFile.onfocus));
        file.read((char*)&this->save_file.flowFile.emergency_stop, sizeof(this->save_file.flowFile.emergency_stop));
        file.read((char*)&this->save_file.flowFile.flow_state, sizeof(this->save_file.flowFile.flow_state));
        file.read((char*)&this->save_file.flowFile.flow_time, sizeof(this->save_file.flowFile.flow_time));
        file.read((char*)&this->save_file.flowFile.scene_ready, sizeof(this->save_file.flowFile.scene_ready));
        //皮带
        file.read((char*)&this->save_file.convFile.conv_state, sizeof(this->save_file.convFile.conv_state));
        file.read((char*)&this->save_file.convFile.conv_cur_storage, sizeof(this->save_file.convFile.conv_cur_storage));
        file.read((char*)&this->save_file.convFile.flow_name, sizeof(this->save_file.convFile.flow_name));
        file.read((char*)&this->save_file.convFile.flow_idx, sizeof(this->save_file.convFile.flow_idx));
        file.read((char*)&this->save_file.convFile.conv_type, sizeof(this->save_file.convFile.conv_type));
        file.read((char*)&this->save_file.convFile.conv_index, sizeof(this->save_file.convFile.conv_index));
        file.read((char*)&this->save_file.convFile.conv_coords, sizeof(this->save_file.convFile.conv_coords));
        file.read((char*)&this->save_file.sizeofAct_flows, sizeof(this->save_file.sizeofAct_flows));
        this->save_file.act_flows.clear();
        for (int i = 0; i < this->save_file.sizeofAct_flows; i++)
        {
            file.read((char*)&this->flowState.flow_num, sizeof(this->flowState.flow_num));
            file.read((char*)&this->flowState.begin, sizeof(this->flowState.begin));
            this->save_file.act_flows.push_back(this->flowState);
        }
        //斗轮机
        file.read((char*)&this->save_file.wheelFile.wheel_state, sizeof(this->save_file.wheelFile.wheel_state));
        file.read((char*)&this->save_file.wheelFile.wheel_mode, sizeof(this->save_file.wheelFile.wheel_mode));
        file.read((char*)&this->save_file.wheelFile.wheel_coords, sizeof(this->save_file.wheelFile.wheel_coords));
        //泊位
        file.read((char*)&this->save_file.berthFile.loader_state, sizeof(this->save_file.berthFile.loader_state));
        file.read((char*)&this->save_file.berthFile.unloader_state, sizeof(this->save_file.berthFile.unloader_state));
        file.read((char*)&this->save_file.berthFile.crab_bucket_hor, sizeof(this->save_file.berthFile.crab_bucket_hor));
        file.read((char*)&this->save_file.berthFile.crab_bucket_ver, sizeof(this->save_file.berthFile.crab_bucket_ver));
        file.read((char*)&this->save_file.berthFile.crab_bucket_switch, sizeof(this->save_file.berthFile.crab_bucket_switch));
        file.read((char*)&this->save_file.berthFile.crab_bucket_loaded, sizeof(this->save_file.berthFile.crab_bucket_loaded));
        file.read((char*)&this->save_file.berthFile.berth_finished, sizeof(this->save_file.berthFile.berth_finished));
        file.read((char*)&this->save_file.berthFile.webUnloaders, sizeof(this->save_file.berthFile.webUnloaders));
        file.read((char*)&this->save_file.berthFile.info_ships_occupied, sizeof(this->save_file.berthFile.info_ships_occupied));
        file.read((char*)&this->save_file.berthFile.berth_occupied, sizeof(this->save_file.berthFile.berth_occupied));
        file.read((char*)&this->save_file.berthFile.unload_mode_gen, sizeof(this->save_file.berthFile.unload_mode_gen));
        file.read((char*)&this->save_file.berthFile.unloader_coords, sizeof(this->save_file.berthFile.unloader_coords));
        file.read((char*)&this->save_file.berthFile.bucket_coords, sizeof(this->save_file.berthFile.bucket_coords));
        file.read((char*)&this->save_file.berthFile.loader_coords, sizeof(this->save_file.berthFile.loader_coords));
        file.read((char*)&this->save_file.sizeofShips, sizeof(this->save_file.sizeofShips));
        this->save_file.ships.clear();
        for (int i = 0; i < this->save_file.sizeofShips; i++)
        {
            file.read((char*)&this->shipFile.ship_name, sizeof(this->shipFile.ship_name));
            file.read((char*)&this->shipFile.ship_state, sizeof(this->shipFile.ship_state));
            file.read((char*)&this->shipFile.wait_pos, sizeof(this->shipFile.wait_pos));
            file.read((char*)&this->shipFile.angle, sizeof(this->shipFile.angle));
            file.read((char*)&this->shipFile.berth_idx, sizeof(this->shipFile.berth_idx));
            file.read((char*)&this->shipFile.ship_total_storage, sizeof(this->shipFile.ship_total_storage));
            file.read((char*)&this->shipFile.ship_current_storage, sizeof(this->shipFile.ship_current_storage));
            file.read((char*)&this->shipFile.ship_index, sizeof(this->shipFile.ship_index));
            file.read((char*)&this->shipFile.ship_type, sizeof(this->shipFile.ship_type));
            file.read((char*)&this->shipFile.ship_coords, sizeof(this->shipFile.ship_coords));
            file.read((char*)&this->shipFile.ship_dist, sizeof(this->shipFile.ship_dist));
            this->save_file.ships.push_back(this->shipFile);
        }
        //火车
        file.read((char*)&this->save_file.trainFile.building_state, sizeof(this->save_file.trainFile.building_state));
        file.read((char*)&this->save_file.trainFile.building_mode, sizeof(this->save_file.trainFile.building_mode));
        file.read((char*)&this->save_file.trainFile.building_coords_A, sizeof(this->save_file.trainFile.building_coords_A));
        file.read((char*)&this->save_file.trainFile.building_coords_B, sizeof(this->save_file.trainFile.building_coords_B));
        file.read((char*)&this->save_file.sizeofTrains, sizeof(this->save_file.sizeofTrains));
        this->save_file.trains.clear();
        for (int i = 0; i < this->save_file.sizeofTrains; i++)
        {
            file.read((char*)&this->train.train_idx, sizeof(this->train.train_idx));
            file.read((char*)&this->train.speed_train, sizeof(this->train.speed_train));
            file.read((char*)&this->train.train_type, sizeof(this->train.train_type));
            file.read((char*)&this->train.train_index, sizeof(this->train.train_index));
            file.read((char*)&this->train.train_lane, sizeof(this->train.train_lane));
            file.read((char*)&this->train.train_state, sizeof(this->train.train_state));
            file.read((char*)&this->train.current_num, sizeof(this->train.current_num));
            file.read((char*)&this->train.first_pos, sizeof(this->train.first_pos));
            file.read((char*)&this->train.max_per_wagon, sizeof(this->train.max_per_wagon));
            file.read((char*)&this->train.train_coords, sizeof(this->train.train_coords));
            file.read((char*)&this->train.total_num, sizeof(this->train.total_num));
            this->train.wagons.clear();
            for (int j = 0; j < this->train.total_num; j++)
            {
                file.read((char*)&this->wagon.wagon_idx, sizeof(this->wagon.wagon_idx));
                file.read((char*)&this->wagon.amount, sizeof(this->wagon.amount));
                file.read((char*)&this->wagon.wagon_type, sizeof(this->wagon.wagon_type));
                file.read((char*)&this->wagon.wagon_index, sizeof(this->wagon.wagon_index));
                this->train.wagons.push_back(this->wagon);
            }
            this->save_file.trains.push_back(this->train);
        }
        //堆场
        file.read((char*)&this->save_file.yardFile.yard_choosed, sizeof(this->save_file.yardFile.yard_choosed));
        file.read((char*)&this->save_file.yardFile.child_choosed, sizeof(this->save_file.yardFile.child_choosed));
        file.read((char*)&this->save_file.yardFile.type_choosed, sizeof(this->save_file.yardFile.type_choosed));
        file.read((char*)&this->save_file.yardFile.index_choosed, sizeof(this->save_file.yardFile.index_choosed));
        file.read((char*)&this->save_file.yardFile.amount_choosed, sizeof(this->save_file.yardFile.amount_choosed));
        file.read((char*)&this->save_file.yardFile.data_send, sizeof(this->save_file.yardFile.data_send));
        file.read((char*)&this->save_file.yardFile.yard_choosing, sizeof(this->save_file.yardFile.yard_choosing));
        file.read((char*)&this->save_file.yardFile.flow_yard_choosed_1, sizeof(this->save_file.yardFile.flow_yard_choosed_1));
        file.read((char*)&this->save_file.yardFile.flow_yard_choosed_2, sizeof(this->save_file.yardFile.flow_yard_choosed_2));
        file.read((char*)&this->save_file.yardFile.flow_num_choose, sizeof(this->save_file.yardFile.flow_num_choose));
        file.read((char*)&this->save_file.yardFile.num_children, sizeof(this->save_file.yardFile.num_children));
        file.read((char*)&this->save_file.yardFile.yard_focus, sizeof(this->save_file.yardFile.yard_focus));
        this->save_file.yards.clear();
        for (int i = 0; i < 68; i++)
        {
            file.read((char*)&this->child.child_name, sizeof(this->child.child_name));
            file.read((char*)&this->child.child_max_amount, sizeof(this->child.child_max_amount));
            file.read((char*)&this->child.child_cur_amount, sizeof(this->child.child_cur_amount));
            file.read((char*)&this->child.child_flux_left, sizeof(this->child.child_flux_left));
            file.read((char*)&this->child.child_flux_right, sizeof(this->child.child_flux_right));
            file.read((char*)&this->child.child_focus, sizeof(this->child.child_focus));
            file.read((char*)&this->child.child_type, sizeof(this->child.child_type));
            file.read((char*)&this->child.child_index, sizeof(this->child.child_index));
            file.read((char*)&this->child.child_state_left, sizeof(this->child.child_state_left));
            file.read((char*)&this->child.child_state_right, sizeof(this->child.child_state_right));
            this->save_file.yards.push_back(this->child);
        }
        //筒仓
        file.read((char*)&this->save_file.siloFile.data_send, sizeof(this->save_file.siloFile.data_send));
        file.read((char*)&this->save_file.siloFile.select_window, sizeof(this->save_file.siloFile.select_window));
        file.read((char*)&this->save_file.siloFile.amount_apply, sizeof(this->save_file.siloFile.amount_apply));
        file.read((char*)&this->save_file.siloFile.selected_index, sizeof(this->save_file.siloFile.selected_index));
        file.read((char*)&this->save_file.siloFile.silo_occupied, sizeof(this->save_file.siloFile.silo_occupied));
        file.read((char*)&this->save_file.siloFile.storage_targets, sizeof(this->save_file.siloFile.storage_targets));
        file.read((char*)&this->save_file.siloFile.storage_process, sizeof(this->save_file.siloFile.storage_process));
        file.read((char*)&this->save_file.siloFile.straight_state, sizeof(this->save_file.siloFile.straight_state));
        file.read((char*)&this->save_file.siloFile.straight_flux, sizeof(this->save_file.siloFile.straight_flux));
        file.read((char*)&this->save_file.siloFile.out_total_0, sizeof(this->save_file.siloFile.out_total_0));
        file.read((char*)&this->save_file.siloFile.out_total_1, sizeof(this->save_file.siloFile.out_total_1));
        file.read((char*)&this->save_file.siloFile.cur_storage, sizeof(this->save_file.siloFile.cur_storage));
        file.read((char*)&this->save_file.siloFile.silo_type, sizeof(this->save_file.siloFile.silo_type));
        file.read((char*)&this->save_file.siloFile.silo_flux, sizeof(this->save_file.siloFile.silo_flux));
        file.read((char*)&this->save_file.siloFile.silo_focus, sizeof(this->save_file.siloFile.silo_focus));
        file.read((char*)&this->save_file.siloFile.silo_state_up, sizeof(this->save_file.siloFile.silo_state_up));
        file.read((char*)&this->save_file.siloFile.silo_state_down, sizeof(this->save_file.siloFile.silo_state_down));
        file.read((char*)&this->save_file.siloFile.silo_coords, sizeof(this->save_file.siloFile.silo_coords));
    }
    file.close();
    //主控
    random_initiating = this->save_file.consoleFile.random_initiating;
    compitence = this->save_file.consoleFile.compitence;
    //仿真
    core.time = this->save_file.coreFile.time;
    core.simuTime = this->save_file.coreFile.simuTime;
    core.simu_pause = this->save_file.coreFile.simu_pause;
    core.run_rate = this->save_file.coreFile.run_rate;
    core.stepping = this->save_file.coreFile.stepping;
    core.reset_zero = this->save_file.coreFile.reset_zero;
    core.reset_rand = this->save_file.coreFile.reset_rand;
    core.runtime_minutes = this->save_file.coreFile.runtime_minutes;
    core.runtime_hours = this->save_file.coreFile.runtime_hours;
    core.simu_deltaTime = this->save_file.coreFile.simu_deltaTime;
    core.freshRequire = this->save_file.coreFile.freshRequire;
    core.freshGapTime = this->save_file.coreFile.freshGapTime;
    core.freshGapSwitch = this->save_file.coreFile.freshGapSwitch;
    core.curGapTime = this->save_file.coreFile.curGapTime;
    core.simu_lastFrame = this->save_file.coreFile.simu_lastFrame;
    core.terminate_time = this->save_file.coreFile.terminate_time;
    core.terminate_countdown = this->save_file.coreFile.terminate_countdown;
    core.runtime_seconds = this->save_file.coreFile.runtime_seconds;
    core.runtime_days = this->save_file.coreFile.runtime_days;
    //能源
    energy.trip = this->save_file.energyFile.trip;
    energy.total_energy = this->save_file.energyFile.total_energy;
    int i1 = 0;
    int i2 = 0;
    for (std::vector<Transformer>::iterator it1 = energy.transformers.begin(); it1 != energy.transformers.end(); it1++)
    {
        it1->cur_pow = this->save_file.energyFile.cur_pow[i1];
        it1->cur_energy = this->save_file.energyFile.cur_energy[i1];
        for (std::vector<switchGear>::iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
        {
            it2->state = this->save_file.energyFile.state[i2];
            it2->time = this->save_file.energyFile.time[i2];
            it2->pow_rate = this->save_file.energyFile.pow_rate[i2];
            i2++;
        }
        i1++;
    }
    //环境
    environment.weather = this->save_file.environmentFile.weather;
    environment.lightPos.x = this->save_file.environmentFile.lightPos[0];
    environment.lightPos.y = this->save_file.environmentFile.lightPos[1];
    environment.lightPos.z = this->save_file.environmentFile.lightPos[2];
    environment.deltaTime = this->save_file.environmentFile.deltaTime;
    environment.lastFrame = this->save_file.environmentFile.lastFrame;
    environment.circle_radius = this->save_file.environmentFile.circle_radius;
    environment.skybox = this->save_file.environmentFile.skybox;
    environment.background = this->save_file.environmentFile.background;
    environment.texture_act = this->save_file.environmentFile.texture_act;
    environment.time_mode = this->save_file.environmentFile.time_mode;
    environment.minute_local = this->save_file.environmentFile.minute_local;
    environment.hour_local = this->save_file.environmentFile.hour_local;
    environment.light = this->save_file.environmentFile.light;
    environment.spotlight = this->save_file.environmentFile.spotlight;
    //片段
    web.fragments.clear();
    for (std::vector<FragmentFile>::const_iterator it1 = this->save_file.fragmentFiles.begin(); it1 != this->save_file.fragmentFiles.end(); it1++)
    {
        web.fragment.froms.clear();
        if (it1->froms_code < 4)
        {
            //通用泊位
            web.fragment.froms.push_back("1#");
            web.fragment.froms.push_back("2#");
            web.fragment.froms.push_back("3#");
            web.fragment.froms.push_back("4#");
        }
        else if (it1->froms_code < 8)
        {
            //4泊位
            web.fragment.froms.push_back("11#");
            web.fragment.froms.push_back("12#");
            web.fragment.froms.push_back("13#");
            web.fragment.froms.push_back("14#");
        }
        else
        {
            web.fragment.froms.push_back(web.find_toName(it1->froms_code));
        }
        web.fragment.to = web.find_toName(it1->to_code);
        web.fragment.total_length = it1->total_length;
        web.fragment.begin = it1->begin;
        web.fragment.end = it1->end;
        web.fragment.disconnected = it1->disconnected;
        web.fragment.amount = it1->amount;
        web.fragment.cargo_type = it1->cargo_type;
        web.fragment.cargo_index = it1->cargo_index;
        web.fragments.push_back(web.fragment);
    }
    //物流网
    for (int i = 0; i < 51; i++)
    {
        web.convAmount[i] = this->save_file.webFile.convAmount[i];
    }
    i1 = 0;
    for (std::vector<Node>::iterator it1 = web.nodes.begin(); it1 != web.nodes.end(); it1++)
    {
        it1->convPos = this->save_file.webFile.convPos[i1];
        it1->wheelMode = this->save_file.webFile.wheelMode[i1];
        it1->out = this->save_file.webFile.out[i1];
        it1->out_flux = this->save_file.webFile.out_flux[i1];
        it1->out_type = this->save_file.webFile.out_type[i1];
        it1->out_index = this->save_file.webFile.out_index[i1];
        i1++;
    }
    //流程
    flow.window_confirm = this->save_file.flowFile.window_confirm;
    flow.windowGroundChoose = this->save_file.flowFile.windowGroundChoose;
    flow.groundSelected = this->save_file.flowFile.groundSelected;
    flow.chooseType = this->save_file.flowFile.chooseType;
    flow.chooseIndex = this->save_file.flowFile.chooseIndex;
    flow.onfocus = this->save_file.flowFile.onfocus;
    flow.emergency_stop = this->save_file.flowFile.emergency_stop;
    i1 = 0;
    for (std::vector<FlowAttrib>::iterator it1 = flow.flows.begin(); it1 != flow.flows.end(); it1++)
    {
        it1->flow_state = this->save_file.flowFile.flow_state[i1];
        it1->flow_time = this->save_file.flowFile.flow_time[i1];
        it1->scene_ready = this->save_file.flowFile.scene_ready[i1];
        i1++;
    }
    //皮带
    i1 = 0;
    for (std::vector<ConvAttrib>::iterator it1 = conv.belts.begin(); it1 != conv.belts.end(); it1++)
    {
        it1->conv_state = this->save_file.convFile.conv_state[i1];
        it1->conv_cur_storage = this->save_file.convFile.conv_cur_storage[i1];
        it1->flow_name = this->save_file.convFile.flow_name[i1];
        it1->flow_idx = this->save_file.convFile.flow_idx[i1];
        it1->conv_type = this->save_file.convFile.conv_type[i1];
        it1->conv_index = this->save_file.convFile.conv_index[i1];
        i1++;
    }
    for (int i = 0; i < 459; i++)
    {
        conv.conv_coords[i] = this->save_file.convFile.conv_coords[i];
    }
    conv.act_flows.clear();
    for (std::vector<FlowState>::const_iterator it1 = this->save_file.act_flows.begin(); it1 != this->save_file.act_flows.end(); it1++)
    {
        conv.flow_state.flow_num = it1->flow_num;
        conv.flow_state.begin = it1->begin;
        conv.act_flows.push_back(conv.flow_state);
    }
    //斗轮机
    i1 = 0;
    for (std::vector<WheelAttrib>::iterator it1 = wheel.wheels.begin(); it1 != wheel.wheels.end(); it1++)
    {
        it1->wheel_state = this->save_file.wheelFile.wheel_state[i1];
        it1->wheel_mode = this->save_file.wheelFile.wheel_mode[i1];
        i1++;
    }
    for (int i = 0; i < 56; i++)
    {
        wheel.wheel_coords[i] = this->save_file.wheelFile.wheel_coords[i];
    }
    //泊位
    berth.loader.loader_state = this->save_file.berthFile.loader_state;
    i1 = 0;
    for (std::vector<ShipUnloader>::iterator it1 = berth.unloaders.begin(); it1 != berth.unloaders.end(); it1++)
    {
        it1->unloader_state = this->save_file.berthFile.unloader_state[i1];
        it1->crab_bucket_hor = this->save_file.berthFile.crab_bucket_hor[i1];
        it1->crab_bucket_ver = this->save_file.berthFile.crab_bucket_ver[i1];
        it1->crab_bucket_switch = this->save_file.berthFile.crab_bucket_switch[i1];
        it1->crab_bucket_loaded = this->save_file.berthFile.crab_bucket_loaded[i1];
        i1++;
    }
    berth.berth_finished = this->save_file.berthFile.berth_finished;
    for (int i = 0; i < 4; i++)
    {
        berth.webUnloaders[i] = this->save_file.berthFile.webUnloaders[i];
    }
    for (int i = 0; i < 34; i++)
    {
        berth.info_ships_occupied[i] = this->save_file.berthFile.info_ships_occupied[i];
    }
    for (int i = 0; i < 5; i++)
    {
        berth.berth_occupied[i] = this->save_file.berthFile.berth_occupied[i];
    }
    berth.unload_mode_gen = this->save_file.berthFile.unload_mode_gen;
    for (int i = 0; i < 40; i++)
    {
        berth.unloader_coords[i] = this->save_file.berthFile.unloader_coords[i];
    }
    for (int i = 0; i < 40; i++)
    {
        berth.bucket_coords[i] = this->save_file.berthFile.bucket_coords[i];
    }
    for (int i = 0; i < 5; i++)
    {
        berth.loader_coords[i] = this->save_file.berthFile.loader_coords[i];
    }
    berth.ships.clear();
    for (std::vector<ShipFile>::const_iterator it1 = this->save_file.ships.begin(); it1 != this->save_file.ships.end(); it1++)
    {
        berth.ship.ship_name = it1->ship_name;
        berth.ship.ship_state = it1->ship_state;
        berth.ship.wait_pos[0] = it1->wait_pos[0];
        berth.ship.wait_pos[1] = it1->wait_pos[1];
        berth.ship.angle = it1->angle;
        berth.ship.ship_total_storage = it1->ship_total_storage;
        berth.ship.ship_current_storage = it1->ship_current_storage;
        berth.ship.ship_index = it1->ship_index;
        berth.ship.ship_type = it1->ship_type;
        for (int i = 0; i < 8; i++)
        {
            berth.ship.ship_coords[i] = it1->ship_coords[i];
        }
        berth.ship.ship_dist = it1->ship_dist;
        berth.ships.push_back(berth.ship);
    }
    //火车
    train.building_A.building_state = this->save_file.trainFile.building_state[0];
    train.building_B.building_state = this->save_file.trainFile.building_state[1];
    train.building_A.building_mode = this->save_file.trainFile.building_mode[0];
    train.building_B.building_mode = this->save_file.trainFile.building_mode[1];
    for (int i = 0; i < 6; i++)
    {
        train.building_coords_A[i] = this->save_file.trainFile.building_coords_A[i];
        train.building_coords_B[i] = this->save_file.trainFile.building_coords_B[i];
    }
    train.trains.clear();
    for (std::vector<TrainAttrib>::const_iterator it1 = this->save_file.trains.begin(); it1 != this->save_file.trains.end(); it1++)
    {
        train.train.train_idx = it1->train_idx;
        train.train.speed_train = it1->speed_train;
        train.train.train_type = it1->train_type;
        train.train.train_index = it1->train_index;
        train.train.train_lane = it1->train_lane;
        train.train.train_state = it1->train_state;
        train.train.current_num = it1->current_num;
        train.train.first_pos = it1->first_pos;
        train.train.max_per_wagon = it1->max_per_wagon;
        for (int i = 0; i < 7; i++)
        {
            train.train.train_coords[i] = it1->train_coords[i];
        }
        train.train.total_num = it1->total_num;
        train.train.wagons.clear();
        for (std::vector<One_wagon>::const_iterator it2 = it1->wagons.begin(); it2 != it1->wagons.end(); it2++)
        {
            train.wagon.wagon_idx = it2->wagon_idx;
            train.wagon.amount = it2->amount;
            train.wagon.wagon_type = it2->wagon_type;
            train.wagon.wagon_index = it2->wagon_index;
            train.train.wagons.push_back(this->wagon);
        }
        train.trains.push_back(train.train);
    }
    //堆场
    yard.yard_choosed = this->save_file.yardFile.yard_choosed;
    yard.child_choosed = this->save_file.yardFile.child_choosed;
    yard.type_choosed = this->save_file.yardFile.type_choosed;
    yard.index_choosed = this->save_file.yardFile.index_choosed;
    yard.amount_choosed = this->save_file.yardFile.amount_choosed;
    yard.data_send = this->save_file.yardFile.data_send;
    yard.yard_choosing = this->save_file.yardFile.yard_choosing;
    yard.flow_yard_choosed_1 = this->save_file.yardFile.flow_yard_choosed_1;
    yard.flow_yard_choosed_2 = this->save_file.yardFile.flow_yard_choosed_2;
    yard.flow_num_choose = this->save_file.yardFile.flow_num_choose;
    i1 = 0;
    std::vector<Yard_child>::const_iterator it3 = this->save_file.yards.begin();
    for (std::vector<Yard_father>::iterator it1 = yard.yards.begin(); it1 != yard.yards.end(); it1++)
    {
        it1->yard_focus = this->save_file.yardFile.yard_focus[i1];
        for (std::vector<Yard_child>::iterator it2 = it1->children.begin(); it2 != it1->children.end(); it2++)
        {
            it2->child_name = it3->child_name;
            it2->child_max_amount = it3->child_max_amount;
            it2->child_cur_amount = it3->child_cur_amount;
            it2->child_flux_left = it3->child_flux_left;
            it2->child_flux_right = it3->child_flux_right;
            it2->child_focus = it3->child_focus;
            it2->child_type = it3->child_type;
            it2->child_index = it3->child_index;
            it2->child_state_left = it3->child_state_left;
            it2->child_state_right = it3->child_state_right;
            it3++;
        }
        i1++;
    }
    //筒仓
    silo.data_send = this->save_file.siloFile.data_send;
    silo.select_window = this->save_file.siloFile.select_window;
    silo.amount_apply = this->save_file.siloFile.amount_apply;
    silo.selected_index = this->save_file.siloFile.selected_index;
    for (int i = 0; i < 14; i++)
    {
        silo.silo_occupied[i] = this->save_file.siloFile.silo_occupied[i];
    }
    silo.storage_targets = this->save_file.siloFile.storage_targets;
    silo.storage_process = this->save_file.siloFile.storage_process;
    silo.straight_state = this->save_file.siloFile.straight_state;
    silo.straight_flux = this->save_file.siloFile.straight_flux;
    silo.out_total_0 = this->save_file.siloFile.out_total_0;
    silo.out_total_1 = this->save_file.siloFile.out_total_1;
    i1 = 0;
    for (std::vector<Silo_unit>::iterator it1 = silo.silos.begin(); it1 != silo.silos.end(); it1++)
    {
        it1->cur_storage = this->save_file.siloFile.cur_storage[i1];
        it1->silo_type = this->save_file.siloFile.silo_type[i1];
        it1->silo_flux = this->save_file.siloFile.silo_flux[i1];
        it1->silo_focus = this->save_file.siloFile.silo_focus[i1];
        it1->silo_state_up = this->save_file.siloFile.silo_state_up[i1];
        it1->silo_state_down = this->save_file.siloFile.silo_state_down[i1];
        for (int i = 0; i < 8; i++)
        {
            it1->silo_coords[i] = this->save_file.siloFile.silo_coords[i][i1];
        }
        i1++;
    }
    message.push_message(u8"游戏读取成功！");
    message.push_sound("res/audio/saved.ogg", false);
}

void Record::save_energy(int mark, Message& message, SimuCore& core, Energy& energy)
{
    this->statistics.total_time = core.time;
    this->statistics.simuTime = core.simuTime;
    this->statistics.total_energy = energy.total_energy;
    int i1 = 0;
    int i2 = 0;
    for (std::vector<Transformer>::const_iterator it1 = energy.transformers.begin(); it1 != energy.transformers.end(); it1++)
    {
        this->statistics.cur_energy[i1] = it1->cur_energy;
        for (std::vector<switchGear>::const_iterator it2 = it1->gears.begin(); it2 != it1->gears.end(); it2++)
        {
            this->statistics.time[i2] = it2->time;
            i2++;
        }
        i1++;
    }
    std::string route = "EnergyRecorder_" + std::to_string(mark) + ".txt";
    std::ofstream file(route.c_str());
    file.write((char*)&this->statistics.total_time, sizeof(this->statistics.total_time));
    file.write((char*)&this->statistics.simuTime, sizeof(this->statistics.simuTime));
    file.write((char*)&this->statistics.total_energy, sizeof(this->statistics.total_energy));
    file.write((char*)&this->statistics.cur_energy, sizeof(this->statistics.cur_energy));
    file.write((char*)&this->statistics.time, sizeof(this->statistics.time));
    file.close();
    message.push_message(u8"能源数据保存成功！");
    message.push_sound("res/audio/saved.ogg", false);
}

bool Record::load_energy(int mark)
{
    std::ifstream file;
    std::string route = "EnergyRecorder_" + std::to_string(mark) + ".txt";
    file.open(route.c_str(), std::ios::in);
    if (!file.is_open())
    {
        return false;
    }
    else
    {
        file.read((char*)&this->statistics.total_time, sizeof(this->statistics.total_time));
        file.read((char*)&this->statistics.simuTime, sizeof(this->statistics.simuTime));
        file.read((char*)&this->statistics.total_energy, sizeof(this->statistics.total_energy));
        file.read((char*)&this->statistics.cur_energy, sizeof(this->statistics.cur_energy));
        file.read((char*)&this->statistics.time, sizeof(this->statistics.time));
    }
    file.close();
    this->muchStatistics.push_back(this->statistics);
    return true;
}

int Record::showGui()
{
    int ret = 0;
    static int saveMark = 1;
    ImGui::Text(u8"存档栏位选择：");
    for (int i = 1; i <= NUM_SLOTS; i++)
    {
        ImGui::SameLine();
        ImGui::RadioButton((u8"存档" + std::to_string(i)).c_str(), &saveMark, i);
    }
    this->pre_button();
    if (ImGui::Button(u8"文件存储"))
    {
        ret = saveMark;
    }
    ImGui::SameLine();
    if (ImGui::Button(u8"文件读取"))
    {
        ret = -saveMark;
    }
    this->post_button();
    return ret;
}

void Record::energyGui(Message& message, SimuCore& core, Energy& energy)
{
    static int saveSlot = 1;
    static bool readEnergy = false;
    ImGui::Text(u8"能源数据栏位选择：");
    for (int i = 1; i <= NUM_SLOTS; i++)
    {
        ImGui::SameLine();
        ImGui::RadioButton((u8"数据" + std::to_string(i)).c_str(), &saveSlot, i);
    }
    this->pre_button();
    if (ImGui::Button(u8"数据存储"))
    {
        this->save_energy(saveSlot, message, core, energy);
    }
    ImGui::SameLine();
    if (ImGui::Button(u8"显示历史能源数据"))
    {
        if (!readEnergy)
        {
            readEnergy = true;
            for (int i = 1; i <= NUM_SLOTS; i++)
            {
                this->load_energy(i);
            }
        }
    }
    this->post_button();
    if (readEnergy)
    {
        int temp = 1;
        for (std::vector<Statistics>::iterator it = this->muchStatistics.begin(); it != this->muchStatistics.end(); it++)
        {
            ImGui::Separator();
            ImGui::Text((u8"档位" + std::to_string(temp)).c_str());
            ImGui::SameLine();
            ImGui::Text((u8"    程序运行时间：" + std::to_string(it->total_time)).c_str());
            ImGui::Text((u8"仿真运行时间：" + std::to_string(it->simuTime)).c_str());
            ImGui::SameLine();
            ImGui::Text((u8"    总能耗：" + std::to_string(it->total_energy)).c_str());
            ImGui::Text(u8"主变能耗：");
            for (int i = 0; i < 8; i++)
            {
                ImGui::Text((u8"  " + std::to_string(i + 1) + u8"号变能耗：" + std::to_string(it->cur_energy[i])).c_str());
                if (i != 7 && i != 3)
                {
                    ImGui::SameLine();
                }
            }
            temp++;
        }
    }
}

void Record::pre_button()
{
    this->style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0, 255);
    this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}

void Record::post_button()
{
    this->style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
    this->style->Colors[ImGuiCol_Button] = ImColor(200, 200, 200, 255);
}
