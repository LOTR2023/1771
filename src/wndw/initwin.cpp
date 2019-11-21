static const int resX = 1280, resY = 720;

void initWindow(char* windowName) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWWindow* window;

    window = glfwCreateWindow(resX, resY, windowName, glfwGetPrimaryDisplay(), nullptr);

}
