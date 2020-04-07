## [시작하기 전에]
프로그래밍 언어엔 메모리를 프로그래머가 직접 관리해주는 unmanaged 언어와 언어단에서 관리해주는 managed 언어가 있다. 대표적인 예로 C/C++가 unmanaged 언어이며 C#, Java가 managed 언어이다.

대부분의 매니지드 언어에선 자동으로 메모리 관리를 하기 위해 다음 중 하나의 기법을 사용한다. 
(해당 포스팅에선 스마트 포인터를 다루기 위한 사전 지식을 위해 간단한 개념만 정리합니다.)

- Garbage collection (가비지 컬렉션 또는 쓰레기 수집)
- Reference counting (레퍼런스 카운팅 또는 참조 횟수)

<br>

## [Garbage collection]
가비지 컬렉션을 한국말로 굳이 번역하면 쓰레기 수집인데, 사용되지 않는 메모리를 언어단에서 수집하여 지운다는 의미로 보면된다. 여기서 메모리는 힙 메모리를 뜻한다. 지역 변수와 같이 스택 메모리에 할당되는 메모리는 자신의 scope를 벗어나면 사라지는 특성을 가지기 때문에 해제에 대한 신경을 써주지 않아도 된다.

반면 heap에 할당된 메모리를 생각해보면, 해당 메모리를 가리키는 포인터 또는 참조 변수가 사라진다고 heap에 할당된 메모리가 자동으로 해제되지 않는다. C/C++의 경우 프로그래머가 직접 delete 또는 free를 통해 해제를 해주어야 했다. 하지만 이와 다르게 메모리 관리를 자동으로 해주어야 하는 C#, JAVA 같은 언어에선 누군가가 사용되지 않는 힙 메모리를 추적하고 해제 해주어야 하는데, 이 역할을 가비지 컬렉터(Garbage collector)가 해주는 것이다. 

가비지 컬렉션은 프로그램 실행 중에 일정 주기마다 또는 특정 조건일 경우 (힙 메모리가 충분치 않을 경우)에 스택과 같이 변수가 저장되는 곳(root)를 훑으면서 사용되지 않는 메모리를 추적하고 지운다. 사용되지 않는 메모리의 기준은 "root로부터 어떻게든 접근될 수 있냐" 이다. 간단하게 그림으로 표현하면 다음과 같다.

