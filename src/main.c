#include "gametank.h"
#include "drawing_funcs.h"
#include "input.h"
#include "persist.h"
#include "music.h"
#include "banking.h"
#include "dynawave.h"
#include "gen/assets/rubber.h"
#include "gen/assets/sfx.h"

#pragma data-name (push, "SAVE")
#pragma data-name (pop)

#define TRACK_PROGRESSION_END   0x306C

#pragma bss-name (push,"ZEROPAGE")
unsigned char tmp, tmp2, tmp3, tmp4;
unsigned char turn;
unsigned char car_x;
unsigned char test;
unsigned char track_offset;
unsigned char track_lengths;
unsigned char finish_line_offset;
unsigned char finish_line_offset_low;
unsigned char finish_line_start;
unsigned char finish_line_end;
unsigned char speed;
unsigned char speed_low;
unsigned char speed_high;
unsigned char landscape;
unsigned char landscape_low;
unsigned char end_track;
unsigned int track_progression;
unsigned char tire;
unsigned char drift_turn_offset;
unsigned char noise;
unsigned char bump;

unsigned char other_car_speed[4];
unsigned char other_car_track_id[4];
unsigned char other_car_track_id_long[4];
unsigned char other_car_track_offset[4];
unsigned char other_car_track_lengths[4];
unsigned int other_car_track_progression[4];
#pragma bss-name (pop)

/*
    ; Bit 0: are we turning left
    ; Bit 1: are we turning right
    ; Bit 2-7: number of steps after the turn (min=1, max=63)
    ; Examples:
    ; 191 = Turn left and wait for 63 steps
    ; 129 = Turn left and wait for 1 step
    ; 127 = Turn right and wait for 63 steps
    ; 65 = Turn right and wait for 1 step
    ; 63 = Wait for 63 steps
    ; $FF = end of the track (game over)
    db 128+20, 64+48, 64+1, 64+63, 128+1, 128+63, 128+1, 128+1, 128+63, 64+1, 64+1, 64+1, 64+1, 64+1, 64+63, 128+1, 128+1, 128+63, $FF
*/
unsigned char track[47] = {
    40, //First segment
    64+1,64+30, 128+1,128+1, //First turn
    40, //Second segment
    64+1,64+20, 128+1,128+1, //Second turn
    40, //Third segment
    64+1,64+1,64+40, 128+1,128+1,128+1, //Third turn
    63, //Fourth segment
    128+1,128+1,128+30, 64+1,64+1,64+1, //Fourth turn
    63, //Fith segment
    64+1,64+1,64+1,64+40, 128+1,128+1,128+1,128+1,
    30,
    128+1,128+30,64+1,64+1,
    16,
    64+1,64+1,64+37,128+1,128+1,128+1,
    60,
    0xFF
};

