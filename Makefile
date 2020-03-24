VULKAN_SDK_PATH = /usr

CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include/vulkan -Isrc
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib/x86_64-linux-gnu `pkg-config --static --libs glfw3` -lvulkan
SOURCES = main.cpp src/render/initvulkan.cpp src/render/vulkan.cpp src/render/cleanupvulkan.cpp src/window/initwin.cpp src/window/cleanupwin.cpp src/appData.cpp src/render/shaders/importshaders.cpp

1771_n: main.cpp
	g++ -g $(CFLAGS) -o /bin/1771_n $(SOURCES) $(LDFLAGS)

.PHONY: test clean

test: 1771_n
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib/x86_64-linux-gnu VK_LAYER_PATH=$(VULKAN_SDK_PATH)/share/vulkan/explicit_layer.d 1771_n
clean:
	rm -f /bin/1771_n
