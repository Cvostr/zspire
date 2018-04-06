#define DEFAULT_WIDTH 0
#define DEFAULT_HEIGHT 0

#define OPENGL3_3 33
#define OPENGL4_6 46
#define VULKAN 11

namespace ZSpire {

	struct ZSWindowDesc {
		unsigned int WIDTH;
		unsigned int HEIGHT;

		char WINDOW_TITLE[256];

		bool isResizable;
		bool isVsyncEnabled;
		bool isFullscreen;
		unsigned int OGL_VERSION;

		ZSWindowDesc() {
			WIDTH = DEFAULT_WIDTH;
			HEIGHT = DEFAULT_HEIGHT;

			isResizable = true;
			isFullscreen = false;
			isVsyncEnabled = false;
		}

	};

	class ZSpireApp {
	protected:
		ZSWindowDesc desc;

	public:
		char* getGpuNameGL();
		char* getGpuVendorGL();

		void PollEvents();
		void ZSDestroyWindow();
		void ZSDestroyApp();
		bool createWindow(ZSWindowDesc desc);
		void setWindowProperties(ZSWindowDesc desc);
		void MSGBox(const char* title, const char* message);
		void postFrame();

		void gl_clear();
	};
}