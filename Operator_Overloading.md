<h1>연산자 오버로딩 (클래스 레벨, 전역 레벨 모두)</h1>

<br>
=, (), [], -> 연산자들은 멤버 함수를 이용해서만 오버로딩 가능

<br>
<br>

cout.operator<< 와 같이 호출자가 클래스 객체가 아닌 cout 자체에 권한이 있는 연산자는 멤버 함수로 오버로딩할 수 없다.
따라서 이 경우 아래와 같이 전역 함수로 오버로딩해야한다. 

<br>
단, 전역 함수에서 Vector의 private 멤버 변수에 접근할 수 없으므로 아래 함수를 Vector 클래스에 friend로 선언해준다.


첫 번째 인자는 좌변(호출자)이며 두 번째 인자는 우변(cout의 인자)이다.
반환 값은 ostream (output stream)이며 os << rhs.x의 결과로 os를 반환하여 다음 인자인 ", "과 연산 수행 후 os 반환을 반복한다.

<br>

```C++  
std::ostream& operator<<(std::ostream& os, const Vector& rhs)
{
  os << rhs.x << ", " << rhs.y << std::endl;
  return os;
}
```

<br>
<br>

멤버 함수로 연산자를 오버로딩 하는 방법은 다음과 같다.

<strong> ex) '+' 연산자 오버로딩</strong>

```C++
Vector operator+(const Vector& rhs) const
{
   Vector ret;
   ret.mX = mX + rhs.mX;
   ret.mY = mY + rhs.mY;
   return ret;
} 

Vector sum = v1 + v2;
Vector sum = v1.operator+(v2);

cout << number;
```