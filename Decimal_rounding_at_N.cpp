#include <cmath>
#include <iostream>

// 소수 넷째 자리에서 반올림하기
void main()
{
   double x = 32.15683;
   x = (std::round(x * 1000)) * 0.001; // 32.157
   
   std::cout << x << std::endl; 
}