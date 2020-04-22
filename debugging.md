안녕하세요.    
오늘은 VS2019 환경에서 디버깅 기능을 사용하는 방법에 대해 알아보겠습니다.

디버깅이란 프로그램 작성 중 발생한 에러를  찾고 고치는 과정을 말합니다. 개발자가 오류를 잡는데 도움을 주도록 보통 개발 툴마다 디버깅 기능을 지원하는데, 그중에서도 비쥬얼 스튜디오는 디버깅 기능이 뛰어나기로 유명합니다.

한 단계 한 단계 따라가며 VS의 디버깅 기능을 이용하다 보면 신기하기도 하고 한번 익혀두시면 많은 도움이 될 거라 생각합니다.

<br>

# 알아볼 것

- 기본적인 디버깅 방법
- 디버깅 중 메모리 영역 확인 (특정 주소에 어떤 값이 들었는지 눈으로 확인)
- 디버깅 중 고급언어로 작성된 소스코드를 저급 언어인 어셈블리어로 변환하여 보는 방법  (Disassemble)

<br>

# 목차

* [1\. 중단점 체크](#breakpoint)
* [2\. 디버깅 시작](#begin)
    - [2-1) 메모리 영역 확인](#memory)   
    - [2-2) Disassemble 기능 ](#disassemble)
* [3\. 디버깅 종료](#end)

<br>

# 환경

![image](https://user-images.githubusercontent.com/46551002/79944485-95bf4800-84a6-11ea-9cf2-9d51990bd63e.png)

<br>

**아래 소스 코드를 기준으로 진행하도록 하겠습니다. ( 편의를 위해 하나의 소스파일에 작성하였습니다. )  
<i>어차피 디버깅 방법을 설명하는 것이기 때문에 소스코드를 이해하실 필요는 없습니다.**</i>

```cpp
/*
        string 타입 벡터의 각 요소에 문자열을 입력받아 

        아래 그림과 같이 '*' 문자를 추가한 벡터를 생성 후 

        추가된 그림을 출력하는 프로그램입니다.
    
                     ***********
        hello        * hello   *   
        hi      =>   * hi      * 
        hola         * hola    * 
        bonjour	     * bonjour * 
                     ***********

        왼쪽의 각 row를 요소로 갖는 벡터를 오른쪽의 row를 요소로 갖는 벡터를 반환합니다
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>  // max()

using std::cout; using std::cin;
using std::vector; using std::string;

// 문자열을 입력받아 저장합니다
int read(vector<string>& words); 

// 가장 긴 문자열의 길이를 반환합니다
string::size_type getMaxLen(const vector<string>& words);

// 기존 벡터에 '*'를 추가한 벡터를 반환합니다
vector<string> frame(const vector<string>& words);

// 벡터의 요소들을 차례대로 출력합니다
int printRows(const vector<string>& words);


int read(vector<string>& words)
{
	string line;

	// EOF 문자나 다른 타입의 문자를 받을 때까지 
	// 문자열을 입력받아 words 벡터에 저장합니다
	while (std::getline(cin, line))
	{
		words.push_back(line);
	}
	
	return 0;
}

string::size_type getMaxLen(const vector<string>& words)
{
	string::size_type maxLen = 0;
	vector<string>::size_type size = words.size();

	// 가장 긴 요소의 길이를 얻습니다
	for (vector<string>::size_type i = 0; i != size; ++i)
	{
		maxLen = std::max(maxLen, words[i].size());
	}

	return maxLen;
}


vector<string> frame(const vector<string>& words)
{
	vector<string> ret;

	// 가장 긴 문자열의 길이를 구합니다
	string::size_type maxLen = getMaxLen(words);

	// 상단, 하단 테두리
	string border(maxLen + 4, '*');

	// 상단 테두리를 추가합니다
	ret.push_back(border);

	// 문자열에 '*' 를 추가합니다
	for (vector<string>::size_type i = 0; i != words.size(); ++i)
	{
		ret.push_back("* " + words[i] + string(maxLen - words[i].size() + 1, ' ') + "*");
	}

	// 하단 테두리를 추가합니다
	ret.push_back(border);

	return ret;
}

int printRows(const vector<string>& words)
{
	// 요소를 한 행씩 모두 출력합니다
	for (const string& tmp : words)
	{
		cout << tmp << "\n";
	}

	return 0;
}

int main(void)
{
	vector<string> words;

	// 벡터에 라인 단위로 문자열을 입력받습니다
	read(words);
	
	// 테두리를 추가합니다
	vector<string> paddedWords = frame(words);

	// 벡터 요소들을 차례대로 출력합니다
	printRows(paddedWords);

	return 0;
}
```

<br>

# 디버깅 1단계: 중단점 체크

먼저 디버깅을 하는 이유는 크게 3가지가 있죠.

1. 에러가 발생했지만 어딘지 전혀 모르겠다     - 프로그램 처음부터 끝까지 진행사항을 훑으며 찾아야겠다.
2. 에러가 발생했지만 어디 부분인지 알 것 같다 - 특정 부분을 확인해보고 싶다
3. 에러는 없는데 그냥 프로그램의 단계 단계를 따라가며 상세히 보고 싶다.

이를 위해 **특정 부분을 체크할만한 지점**이 필요합니다.

이 지점이 바로 좌측 회색 라인의 빨간 점들인 <b>중단점(Breakpoint)</b>입니다.

![image](https://user-images.githubusercontent.com/46551002/79944452-80e2b480-84a6-11ea-84af-6232b7b788b2.png)

<br>

마우스 좌클릭으로 라인별로 1개씩 여러 라인을 체크할 수 있고, 다시 클릭하게되면 해제가 됩니다. 디버깅을 시작하게 되면 **중단점이 체크된 첫 라인에 노란 화살표**가 나타나고 **프로그램이 일시 정지**하게 되는데, 이 때 해당 라인의 코드는 **아직 실행되지 않은 상태가** 됩니다.

즉, 위에서 **125라인의 read함수**의 실행 부분을 보고 싶거나 이 부분부터 천천히 보고 싶다면 **125라인에 중단점을 설정**하거나 그 위쪽 라인으로 설정하면 됩니다. 중단점을 설정하지 않고 디버깅을 하게 되면 프로그램이 멈출 부분이 없으니 끝까지 실행하고 알아서 종료하게 됩니다.
(의미가 없게 되죠)

<br>

# 디버깅 2단계: 디버깅 시작

위 처럼 중단점을 설정했으면 상단의 메뉴바를 봅니다.

![image](https://user-images.githubusercontent.com/46551002/79944694-2ac24100-84a7-11ea-9894-3be47ca40a92.png)

상단바에서   

**Debug - Start Debugging (F5)**

또는

**▶ Local Windows Debugger** 버튼을 클릭하게 되면 콘솔 창이 뜨면서 디버깅 모드에 진입하게 됩니다.

그러면 다음과 같이 **125라인의 중단점에 노란 화살표**가 나타납니다.

![image](https://user-images.githubusercontent.com/46551002/79944700-2e55c800-84a7-11ea-8616-5988d8399457.png)

아까 말했듯 디버깅을 시작하면    

(프로그램 흐름상) **첫 중단점에서 프로그램이** **일시 정지**되고 **아직 해당 라인의 코드를 수행하지 않은 상태**가 됩니다.

즉 main함수가 실행이 되고 122라인의 words라는 이름의 string 타입 벡터가 할당이 된 상태에서 **read 함수가 실행되기를 기다리고 있는 상태**입니다. 이제 다시 메뉴바를 보시면 다음과 같이 변경이 된 걸 볼 수 있습니다.

![image](https://user-images.githubusercontent.com/46551002/79944706-31e94f00-84a7-11ea-986b-ceb9080145fb.png)


<br>

***

<b>[잠깐 버튼 설명]</b>

- **빨간 정지 버튼**:  먼저 디버깅을 종료하고 싶을 땐 콘솔창의 X표를 눌러서 종료하지 말고 해당 버튼을 통해 디버깅을 정지시킨 뒤 콘솔창을 종료합니다.

- **Restart 버튼**: 정지버튼 옆에 새로고침 버튼과 비슷한 Restart 버튼은 디버깅을 재시작하는 기능입니다.   
( 저는 디버깅 중 코드 수정하고 restart 버튼 자주 사용합니다 )

- **빨간 박스의 첫 번째 화살표 버튼**: Step Into  (F11)  - 제일 작은 단위 실행 ( 한 단계씩 실행)

- **빨간 박스의 두번째 화살표 버튼**: Step Over (F10)  - 한 라인씩 실행 ( 프로시저 단위 실행 )

- **빨간 박스의 세번째 화살표 버튼**: Step Out   ( Shift + F11 ) - 프로시저 나가기 ( 그전에 중단점이 있다면 중단점에서 멈춤 )

***

이제 직접 버튼을 사용하면서 기능을 알아보도록 하겠습니다.

우선 아까와 같이 디버깅을 시작하셨다면 노란 화살표가 125Line (**read(words)**)을 가리키고 있을텐데요.
이 상태에서 1번 버튼인 Step Into 버튼을 누르면 현재 라인의 코드를 실행하게 됩니다. 

만약 현재 라인에 사용자가 정의한 함수(프로시저)가 있다면 함수 실행을 위해 그 함수의 내부로 들어가게 됩니다. read 함수는 우리가 직접 작성한 사용자 정의 함수이기 때문에 다음과 같이 read함수 내부로 이동하게 됩니다.

![image](https://user-images.githubusercontent.com/46551002/79945530-e3d54b00-84a8-11ea-9c94-50700d4bccd2.png)

위와 같이 프로그램이 read 함수 내부로 이동하여 화살표가 첫 행을 가리키고 있습니다. 이제 read함수의 실행단계를 차례로 보기 위해 **Step Into(F11)** 또는 **Step Over(F10)** 를 클릭합니다.



    Step Into 버튼은 아까 말했듯이 현재 라인을 실행하되, 사용자 정의 함수가 있다면 그 함수 안으로 이동합니다. 반면 Step Over 버튼은 함수 안으로 들어가지 않고 그냥 라인 단위로 실행해버립니다.


<br>

read 함수 내부엔 사용자 정의 함수가 없기 때문에 **Step Into**와 **Step Over** 모두 동일하게 라인 단위로 실행합니다. 

![image](https://user-images.githubusercontent.com/46551002/79945536-e768d200-84a8-11ea-9a6d-923a806cd454.png)

화살표가 37라인에 있을 때 한 번 더 버튼을 눌러 해당 라인을 실행하면 화살표가 사라지게 됩니다.

**std::getline** 은 문자열을 라인 단위로 입력받는 함수인데, **문자열을 입력받아야 다음 단계로 넘어갈 수 있게 됩니다.**
콘솔 화면에 **"hello"를 입력 후 엔터**를 누르게 되면 화살표가 **39라인**을 가리키게 될 것입니다.

<br>
다음으로,

**line**에 문자열이 잘 들어갔는지 확인하기 위해 변수 **line**에 마우스를 가져가서 왼쪽 화살표를 눌러보겠습니다.

![image](https://user-images.githubusercontent.com/46551002/79946358-c608e580-84aa-11ea-8442-7913abb78c30.png)

<br>
이처럼 디버깅 중에 어떤 변수에 저장된 값을 위와 같이 상세하게 확인할 수 있습니다. 또는 아래와 같이 **Autos 및 Locals 창**에서도 확인할 수 있습니다.

![image](https://user-images.githubusercontent.com/46551002/79946365-ca350300-84aa-11ea-8517-971bb822a7af.png)

<br>
이외에도 추가로 자신이 원하는 변수를 직접 입력하여 확인할 수 있는 <b>Watch(조사식)</b> 창도 있습니다. 

위 Autos 화면에서 **line에 오른쪽 마우스 버튼**을 클릭 후  **Add Watch** 또는 **조사식 추가**를 클릭합니다.

![image](https://user-images.githubusercontent.com/46551002/79946644-56dfc100-84ab-11ea-96c1-fa3d72eeb97a.png)

위와 같이 원하는 변수를 입력하여 값을 확인할 수 있으며 **해당 시점에서 할당이 안된 변수**는 **회색으로 표시**됩니다. 현재 39라인이 아직 실행되지 않은 상태이기에 words 벡터에 값이 할당되지 않은 상태입니다.
또한 현재 scope 내의 변수만 확인 가능합니다.



이와 비슷하게 line변수가 **메모리 상에 할당된 것**을 한번 직접 확인해보겠습니다.   

**Debug -  Windows - Memory - Memory1** 메뉴를 통해 메모리 화면을 띄웁니다.

![image](https://user-images.githubusercontent.com/46551002/79946791-a0c8a700-84ab-11ea-9eae-a77cde2b3ec5.png)

그러면 위와 같은 화면을 볼 수 있는데 **Address** 부분에 확인을 원하는 변수의 주소를 적어 해당 주소에 저장된 값을 볼 수가 있습니다. 이 때는 미리 해당 변수의 주소를 조사식에서 구해놓고 원할 때 주소를 직접 입력하여 접근합니다.   

<br>
또한 현재 화살표가 A 영역에 있다면 A 영역의 변수명으로도 접근할 수 있습니다. 즉 변수 value의 메모리를 확인하고 싶을 때, 위 Address 칸에 value를 입력하면 자동으로 value의 메모리 주소로 변환되어 보여줍니다. 다만 현재 main영역에 있는데 다른 함수 내의 변수명을 입력한다거나 하는 것은 안됩니다. 
<br><br>

먼저 line변수의 주소를 얻기 위해 **조사식 창**에서 **&line을 입력**하면 현재 line이 **할당된 주소 값이** 나오게 됩니다.  
저는 **0x012ffc04**에 line 변수가 할당이 되어있습니다. ( **컴퓨터마다 다름** )  
또는 line을 Address 칸에 직접 입력합니다.

![image](https://user-images.githubusercontent.com/46551002/79946798-a45c2e00-84ab-11ea-98ac-cdc07ab2ed1d.png)

<i>"hello" 각 문자가 아스키코드 16진수로 68 65 6c 6c 6f 에  해당</i>

<br>
또는 아래와 같이 유니코드로도 확인 가능합니다.

![image](https://user-images.githubusercontent.com/46551002/79946807-a9b97880-84ab-11ea-8456-301b03a7b396.png)

  
이 메모리 직접 확인하는 기능 쓸 데 없다고 생각하실 수 있는데, 익혀두시면 무조건 도움됩니다.  
디버깅하면서 메모리가 제대로 할당되는지 제대로 해제되는지 어떻게 저장되는지 등등

<br>  
이제 값을 확인했으니 계속 이어서 진행 해보겠습니다.

![image](https://user-images.githubusercontent.com/46551002/79948016-fdc55c80-84ad-11ea-8a8d-a42595469dfe.png)

현재 화살표는 39라인을 가리키고 있고, 아직 words 벡터에 입력받은 문자열을 저장하지 않은 상태입니다.

이제 **Step Into**를 누르면 words 벡터에 "hello" 문자열이 할당됩니다. **Step Into**를 계속 누르면 다시 **while문의 첫 행**으로 가게 되고, while문이 조건을 만족하지 않을 때까지 반복하게 됩니다. 해당 while문의 종료 조건은 **EOF문자를 읽거나** 벡터의 타입과 **다른 타입의 값을 받을 때** 종료하게 됩니다.

문자열을 적당히 입력 후 다음 문자열 입력 시 다음과 같이   

**윈도우 기준 Ctrl+Z(^Z = EOF) 를** 입력하면 while문을 벗어나면서 read함수를 종료할 수 있게 됩니다.

![image](https://user-images.githubusercontent.com/46551002/79951390-8e526b80-84b3-11ea-87df-cffe9a7ec7d7.png)

<br> 이제 **Step Into**를 클릭해 read 함수를 빠져나갑니다.

![image](https://user-images.githubusercontent.com/46551002/79951399-927e8900-84b3-11ea-9a46-a36794c5940d.png)


<br>

read 함수를 빠져나가면 read 함수를 호출했던 main 함수의 125라인으로 돌아오게 됩니다. 이제 **Step Into** 또는 **Step Over** 클릭 시 128라인으로 넘어가게 됩니다.

![image](https://user-images.githubusercontent.com/46551002/79951560-dec9c900-84b3-11ea-8ba7-68a5dc8e9f1b.png)

128라인에서

- **Step Into**를 누르게 되면 **frame 함수 내부**로 들어가게 되고   

- **Step Over** 클릭 시 frame 함수의 **상세 실행은 스킵** 후 **131라인으로 넘어가고,**    

- **Step Out** 클릭 시 현재 함수를 종료해야 하지만, 135라인에 중단점이 있기 때문에 **135라인에서 멈추게** 됩니다.

**Step Into**를 통해 함수를 상세 실행하는 방법은 아까 살펴보았으니, frame 함수는 **Step Over**로 상세 실행을 스킵하고 **131라인**으로 넘어가 보겠습니다.

![image](https://user-images.githubusercontent.com/46551002/79951827-6283b580-84b4-11ea-859c-0719004e88c2.png)

<br>콘솔에 차례로 출력되는 결과를 보기 위해 131라인에서 **Step Into**를 눌러 **printRows 함수로 이동**해보겠습니다.

![image](https://user-images.githubusercontent.com/46551002/79951837-66afd300-84b4-11ea-943c-9618061c5d88.png)

<br>

## 컴파일된 코드를 어셈블리 코드로 보기

**[< 스킵하기 >](#skip)**

**여기서 행을 출력하기 전에 잠깐 Disassemble 기능을 아주 얕게 소개하겠습니다.**

**Disassemble**은 **C++(고급언어)로 작성된 언어를 Assembly(저급 언어)** 로 볼 수 있게 해주는 기능입니다.

어셈블리어를 배웠다면 더 상세하게 내부적으로 어떤 과정을 거치는지 확인할 수 있습니다.

소스코드 작성 화면에서 **마우스 오른쪽 버튼 클릭 - Go to disassembly (Alt + G)** 를 클릭 

![image](https://user-images.githubusercontent.com/46551002/79952224-0ec59c00-84b5-11ea-844a-a06f1e5c6fb6.png)

**현재 실행하고 있는 라인의 코드**와 그 코드를 **어셈블리어로 나타낸 것**입니다. 여기서도 노란색 화살표가 보이는데 마찬가지로 **Step 3가지 버튼**으로 이동하며 볼 수 있습니다. 저도 어셈블리 코드 해석은 잘 못하기 때문에 이러한 기능이 있다고만 말씀드리고 넘어가겠습니다.

Disassembly를 닫고 다시 본문으로 돌아가 행을 출력해보도록 하겠습니다.

---

<br>

**Step Into** 또는 **Step Over로** **114라인까지** **이동합니다.**

![image](https://user-images.githubusercontent.com/46551002/79952354-446a8500-84b5-11ea-9cea-72a790f90d11.png)

<br>이제 **Step Into** 또는 **Step Over**로 114라인을 실행하게 되면 **콘솔 창에 결과가 한 줄 출력**되는 걸 볼 수 있습니다.

![image](https://user-images.githubusercontent.com/46551002/79952369-49c7cf80-84b5-11ea-84ab-91fdd1893739.png)

만약 여기서 **Step Out**으로 함수를 종료한다면 나머지 문자열이 한 번에 출력됩니다.


<br>저희는 **Step Into** 또는 **Step Over**로 한 줄씩 끝까지 출력해보겠습니다.

![image](https://user-images.githubusercontent.com/46551002/79952485-7845aa80-84b5-11ea-9fb6-c9adc6a66910.png)


<br>이제 printRows 함수를 빠져나가고 main 함수로 돌아옵니다.

![image](https://user-images.githubusercontent.com/46551002/79952498-7e3b8b80-84b5-11ea-962d-bc56aac75005.png)

<br>

# 디버깅 3단계: 디버깅 종료

**이제 Step Out을** 두번 누르면 다음과 같이 프로그램이 정상적으로 끝나며 디버깅 모드가 종료됩니다.

![image](https://user-images.githubusercontent.com/46551002/79952655-b5aa3800-84b5-11ea-83fd-4a6be6830024.png)

여기까지 기초적인 디버깅 방법을 한번 알아보았습니다. 이정도면 웬만한 프로그램은 모두 디버깅할 수 있습니다.

이 외에도 디버깅 도중 변수 값 바꿔서 실행해보기, 실행 단계 back 등 다양한 기능들이 있습니다. 더 자세한 내용은 MS홈페이지를 참조하시는 게 좋을 것 같습니다.

[https://docs.microsoft.com/ko-kr/visualstudio/debugger/getting-started-with-the-debugger-cpp?view=vs-2019#edit-code-and-continue-debugging](https://docs.microsoft.com/ko-kr/visualstudio/debugger/getting-started-with-the-debugger-cpp?view=vs-2019#edit-code-and-continue-debugging)



<br>**코드 참고: Accelerated C++**