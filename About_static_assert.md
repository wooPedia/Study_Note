# C++ 11에서 등장한 static_assert 키워드에 대해 알아보자.

<br>

기존 assert의 기능 자체에 대해선 간단한 소개만 할 것이니, assert에 대한 더 자세한 내용을 원하면 <a href="https://woo-dev.tistory.com/52" target="_blank"> 여기</a>를 클릭   

<br>




<br>

## [assert 기능 및 목적]
<br>

assert는 프로그램 개발 시 사용자의 실수를 개선해주기 위한 매크로로서, 어떤 조건을 인자로 받아 해당 조건이 거짓일 경우 프로그램을 중단시킨다. 이는 보통 프로그램 로직 상 일어나서는 안되는 조건을 인자로 넣어주어 프로그램 실행 시 assertion이 발생하면 그 조건을 찾아 디버깅 하는 것이다. assert는 프로그램 성능에 영향을 미치지 않으므로 많이 사용해도 상관없다.

<br>

```C++
void main()
{
   int n = 3;
   
   assert(n != NULL); // 조건이 거짓일 경우 프로그램 중지
}
```

<br>

## [static_assert]

그럼 static_assert는 무엇이 다를까?

<br>
static_assert는 이름 그대로 assert의 정적인 버전이다. 보통 static 키워드가 들어가면 컴파일 타임과 관련이 있다고 생각하면 된다. assert가 런타임에 조건을 검사하여 에러를 띄워줬다면 static_assert는 컴파일 시간에 검사하여 컴파일 에러를 발생시킨다.

<br>
<br>

그렇기 때문에 프로그램을 실행시키기 전 컴파일 때 미리 에러를 잡을 수 있다는 장점이 있다. 물론 컴파일 타임에 에러를 잡아야하기 때문에 런타임에 바인딩되는 것들은 검사하지 못한다. 그리고 실행 전에 바인딩되는 정적 바인딩과 런타임에 바인딩되는 동적 바인딩을 구분하는 것도 중요하기 때문에 static_assert를 사용함으로써 컴파일 타임과 런타임을 구분하는 것도 좋은 학습이 된다. 

<br>

또한 assert와 달리 static_assert는 메세지를 추가로 인자로 받는다. 따라서 메세지를 적절하게 작성하면 어떤 부분이 문제인지 더 쉽게 알 수 있다는 장점이 있다. 그리고 기존 assert는 debug 모드에서만 동작하지만 static_assert는 Release 모드에서도 동작한다. 


몇 가지 사용 예를 보며 사용법을 확인해보자.


## [Example]

### 1. 다음은 구조체의 크기를 체크한다.

```C++
struct StudentInfo
{
	char mStudentID[10];
	char mName[10];
	char mMajor[20];
	unsigned int mAge;
};

struct TeacherInfo
{
	char mTeacherID[10];
	char mName[10];
	unsigned int mAge;
 	string mTeacherAccount;
 	string mTeacherCardNumber;
};

void main()
{
	enum { STRUCT_MAX_SIZE = 64 };

	static_assert(sizeof(StudentInfo) <= STRUCT_MAX_SIZE,
		"Struct size must be smaller than 64byte");

	static_assert(sizeof(TeacherInfo) <= STRUCT_MAX_SIZE,
		"Struct size must be smaller than 64byte"); // 컴파일 에러
}
```
<br>
: 구조체의 최대 크기를 정의해놓고 크기를 체크한다.  StudentInfo 구조체는 크기가 64바이트 이하기 때문에 첫번째 assert문을 통과한다. TeacherInfo 구조체는 64바이트를 초과하기 때문에 다음과 같이 메세지와 함께 컴파일 에러를 발생시킨다.

#


### 2. 다음은 해당 타입이 <i><strong>*POD</strong></i> 객체 타입인지 확인한다. 

```C++
static_assert(is_pod<int>::value, "The type must be POD");
static_assert(is_pod<char>::value, "The type must be POD");
static_assert(is_pod<double>::value, "The type must be POD");
static_assert(is_pod<float>::value, "The type must be POD");
static_assert(is_pod<StudentInfo>::value, "The type must be POD");
static_assert(is_pod<string>::value, "The type must be POD"); // 컴파일 에러
```
<br>
: string 클래스는 POD 타입이 아니기 때문에 컴파일 에러가 발생한다.


<i>*POD(Plain Old Data): 연속된 주소로 이루어진 데이터 구조로, 보통 필드(속성)로만 이루어진 객체를 말함.</i>


# 

이처럼 static_assert는 잘 사용하면 사용자의 실수를 컴파일 전에 쉽게 막아줄 수 있다. 컴파일 타임에 체크가 가능한 것은 모두 static_assert를 사용하고 런타임에서 체크 가능한 것만 assert를 사용하는 것이 assert를 최대한 효율적으로 활용하는 방법이다.