VULKAN_SDK_PATH = /usr
INCL_PATH_WIN = -Isrc

CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include/vulkan
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib/x86_64-linux-gnu `pkg-config --static --libs glfw3` -lvulkan
SOURCES = main.cpp src/rndr/initvulkan.cpp src/rndr/vulkan.cpp src/wndw/initwin.cpp

1771_n: main.cpp
	g++ -g $(CFLAGS) $(INCL_PATH_WIN) -o /bin/1771_n main.cpp $(LDFLAGS)

.PHONY: test clean

test: 1771_n
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib/x86_64-linux-gnu VK_LAYER_PATH=$(VULKAN_SDK_PATH)/share/vulkan/explicit_layer.d 1771_n
clean:
	rm -f /bin/1771_n
