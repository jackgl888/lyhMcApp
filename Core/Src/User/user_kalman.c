/*-----------------------------------------------------------------------------
 * @file        : user_kalman.c
 * @author      : Lyric
 * @version     : V1.0
 * @MCU			: STM32H743IIT6
 * @Created on  : 2021/09/30
 * @date changed: 2021/09/30
 * @brief       : 卡尔曼滤波
 * @Copyright(C): 广东利元亨智能装备股份有限公司
 *----------------------------------------------------------------------------*/

#include <user_kalman.h>


/*-----------------------------------------------------------------------------
 * @name  : vKalmanInit
 * @brief : 卡尔曼滤波器初始化
 * @param : pkalman：卡尔曼滤波器结构体
 *          init_q：预测（过程）噪声方差 实验发现修改这个值会影响收敛速率
 *          init_r：测量（观测）噪声方差
 * @retval:
 * @date  : 2021/04/14
 * @note  : Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
            R:测量噪声，R增大，动态响应变慢，收敛稳定性变好
            R值固定，Q值越大，代表越信任测量值，Q值无穷大，代表只用测量值。Q值越小，代表越信任模型预测值，Q值为0，代表只用模型预测值。
 * ---------------------------------------------------------------------------*/
void vKalmanInit(KalmanStruct_T *pKalman, float init_q, float init_r)
{
	pKalman->x = 0.0; //待测量的初始值，如有中值一般设成中值
    pKalman->P = 1.0; //后验状态估计值误差的方差的初始值
    pKalman->A = 1.0;
    pKalman->H = 1.0;
    pKalman->kGain = 0;

    //这里两个参数是最关键的，Q 控制误差、  R 控制响应速度
    pKalman->Q = init_q; //预测（过程）噪声方差
    pKalman->R = init_r; //测量（观测）噪声方差
}

/*-----------------------------------------------------------------------------
 * @name  : fKalmanFilter
 * @brief : 一维卡尔曼滤波器
 * @param : pkalman：卡尔曼滤波器结构体
 *          measure：测量值
 * @retval: 返回滤波后的值
 * @date  : 2021/04/14
 * @note  :
 * ---------------------------------------------------------------------------*/
float fKalmanFilter(KalmanStruct_T *pKalman, float measure)
{
    /* Predict */
	pKalman->x = pKalman->A * pKalman->x;
	pKalman->P = (pKalman->A * pKalman->A * pKalman->P) + pKalman->Q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

    /* Measurement */
    pKalman->kGain = (pKalman->P * pKalman->H) / ((pKalman->P * pKalman->H * pKalman->H) + pKalman->R);	/* 计算卡尔曼增益 */
    pKalman->x = pKalman->x + pKalman->kGain * (measure - pKalman->H * pKalman->x); /* 计算本次滤波估计值 */
    pKalman->P = (1 - pKalman->kGain * pKalman->H) * pKalman->P; /* 更新测量方差 */

    return pKalman->x;
}
