VULKAN_SDK_PATH = /usr

CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -Isrc
LDFLAGS = -lglfw3 -lvulkan -ldl -lpthread
SOURCES = main.cpp src/render/vulkan/initvulkan.cpp src/render/vulkan/vulkan.cpp src/render/vulkan/cleanupvulkan.cpp src/window/initwin.cpp src/window/cleanupwin.cpp src/appData.cpp src/render/shaders/importshaders.cpp src/render/object/mesh.cpp

1771: main.cpp
	g++ -g $(CFLAGS) -o /bin/1771 $(SOURCES) $(LDFLAGS)

.PHONY: test clean

test: 1771
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib/x86_64-linux-gnu VK_LAYER_PATH=$(VULKAN_SDK_PATH)/share/vulkan/explicit_layer.d 1771
clean:
	rm -f /bin/1771