// 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59  60  61  62  63
char tracks[20][64] = {
{ 64,  1, 66,  3, 68,  5, 70,  7, 72, 73, 10, 11, 76, 77, 14, 15, 16, 81, 82, 83, 20, 21, 22, 23, 88, 89, 90, 91, 28, 29, 30, 31, 32, 97, 98, 99,100,101,102, 39, 40, 41, 42, 43, 44, 45,110,111,112,113,114,115,116,117, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72, 73, 10, 11, 76, 77, 14, 15, 16, 81, 82, 83, 20, 21, 22, 23, 88, 89, 90, 91, 28, 29, 30, 31, 32, 97, 98, 99,100,101,102, 39, 40, 41, 42, 43, 44, 45,110,111,112,113,114,115,116,117,118, 55, 56, 57, 58, 59, 60, 61, 62, 63},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72, 73, 10, 11, 76, 77, 14, 15, 16, 81, 82, 83, 20, 21, 22, 23, 88, 89, 90, 91, 92, 29, 30, 31, 32, 33, 98, 99,100,101,102,103, 40, 41, 42, 43, 44, 45, 46,111,112,113,114,115,116,117,118,119, 56, 57, 58, 59, 60, 61, 62, 63},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72, 73, 10, 11, 76, 77, 14, 15, 16, 81, 82, 83, 20, 21, 22, 23, 88, 89, 90, 91, 92, 29, 30, 31, 32, 33, 98, 99,100,101,102,103, 40, 41, 42, 43, 44, 45, 46, 47,112,113,114,115,116,117,118,119,120, 57, 58, 59, 60, 61, 62, 63},
{  0, 65,  2, 67,  4, 69,  6, 71,  8, 73, 74, 11, 12, 77, 78, 15, 16, 17, 82, 83, 84, 21, 22, 23, 24, 89, 90, 91, 92, 93, 30, 31, 32, 33, 34, 99,100,101,102,103,104, 41, 42, 43, 44, 45, 46, 47, 48,113,114,115,116,117,118,119,120,121, 58, 59, 60, 61, 62, 63},
{  0, 65,  2, 67,  4, 69,  6, 71,  8, 73, 74, 11, 12, 77, 78, 15, 16, 17, 82, 83, 84, 21, 22, 23, 24, 89, 90, 91, 92, 93, 30, 31, 32, 33, 34, 99,100,101,102,103,104,105, 42, 43, 44, 45, 46, 47, 48, 49,114,115,116,117,118,119,120,121,122, 59, 60, 61, 62, 63},
{  0, 65,  2, 67,  4, 69,  6, 71,  8, 73, 74, 11, 12, 77, 78, 15, 16, 17, 82, 83, 84, 21, 22, 23, 24, 89, 90, 91, 92, 93, 30, 31, 32, 33, 34, 35,100,101,102,103,104,105,106, 43, 44, 45, 46, 47, 48, 49, 50,115,116,117,118,119,120,121,122,123, 60, 61, 62, 63},
{  0, 65,  2, 67,  4, 69,  6, 71,  8, 73, 74, 11, 12, 77, 78, 15, 16, 17, 82, 83, 84, 21, 22, 23, 24, 89, 90, 91, 92, 93, 30, 31, 32, 33, 34, 35,100,101,102,103,104,105,106, 43, 44, 45, 46, 47, 48, 49, 50,115,116,117,118,119,120,121,122,123,124, 61, 62, 63},
{  0, 65,  2, 67,  4, 69,  6, 71,  8,  9, 74, 75, 12, 13, 78, 79, 16, 17, 18, 83, 84, 85, 22, 23, 24, 25, 90, 91, 92, 93, 94, 31, 32, 33, 34, 35, 36,101,102,103,104,105,106,107, 44, 45, 46, 47, 48, 49, 50, 51,116,117,118,119,120,121,122,123,124,125, 62, 63},
{  0, 65,  2, 67,  4, 69,  6, 71,  8,  9, 74, 75, 12, 13, 78, 79, 16, 17, 18, 83, 84, 85, 86, 23, 24, 25, 26, 91, 92, 93, 94, 95, 32, 33, 34, 35, 36, 37,102,103,104,105,106,107,108, 45, 46, 47, 48, 49, 50, 51, 52,117,118,119,120,121,122,123,124,125,126, 63},
{  0, 65,  2, 67,  4, 69,  6, 71,  8,  9, 74, 75, 12, 13, 78, 79, 80, 17, 18, 19, 84, 85, 86, 87, 24, 25, 26, 27, 92, 93, 94, 95, 96, 33, 34, 35, 36, 37, 38,103,104,105,106,107,108,109, 46, 47, 48, 49, 50, 51, 52, 53,118,119,120,121,122,123,124,125,126,127},
{  0, 65,  2, 67,  4, 69,  6, 71,  8,  9, 74, 75, 12, 13, 78, 79, 80, 17, 18, 19, 84, 85, 86, 87, 24, 25, 26, 27, 92, 93, 94, 95, 96, 33, 34, 35, 36, 37, 38,103,104,105,106,107,108,109, 46, 47, 48, 49, 50, 51, 52, 53, 54,119,120,121,122,123,124,125,126,127},
{  0, 65,  2, 67,  4, 69,  6, 71,  8,  9, 74, 75, 12, 13, 78, 79, 80, 17, 18, 19, 84, 85, 86, 87, 24, 25, 26, 27, 28, 93, 94, 95, 96, 97, 34, 35, 36, 37, 38, 39,104,105,106,107,108,109,110, 47, 48, 49, 50, 51, 52, 53, 54, 55,120,121,122,123,124,125,126,127},
{  0, 65,  2, 67,  4, 69,  6, 71,  8,  9, 74, 75, 12, 13, 78, 79, 80, 17, 18, 19, 84, 85, 86, 87, 24, 25, 26, 27, 28, 93, 94, 95, 96, 97, 34, 35, 36, 37, 38, 39,104,105,106,107,108,109,110,111, 48, 49, 50, 51, 52, 53, 54, 55, 56,121,122,123,124,125,126,127},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72,  9, 10, 75, 76, 13, 14, 79, 80, 81, 18, 19, 20, 85, 86, 87, 88, 25, 26, 27, 28, 29, 94, 95, 96, 97, 98, 35, 36, 37, 38, 39, 40,105,106,107,108,109,110,111,112, 49, 50, 51, 52, 53, 54, 55, 56, 57,122,123,124,125,126,127},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72,  9, 10, 75, 76, 13, 14, 79, 80, 81, 18, 19, 20, 85, 86, 87, 88, 25, 26, 27, 28, 29, 94, 95, 96, 97, 98, 35, 36, 37, 38, 39, 40, 41,106,107,108,109,110,111,112,113, 50, 51, 52, 53, 54, 55, 56, 57, 58,123,124,125,126,127},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72,  9, 10, 75, 76, 13, 14, 79, 80, 81, 18, 19, 20, 85, 86, 87, 88, 25, 26, 27, 28, 29, 94, 95, 96, 97, 98, 99, 36, 37, 38, 39, 40, 41, 42,107,108,109,110,111,112,113,114, 51, 52, 53, 54, 55, 56, 57, 58, 59,124,125,126,127},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72,  9, 10, 75, 76, 13, 14, 79, 80, 81, 18, 19, 20, 85, 86, 87, 88, 25, 26, 27, 28, 29, 94, 95, 96, 97, 98, 99, 36, 37, 38, 39, 40, 41, 42,107,108,109,110,111,112,113,114, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,125,126,127},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72, 73, 10, 11, 76, 77, 14, 15, 80, 81, 82, 19, 20, 21, 86, 87, 88, 89, 26, 27, 28, 29, 30, 95, 96, 97, 98, 99,100, 37, 38, 39, 40, 41, 42, 43,108,109,110,111,112,113,114,115, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,126,127},
{ 64,  1, 66,  3, 68,  5, 70,  7, 72, 73, 10, 11, 76, 77, 14, 15, 80, 81, 82, 19, 20, 21, 22, 87, 88, 89, 90, 27, 28, 29, 30, 31, 96, 97, 98, 99,100,101, 38, 39, 40, 41, 42, 43, 44,109,110,111,112,113,114,115,116, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,127}
};
// 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59  60  61  62  63

