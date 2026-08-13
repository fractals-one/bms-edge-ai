/**
  ******************************************************************************
  * @file    batterystate.c
  * @author  System Reserach and Application
  * @date    2023-12-01
  * @version 1.0.0
  * @brief   This file contains the definitions of constants and 
  *          variables related to the characterization of a battery cell.
  *          The adopted model is a second order RC model. The value of the 
  *          resistors and the capacitors depends on both the state of charge
  *          and the temperature.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "cellcharact.h"

/** @addtogroup BSE_CELLCHAR
  * @{
  */

/** @addtogroup BSE_CELLCHAR_Model_Conf
  * @{
  */
/**
  * @brief An array of SOC (state of charge) values for the battery cell.
  * This array contains the SOC values for the battery cell at different 
  * characterization points. The array has BSE_NSOC_CHARACT elements, each 
  * representing the SOC value as a number in [0, 1].
  */
const float BSE_soc_values_charact[BSE_NSOC_CHARACT] = {0, 0.03, 0.06, 0.09, 0.12, 0.15, 0.25, 0.35, 0.45, 0.55, 0.65, 0.75, 0.85, 0.88, 0.91, 0.94, 1.0};


/**
 * @brief An array of temperature values for the battery cell.
 * This array contains the temperature values for the battery cell at different 
 * characterization points. The array has BSE_NTEMPERATURE_CHARACT elements, 
 * each representing the temperature value in Celsius.
 */
const float BSE_temperature_values_charact[BSE_NTEMPERATURE_CHARACT] = {0.0, 10.0, 20.0, 30.0, 40.0};


/**
 * @brief An array of OCV (open circuit voltage) values for the battery cell.
 *
 * This array contains the characteristic values of the battery's OCV at 
 * different states of charge (SOC) and temperatures. The array has 
 * BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT elements, each representing the 
 * OCV value as a floating-point number.
 *
 * @note The values in the array are listed in a specific order. The first 
 * BSE_NTEMPERATURE_CHARACT values correspond to the OCV values at 
 * the lowest SOC value and the first temperature value, the next BSE_NTEMPERATURE_CHARACT 
 * values correspond to the OCV values at the second-lowest SOC value 
 * and the first temperature value, and so on. The pattern repeats for each temperature 
 * value until all SOC values have been covered.
 */
const float BSE_ocv_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT] = {3.29015000, 3.29885000, 3.31065000, 3.32687500, 3.35526250, 3.40635000, 3.51208750, 3.59210000, 3.65407500, 3.74373750, 3.84083750, 3.93142500, 4.04278750, 4.06377500, 4.07447500, 4.08467500, 4.13116569, 

3.22193750, 3.23416250, 3.25173750, 3.28116250, 3.35905000, 3.41811250, 3.52443750, 3.60407500, 3.66347500, 3.75385000, 3.85458750, 3.95032500, 4.05255000, 4.07001250, 4.07950000, 4.09096250, 4.15371394, 

3.16157500, 3.18280000, 3.21891250, 3.30371250, 3.37597500, 3.42250000, 3.52511250, 3.60852500, 3.66631250, 3.75607500, 3.86671250, 3.95292500, 4.05438750, 4.07207500, 4.08172500, 4.09293750, 4.15654666, 

3.10080000, 3.15134286, 3.23547143, 3.31615714, 3.38235714, 3.42518571, 3.52580000, 3.61195714, 3.66971429, 3.75961429, 3.87467143, 3.95462857, 4.05518571, 4.07312857, 4.08247143, 4.09411429, 4.15592275, 

3.07348750, 3.16695000, 3.25020000, 3.32608750, 3.38491250, 3.42762500, 3.52565000, 3.61333750, 3.67255000, 3.75997500, 3.87443750, 3.95323750, 4.05366250, 4.07270000, 4.08267500, 4.09488750, 4.15695963};

/**
 * @brief An array of OCV (open circuit voltage) derivative values for the battery cell.
 *
 * This array contains the characteristic values of the battery's OCV derivative at 
 * different states of charge (SOC) and temperatures. The array has 
 * BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT elements, each representing the 
 * OCV derivative value as a floating-point number.
 *
 * @note The values in the array are listed in a specific order. The first 
 * BSE_NTEMPERATURE_CHARACT values correspond to the OCV derivative values at 
 * the lowest SOC value and the first temperature value, the next BSE_NTEMPERATURE_CHARACT 
 * values correspond to the OCV derivative values at the second-lowest SOC value 
 * and the first temperature value, and so on. The pattern repeats for each temperature 
 * value until all SOC values have been covered.
 */
