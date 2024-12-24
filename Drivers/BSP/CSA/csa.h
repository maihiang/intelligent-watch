#ifndef _CSA_H_
#define _CSA_H_
//#include "stm32f10x.h"
#include "math.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "arm_common_tables.h"
//#include "stdlib.h"
#define M_PI 3.14159
#define C 3e8//光速
#define Theta (0 / 180.0 * M_PI)  // 斜视角Convert degree to radians
#define Rc 4.17// 场景中心斜距
#define R0 (Rc * cos(Theta))// 场景中心最近距离
#define lambda 0.03// 发射信号波长
#define fc (C / lambda)// 发射信号中心频率
#define Tr 2e-6// 脉冲持续时间
#define Br 60e6// 带宽
#define Kr (Br / Tr)// 距离调频率
#define V 5 // 平台飞行速度
#define Dbunch 5 // 方位向天线长度
#define Beta (lambda / Dbunch) // 波束宽度
#define Lsar (lambda * Rc / Dbunch)// 正侧视合成孔径长度
#define Lsar_squint (R0 * (tan(Theta + Beta / 2) - tan(Theta - Beta / 2)))// 正侧视合成孔径长度（斜视）
#define Tsar (Lsar_squint / V)// 合成孔径时间

extern float32_t echo_use[16][512];
typedef struct 
{
    float32_t scene_center[2]; // 场景中心坐标 (X, Y)
    float32_t Ymin,Ymax,Xmin,Xmax;//X,Y方向范围
    float32_t targets[3][3];//目标坐标
} SceneParams;
SceneParams Get_scene(float32_t range_width, float32_t azimuth_width );
typedef struct {
    float32_t ta_start;      // 计算开始时间
    float32_t ta_end;        // 计算结束时间
    float32_t Ka;            // 加速度常数
    float32_t Ba;            // 带宽常数
    uint16_t PRF;           // 脉冲重复频率
    uint16_t Na;            // 目标数量（以 2 的幂为单位）
              // Doppler 频率
    float32_t Tslow[16];        // 慢时间（Tslow）
    //float32_t distance_azimuth[4]; // 目标距离-方位关系
}RadarParams;//慢时间参数
RadarParams Get_radar_params(float32_t Xmin, float32_t Xmax, float32_t Ymin, float32_t Ymax, float32_t Tsar_in) ;

typedef struct {
    float32_t Rnear;          // 最近距离
    float32_t Rfar;           // 最远距离
    //float32_t PRFmin;         // 最小脉冲重复频率
    //float32_t RFmax;          // 最大频率
    float32_t Rmid;           // 中心距离
    uint32_t Fr;             // 频率
    uint16_t Nr;             // 样本数量
    float32_t Tfast[256];         // 快时间数组

} RadarRangeParams;//快时间参数
RadarRangeParams Get_radar_range_params(float32_t Ymin, float32_t Ymax, float32_t Tr_in);
void fty(float32_t signal_x[][512],uint8_t fftflag);//距离向傅里叶变换
void ftx(float32_t   signal_x[][512],uint8_t fftflag);//方位向傅里叶变换

void H_Phase(float32_t signal_x[][512],float32_t* fazimuth,float32_t* frange,float32_t* D,float32_t* R);//三次相位补偿
void CS_process(float32_t signal_x[][512],float32_t* Tfast,float32_t* CSf,float32_t* Km_in);

void Km_process(float32_t* Km_in,float32_t* D,float32_t* f_azimuth);//新的调频率获取
void Rangecomp_process(float32_t signal_x[][512],float32_t* frange,float32_t* Km_in, float32_t* CS_F);
void azimuthcomp_process(float32_t signal_x[][512],float32_t* R,float32_t* Cs_f,float32_t* f_azimuth,float32_t* D_in,float32_t* Km_in);

void process_CSA1(void);

extern void fftshift(void);
extern void echo_init(void);
#endif
