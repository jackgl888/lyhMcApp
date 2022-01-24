/*-----------------------------------------------------------------------------
 * @file        : user_kalman.h
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/09/30
 * @date changed: 2021/09/30
 * @brief       : 卡尔曼滤波
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

#ifndef INC_USER_USER_KALMAN_H_
#define INC_USER_USER_KALMAN_H_

#include "math.h"

//一维卡尔曼滤波
typedef struct
{
    float x;     // 系统的状态量
    float A;     // x(n)=A*x(n-1)+u(n), u(n)~N(0,q)
    float H;     // z(n)=H*x(n)+w(n), w(n)~N(0,r)
    float Q;     // 预测过程噪声协方差
    float R;     // 测量过程噪声协方差
    float P;     // 估计误差协方差
    float kGain; // 卡尔曼增益
}KalmanStruct_T;

KalmanStruct_T tAdcVol; //掉电检测ADC


void vKalmanInit(KalmanStruct_T *pKalman, float init_q, float init_r);
float fKalmanFilter(KalmanStruct_T *pKalman, float measure);

#endif /* INC_USER_USER_KALMAN_H_ */
