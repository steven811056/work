#ifndef _CLOCK_h
#define _CLOCK_h

#include "arduino.h"

class ClockClass
{

protected:
	uint32_t time_micros = 0;
	int32_t time = 0;

public:

	/*
	* update time
	* 更新時鐘時間
	*/
	void update();

	/*
	* calculate pass time
	* 計算經過時間
	* @param [in] last last time point 上一個時間點
	* @retval long time passed 時間差
	*/
	long pass(int32_t last);

	/*
	* pass through or not
	* 是否經過時間
	* @param [in,out] last last time point 上一個時間點
	* @param [in] thr target time duration 經過的時間
	* @retval bool  time pass through or not，if true then last += thr  是否經過時間，若有則 last += thr
	*/
	bool pass(int32_t& last, int32_t thr);

	/*
	* pass through times count
	* 經過時間次數計算
	* @param [in,out] last last time point，after this function last += result * uint 上一個時間點，輸出 last += result * uint 
	* @param [in] thr_uint every time duration 經過的時間長
	* @retval uint16_t pass through times 經過該時間的次數
	*/
	uint16_t pass_t(int32_t& last, int32_t thr_uint);

	/*
	* get current time
	* 取得現在時間
	*/
	long get();
};

extern ClockClass Clock;

#endif

