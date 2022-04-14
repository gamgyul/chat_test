clang++ -std=c++17  -g -I$CHAT_HOME/src/include -I$CHAT_HOME/src/ -lpthread -lboost_system -lboost_thread -o `basename $1 .cc`.out $@
