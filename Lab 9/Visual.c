/*Visual.c
 *Enrique Perez-Osborne
 *Juliana Pulido
 *TA: Cody Horton
 *Last Edited: 11/14/17
 */

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "calib.h"
#include "fixed.h"


uint8_t temper[2];
//array for graphing purposes
uint8_t last100Values[100];
uint8_t arrIndex=0;


uint8_t adcValueTable[4096]=
										 //0-59
										{249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 //59-106(47)
										 40,40,39,39,39,39,39,39,39,39,
										 39,39,39,39,39,39,39,39,39,39,
										 39,39,39,39,39,39,39,39,39,39,
										 39,39,39,39,39,39,39,39,39,39,
										 39,39,39,39,39,39,39,


										 //107-155(48)
										 39,39,39,39,39,39,39,39,39,39,
										 39,39,39,39,39,39,39,39,39,39,
										 39,39,39,39,39,39,39,39,39,39,
										 39,39,39,38,38,38,38,38,38,38,
										 38,38,38,38,38,38,38,38,

										 //156-205(49)
										 38,38,38,38,38,38,38,38,38,38,
										 38,38,38,38,38,38,38,38,38,38,
										 38,38,38,38,38,38,38,38,38,38,
										 38,38,38,38,38,38,38,38,38,38,
										 38,38,38,38,38,38,38,38,38,

										 //206-256(50)
										 38,38,38,38,38,38,38,38,38,38,
										 38,38,38,38,38,38,38,38,38,38,
										 37,37,37,37,37,37,37,37,37,37,
										 37,37,37,37,37,37,37,37,37,37,
										 37,37,37,37,37,37,37,37,37,37,

										 //257-309(52)
										 37,37,37,37,37,37,37,37,37,37,
										 37,37,37,37,37,37,37,37,37,37,
										 37,37,37,37,37,37,37,37,37,37,
										 37,37,37,37,37,37,37,37,37,37,
										 37,37,37,37,37,37,37,37,37,37,
										 37,37,


										 //310-362(52)
										 37,36,36,36,36,36,36,36,36,36,
	                   36,36,36,36,36,36,36,36,36,36,
										 36,36,36,36,36,36,36,36,36,36,
	                   36,36,36,36,36,36,36,36,36,36,
	                   36,36,36,36,36,36,36,36,36,36,
										 36,36,


										 //363-416(53)
										 36,36,36,36,36,36,36,36,36,36,
	                   36,36,36,36,36,36,36,36,36,36,
										 36,36,36,36,36,36,36,36,36,36,
									   36,36,36,36,36,36,36,35,35,35,
										 35,35,35,35,35,35,35,35,35,35,
										 35,35,35,



										 //417-472(55)
										 35,35,35,35,35,35,35,35,35,35,
										 35,35,35,35,35,35,35,35,35,35,
										 35,35,35,35,35,35,35,35,35,35,
										 35,35,35,35,35,35,35,35,35,35,
										 35,35,35,35,35,35,35,35,35,35,
										 35,35,35,35,35,




										 //473-528(55)
										35,35,35,35,35,35,35,35,35,35,
										35,35,35,35,35,35,35,35,35,35,
										35,34,34,34,34,34,34,34,34,34,
										34,34,34,34,34,34,34,34,34,34,
										34,34,34,34,34,34,34,34,34,34,
										34,34,34,34,34,


										 //529-586(57)
										 34,34,34,34,34,34,34,34,34,34,
										 34,34,34,34,34,34,34,34,34,34,
										 34,34,34,34,34,34,34,34,34,34,
										 34,34,34,34,34,34,34,34,34,34,
										 34,34,34,34,34,34,34,34,34,34,
										 34,34,34,34,34,34,34,

										 //587-645(58)
										 34,33,33,33,33,33,33,33,33,33,
										 33,33,33,33,33,33,33,33,33,33,
										 33,33,33,33,33,33,33,33,33,33,
										 33,33,33,33,33,33,33,33,33,33,
										 33,33,33,33,33,33,33,33,33,33,
										 33,33,33,33,33,33,33,33,

										 //646-705(59)
										 33,33,33,33,33,33,33,33,33,33,
										 33,33,33,33,33,33,33,33,33,33,
										 33,33,33,33,33,33,33,33,33,33,
										 33,33,33,33,33,33,33,33,33,
										 32,32,32,32,32,32,32,32,32,32,
										 32,32,32,32,32,32,32,32,32,32,

										 //706-766(60)
										 32,32,32,32,32,32,32,32,32,32,
										 32,32,32,32,32,32,32,32,32,32,
										 32,32,32,32,32,32,32,32,32,32,
										 32,32,32,32,32,32,32,32,32,32,
										 32,32,32,32,32,32,32,32,32,32,
										 32,32,32,32,32,32,32,32,32,32,


										 //767-828(61)
										 32,32,32,32,32,32,32,32,32,32,
										 32,32,32,32,32,32,32,32,32,32,
										 32,31,31,31,31,31,31,31,31,31,
										 31,31,31,31,31,31,31,31,31,31,
										 31,31,31,31,31,31,31,31,31,31,
										 31,31,31,31,31,31,31,31,31,31,
									   31,


										 //829-892(63)
										 31,31,31,31,31,31,31,31,31,31,
										 31,31,31,31,31,31,31,31,31,31,
										 31,31,31,31,31,31,31,31,31,31,
										 31,31,31,31,31,31,31,31,31,31,
										 31,31,31,31,31,31,31,31,31,31,
										 31,31,31,31,31,31,31,31,31,31,
										 31,31,31,

										 //893-957(64)
										 31,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,


										 //958-1024(66)
										 30,30,30,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,
										 30,30,30,30,30,30,30,30,30,30,
										 30,30,30,30,30,30,30,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,


										 //1025-1091(66)
	                   29,29,29,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,


										 //1092-1160(68)
										 29,29,29,29,29,29,29,29,29,29,
										 29,29,29,29,29,29,29,29,29,29,
										 29,28,28,28,28,28,28,28,28,28,
	                   28,28,28,28,28,28,28,28,28,28,
										 28,28,28,28,28,28,28,28,28,28,
										 28,28,28,28,28,28,28,28,28,28,
	                   28,28,28,28,28,28,28,28,


										 //1161-1230(69)
										 28,28,28,28,28,28,28,28,28,28,
	                   28,28,28,28,28,28,28,28,28,28,
										 28,28,28,28,28,28,28,28,28,28,
										 28,28,28,28,28,28,28,28,28,28,
	                   28,28,28,28,28,28,28,28,28,28,
										 28,28,28,28,28,28,28,28,28,28,
										 28,28,28,28,28,28,28,28,28,


										 //1231-1302(71)
										 28,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,

										 //1303-1375(72)
										 27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,27,27,
										 27,27,27,27,27,27,27,27,26,26,
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,26,26,26,26,26,26,


										 //1376-1450(74)
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,


										 //1451-1526(75)
										 26,26,26,26,26,26,26,26,26,26,
										 26,26,26,26,26,26,26,26,26,26,
										 26,25,25,25,25,25,25,25,25,25,
									   25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,



										 //1527-1603(76)
										 25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,25,25,25,25,
									   25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,25,25,25,25,
										 25,25,25,25,25,25,





										 //1604-1682(78)
										 25,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,

										 //1683-1762(79)
										 24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,24,24,
										 24,24,24,24,24,24,24,24,23,23,
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,

										 //1763-1844(81)
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,
									   23,


										 //1845-1928(83)
										 23,23,23,23,23,23,23,23,23,23,
										 23,23,23,23,23,23,23,23,23,23,
										 23,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
									   22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,

										 //1929-2013(84)
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
									   22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,22,22,22,22,22,22,
										 22,22,22,22,

										 //2014-2099(85)
										 22,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,


									   //2100-2187(87)
								     21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,21,21,
										 21,21,21,21,21,21,21,21,20,20,
										 20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,20,

										 //2188-2277(89)
										 20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,20,
									   20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,



										 //2278-2369(91)
										 20,20,20,20,20,20,20,20,20,20,
										 20,20,20,20,20,20,20,20,20,20,
										 20,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,
										 //2370-2462(92)
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,19,19,19,19,19,19,19,19,
										 19,19,

										 //2463-2556(93)
										 19,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,


										 //2557-2653(96)
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,18,18,18,18,18,18,
										 18,18,18,18,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,



										 //2654-2751(97)
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,

										 //2752-2851(99)
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,17,17,17,17,17,
										 17,17,17,17,17,
										 16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,




										 //2852-2952(100)
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,
										 16,16,16,16,16,16,16,16,16,16,


										 //2953-3056(103)
										 16,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,

										 //3057-3161(104)
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,15,15,15,
										 15,15,15,15,15,15,15,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,

										 //3162-3268(106)
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,



										 //3269-3376(107)
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,14,
										 14,14,14,14,14,14,14,14,14,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,
										 //3377-3487(110)
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,
										 13,13,13,13,13,13,13,13,13,13,

										 //3488-3599(111)
										 13,13,13,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,
										 //3600-3713(113)
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,12,12,12,
										 12,12,12,12,12,12,12,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,


										 //3714-3829(115)
										 11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,


										 //3830-3946(116)
										 11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,11,11,11,11,11,11,11,11,11,
										 11,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,

										//3947-4067(120)
		                 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 10,10,10,10,10,10,10,10,10,10,
										 //4068-4096
										 250,250,250,250,250,250,250,250,250,250,
										 250,250,250,250,250,250,250,250,250,250,
										 250,250,250,250,250,250,250,250};