const char finish_offset_start[200] = {
    0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,13,13,13,13,14,14,14,14,14,14,15,15,15,15,16,16,17,17,17,17,18,18,18,18,19,19,20,20,20,20,21,21,21,21,22,23,24,24,24,24,25,25,25,25,26,27,28,28,29,29,30,30,30,30,31,32,33,33,34,34,35,35,36,36,37,38,39,39,40,40,41,42,43,43,44,45,46,46,47,48,49,50,51,51,52,53,54,55,56,57,58,59,60,61,62,63
};

const char finish_offset_end[200] = {
    2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,13,13,13,13,14,14,14,14,14,14,15,15,15,15,16,16,17,17,17,17,18,18,18,18,19,19,20,20,20,20,21,21,21,21,22,23,24,24,24,24,25,25,25,25,26,27,28,28,29,29,30,30,30,30,31,32,33,33,34,34,35,35,36,36,37,38,39,39,40,40,41,42,43,43,44,45,46,46,47,48,49,50,51,51,52,53,54,55,56,57,58,59,60,61,62,63,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
};
/*
const char finish_offset[] = {
0,2,0,2,0,2,0,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,2,4,2,4,2,4,2,4,2,4,2,4,2,4,2,4,2,4,2,4,3,5,3,5,3,5,3,5,3,5,3,5,3,5,3,5,3,5,3,5,4,6,4,6,4,6,4,6,4,6,4,6,4,6,4,6,4,6,4,6,
5,7,5,7,5,7,5,7,5,7,5,7,5,7,5,7,5,7,5,7,6,8,6,8,6,8,6,8,6,8,6,8,6,8,6,8,6,8,6,8,7,9,7,9,7,9,7,9,7,10,7,10,7,10,7,10,7,10,7,10,8,11,8,11,8,11,8,11,8,12,8,12,8,12,8,12,8,12,8,12,
9,13,9,13,9,13,9,13,10,14,10,14,10,14,10,14,10,14,10,14,11,15,11,15,11,15,11,15,12,16,12,16,12,17,12,17,12,17,12,17,13,18,13,18,13,18,13,18,14,19,14,19,14,20,14,20,14,20,14,20,
15,21,15,21,15,21,15,21,16,22,16,23,17,24,17,24,17,24,17,24,18,25,18,25,18,25,18,25,19,26,19,27,20,28,20,28,20,29,20,29,21,30,21,30,21,30,21,30,22,31,23,32,24,33,24,33,24,34,
24,34,25,35,25,35,25,36,25,36,26,37,27,38,28,39,28,39,29,40,29,40,30,41,30,42,30,43,30,43,31,44,32,45,33,46,33,46,34,47,34,48,35,49,35,50,36,51,36,51,37,52,38,53,39,54,39,55,
40,56,40,57,41,58,42,59,43,60,43,61,44,62,45,63,46,64,46,64,47,64,48,64,49,64,50,64,51,64,51,64,52,64,53,64,54,64,55,64,56,64,57,64,58,64,59,64,60,64,61,64,62,64,63,64};
*/

