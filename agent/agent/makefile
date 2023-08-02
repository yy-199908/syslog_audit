
all:clean libYYMysql.so agent
	./agent 127.0.0.1

libYYMysql.so: ../../YYMysql/*.cpp  ../../YYMysql/*.h
	g++ -fPIC -shared $^ -o $@ -I/usr/include/msyql -lmysqlclient
	cp $@ /usr/lib/


agent:agent.cpp YAgent.cpp YAgent.h
	g++ $^ -o $@ -I ../../YYMysql -lYYMysql
	

clean:
	rm -rf *.so
	rm -rf /usr/lib/libYYMysql.so
	rm -rf agent