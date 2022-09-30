CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
all:
	chmod +X core/shaders/compile.sh
	bash core/shaders/compile.sh
	g++ $(CFLAGS) -o VulkanTest.exe *.cpp $(LDFLAGS) 
	./VulkanTest.exe
	rm -r VulkanTest.exe
	rm -r vert.spv
	rm -r frag.spv
