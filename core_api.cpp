/* 046267 Computer Architecture - Spring 2020 - HW #4 */

#include "core_api.h"
#include "sim_api.h"

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

typedef enum {
	READY,
	WAIT,
	FINISHED
} STATUS;

//Globals:
int threads_num = SIM_GetThreadsNum();
int load_latencey = SIM_GetLoadLat();
int store_latencey = SIM_GetStoreLat();
int switch_latencey = SIM_GetSwitchCycles();


class thread_data {
	public:
		int clocks_to_wait;
		tcontext context;
		bool finished;
		int instruction_id;
		STATUS status;

		thread_data();
		~thread_data(){};
};

thread_data::thread_data(){
	clocks_to_wait = 0;
	finished = false;
	for(int i = 0; i<REGS_COUNT; i++){
		context.reg[i] = 0;  
	}
	instruction_id = 0;
	status = READY;
}


class MT
{

public:
	vector<thread_data> threads_array;
	int instructions_counter;
	int cycles_counter;
	MT(int threds_num);
	~MT(){};
};

MT::MT(int threds_num)
{
	for(int i = 0; i < threads_num; i++){
		threads_array.push_back(thread_data());
	}
	instructions_counter = 0;
	cycles_counter = 0;
}


MT blocked_mt(threads_num);
MT fine_mt(threads_num);


void CORE_BlockedMT() {
	int next_th_id;//??
	int unfinished_threads = threads_num;
	Instruction curr_inst;
	int curr_thread = 0;

	while(unfinished_threads > 0)
	{
		if(blocked_mt.threads_array[curr_thread].status == READY){
			SIM_MemInstRead(blocked_mt.threads_array[curr_thread].instruction_id, &curr_inst, curr_thread);
			blocked_mt.threads_array[curr_thread].instruction_id++;
			blocked_mt.instructions_counter++;
		}
		else{

		}



		switch (curr_inst.opcode)
		{
		case CMD_NOP:
			break;
		case CMD_ADD:
			blocked_mt.threads_array[curr_thread].context.reg[curr_inst.dst_index] = 
				blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src1_index] +
				blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src2_index_imm];
			break;
		case CMD_SUB:
			blocked_mt.threads_array[curr_thread].context.reg[curr_inst.dst_index] = 
				blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src1_index] -
				blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src2_index_imm];
			break;
		case CMD_ADDI:
			blocked_mt.threads_array[curr_thread].context.reg[curr_inst.dst_index] = 
				blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src1_index] +
				curr_inst.src2_index_imm;
			break;
		case CMD_SUBI:
			blocked_mt.threads_array[curr_thread].context.reg[curr_inst.dst_index] = 
				blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src1_index] -
				curr_inst.src2_index_imm;
			break;
		case CMD_LOAD:
			int addr = blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src1_index];
			if(curr_inst.isSrc2Imm){
				addr += curr_inst.src2_index_imm;
			} 
			else{
				addr += blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src2_index_imm];
			}
			int32_t dst = (int32_t) blocked_mt.threads_array[curr_thread].context.reg[curr_inst.dst_index];
			SIM_MemDataRead((uint32_t) addr, &dst);
			blocked_mt.threads_array[curr_thread].context.reg[curr_inst.dst_index] = (int) dst;
			blocked_mt.threads_array[curr_thread].status = WAIT;
			blocked_mt.threads_array[curr_thread].clocks_to_wait = load_latencey;
			break;
		case CMD_STORE:
			int dst = blocked_mt.threads_array[curr_thread].context.reg[curr_inst.dst_index];
			if(curr_inst.isSrc2Imm){
				dst += curr_inst.src2_index_imm;
			} 
			else{
				dst += blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src2_index_imm];
			}
			SIM_MemDataWrite((uint32_t) dst, (int32_t) blocked_mt.threads_array[curr_thread].context.reg[curr_inst.src1_index]);
			blocked_mt.threads_array[curr_thread].status = WAIT;
			blocked_mt.threads_array[curr_thread].clocks_to_wait = store_latencey;
			break;
		case CMD_HALT:
				blocked_mt.threads_array[curr_thread].status = FINISHED;
				unfinished_threads--;
			break;

		default:
			cout<< "DEBUG ERROR - DEFUALT"<<endl;
			break;
		}










		for (int i = 0; i < th_num; i++) {
			if (blocked_mt.threads_array[curr_thread].status == WAIT) {
				blocked_mt.threads_array[curr_thread].clocks_to_wait-= switch_latencey;
				if (B_th_data_array[i]._clocks_to_wait <= 0) 
					queue_data->_queue[i] = READY;
			}
		}



		blocked_mt.cycles_counter++;



		curr_thread = next_thred();
	}
	
}

void CORE_FinegrainedMT() {
}

double CORE_BlockedMT_CPI(){
	double cpi = blocked_mt.cycles_counter/blocked_mt.instructions_counter;
	return cpi;
}

double CORE_FinegrainedMT_CPI(){
	double cpi = fine_mt.cycles_counter/fine_mt.instructions_counter;
	return cpi;
}

void CORE_BlockedMT_CTX(tcontext* context, int threadid) {
	if(context==NULL) return;
	for(int i=0; i<REGS_COUNT; i++){
		context->reg[i] = blocked_mt.th_regs[threadid]->reg[i];
	}	return;
}

void CORE_FinegrainedMT_CTX(tcontext* context, int threadid) {
	if(context==NULL) return;
	for(int i=0; i<REGS_COUNT; i++){
		context->reg[i] = fine_mt.th_regs[threadid]->reg[i];
	}
	return;
}
