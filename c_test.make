SRCS = c_src/BulletEngine.c c_test/BulletEngineTest.c c_test/test-BulletEngine.c
OBJS = $(addprefix .objs/,${SRCS:.c=.o})

CXX = clang
CXXFLAGS = $(if $(DEBUG),-ggdb) -Wall -I./c_test -I./c_src -I/usr/include/bullet -I/usr/local/include
LDFLAGS = -Wall -lBulletDynamics -lBulletCollision -lLinearMath -lBulletSoftBody -lstdc++ -lHorde3D -lHorde3DUtils -lglfw


all: test-BulletEngine

test-BulletEngine: $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

.objs/%.o: %.cpp .objs
	mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) $< -o $@

.objs:
	mkdir -p .objs


clean:
	rm -f test-BulletEngine
	rm -rf .objs

.PHONY: all clean

# vim: ft=make
