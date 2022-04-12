clang++ -std=c++11  -g -lpthread -lboost_system -lboost_thread -o `basename $@ .cc`.out $@