![image](https://user-images.githubusercontent.com/46551002/78681552-dd32c980-7927-11ea-9139-237bdc135434.png)


출처: https://www.telerik.com/blogs/understanding-net-garbage-collection   <br><br>


위 그림의 Before에서 Root로부터 직접적으로 접근될 수 있는 오브젝트(메모리)는 A, C, E, F이다. 그리고 A, C는 각각 B, G를 가리킨다. 반면 D, H는 누구로부터도 접근될 수 없다. 따라서 D와 H는 가비지 컬렉션의 대상이 되며, 다음에 가비지 컬렉션이 실행될 때 D, H는 지워질 것이다.

하지만 가비지 컬렉션에는 단점이 존재한다. 일단 가비지 컬렉터는 실시간으로 사용되지 않는 메모리를 지워주는 것이 아니다. 위에서 설명했듯이 주기적으로 돌면서 확인하고 지우는 것이기 때문에 사용되지 않는 메모리가 지워지지 않고 충분히 남아있을 수 있다. 이 말은 프로그램이 종료될 때까지 사용되지 않는 메모리가 남아있을 수 있다는 말이다. 따라서 가비지 컬렉터는 메모리 누수를 완벽하게 막지 못할 수 있다. 또한 가비지 컬렉터의 내부적인 알고리듬 구현이 효율적이지 못하다던지, 최적화가 어느정도 되어있지 않다면 가비지 컬렉터가 실행될 때 프로그램의 성능 저하 또는 속도 저하가 발생할 수 있다.
(C#의 가비지 컬렉션 최적화 https://docs.microsoft.com/ko-kr/windows/uwp/debug-test-perf/improve-garbage-collection-performance)

예를 들어 안드로이드 환경에서 애플리케이션을 사용해본 경험이 있다면 가끔가다 버벅이는 현상을 겪어봤을 것이다. 물론 기본적으로 안드로이드는 JVM이라는 가상 환경에서 실행되기 때문도 있지만, Java가 가비지 컬렉션을 사용하기 때문에 주로 Java로 작성되는 안드로이드 App의 경우 실행 도중 가비지 컬렉션이 실행되어 잠시 버벅일 수 있다는 것이다. 반면 아이폰을 사용해본 사람이라면 앱 사용 도중 버벅임을 잘 느껴보진 못했을 것이다. ios 앱을 작성하는 데 사용되는 objective c와 swift는 가비지 컬렉션이 아닌 아래에서 소개할 참조 카운팅이라는 기법을 사용하기 때문이다. 잠시 후 살펴보겠지만, 참조 카운팅은 가비지 컬렉션처럼 어느 한 순간에서 팍 느려지는 구조가 아니기 때문에 버벅임이 없고 부드럽게 보일 수 있다.  

<br>
 

## [Reference counting]
Reference counting은 참조 횟수 또는 참조 카운트정도로 번역할 수 있다. 참조 카운팅의 동작 방식은 간단하다. 동적으로 할당된 메모리 주소가 참조될 때마다 count를 1 증가시키고, 참조를 끊을 땐 1 감소시킴으로써 count를 체크하여 0이 될 경우(더 이상 참조되지 않는 메모리) 메모리를 해제하는 방식이다. 스마트 포인터의 std::shared_ptr가 참조 카운트 방식으로 동작하므로 해당 개념을 알아두면 좋다.

![image](https://user-images.githubusercontent.com/46551002/78681762-1a975700-7928-11ea-923e-b4e72fff85f6.png)


출처: http://assets.iosappsdev.org/objective-c/tutorials/objective-c/memory-management.html


<br>

예를 들어 포인터 p에 동적으로 할당한 주소 0x0f를 저장했다고 생각해보자. 이 경우 해당 주소를 참조하는 포인터는 p 하나이기 때문에 참조 횟수는 1이 된다. 

```cpp
void foo()
{
   int* p = new int(5); // reference count: 1
}
```

<br>

만약 p2에 p를 복사하면 p와 별개로 p2도 해당 주소를 참조하므로 참조 카운트를 1 증가시킨다.

```cpp
void foo()
{
   int* p = new int(5); // reference count: 1
   int* p2 = p;         // reference count: 2
}
```

<br>

만약 p2와 p가 해당 scope를 벗어나 스택에서 사라졌다고 생각해보자. 우리가 알던 방식대로라면 할당한 메모리 주소를 해제하지 않고, 해당 메모리를 가리키는 포인터를 잃어버렸으므로 이는 명백한 메모리 누수이다. 그러나 참조카운트 방식에선 자신을 참조하는 포인터의 개수를 어딘가에 저장하고 있기 때문에 p와 p2가 사라지면 count가 0이 됨과 동시에 자신은 더 이상 사용되지 않는 메모리라 판단하고 지워버린다. 

```cpp
void foo()
{
   int* p = new int(5); // reference count: 1
   int* p2 = p;         // reference count: 2
   
   // 함수가 종료되면 p와 p2는 사라지고 count 또한 0으로 감소한다.
   // count가 0이므로 new로 할당된 메모리 주소를 지운다.
}
```

<br>
따라서 메모리 누수도 없으며, 일정 주기마다 한번 실행되어 조사하고 해제하는 가비지 컬렉터와 달리 한 순간에서 큰 오버헤드가 생기진 않는다. 다만 참조 카운트 방식은 매번마다 count를 증가, 감소시키고 0인지 체크하기 때문에 매번 발생하는 조그만 오버헤드가 더해질 순 있다. 즉 가비지 컬렉터가 한 순간에 큰 오버헤드를 발생시켜 버벅임을 느끼게 만들었다면, 참조 카운트는 큰 오버헤드를 균등하게 분배시켜 버벅임을 못 느끼도록 하는 것이라고 볼 수 있다.

<br><br>    

## [참조 카운트를 사용한 자동 메모리 관리 예제]

다음은 메모리(인스턴스)에 대한 참조 카운트를 관리하는 간단한 클래스 예제이고 모든 함수를 살펴보진 않을 것이다. 메모리 주소가 참조되는 횟수를 어떻게 증가, 감소시키고 체크 및 해제하는지 확인하면 된다.

<br>(아래 코드들은 다음 책의 코드를 참고해서 작성했음)   
http://libertar.io/lab/wp-content/uploads/2017/03/Andrew-Koening-and-Barbara-E.-Moo-Accelerated-C-.pdf


```cpp
template <class T>
class RefHandle
{
public:
    RefHandle();
    RefHandle(T* ptr);
    RefHandle(const RefHandle& other);
    ~RefHandle();
    RefHandle& operator=(const RefHandle& rhs);
    
    inline size_t GetRefCount() const { return *mRefCount; };

    operator bool() const { return mPtr; }
    T& operator*() const;
    T* operator->() const;

private:
    T* mPtr;
    size_t* mRefCount;
};
```

위 클래스의 객체에 원하는 타입의 인스턴스를 바인딩하면 내부적으로 mPtr이 해당 인스턴스를 가리키게 된다. 즉 동적으로 할당한 메모리(인스턴스)를 직접 포인터 변수에 넣어서 사용하는 것이 아니라 RefHandle 클래스의 객체에 넣어 포인터를 사용하듯이 사용하면 내부적으로 해당 인스턴스를 관리할 것이다. 

<br>

다음은 특정 인스턴스를 저장한 RefHandle 클래스 객체가 복사될 때 참조 카운트가 어떻게 증가되는지 보여준다. 복사 생성자라면 해당 인스턴스를 새로운 객체가 추가로 가리키는 것이므로 참조 카운트를 1 증가시킨다. 할당 연산자가 불린다면 이미 가리키고 있던 인스턴스에 대한 참조를 끊고 새로운 인스턴스를 참조하는 것이다. 따라서 이전 인스턴스에 대한 참조 카운트를 1 감소시키고 만약 0이 된다면 인스턴스를 해제한다. 그리고 새로운 인스턴스를 가리키고 새로운 인스턴스에 대한 참조 카운트를 받고 1 증가시킨다.

```cpp
template <class T>
RefHandle<T>::RefHandle(const RefHandle& other)
    : mPtr(other.mPtr)
    , mRefCount(other.mRefCount)
{
    ++(*mRefCount);
}


template <class T>
RefHandle<T>& RefHandle<T>::operator=(const RefHandle& rhs)
{
    if (this == &rhs || mPtr == rhs.mPtr)
    {
        return *this;
    }

    if (mPtr && --(*mRefCount) == 0)
    {
        delete mPtr;
        delete mRefCount;
    }

    ++(*rhs.mRefCount);
    delete mPtr;
    delete mRefCount;

    mPtr = rhs.mPtr;
    mRefCount = rhs.mRefCount;

    return *this;
}
``` 


<br>   

다음은 RefHandle 객체가 소멸될 때 동작을 보여준다. RefHandle 객체가 소멸된다는 것은 특정 인스턴스를 참조하는 포인터가 1개 줄어든다는 뜻이다. 즉 참조 카운트가 1 감소하며, 만약 0이 된다면 해당 인스턴스를 지운다.

```cpp
template <class T>
RefHandle<T>::~RefHandle()
{
    if (--(*mRefCount) == 0)
    {
        delete mPtr;
        delete mRefCount;
    }
}
```

<br>

다음은 해당 객체를 포인터처럼 사용하기 위한 *, -> 연산자에 대한 오버로딩이다.
```cpp
template <class T>
T& RefHandle<T>::operator*() const
{
    assert(mPtr);

    return *mPtr;
}

template <class T>
T* RefHandle<T>::operator->() const
{
    assert(mPtr);

    return mPtr;
}
```

<br>

Blog: https://woo-dev.tistory.com/61
