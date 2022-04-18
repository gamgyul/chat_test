# 프로젝트 계획서
boost asio를 이용한 간단한 채팅 프로그램 개발

## 기능
아래와 같은 간단한 기능을 개발하고자함.
1. 유저명을 가지고 접속
2. 방생성, 방접속
3. 방에서 대화


## 메세지 프로토콜
필요한 메세지를 기능으로 정리
|Request|Reply|
|---------------------|-----|
|연결요청| |
|방생성요청| 
|방목록요청|
|방접속요청|
|메세지 send|
||메세지 받음|

간단하게 구글 프로토를 이용해서 헤더 바디로 나누어서 사용하려고함.
프토로콜 내역은 google proto문서를 만들어서 사용하고자함.

#


## 서버 구조

### 쓰레드 구조
* Acceptor 쓰레드. client connection 을 받는 쓰레드
* 메세지를 받는 스레드
* 받은 메세지를 처리하는 스레드(Room내에서 동기화를 위해 Room에 비례해서 할당)



<!-- ### Message
헤더 및 Body에 대한 Read Write을 하는 기능을 하는 클래스를 만들려고 함.
### MsgReadWriter
가져온 메세지의 프로토콜에 따라 동작을 수행하는 클래스
Body를 serialize, deserialize 하는것과 같은 동작. -->

## TODO
* 메세지 클래스
* ~~구글 protocol buffer 적용해보기~~  적용 완료.



* proj.env 파일 python으로 변경해보기
### LOG
* Log 현재 사용하고 있는 temp cout 대신 boost log 적용해보기.
* Log 큐 생성해서 log write 스레드 생성

### Asio
* 복수의 io_context가 아닌 Strand  사용해서 동기화 맞춰보기.
* deadline_timer 사용해서 주기적으로 Timeout 루프 만들기.