const float BSE_ocv_der_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT] = {0.27622929, 0.52966619, 0.71908360, 0.85393786, 0.94304197, 0.99456561, 0.98671315, 0.86603357, 0.78023513, 0.79760421, 0.90700531, 1.01788104, 0.96025212, 0.87489745, 0.74434562, 0.55982563, 0.27622929, 

1.15053919, 1.23380885, 1.27889861, 1.29251257, 1.28086459, 1.24967823, 1.06813407, 0.87753691, 0.77184108, 0.78447286, 0.88833049, 0.99578413, 0.95867589, 0.88973886, 0.78272585, 0.63044979, 0.15890885, 

2.33569537, 2.09273997, 1.87357553, 1.67749767, 1.50373784, 1.35146332, 0.98705675, 0.81244096, 0.78132474, 0.83949412, 0.92481231, 0.96721975, 0.88873407, 0.82897867, 0.74820701, 0.64389644, 0.35424706, 

3.42030909, 2.85432964, 2.37667334, 1.97865965, 1.65201311, 1.38886332, 0.87746771, 0.74844790, 0.80782478, 0.91162803, 0.96589614, 0.92667636, 0.80002475, 0.75205339, 0.70330911, 0.65658836, 0.58242875, 

3.83070858, 3.12620216, 2.53938647, 2.05780951, 1.66965321, 1.36373347, 0.80722108, 0.71328621, 0.82032184, 0.94498716, 0.98220765, 0.90517500, 0.76534713, 0.72849983, 0.70201810, 0.69141201, 0.74303688};

/**
 * @brief An array of R0 values for the battery cell.
 *
 * This array contains the characteristic values of the cell's R0 resistance
 * at different states of charge (SOC) and temperatures. The array has 
 * BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT elements, each representing the 
 * R0 value as a floating-point number.
 *
 * @note The values in the array are listed in a specific order. The first 
 * BSE_NTEMPERATURE_CHARACT values correspond to the R0 values at 
 * the lowest SOC value and the first temperature value, the next BSE_NTEMPERATURE_CHARACT 
 * values correspond to the R0 values at the second-lowest SOC value 
 * and the first temperature value, and so on. The pattern repeats for each temperature 
 * value until all SOC values have been covered.
 */
const float BSE_r0_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT] = {0.08871970, 0.08836742, 0.08833333, 0.08773485, 0.08534091, 0.07955682, 0.07703788, 0.07456439, 0.07306818, 0.07261364, 0.07288258, 0.07295833, 0.08097727, 0.08122727, 0.08282576, 0.08448864, 0.08482955, 

0.07986364, 0.07877273, 0.07871212, 0.07725379, 0.07689015, 0.07323864, 0.06900758, 0.06734470, 0.06673106, 0.06652652, 0.06639394, 0.06656061, 0.07087121, 0.07170076, 0.07244318, 0.07396970, 0.07579545, 

0.07015152, 0.07018561, 0.07003409, 0.06953030, 0.06741288, 0.06363636, 0.06020833, 0.05928788, 0.05910227, 0.05904545, 0.05895833, 0.05914015, 0.06150379, 0.06203030, 0.06278788, 0.06379167, 0.06531439, 

0.06736364, 0.06675325, 0.06548918, 0.06332035, 0.06156710, 0.05917316, 0.05796537, 0.05765801, 0.05728571, 0.05720779, 0.05701299, 0.05706494, 0.05820779, 0.05849351, 0.05895238, 0.05969264, 0.06107359, 

0.07131818, 0.06884848, 0.06684848, 0.06485985, 0.06339015, 0.06228788, 0.06084470, 0.06002652, 0.05949621, 0.05937121, 0.05913636, 0.05907197, 0.05973864, 0.06010985, 0.06004167, 0.06034848, 0.06134848};

/**
 * @brief An array of R1 values for the battery cell.
 *
 * This array contains the characteristic values of the cell's R1 resistance
 * at different states of charge (SOC) and temperatures. The array has 
 * BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT elements, each representing the 
 * R1 value as a floating-point number.
 *
 * @note The values in the array are listed in a specific order. The first 
 * BSE_NTEMPERATURE_CHARACT values correspond to the R1 values at 
 * the lowest SOC value and the first temperature value, the next BSE_NTEMPERATURE_CHARACT 
 * values correspond to the R1 values at the second-lowest SOC value 
 * and the first temperature value, and so on. The pattern repeats for each temperature 
 * value until all SOC values have been covered.
 */