char track_width[64] = {
    4,6,8,9,11,13,15,17,18,20,22,24,26,28,29,31,33,35,37,38,40,42,44,46,48,49,51,53,55,57,58,60,62,64,66,67,69,71,73,75,76,78,80,82,84,86,87,89,91,93,95,96,98,100,102,104,106,107,109,111,113,115,116,118
};

char track_x_shifts[9][64] = {
{5,7,9,12,13,16,17,19,20,21,22,23,24,24,26,25,27,26,27,27,27,27,27,27,27,28,26,27,26,26,26,25,25,24,24,23,23,22,22,20,21,20,19,18,18,17,16,16,14,15,13,13,12,11,10,9,8,8,8,6,6,4,4,3},
{17,19,20,22,23,25,24,26,27,27,28,28,29,29,30,29,30,29,30,30,30,30,29,29,29,29,28,28,27,27,27,26,26,25,24,24,24,22,23,21,21,20,19,19,18,17,16,16,15,15,13,13,12,11,10,9,8,8,8,6,6,4,4,3},
{31,32,32,33,33,34,33,34,35,35,35,34,34,34,35,34,34,33,34,33,33,32,32,31,31,31,30,30,28,29,28,27,27,26,25,24,25,23,23,21,22,21,20,19,18,17,16,17,15,15,13,13,12,11,10,9,9,8,8,6,6,4,4,3},
{44,44,43,44,43,43,42,43,42,42,41,41,40,40,40,38,39,37,37,37,36,35,35,34,33,33,32,32,30,30,30,29,28,27,26,25,26,24,24,22,22,21,20,20,19,18,17,17,15,15,13,13,12,11,11,10,9,8,8,6,6,4,4,3},
{62,61,60,60,58,58,56,56,55,54,53,52,51,50,50,48,48,46,46,45,44,43,42,41,40,40,38,38,36,36,35,34,33,32,31,30,30,28,28,26,26,25,24,23,22,21,20,20,18,18,16,16,15,14,13,12,11,10,10,8,8,6,6,5},
{78,76,75,74,71,71,68,67,66,64,63,61,60,58,58,56,55,53,53,51,50,49,47,46,45,45,42,42,40,40,38,37,36,35,34,33,32,30,30,28,28,27,26,24,23,22,21,21,19,19,17,17,16,15,13,12,11,10,10,8,8,6,6,5},
{91,88,86,85,81,80,77,76,73,71,69,68,66,64,63,60,60,57,56,55,53,52,50,49,47,47,44,44,42,41,40,39,37,36,35,34,33,31,31,29,28,27,26,25,24,23,22,21,19,19,17,17,16,15,14,13,11,10,10,8,8,6,6,5},
{105,101,98,96,91,89,86,84,81,79,76,74,71,69,68,65,64,61,60,58,56,54,53,51,49,49,46,46,43,43,41,40,38,37,36,34,34,32,31,29,29,28,27,25,24,23,22,22,19,19,17,17,16,15,14,13,12,10,10,8,8,6,6,5},
{117,113,109,106,101,98,93,91,88,85,82,79,76,74,72,69,67,64,63,61,59,57,55,53,51,50,48,47,44,44,42,41,39,38,36,35,35,32,32,30,29,28,27,26,24,23,22,22,20,19,17,17,16,15,14,13,12,10,10,8,8,6,6,5}
};

