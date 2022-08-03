# KOSS-BootCamp-PyQt5
<h3>KOSS BootCamp 5주차 - PyQt</h3>
<br/>
멘토 - 윤민우, 박진우, 이준혁
<br/>

## What is PyQt5?  

### 기존의 파이썬은?
기존의 파이썬으로는 TUI(Text-based User Interface), 즉 결과를 text로만 출력을 하는 방식으로 결과를 출력해왔다.
```python
print("Hello World!")
Hello World!
```

### GUI?
GUI(Graphical User Interface), 즉 우리가 사용하는 윈도우 창과 같은 그래픽 요소를 통해 사용자와 컴퓨터 간의 인터페이스를 구현한 방식이다. wxPython, TkInter, PyQt 등의 GUI 모듈일 있지만, 우리는 PyQt를 사용해보고자 한다.

### PyQt의 특징
- TkInter와는 다르게 추가적인 모듈 설치가 필요하다. [설치 방법](https://github.com/ymw0407/KOSS-BootCamp-PyQt#how-to-install-pyqt5) 
- Qt라는 GUI 프로그램 개발에 널리 사용되는 크로스 플랫폼 프레임워크(C++)를 Python으로 바인딩한 것이 PyQt!
- 여기서 크로스 플랫폼 프레임워크란... 윈도우나 리눅스와 같은 운영체제에 상관없이 같은 코드로 각 운영체제에서 동작하는 프로그램 개발을 지원하는 것을 의미한다!

## How to install PyQt5?
```bash
$ pip install PyQt5
```
### Run 했을 때 다음과 같은 오류가 발생한다...
```python
qt.qpa.plugin: Could not load the Qt platform plugin "xcb" in "" even though it was found.
```
위와 같은 오류가 발생하면, 다음과 같이 설치해주면 된다.
```bash
$ apt install libxcb-xinerama0
```
-------------------------------------------------------------------------------------------------------------------------
# PyQt에서 Matplotlib 사용하기

```bash
pip install matplotlib
```

### [링크](https://codetorial.net/articles/matplotlib_in_pyqt5.html)


# MongoDB와 Python 연결하기!

## pymongo와 MongoClient 설치

```bash
$ pip install pymongo
$ pip install dnspython
```

## pymongo 실습
```python
from pymongo import MongoClient

client = MongoClient("mongodb+srv://jjunh33:aofhs31@cluster0.iwc5u.mongodb.net/?retryWrites=true&w=majority")

db = client['test'] # test라는 이름의 데이터베이스에 접속

for d, cnt in zip(db['sensors'].find(), range(10)):
    print(d['pm2'])
```
DB에 데이터 넣는 실습을 안해보신 분은 우선 제것을 사용하여 실습하셔도 좋습니다!
<br/>
<br/>

[MongoDB](https://account.mongodb.com/account/login)에 로그인

<br/>

### MongoClient url 찾기!

![img](mongodb.png)

Cluster0 옆에 있는 Connect 클릭 <br/>
-> Connect your application 클릭 <br/>
-> mongodb+srv://yun1211:\<password\>@cluster0.qypul.mongodb.net/\<dbname\>?retryWrites=true&w=majority과 같은 형태의 링크 복사<br/>
-> \<password>를 자신의 비밀번호로 수정 & \<dbname\>을 사진의 Cluster0과 같은(대소문자 주의) 데이터베이스 이름으로 수정<br/>
```python
client = MongoClient("<여기에 넣으세요!>")
```
### Database 이름과 collections 이름 찾기!

![img](database.png)

Connect의 왼쪽에 있는 Cluster0 클릭 <br/>
-> Collections 클릭 <br/>
-> 사진에 보이는 Sensor, test와 같은 것들이 데이터 베이스 이름
```python
db = client['<여기에 넣으세요!>']
```
-> 사진에 보이는 BME280, bme280, sensors와 같은 것들이 collections의 이름
```python
for d, cnt in zip(db['<여기에 넣으세요!>'].find(), range(10)):
```
### Key 값 찾기!

![img](key.png)

사진에 보이는 tmp, hum, alt 등등의 것들이 key값, 그리고 그 옆에 있는 값들이 value값

```python
    print(d['<여기에 넣으세요!-1>'], d['<여기에 넣으세요!-2>'], d['<여기에 넣으세요!-3>'])
```