const float BSE_r1_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT] = {0.12071515, 0.12310909, 0.12602727, 0.13039394, 0.13913333, 0.08492424, 0.04526970, 0.04184242, 0.03683030, 0.03456667, 0.03258485, 0.03306970, 0.03391212, 0.03591212, 0.03606970, 0.03756667, 0.04207576, 

0.11124848, 0.11511212, 0.11938485, 0.12581212, 0.06623333, 0.04121515, 0.03139091, 0.03193333, 0.02741515, 0.02517273, 0.02566970, 0.02720114, 0.02493636, 0.02475758, 0.02395758, 0.02362424, 0.02549091, 

0.10437576, 0.10950909, 0.10147879, 0.05315455, 0.03623333, 0.02891515, 0.02457576, 0.02749394, 0.02270303, 0.02002121, 0.02243129, 0.02303333, 0.02026364, 0.01976667, 0.01823333, 0.01708485, 0.01765758, 

0.09177143, 0.06763290, 0.04060260, 0.02917403, 0.02525022, 0.02239307, 0.01965368, 0.02368485, 0.01867359, 0.01658182, 0.01978627, 0.01936277, 0.01671688, 0.01605541, 0.01419221, 0.01283810, 0.01290736, 

0.03267955, 0.02194508, 0.01677538, 0.01451742, 0.01364470, 0.01248864, 0.01095379, 0.01341288, 0.01079129, 0.00935985, 0.01246402, 0.01102500, 0.00935455, 0.00879356, 0.00759015, 0.00664015, 0.00661591};

/**
 * @brief An array of R2 values for the battery cell.
 *
 * This array contains the characteristic values of the cell's R2 resistance
 * at different states of charge (SOC) and temperatures. The array has 
 * BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT elements, each representing the 
 * R2 value as a floating-point number.
 *
 * @note The values in the array are listed in a specific order. The first 
 * BSE_NTEMPERATURE_CHARACT values correspond to the R2 values at 
 * the lowest SOC value and the first temperature value, the next BSE_NTEMPERATURE_CHARACT 
 * values correspond to the R2 values at the second-lowest SOC value 
 * and the first temperature value, and so on. The pattern repeats for each temperature 
 * value until all SOC values have been covered.
 */
const float BSE_r2_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT] = {0.03017881, 0.03077730, 0.03150684, 0.03259851, 0.03478336, 0.02123108, 0.01309992, 0.01211159, 0.01066075, 0.00972636, 0.00858440, 0.00957638, 0.00919608, 0.00897804, 0.00901743, 0.00939167, 0.01051895, 

0.02781212, 0.02877803, 0.02984621, 0.03145303, 0.01655833, 0.01030379, 0.00938338, 0.00956430, 0.00745428, 0.00654632, 0.00763843, 0.00824963, 0.00626299, 0.00618939, 0.00598939, 0.00590606, 0.00637273, 

0.02609394, 0.02737727, 0.02536970, 0.01328864, 0.00905833, 0.00722879, 0.00668925, 0.00822042, 0.00573040, 0.00505349, 0.00855478, 0.00689868, 0.00506591, 0.00494167, 0.00455833, 0.00427121, 0.00441439, 

0.02294286, 0.01690823, 0.01042377, 0.00754439, 0.00646061, 0.00559827, 0.00494442, 0.00605152, 0.00466840, 0.00415529, 0.00834149, 0.00532605, 0.00417922, 0.00401385, 0.00354805, 0.00320952, 0.00322684, 

0.02959318, 0.01986932, 0.01514508, 0.01310379, 0.01228712, 0.01123864, 0.00988712, 0.01208712, 0.00972008, 0.00844318, 0.01143192, 0.00994470, 0.00843333, 0.00791477, 0.00683409, 0.00597348, 0.00596742};

/**
 * @brief An array of C1 values for the battery cell.
 *
 * This array contains the characteristic values of the cell's C1 resistance
 * at different states of charge (SOC) and temperatures. The array has 
 * BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT elements, each representing the 
 * C1 value as a floating-point number.
 *
 * @note The values in the array are listed in a specific order. The first 
 * BSE_NTEMPERATURE_CHARACT values correspond to the C1 values at 
 * the lowest SOC value and the first temperature value, the next BSE_NTEMPERATURE_CHARACT 
 * values correspond to the C1 values at the second-lowest SOC value 
 * and the first temperature value, and so on. The pattern repeats for each temperature 
 * value until all SOC values have been covered.
 */