uint8_t adcValueDecimalTable[4068]=
										 //0-59
										 {249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 249,249,249,249,249,249,249,249,249,249,
										 //59-106
										 0,99,98,96,95,94,93,91,90,89,
										 88,86,83,82,81,80,79,76,75,74,
										 72,71,70,69,66,65,64,63,61,60,
										 59,58,57,55,54,53,52,50,49,48,
										 47,46,45,44,43,42,41,



										 //107-155
										 40,39,38,36,35,34,32,31,30,29,
										 28,27,26,24,23,22,21,20,19,17,
										 16,15,13,12,11,10,8,7,5,4,
										 3,1,0,98,96,95,93,92,91,89,
										 88,87,86,85,84,83,82,81,


										 //156-205
										 80,79,77,76,74,73,72,70,69,68,
										 65,63,62,61,60,59,58,55,54,52,
										 51,50,49,48,47,46,44,43,42,41,
										 40,39,38,37,36,35,33,32,31,30,
										 29,28,27,26,25,24,23,22,21,


										 //206-256
										 20,19,18,17,16,15,14,13,12,11,
										 9,8,7,6,5,4,3,2,1,0,
										 99,98,97,96,95,93,92,90,89,88,
										 86,85,84,83,81,80,78,77,76,73,
										 72,70,69,68,67,66,65,64,63,61,



										 //257-309
										 60,59,57,56,55,54,53,52,51,50,
										 50,49,48,46,45,44,42,41,40,39,
										 37,36,35,33,31,30,29,28,27,26,
										 24,23,22,21,19,18,17,16,15,13,
										 12,11,10,9,8,7,6,5,4,3,
										 2,1,



										 //310-362
										 0,99,98,96,95,94,93,92,91,90,
										 89,88,87,86,83,82,81,80,79,78,
										 76,75,74,72,71,70,69,68,66,65,
										 64,63,61,60,59,58,57,56,55,54,
										 53,52,50,49,48,47,46,45,44,43,
										 42,41,



										 //363-416
										 40,39,38,37,36,35,34,32,31,30,
										 29,28,27,26,25,24,23,22,21,20,
										 19,18,17,16,15,14,13,12,11,10,
										 8,7,5,4,3,1,0,98,97,96,
										 95,93,92,91,89,88,87,86,85,84,
										 83,82,81,



										 //417-472
										 80,79,77,76,75,74,73,72,71,70,
										 69,68,66,65,64,63,62,61,60,59,
										 58,57,55,54,53,52,51,50,49,48,
										 47,46,44,43,42,41,40,39,38,37,
										 36,35,33,32,31,30,29,28,27,26,
										 25,24,23,22,21,


										 //473-528
										 20,19,18,17,16,15,14,13,12,11,
										 9,8,7,6,5,4,3,2,1,0,
										 0,99,98,97,96,95,94,93,92,90,
										 89,88,86,85,84,83,82,81,80,78,
										 77,76,75,73,72,71,70,69,68,67,
										 66,65,64,63,61,



										 //529-586
										 60,59,57,56,55,54,53,52,51,50,
										 50,49,48,47,46,45,44,43,42,41,
										 40,39,37,36,35,33,32,31,30,29,
										 28,27,26,24,23,22,21,20,19,18,
										 17,16,15,14,13,12,11,10,9,8,
										 7,6,5,4,3,2,1,


										 //587-645
										 0,99,98,97,96,95,94,93,92,90,
										 89,88,87,86,85,83,82,81,80,79,
										 78,77,76,75,74,73,72,71,70,69,
										 68,67,66,65,64,63,62,61,60,59,
										 58,57,56,55,54,53,52,51,50,49,
										 48,47,46,45,44,43,42,41,

										 //646-705
										 40,39,38,37,36,35,34,33,32,31,
										 30,29,28,27,26,25,24,23,22,21,
										 20,19,18,17,16,15,14,13,13,12,
										 11,10,8,7,5,4,3,1,0,
										 98,97,96,95,94,93,92,91,90,89,
										 88,87,87,86,86,85,84,83,82,81,




										 //706-766
										 80,79,78,77,76,75,74,73,72,71,
										 70,69,68,67,66,65,64,63,62,61,
										 60,59,58,57,56,55,54,53,52,51,
										 50,49,48,47,46,45,44,43,42,41,
										 40,39,38,37,36,35,34,33,32,31,
										 30,29,28,27,26,25,24,23,22,21,



										 //767-828
										 20,19,18,17,16,15,14,13,12,11,
										 9,8,7,6,5,4,3,2,1,0,
										 0,99,99,98,97,96,95,94,93,92,
										 91,90,89,88,87,86,85,84,83,82,
										 81,80,79,78,77,76,75,74,73,72,
										 71,70,69,68,67,66,65,64,63,62,
										 61,



										 //829-892
										 60,59,58,57,56,55,54,53,52,51,
										 50,50,49,49,48,48,47,46,45,44,
										 43,42,41,40,39,38,37,36,35,34,
										 33,32,31,30,29,28,27,26,25,24,
										 23,22,21,20,19,18,17,16,15,14,
										 13,12,11,10,9,8,7,6,5,4,
										 3,2,1,


										 //893-957
										 0,99,98,97,96,95,94,93,92,91,
										 91,90,89,88,87,86,85,84,83,83,
										 82,81,80,79,78,78,77,77,76,75,
										 74,73,72,71,70,69,68,67,66,65,
										 64,63,62,61,60,59,58,57,56,55,
										 54,53,52,51,50,49,48,47,46,45,
										 44,43,42,41,


										 //958-1024
										 40,39,38,37,36,35,34,33,32,31,
										 30,29,28,27,26,25,24,23,22,21,
										 20,19,18,17,16,15,
										 14,13,13,12,11,10,9,8,8,7,6,5,
										 5,4,3,3,2,1,0,99,99,98,
										 98,97,96,95,94,93,92,91,90,89,
										 88,87,87,86,86,85,84,83,82,81,

										 //1025-1091
										 80,79,78,77,76,75,74,73,72,71,
										 70,69,69,68,67,66,65,64,63,62,
										 61,61,60,60,59,58,57,56,55,54,53,52,
										 51,50,49,48,47,46,45,44,43,42,
										 42,41,40,39,38,37,36,35,34,33,
										 32,31,30,29,28,27,26,25,24,23,
										 22,21,21,20,


										 //1092-1160
										 20,19,18,17,16,15,14,13,12,11,
										 9,8,7,6,5,4,3,2,1,0,
										 0,99,99,98,98,97,97,96,96,95,
										 95,94,94,93,93,92,92,91,90,89,
										 88,87,86,85,84,83,82,81,80,79,
										 78,77,76,75,74,73,72,71,70,69,
										 68,67,66,65,64,63,62,61,


										 //1161-1230
										 60,59,58,57,56,55,54,53,52,51,
										 50,50,49,49,48,48,47,47,46,46,
										 45,45,44,44,43,43,42,41,40,39,
										 38,37,36,35,34,33,32,31,30,29,
										 28,27,26,25,24,23,22,21,20,19,
										 18,17,16,15,14,13,12,11,10,9,
										 8,7,6,5,4,3,2,1,1,




										 //1231-1302
										 0,99,98,97,96,95,94,93,92,91,
										 91,90,90,89,89,88,88,87,87,86,
										 86,85,85,84,84,83,83,82,81,80,
										 79,78,78,77,77,76,75,74,73,72,
										 71,70,69,68,67,66,65,64,63,62,
										 61,60,59,58,57,56,55,54,53,52,
										 51,50,49,48,47,46,45,44,43,42,
										 41,


										 //1303-1375
										 40,39,
										 38,37,36,35,34,33,32,31,30,29,
										 28,27,27,26,26,25,25,24,24,23,
										 23,22,21,20,19,18,18,17,16,15,
										 14,13,13,12,11,10,9,8,8,7,6,5,
										 5,4,3,3,2,1,0,99,99,98,
										 98,97,96,95,94,93,92,91,90,89,
										 88,87,87,86,86,85,84,83,82,81,


										 //1376-1450
										 80,79,78,77,76,75,74,73,72,71,
										 70,69,69,68,67,66,66,65,65,64,
										 64,63,63,62,62,61,61,60,60,59,
										 59,58,58,57,57,56,55,54,53,52,
										 51,50,49,48,47,46,45,44,43,42,
										 42,41,40,39,38,37,36,35,34,33,
										 32,31,30,29,28,27,26,25,24,23,
										 22,21,21,20,


										 //1451-1526
										 20,19,18,17,16,15,14,13,12,11,
										 9,8,7,6,5,4,3,2,1,0,
										 0,99,99,98,98,97,97,96,96,95,
										 95,94,94,93,93,92,92,91,91,90,
										 90,89,89,88,88,87,87,86,86,85,
										 85,84,83,82,81,80,79,78,77,76,
										 75,74,73,72,71,70,69,68,67,66,
										 65,64,63,62,61,



										 //1527-1603
										 60,59,58,57,56,55,54,53,52,51,
										 50,50,49,49,48,48,47,47,46,46,
										 45,45,44,44,43,43,42,42,41,41,
										 40,40,39,39,38,38,37,37,36,36,
										 35,35,34,33,32,31,30,29,28,27,
										 26,25,24,23,22,21,20,19,18,17,
										 16,15,14,13,12,11,10,9,8,7,
										 6,5,4,3,2,1,



										 //1604-1682
										 0,99,98,97,96,95,94,93,92,91,
										 91,90,90,89,89,88,88,87,87,86,
										 86,85,85,84,84,83,83,82,81,80,
										 79,78,78,77,77,76,76,75,75,74,
										 74,73,73,72,71,70,69,68,67,66,
										 65,64,63,62,61,60,59,58,57,56,
										 55,54,53,52,51,50,49,48,47,46,
										 46,45,45,44,43,42,41,41,



										 //1683-1762
										 40,39,38,37,36,35,35,34,34,33,
										 33,32,32,31,31,30,30,29,29,28,
										 28,27,27,26,26,25,25,24,24,23,
										 23,22,21,20,19,18,18,17,16,15,
										 14,13,13,12,11,10,9,8,8,7,6,5,
										 5,4,3,3,2,1,0,99,98,
										 98,97,96,95,94,93,92,91,90,89,
										 88,87,87,86,86,85,84,83,82,81,


										 //1763-1844
										 80,79,78,77,76,75,74,73,72,71,
										 70,69,69,68,67,66,66,65,65,64,
										 64,63,63,62,62,61,61,60,60,59,
										 59,58,58,57,57,56,56,55,55,54,
										 54,53,53,52,52,51,51,50,50,49,
										 49,48,48,47,46,45,44,43,42,41,
										 40,39,38,37,36,35,34,33,32,31,
										 30,29,28,27,26,25,24,23,22,21,
										 21,

										 //1845-1928
										 20,19,18,17,16,15,14,13,12,11,
										 9,8,7,6,5,4,3,2,1,0,
										 0,99,99,98,98,97,97,96,96,95,
										 95,94,94,93,93,92,92,91,91,90,
										 90,89,89,88,88,87,87,86,86,85,
										 85,84,84,83,83,82,82,81,81,80,
										 80,79,79,78,77,76,75,74,73,72,
										 71,70,69,68,67,66,66,65,65,64,
										 63,62,61,




										 //1929-2013
										 60,59,58,57,56,55,54,53,52,51,
										 50,50,49,49,48,48,47,47,46,46,
										 45,45,44,44,43,43,42,42,41,41,
										 40,40,39,39,38,38,37,37,36,36,
										 35,35,34,34,33,33,32,32,31,31,
										 30,30,29,29,28,28,27,27,26,26,
										 25,24,23,22,21,20,19,18,17,16,
										 15,14,13,12,11,10,9,8,7,6,5,4,
										 3,3,2,1,

										 //2014-2099
										 0,99,98,97,96,95,94,93,92,91,
										 90,89,88,87,86,
										 85,84,83,82,81,80,79,78,77,76,
										 76,75,75,74,74,73,73,72,72,71,
										 71,70,70,69,69,68,68,67,67,66,
										 66,65,65,64,64,63,63,62,62,61,
										 61,60,60,59,59,58,58,57,57,56,
										 56,55,55,54,54,53,53,52,52,51,
										 50,49,48,47,46,45,44,43,42,41,

										 //2100-2187
										 40,39,38,37,36,35,35,34,34,33,
										 33,32,32,31,31,30,30,29,29,28,
										 28,27,27,26,26,25,25,24,24,23,
										 23,22,22,21,21,20,20,19,19,18,
										 18,17,17,16,16,15,15,14,14,13,
										 13,12,12,11,11,9,8,
										 8,7,6,5,4,3,2,1,0,99,
										 98,97,96,95,94,93,92,91,90,89,
										 88,87,87,86,86,85,84,83,82,81,



										 //2188-2277
										 80,79,78,77,76,75,74,73,72,71,
										 70,69,69,68,67,66,66,65,65,64,
										 64,63,63,62,62,61,61,60,60,59,
										 59,58,58,57,57,56,56,55,55,54,
										 54,53,53,52,52,51,51,50,50,49,
										 49,48,48,47,47,46,46,45,45,44,
										 44,43,43,42,42,41,41,40,40,39,
										 38,37,36,35,34,33,33,32,32,31,
										 30,29,28,27,26,25,24,23,21,



										 //2278-2369
										 20,19,18,17,16,15,14,13,12,11,
										 9,8,7,6,5,4,3,2,1,0,
										 0,99,99,98,98,97,97,96,96,95,
										 95,94,94,93,93,92,92,91,91,90,
										 90,89,89,88,88,87,87,86,86,85,
										 85,84,84,83,83,82,82,81,81,80,
										 80,79,79,78,78,77,77,76,76,75,
										 75,74,73,72,71,70,70,69,69,68,
										 68,67,67,66,66,65,65,64,63,62,
										 61,



										 //2370-2462
										 60,59,58,57,56,55,54,53,52,51,
										 50,50,49,49,48,48,47,47,46,46,
										 45,45,44,44,43,43,42,42,41,41,
										 40,40,39,39,38,38,37,37,36,36,
										 35,35,34,34,33,33,32,32,31,31,
										 30,30,29,29,28,28,27,27,26,26,
										 25,25,24,24,23,23,22,22,21,21,
										 20,20,19,18,17,16,15,14,13,12,
										 11,10,9,8,7,6,5,4,3,3,
										 2,1,

										 //2463-2556
										 0,99,98,97,96,95,94,93,92,91,
										 90,89,88,
										 87,86,85,84,84,83,83,82,82,81,
										 81,80,80,79,79,78,78,77,77,76,
										 76,75,75,74,74,73,73,72,72,71,
										 71,70,70,69,69,68,68,67,67,66,
										 66,65,65,64,64,63,63,62,62,61,
										 61,60,60,59,59,58,58,57,57,56,
										 56,55,55,54,54,53,53,52,52,51,
										 50,49,48,47,46,45,44,43,42,41,


										 //2557-2653
										 40,39,38,37,36,35,35,34,34,33,
										 33,32,32,31,31,30,30,29,29,28,
										 28,27,27,26,26,25,25,24,24,23,
										 23,22,22,21,21,20,20,19,19,18,
										 18,17,17,16,16,15,15,14,14,13,
										 13,12,12,11,11,10,10,9,9,8,
										 8,7,7,6,6,5,5,4,4,3,
										 3,2,2,1,1,0,99,98,98,97,96,95,
										 94,93,92,91,90,89,88,87,87,86,
										 86,85,84,83,82,81,

										 //2654-2751
										 80,79,78,77,76,75,74,73,72,71,
										 70,69,69,68,67,66,66,65,65,64,
										 64,63,63,62,62,61,61,60,60,59,
										 59,58,58,57,57,56,56,55,55,54,
										 54,53,53,52,52,51,51,50,50,49,
										 49,48,48,47,47,46,46,45,45,44,
										 44,43,43,42,42,41,41,40,40,39,
										 39,38,38,37,37,36,36,35,35,34,
										 34,33,33,32,32,31,31,30,30,29,
										 28,27,26,25,24,23,21,




										 //2752-2851
										 20,19,18,17,16,15,14,13,12,11,
										 10,10,9,9,8,8,7,7,6,6,5,5,4,4,
										 3,2,1,0,
										 0,99,99,98,98,97,97,96,96,95,
										 95,94,94,93,93,92,92,91,91,90,
										 90,89,89,88,88,87,87,86,86,85,
										 85,84,84,83,83,82,82,81,81,80,
										 80,79,79,78,78,77,77,76,76,75,
										 75,74,74,73,73,72,72,71,71,70,
										 70,69,69,68,68,67,67,66,66,65,
										 65,64,63,62,61,


										 //2852-2952
										 60,59,58,57,56,55,54,53,52,51,
										 50,50,49,49,48,48,47,47,46,46,
										 45,45,44,44,43,43,42,42,41,41,
										 40,40,39,39,38,38,37,37,36,36,
										 35,35,34,34,33,33,32,32,31,31,
										 30,30,29,29,28,28,27,27,26,26,
										 25,25,24,24,23,23,22,22,21,21,
										 20,20,19,19,18,18,17,17,16,16,
										 15,15,14,14,13,13,12,12,11,11,
										 10,9,8,7,6,5,4,3,2,1,

										 //2953-3056(103)
										 0,99,98,97,96,96,95,95,94,94,93,92,91,
										 91,90,90,89,89,88,88,87,87,86,
										 86,85,85,84,84,83,83,82,82,81,
										 81,80,80,79,79,78,78,77,77,76,
										 76,75,75,74,74,73,73,72,72,71,
										 71,70,70,69,69,68,68,67,67,66,
										 66,65,65,64,64,63,63,62,62,61,
										 61,60,60,59,59,58,58,57,57,56,
										 56,55,55,54,54,53,53,52,52,51,
										 50,49,48,47,46,45,44,43,42,41,



										 //3057-3161(104)
										 40,39,38,37,36,35,35,34,34,33,
										 33,32,32,31,31,30,30,29,29,28,
										 28,27,27,26,26,25,25,24,24,23,
										 23,22,22,21,21,20,20,19,19,18,
										 18,17,17,16,16,15,15,14,14,13,
										 13,12,12,11,11,10,10,9,9,8,
										 8,7,7,6,6,5,5,4,4,3,
										 3,2,2,1,1,0,0,99,99,98,
										 98,97,97,96,96,95,95,94,94,93,
										 93,92,91,90,89,88,87,86,85,84,
										 83,82,81,80,

										 //3162-3268(106)
										 80,79,78,77,76,75,
										 74,73,73,72,72,71,71,70,70,69,
										 69,68,68,67,67,66,66,65,65,64,
										 64,63,63,62,62,61,61,60,60,59,
										 59,58,58,57,57,56,56,55,55,54,
										 54,53,53,52,52,51,51,50,50,49,
										 49,48,48,47,47,46,46,45,45,44,
										 44,43,43,42,42,41,41,40,40,39,
										 39,38,38,37,37,36,36,35,35,34,
										 34,33,33,32,32,31,31,30,30,29,
										 29,28,27,26,25,24,23,22,21,21,

										 //3269-3376(107)
										 20,19,18,17,16,15,14,13,12,11,10,
										 10,9,9,8,8,7,7,6,6,5,
										 5,4,4,3,3,2,2,1,1,0,
										 0,99,99,98,98,97,97,96,96,95,
										 95,94,94,93,93,92,92,91,91,90,
										 90,89,89,88,88,87,87,86,86,85,
										 85,84,84,83,83,82,82,81,81,80,
										 80,79,79,78,78,77,77,76,76,75,
										 75,74,74,73,73,72,72,71,71,70,
										 70,69,69,68,68,67,67,66,66,65,
										 65,64,63,62,61,60,


										 //3377-3487(110)
										 60,59,58,57,56,
										 55,55,54,54,53,53,52,52,51,51,
										 50,50,49,49,48,48,47,47,46,46,
										 45,45,44,44,43,43,42,42,41,41,
										 40,40,39,39,38,38,37,37,36,36,
										 35,35,34,34,33,33,32,32,31,31,
										 30,30,29,29,28,28,27,27,26,26,
										 25,25,24,24,23,23,22,22,21,21,
										 20,20,19,19,18,18,17,17,16,16,
										 15,15,14,14,13,13,12,12,11,11,
										 10,10,9,9,8,8,7,7,6,6,
										 5,4,3,2,1,

										 //3488-3599(111)
										 1,0,0,99,99,98,98,97,97,96,
										 96,95,95,94,94,93,93,92,92,91,
										 91,90,90,89,89,88,88,87,87,86,
										 86,85,85,84,84,83,83,82,82,81,
										 81,80,80,79,79,78,78,77,77,76,
										 76,75,75,74,74,73,73,72,72,71,
										 71,70,70,69,69,68,68,67,67,66,
										 66,65,65,64,64,63,63,62,62,61,
										 61,60,60,59,59,58,58,57,57,56,
										 56,55,55,54,54,53,53,52,52,51,
										 51,49,48,47,46,46,45,44,43,42,
										 41,



										 //3600-3713(113)
										 40,39,38,37,36,35,35,34,34,33,
										 33,32,32,31,31,30,30,29,29,28,
										 28,27,27,26,26,25,25,24,24,23,
										 23,22,22,21,21,20,20,19,19,18,
										 18,17,17,16,16,15,15,14,14,13,
										 13,12,12,11,11,10,10,9,9,8,
										 8,7,7,6,6,5,5,4,4,3,
										 3,2,2,1,1,0,0,99,99,98,
										 98,97,97,96,96,95,95,94,94,93,
										 93,92,92,91,91,90,90,89,89,88,
										 88,87,87,86,86,85,85,84,84,83,
										 82,81,80,

										 //3714-3829(115)
										 80,79,78,77,77,76,76,75,75,74,
										 74,73,73,72,72,71,71,70,70,69,
										 69,68,68,67,67,66,66,65,65,64,
										 64,63,63,62,62,61,61,60,60,59,
										 59,58,58,57,57,56,56,55,55,54,
										 54,53,53,52,52,51,51,50,50,49,
										 49,48,48,47,47,46,46,45,45,44,
										 44,43,43,42,42,41,41,40,40,39,
										 39,38,38,37,37,36,36,35,35,34,
										 34,33,33,32,32,31,31,30,30,29,
										 29,28,28,27,27,26,26,25,25,24,
										 24,23,23,22,21,



										 //3830-3946(116)
										 20,19,18,17,17,16,
										 15,14,14,13,13,12,12,11,11,10,
										 10,9,9,8,8,7,7,6,6,5,
										 5,4,4,3,3,2,2,1,1,0,
										 0,99,99,98,98,97,97,96,96,95,
										 95,94,94,93,93,92,92,91,91,90,
										 90,89,89,88,88,87,87,86,86,85,
										 85,84,84,83,83,82,82,81,81,80,
										 80,79,79,78,78,77,77,76,76,75,
										 75,74,74,73,73,72,72,71,71,70,
										 70,69,69,68,68,67,67,66,66,65,
										 65,64,64,63,63,62,62,61,61,60,

										 //3947-4067(120)
										 60,59,59,58,58,57,57,56,56,55,
										 55,54,54,53,53,52,52,51,51,50,
										 50,49,49,48,48,47,47,46,46,45,
										 45,44,44,43,43,42,42,41,41,40,
										 40,39,39,38,38,37,37,36,36,35,
										 35,34,34,33,33,32,32,31,31,30,
										 30,29,29,28,28,27,27,26,26,25,
										 25,24,24,23,23,22,22,21,21,20,
										 20,19,19,18,18,17,17,16,16,15,
										 15,14,14,13,13,12,12,11,11,10,
										 10,9,9,8,8,7,7,6,6,5,
										 5,4,4,3,3,2,2,1,1,0,
										 //4068-4096(28)
										 250,250,250,250,250,250,250,250,250,250,
										 250,250,250,250,250,250,250,250,250,250,
										 250,250,250,250,250,250,250,250};

