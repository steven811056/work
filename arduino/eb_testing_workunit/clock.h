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
	* ��s�����ɶ�
	*/
	void update();

	/*
	* calculate pass time
	* �p��g�L�ɶ�
	* @param [in] last last time point �W�@�Ӯɶ��I
	* @retval long time passed �ɶ��t
	*/
	long pass(int32_t last);

	/*
	* pass through or not
	* �O�_�g�L�ɶ�
	* @param [in,out] last last time point �W�@�Ӯɶ��I
	* @param [in] thr target time duration �g�L���ɶ�
	* @retval bool  time pass through or not�Aif true then last += thr  �O�_�g�L�ɶ��A�Y���h last += thr
	*/
	bool pass(int32_t& last, int32_t thr);

	/*
	* pass through times count
	* �g�L�ɶ����ƭp��
	* @param [in,out] last last time point�Aafter this function last += result * uint �W�@�Ӯɶ��I�A��X last += result * uint 
	* @param [in] thr_uint every time duration �g�L���ɶ���
	* @retval uint16_t pass through times �g�L�Ӯɶ�������
	*/
	uint16_t pass_t(int32_t& last, int32_t thr_uint);

	/*
	* get current time
	* ���o�{�b�ɶ�
	*/
	long get();
};

extern ClockClass Clock;

#endif

