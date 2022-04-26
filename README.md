# 프로젝트 개요
* 방 단위로 채팅이 가능한 채팅 서버 및 클라이언트 샘플
* boost asio를 사용해서 비동기적으로 처리
* accept 및 네트워크 처리와 로직처리 쓰레드 구분

# 프로젝트 필요환경
* cmake 3.10 이상
* protobuf (proto3 문법을 사용)
* boost asio 1.67 이상(io_context를 사용했고 테스트 환경은 1.69)
* linux
# 프로젝트 빌드

프로젝트 홈에서 아래와 같은 command를 수행한다.

```cmd
. proj.env `pwd`
./gen.sh
```