const float BSE_c1_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT] = {239.11130546, 234.37843820, 228.90495087, 221.44127859, 207.31872420, 339.89889165, 636.04472669, 687.80145769, 781.37503960, 832.36101288, 883.13168870, 869.81622349, 848.09192257, 801.05637494, 796.84284528, 766.19034726, 684.68917974, 

180.26461593, 174.19286462, 167.88156709, 159.23351740, 302.02765803, 485.45164388, 637.24995544, 626.37769754, 729.62862333, 794.60164613, 779.27029844, 735.31926073, 802.11703102, 808.01373212, 834.83539776, 846.65696228, 785.33116406, 

191.79430761, 182.78519738, 197.24463056, 376.63515982, 552.35380628, 692.07792206, 814.00656679, 727.60273046, 881.38879503, 999.21649962, 891.65024848, 868.47909597, 987.26435121, 1011.97600499, 1097.32277395, 1171.04131278, 1133.20187454, 

218.04745827, 295.83035588, 492.66230192, 685.71780253, 792.32748458, 893.69117498, 1017.82019921, 844.77154125, 1071.99587169, 1206.62317624, 1010.87405865, 1033.20957452, 1197.01488550, 1246.24227572, 1410.22535132, 1559.70674885, 1549.86376599, 

619.39050980, 922.50702698, 1209.43453639, 1397.83039184, 1490.49745804, 1629.53026470, 1852.75646673, 1515.43003346, 1884.38886327, 2169.50196498, 1627.87835099, 1841.87273194, 2171.91405744, 2314.66097132, 2680.71431534, 3067.95180984, 3070.40651198};

/**
 * @brief An array of C2 values for the battery cell.
 *
 * This array contains the characteristic values of the cell's C2 resistance
 * at different states of charge (SOC) and temperatures. The array has 
 * BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT elements, each representing the 
 * C2 value as a floating-point number.
 *
 * @note The values in the array are listed in a specific order. The first 
 * BSE_NTEMPERATURE_CHARACT values correspond to the C2 values at 
 * the lowest SOC value and the first temperature value, the next BSE_NTEMPERATURE_CHARACT 
 * values correspond to the C2 values at the second-lowest SOC value 
 * and the first temperature value, and so on. The pattern repeats for each temperature 
 * value until all SOC values have been covered.
 */
const float BSE_c2_values_charact[BSE_NSOC_CHARACT * BSE_NTEMPERATURE_CHARACT] = {3056.45852061, 2998.45916273, 2930.22439012, 2831.17720539, 2654.34583372, 4354.04115937, 67168.87335364, 72624.40431957, 82508.14060096, 82598.12560765, 70066.38402123, 91773.73445429, 75681.23234195, 10310.91851666, 10286.86934602, 9913.51145292, 8845.11727062, 

3605.29226318, 3483.85723882, 3357.63129018, 3184.67029929, 6040.55307414, 9709.03274937, 105995.72648492, 104567.63209761, 95055.71522121, 84045.75073677, 128431.62209392, 121264.01526697, 33057.43663387, 16160.27439363, 16696.70768395, 16933.13893788, 15706.62323996, 

3835.88609231, 3655.70389175, 3944.89255085, 7532.70306250, 11047.07597807, 13841.55833935, 106529.75756551, 121057.73501826, 41263.60176508, 52834.05127691, 117336.90970198, 144984.09336656, 19745.28676212, 20239.51948934, 21946.45467680, 23420.82588892, 22664.03797248, 

4360.94909493, 5916.60703470, 43325.45742620, 63759.59079132, 66038.29281296, 17873.82322732, 32820.94472167, 32317.16901164, 21439.91711642, 42265.57059093, 120134.67181847, 102467.27330008, 23940.29771535, 24924.84513465, 28204.50625674, 31194.13450174, 30997.27539037, 

3482.67379470, 5186.04912992, 6777.39029012, 7831.28932300, 8329.40045895, 9100.03213526, 10379.85360212, 8475.10920289, 10533.27977188, 12146.21093723, 21749.20975387, 10311.34705400, 12151.86344081, 12927.98445878, 14978.14899569, 17125.47295412, 17187.81334897};

/** 
  * @} BSE_CELLCHAR_Private_Const
  */

/** 
  * @} BSE_CELLCHAR
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