unsigned char track_map_x[98] = {
    21, 22, 24, 25, 26, 28, 29, 31, 32, 33, 34, 35, 35, 36, 37, 37, 38, 39, 39, 40, 40, 40, 40, 39, 39, 39, 38, 38, 38, 37, 36, 35, 34, 33, 31, 30, 29, 29, 28, 27, 26, 25, 24, 23, 22, 22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 0, 0, 0, 0, 1, 2, 2, 3, 4, 5, 5, 5, 5, 5, 5, 4, 3, 3, 3, 3, 4, 5, 6, 7, 9, 10, 11, 13, 14, 15, 17, 18, 19
};
unsigned char track_map_y[98] = {
    96, 96, 96, 96, 96, 96, 96, 96, 96, 97, 98, 99, 100, 101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 114, 115, 116, 118, 119, 120, 122, 123, 123, 124, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 110, 110, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 122, 122, 121, 120, 119, 118, 117, 115, 114, 113, 112, 111, 110, 109, 107, 106, 105, 103, 102, 101, 100, 98, 97, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96
};

const char drift[27] = {
    0, 2, 4,
    0, 1, 3,
    0, 1, 2,
    0, 0, 1,
    0, 0, 0,
    0, 0,-1,
    0,-1,-2,
    0,-1,-3,
    0,-2,-4
};

char track_id = 0;
char *track_x_shift = (char *)track_x_shifts[4];
char *track_x_shift0 = (char *)track_x_shifts[4];
unsigned char pitch_engine;
unsigned char pitch_engine_long;
unsigned char track_id_long;

#define DRAW_LINE(line) \
    if (line < finish_line_start) tmp2 = 0; \
    else if (line == finish_line_end) tmp2 = 1;  \
    draw_sprite(track_x_shift[line], line+48, track_width[line], 1, track_x_shift0[line], tracks[track_id][line], tmp2);

void breakpoint() {}

