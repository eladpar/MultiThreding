/* 046267 Computer Architecture - Spring 2020 - HW #4 */

#include "core_api.h"
#include "sim_api.h"

#include <stdio.h>
#include <iostream>
#include <list>

using namespace std;

class MT
{
private:
	/* data */
public:
	int instructions_counter;
	int cycles_counter;
	tcontext* th_regs;

	MT(int threds_num);
	~MT();
};

MT::MT(int threds_num)
{
	instructions_counter = 0;
	cycles_counter = 0;
	th_regs = new tcontext[threds_num];
}

MT::~MT()
{
	delete[] th_regs;
}

//Globals:
int threads_num = SIM_GetThreadsNum();
int load_latencey = SIM_GetLoadLat();
int store_latencey = SIM_GetStoreLat();
int switch_latencey = SIM_GetSwitchCycles();

MT blocked_mt(threads_num);
MT fine_mt(threads_num);


void CORE_BlockedMT() {
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
