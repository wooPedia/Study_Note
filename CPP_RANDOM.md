# 비결정적 난수 생성

<br>

## [개요]

C++ 11 이전엔  C 스타일의 rand() 함수를 이용하여 난수를 생성하였다.
C++ 11부턴 C++ 스타일의 난수 생성 함수가 나왔으며 <b><i>random</i></b> 헤더 파일에 정의되어 있다. C 스타일의 난수 생성에 비해 난수의 형, 범위 등을 세세하게 조절할 수 있으며 기능적으로 많이 향상되었다.

<br>
난수 생성 방법에는 동일한 난수표를 사용하여 <u>첫 실행 이후부터는 예측이 되는 방법</u>과 <u>매번 달라지는 난수 생성 방법</u>이 있다. 여기선 매번 달라지는 <u>비결정적 난수 생성 방법</u>을 다룬다. 이 방법은 실제로 하드웨어 노이즈나 마우스 움직임 등을 통해 예측할 수 없는 난수 생성을 위한 리소스를 만들어 사용한다.


<br><br>
## [헤더 및 클래스 소개]

<random> 이라는 헤더 안에 난수 생성기와 난수 분포기 관련 클래스가 들어있다. 난수 생성기는 난수를 생성하며 난수 분포기는 형(type)과 범위를 정한다.

- std::random_device 클래스는 이용 가능한 하드웨어로부터 난수를 생성하는 클래스이다.
- std::uniform_int_distribution<T> 클래스는 타입과 범위를 받아 정수형 난수를 분포한다.
- std::uniform_real_distribution<T> 클래스는 타입과 범위를 받아 실수형 난수를 분포한다

<br>
<b>a~b의 범위의 난수를 생성할 경우 정수 타입 클래스는 [a, b] 범위로 a와 b를 모두 포함한다. 반면 실수 클래스는 a는 포함하지만 b는 포함하지 않는다.</b>

<br>
<br>
<br>

## [예제 코드]

아래 예제는 -100 ~ 100 범위의 정수와 실수를 랜덤으로 생성하여 출력한다.

```C++
#include <iostream>
#include <random>

std::random_device rng;

std::uniform_int_distribution<int> dist1(-100, 100); // [-100, 100] 범위
std::uniform_real_distribution<double> dist2(-100.0, 100.0);  // [-100.0, 100.0) 범위


void main()
{
   // [-100, 100] 범위의 정수 5개 생성 및 출력
   for(size_t i = 0; i != 5; ++i)
   {
      std::cout << dist1(rng) << " ";
   }
   std::cout << std::endl;
   
   // [-100.0, 100.0] 범위의 실수 5개 생성 및 출력
   for(size_t i = 0; i != 5; ++i)
   {
      std::cout << dist2(rng) << " ";
   }
   std::cout << std::endl;
}
```

<br><br>
다음과 같이 함수 템플릿으로 활용할 수도 있다.

```C++
#include <iostream>
#include <random>
#include <type_traits>

std::random_device rng;

template <typename T>
void PrintRandomIntegerNumber()
{
   // 정수 타입만 받습니다.
   static_assert(std::is_integral<T>::value);
   
   std::uniform_int_distribution<T> dist(-100.0, 100.0);

   for(size_t i = 0; i != 5; ++i)
   {
      std::cout << dist(rng) << " ";
   }
}

void main()
{
   PrintRandomIntegerNumber<unsigned int>(); 
}
```