int main () {
    init_graphics();
    init_dynawave();
    init_music();

    flip_pages();
    clear_border(0);
    await_draw_queue();
    flip_pages();
    await_draw_queue();
    clear_border(0);

    load_spritesheet((char *)&ASSET__rubber__track_bmp, 0);
    load_spritesheet((char *)&ASSET__rubber__track2_bmp, 1);
    load_spritesheet((char *)&ASSET__rubber__cars_bmp, 2);
    load_spritesheet((char *)&ASSET__rubber__track_map_bmp, 3);
    load_spritesheet((char *)&ASSET__rubber__splash_bmp, 4);
    change_rom_bank(SAVE_BANK_NUM);
    pitch_engine = 0;
    pitch_engine_long = 0;
    track_id_long = 0;
    speed = 1;
    speed_high = 0;
    speed_low = 1;
    car_x = 50;
    tmp = track[0];
    turn = 4;
    track_offset = 0;
    tmp = track[track_offset];
    track_lengths = tmp & 63;
    track_progression = 0;
    landscape = 0;
    landscape_low = 0;
    end_track = 0;
    finish_line_end = 100;
    finish_line_start = 100;
    tire = 0;
    drift_turn_offset = 12;
    bump = 0;

    finish_line_offset = 0;
    finish_line_offset_low = 0;
    noise = 0;

    for (tmp=0; tmp<4; tmp++) {
        other_car_track_lengths[tmp] = track_lengths;
        other_car_track_offset[tmp] = 0;
        other_car_speed[tmp] = 18+(tmp << 2);
        other_car_track_id[tmp] = tmp << 3;
        other_car_track_id_long[tmp] = 0;
        other_car_track_progression[tmp] = tmp << 3;
    }

    play_sound_effect(&ASSET__sfx__engine_bin , 1);


    while (1) {                                     //  Run forever
        clear_screen(0);

        draw_sprite(landscape, 0, 127-landscape, 32, 0, 0, 2);      // Draw landscape
        draw_sprite(0, 0, landscape, 32, 127-landscape, 0, 2);
        draw_box(0, 32, 127, 64, 27);                               // Draw grass

        tmp2 = 0;
        if (end_track) {
            finish_line_end = finish_offset_end[finish_line_offset] - 1;
            finish_line_start = finish_offset_start[finish_line_offset];

            finish_line_offset += speed_high;
            finish_line_offset_low += speed_low;
            if (finish_line_offset_low & 8) {
                finish_line_offset++;
                finish_line_offset_low -= 8;
            }

            if (finish_line_offset >= 200) {
                end_track = 0;
                finish_line_end = 100;
                finish_line_start = 100;
                track_offset = 0;
                track_progression = 0;
                tmp = track[track_offset];
                track_lengths = tmp & 63;
            }
        }

        // Draw track
        for (tmp=63; tmp!=255; tmp--) {
            if (tmp < finish_line_start) tmp2 = 0;
            else if (tmp == finish_line_end) tmp2 = 1;
            draw_sprite(track_x_shift[tmp], tmp+32, track_width[tmp], 1, track_x_shift0[tmp], tracks[track_id][tmp], tmp2);
        }

        // Draw other cars
        if (other_car_track_progression[3] >= track_progression && other_car_track_progression[3] < track_progression + 55) {
            tmp = other_car_track_progression[3] - track_progression;
            tmp2 = tmp & 0xF0;
            tmp4 = 63 - tmp;
            tmp3 = track_x_shift[tmp4];
            tmp3 += 32;
            tmp3 -= track_x_shifts[4][tmp4];
            tmp = 80 - tmp;
            draw_sprite(32+tmp3, tmp, 32, 16, 96, 32+tmp2, 2);
        }
        if (other_car_track_progression[2] >= track_progression && other_car_track_progression[2] < track_progression + 55) {
            tmp = other_car_track_progression[2] - track_progression;
            tmp2 = tmp & 0xF0;
            tmp4 = 63 - tmp;
            tmp3 = track_x_shift[tmp4];
            tmp3 += 32;
            tmp3 -= track_x_shifts[4][tmp4];
            tmp = 80 - tmp;
            draw_sprite(32+tmp3, tmp, 32, 16, 64, 32+tmp2, 2);
        }
        if (other_car_track_progression[1] >= track_progression && other_car_track_progression[1] < track_progression + 55) {
            tmp = other_car_track_progression[1] - track_progression;
            tmp2 = tmp & 0xF0;
            tmp4 = 63 - tmp;
            tmp3 = track_x_shift[tmp4];
            tmp3 += 32;
            tmp3 -= track_x_shifts[4][tmp4];
            tmp = 80 - tmp;
            draw_sprite(32+tmp3, tmp, 32, 16, 32, 32+tmp2, 2);
        }
        if (other_car_track_progression[0] >= track_progression && other_car_track_progression[0] < track_progression + 55) {
            tmp = other_car_track_progression[0] - track_progression;
            tmp2 = tmp & 0xF0;
            tmp4 = 63 - tmp;
            tmp3 = track_x_shift[tmp4];
            tmp3 += 32;
            tmp3 -= track_x_shifts[4][tmp4];
            tmp = 80 - tmp;
            draw_sprite(32+tmp3, tmp, 32, 16, 0, 32+tmp2, 2);
        }

        // Draw player's car
        if (bump && (track_id & 1)) {
            draw_sprite(car_x >> 1, 79, 32, 16, ((track_id&3) << 5), 96, 2);
        } else {
            draw_sprite(car_x >> 1, 80, 32, 16, ((track_id&3) << 5), 96, 2);
        }

        // Draw speedometer
        draw_sprite(64, 109, speed, 6, 80, 0, 3);
        // Draw track map
        draw_sprite(0, 96, 48, 32, 0, 0, 3);
        tmp = *((unsigned char *)&other_car_track_progression+1) << 1;
        if (*((unsigned char *)&other_car_track_progression) & 0x80) { tmp++; }
        draw_sprite(track_map_x[tmp], track_map_y[tmp], 5, 5, 64, 0, 3);
        tmp = *((unsigned char *)&other_car_track_progression+3) << 1;
        if (*((unsigned char *)&other_car_track_progression+2) & 0x80) { tmp++; }
        draw_sprite(track_map_x[tmp], track_map_y[tmp], 5, 5, 72, 0, 3);
        tmp = *((unsigned char *)&other_car_track_progression+5) << 1;
        if (*((unsigned char *)&other_car_track_progression+4) & 0x80) { tmp++; }
        draw_sprite(track_map_x[tmp], track_map_y[tmp], 5, 5, 64, 8, 3);
        tmp = *((unsigned char *)&other_car_track_progression+7) << 1;
        if (*((unsigned char *)&other_car_track_progression+6) & 0x80) { tmp++; }
        draw_sprite(track_map_x[tmp], track_map_y[tmp], 5, 5, 72, 8, 3);
        tmp = (*((unsigned char *)&track_progression+1)) << 1;
        if (*((unsigned char *)&track_progression) & 0x80) { tmp++; };
        draw_sprite(track_map_x[tmp], track_map_y[tmp], 5, 5, 48, 0, 3);

        // Steering
        update_inputs();
        if(player1_buttons & INPUT_MASK_LEFT) {
            car_x--;
            car_x--;
        } else if (player1_buttons & INPUT_MASK_RIGHT) {
            car_x++;
            car_x++;
        }

        // Compute the drift
        tmp = speed_high >> 1;
        car_x += drift[drift_turn_offset + tmp];

        if (car_x <= 30) {
            car_x = 30;
            if (speed > 2) {
                speed-=2;
                speed_high = speed >> 3;
                speed_low = speed & 7;
                pitch_engine_long--;
                pitch_engine = pitch_engine_long >> 2;
            }
            bump = 1;
        } else if (car_x >= 180) {
            car_x = 180;
            if (speed > 2) {
                speed-=2;
                speed_high = speed >> 3;
                speed_low = speed & 7;
                pitch_engine_long--;
                pitch_engine = pitch_engine_long >> 2;
            }
            bump = 1;
        } else {
            bump = 0;
        }

        // Player input (acceleration and deceleration)
        if((player1_buttons & INPUT_MASK_UP) && !bump) {
            if (speed != 32) {
                speed++;
                speed_high = speed >> 3;
                speed_low = speed & 7;
                pitch_engine_long++;
                pitch_engine = pitch_engine_long >> 2;
            }
        } else if(player1_buttons & INPUT_MASK_DOWN) {
            if (speed != 1) {
                speed--;
                speed_high = speed >> 3;
                speed_low = speed & 7;
                pitch_engine_long--;
                pitch_engine = pitch_engine_long >> 2;
            }
        }

        // landscape += (turn >> 2) * speed / 32
        if (!(turn & 4)) {
            tmp = (4 - turn) * speed;
            tmp2 = tmp >> 7;
            tmp3 = tmp & 127;
            landscape_low += tmp3;
            if (landscape_low & 0x80) {
                landscape_low -= 128;
                landscape++;
            }
            landscape += tmp2;
        } else if (turn == 4) {

        } else {
            tmp = (turn-4) * speed;
            tmp2 = tmp >> 7;
            breakpoint();
            tmp3 = tmp & 127;
            landscape_low += tmp3;
            if (landscape_low & 0x80) {
                landscape_low -= 128;
                landscape--;
            }
            landscape -= tmp2;
        }
        if (landscape & 0x80) landscape -= 128;
        
        // Play the engine noise
        noise++;
        if (noise == 2) {
            play_sound_effect(&ASSET__sfx__engine_bin , 1);
            noise = 0;
        }
        tick_music();

        // test
        track_id_long += speed;
        tmp = track_id;
        track_id = track_id_long >> 3;
        tmp = track_id - tmp;
        track_progression += tmp;
        if (track_id >= 20) {
            track_id -= 20;
            track_id_long -= 160;
            track_lengths--;
            if (!track_lengths) {
                track_offset++;
                tmp = track[track_offset];
                if (tmp == 255) {
                    end_track = 1;
                    finish_line_offset = 0;
                    finish_line_offset_low = 0;
                } else {
                    track_lengths = tmp & 63;
                    if (tmp & 0x80) {
                        turn--;
                        drift_turn_offset -= 3;
                    }
                    else if (tmp & 0x40) {
                        turn++;
                        drift_turn_offset += 3;
                    }
                    track_x_shift = (char *)track_x_shifts[turn];
                }
            }
        }

        other_car_track_id_long[0] += other_car_speed[0];
        tmp = other_car_track_id[0];
        other_car_track_id[0] = other_car_track_id_long[0] >> 3;
        tmp = other_car_track_id[0] - tmp;
        other_car_track_progression[0] += tmp;
        if (other_car_track_id[0] >= 20) {
            other_car_track_id[0] -= 20;
            other_car_track_id_long[0] -= 160;
        }
        if (other_car_track_progression[0] >= TRACK_PROGRESSION_END) other_car_track_progression[0] = 0;

        other_car_track_id_long[1] += other_car_speed[1];
        tmp = other_car_track_id[1];
        other_car_track_id[1] = other_car_track_id_long[1] >> 3;
        tmp = other_car_track_id[1] - tmp;
        other_car_track_progression[1] += tmp;
        if (other_car_track_id[1] >= 20) {
            other_car_track_id[1] -= 20;
            other_car_track_id_long[1] -= 160;
        }
        if (other_car_track_progression[1] >= TRACK_PROGRESSION_END) other_car_track_progression[1] = 0;
        
        other_car_track_id_long[2] += other_car_speed[2];
        tmp = other_car_track_id[2];
        other_car_track_id[2] = other_car_track_id_long[2] >> 3;
        tmp = other_car_track_id[2] - tmp;
        other_car_track_progression[2] += tmp;
        if (other_car_track_id[2] >= 20) {
            other_car_track_id[2] -= 20;
            other_car_track_id_long[2] -= 160;
        }
        if (other_car_track_progression[2] >= TRACK_PROGRESSION_END) other_car_track_progression[2] = 0;
        
        other_car_track_id_long[3] += other_car_speed[3];
        tmp = other_car_track_id[3];
        other_car_track_id[3] = other_car_track_id_long[3] >> 3;
        tmp = other_car_track_id[3] - tmp;
        other_car_track_progression[3] += tmp;
        if (other_car_track_id[3] >= 20) {
            other_car_track_id[3] -= 20;
            other_car_track_id_long[3] -= 160;
        }
        if (other_car_track_progression[3] >= TRACK_PROGRESSION_END) other_car_track_progression[3] = 0;

        await_draw_queue();
        PROFILER_END(0);
        sleep(1);
        flip_pages();
        PROFILER_START(0);
    }

  return (0);                                     //  We should never get here!
}