//updates graph on LCD
void graphADC(uint8_t tempWhole){
	//shifts values over, kicking the element at index 0 out and placing the latest
	//read value in index 99
	if(arrIndex==100){
		for(uint8_t i = 0; i < 99; i++){
			last100Values[i]=last100Values[i+1];
		}
		last100Values[99]=tempWhole;
	}else{
		last100Values[arrIndex]=tempWhole;
		arrIndex++;
	}

	ST7735_XYplot(100,arrIndex,last100Values,0);
}

//updates screen to new temp and adc values
void updateScreen(uint8_t tempWhole,uint8_t tempDec, uint16_t rawData){
	tempWhole += offset;
	if(tempDec>=50){
		tempWhole++;
		tempDec -= offsetDecimal;
	}else{
		tempDec+=offsetDecimal;
	}


	char zero = '0';
	char Char1= (tempWhole/10)+zero;
	char Char2= (tempWhole%10)+zero;
	char Char3=(tempDec/10)+zero;
	char Char4=(tempDec%10)+zero;
	char str[5]={Char1,Char2,'.',Char3,Char4};
	ST7735_DrawString(3,2,str,ST7735_BLUE);




	char Char5=(rawData/1000)+zero;
	rawData=rawData%1000;
	char Char6=(rawData/100)+zero;
	rawData=rawData%100;
	char Char7=(rawData/10)+zero;
	char Char8=(rawData%10)+zero;

	char str2[5]={Char5,Char6,Char7,Char8,' '};
	ST7735_DrawString(3,3,str2,ST7735_BLUE);
	graphADC(tempWhole);

}

//displays what kind of error occured if 
//adc value was out of bounds (59-4067)
void displayError(uint8_t val){

	if(val==250){
		//openError
			char cPTR[10]={'o','p','e','n',' ','e','r','r','o','r'};
			ST7735_DrawString(3,1,cPTR,ST7735_BLUE);

		}else if(temper[0]==249){
			//shorted error
			char cPTR[11]={'s','h','o','r','t',' ','e','r','r','o','r'};
			ST7735_DrawString(3,1,cPTR,ST7735_BLUE);
		}else{
			char cPTR[11] ={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
			ST7735_DrawString(3,1,cPTR,ST7735_BLUE);
		}
}

//returns whole value of temperature
uint8_t fixedPoint(uint16_t rawData){
	if(rawData<=4067&&rawData>=59){
		temper[0]=adcValueTable[rawData];
		displayError(temper[0]);
		return temper[0];
	}
	return 0;
}

//returns decimal value of temperature
uint8_t fixedPointDecimal(uint16_t rawData){
	if(rawData<=4067&&rawData>=59){
	  temper[1]=adcValueDecimalTable[rawData];
		if(temper[1]==250){
			displayError(temper[1]);
			return 0;
		}
		return temper[1];
	}
	return 0;
}
