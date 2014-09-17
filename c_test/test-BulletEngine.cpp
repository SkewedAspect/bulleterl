#include "BulletEngineTest.h"
#include "log.h"

#include <GLFW/glfw3.h>

#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>


class H3DView;
H3DView* view = NULL;

H3DRes modelRes;


union H3DNodeModel
{
	Model m;
	H3DNode n;
}; // end H3DNodeModel

static Model makeModelCB()
{
	H3DNodeModel nm;
	nm.n = h3dAddNodes(H3DRootNode, modelRes);
	h3dSetNodeFlags(nm.n, H3DNodeFlags::NoCastShadow, true);

	LOG("Created node %d.", nm.n);

	return nm.m;
} // end makeModelCB

static void updateModelCB(Model model, float* mat)
{
	H3DNodeModel nm;
	nm.m = model;

#	ifdef DEBUG_UPDATES
	LOG("Updating node %d: [%.3f %.3f %.3f %.3f] [%.3f %.3f %.3f %.3f] [%.3f %.3f %.3f %.3f] [%.3f %.3f %.3f %.3f]", nm.n,
			mat[0], mat[1], mat[2], mat[3],
			mat[4], mat[5], mat[6], mat[7],
			mat[8], mat[9], mat[10], mat[11],
			mat[12], mat[13], mat[14], mat[15]
			);
#	endif // DEBUG_UPDATES

	h3dSetNodeTransMat(nm.n, mat);
} // end updateModelCB


class H3DView : public BulletEngineTest
{
	//H3DNode model;
	H3DNode cam;

	H3DRes fontMatRes;
	H3DRes panelMatRes;
	H3DRes pipeRes;

	int frameNum;
	char* text;
	size_t textLen;

	double _rx;
	double _ry;
	double _prev_x;
	double _prev_y;
	bool _mouseEnteredThisFrame;

public:
	H3DView(int winWidth, int winHeight) :
			BulletEngineTest(),
			//model(0),
			cam(0),
			frameNum(0),
			text(NULL), textLen(32),
			_rx(15), _ry(-50),
			//_rx(0), _ry(0),
			_prev_x(0), _prev_y(0),
			_mouseEnteredThisFrame(false)
	{
		assert(view == NULL);
		view = this;

		text = new char[textLen];
		memset(text, 0, textLen);

		LOG("Initializing Horde3D...");
		h3dInit();

		h3dSetOption(H3DOptions::MaxAnisotropy, 8);
		h3dSetOption(H3DOptions::SampleCount, 16);

		//h3dSetOption(H3DOptions::WireframeMode, 1);
		//h3dSetOption(H3DOptions::DebugViewMode, 1);
		h3dSetOption(H3DOptions::GatherTimeStats, 1);

		LOG("Declaring resources...");

		pipeRes = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);

		H3DRes skyBoxRes = h3dAddResource(H3DResTypes::SceneGraph, "models/skybox/skybox.scene.xml", 0);

		//modelRes = h3dAddResource(H3DResTypes::SceneGraph, "models/AssJacket/AssJacket.scene.xml", 0);
		//modelRes = h3dAddResource(H3DResTypes::SceneGraph, "models/ares/ares.scene.xml", 0);
		modelRes = h3dAddResource(H3DResTypes::SceneGraph, "models/man/man.scene.xml", 0);

		fontMatRes = h3dAddResource(H3DResTypes::Material, "overlays/font.material.xml", 0);
		panelMatRes = h3dAddResource(H3DResTypes::Material, "overlays/panel.material.xml", 0);

		LOG("Loading resources from disk...");
		h3dutLoadResourcesFromDisk("/home/whitelynx/devel/other/Horde3D/build-make/Binaries/Content"
				"|/home/whitelynx/devel/skewedaspect/precursors-client/resources/content");

		LOG("Building scene...");
		/*
		// Add model to scene
		model = h3dAddNodes(H3DRootNode, modelRes);
		h3dSetNodeTransform(model,
				0, 0, -500,  // Translation
				0, 0, 0,     // Rotation
				1, 1, 1      // Scale
				);
		*/

		// Add light source
		H3DNode light = h3dAddLightNode(H3DRootNode, "Light1", 0, "LIGHTING", "SHADOWMAP");
		// Set light position and radius
		h3dSetNodeTransform(light,
				0, 20, 0,
				0, 0, 0,
				10, 10, 10
				);
		h3dSetNodeParamF(light, H3DLight::RadiusF, 0, 50.0f);

		LOG("Setting up camera...");
		cam = h3dAddCameraNode(H3DRootNode, "Camera", pipeRes);

		resizeViewport(winWidth, winHeight);

#if 1
		H3DNode sky = h3dAddNodes(H3DRootNode, skyBoxRes);
		h3dSetNodeTransform(sky,
				0, 0, 0,      // Translation
				0, 0, 0,      // Rotation
				210, 50, 210  // Scale
				);
		h3dSetNodeFlags(sky, H3DNodeFlags::NoCastShadow, true);
#endif

		LOG("Setting BulletEngine callbacks...");
		setMakeModelCallback(&makeModelCB);
		setUpdateModelCallback(&updateModelCB);

		LOG("Initializing physics...");
		initPhysics();

		LOG("Ready.");
	} // end H3DView

	~H3DView()
	{
		// Release engine
		h3dRelease();
	} // end ~H3DView

	void resizeViewport(int width, int height)
	{
		// Set up viewport and render target sizes
		h3dSetNodeParamI(cam, H3DCamera::ViewportXI, 0);
		h3dSetNodeParamI(cam, H3DCamera::ViewportYI, 0);
		h3dSetNodeParamI(cam, H3DCamera::ViewportWidthI, width);
		h3dSetNodeParamI(cam, H3DCamera::ViewportHeightI, height);

		h3dSetupCameraView(cam, 45.0f, float(width) / height, 0.1f, 16384.0f);

		h3dResizePipelineBuffers(pipeRes, width, height);
	} // end resizeViewport

	void mouseMoved(int x, int y)
	{
		if(_mouseEnteredThisFrame)
		{
			_prev_x = x;
			_prev_y = y;
			_mouseEnteredThisFrame = false;
			return;
		} // end if

		float dx = x - _prev_x;
		float dy = _prev_y - y;

		// Look left/right
		_ry -= dx * 0.3f;

		// Loop up/down but only in a limited range
		_rx += dy * 0.3f;

		if(_rx > 90)
		{
			_rx = 90;
		} // end if

		if(_rx < -90)
		{
			_rx = -90;
		} // end if

		LOG("_rx, _ry = %f, %f", _rx, _ry);

		_prev_x = x;
		_prev_y = y;
	} // end resizeViewport

	void mouseEntered(int entered)
	{
		_mouseEnteredThisFrame = (bool) entered;
	} // end mouseEntered

	void gameLoop()
	{
		frameNum++;

		//LOG("doFrame()...");
		doFrame();

		h3dSetNodeTransform(cam,
				//0, 0, 0,
				0, 10, 10,
				_rx ,_ry, 0,
				1, 1, 1
				);

		//LOG("h3dRender(cam)...");
		h3dRender(cam);

		//LOG("h3dutShowFrameStats(fontMatRes, panelMatRes, 2)...");
		h3dutShowFrameStats(fontMatRes, panelMatRes, 2);

#if 0
		//int n = snprintf(text, textLen - 1, "Frame %d: %f ms", frameNum, msSinceLastFrame());
		//int n = snprintf(text, textLen - 1, "Frame %d", frameNum % 32);
		int n = snprintf(text, textLen - 1, "Frame %d", frameNum);

		LOG("Showing text \"%s\" (wrote %d chars) with color (%d, %d, %d)", text, n, frameNum % 2, (frameNum >> 1) % 2, (frameNum >> 2) % 2);
		h3dutShowText(text,
				0.5, 0.5,   // Position
				0.05,       // Size (scale)
				frameNum % 2, (frameNum >> 1) % 2, (frameNum >> 2) % 2, // Color (RGB)
				fontMatRes  // Font material
				);
#endif

		//LOG("h3dFinalizeFrame()...");
		h3dFinalizeFrame();

		//LOG("Frame finished.");
	} // end gameLoop

	static void error_callback(int error, const char* description)
	{
		fputs(description, stderr);
	} // end error_callback

	/*
	static void windowClose_callback(GLFWwindow* window)
	{
	} // end windowClose_callback
	*/

	static void windowResize_callback(GLFWwindow* window, int width, int height)
	{
		H3DView* view = static_cast<H3DView*>(glfwGetWindowUserPointer(window));

		if(view)
		{
			view->resizeViewport(width, height);
		} // end if
	} // end windowResize_callback

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		} // end if
	} // end key_callback

	static void cursorPos_callback(GLFWwindow* window, double x, double y)
	{
		H3DView* view = static_cast<H3DView*>(glfwGetWindowUserPointer(window));

		if(view)// && view->_running)
		{
			view->mouseMoved(x, y);
		} // end if
	} // end cursorPos_callback

    static void cursorEnter_callback(GLFWwindow* window, int entered)
	{
		view->mouseEntered(entered);
	} // end cursorEnter_callback

}; // end H3DView


int main(int argc, char** argv)
{
	glfwSetErrorCallback(H3DView::error_callback);

    if(!glfwInit())
	{
		exit(EXIT_FAILURE);
	} // end if

	GLFWwindow* window = glfwCreateWindow(1024, 768, "h3ddemo", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
		exit(EXIT_FAILURE);
    } // end if

    glfwMakeContextCurrent(window);

	H3DView* view = new H3DView(1024, 768);
	glfwSetWindowUserPointer(window, view);

    //glfwSetWindowCloseCallback(window, H3DView::windowClose_callback);
    glfwSetWindowSizeCallback(window, H3DView::windowResize_callback);
	glfwSetKeyCallback(window, H3DView::key_callback);
	glfwSetCursorPosCallback(window, H3DView::cursorPos_callback);
    glfwSetCursorEnterCallback(window, H3DView::cursorEnter_callback);

	while(!glfwWindowShouldClose(window))
	{
		view->gameLoop();

		glfwSwapBuffers(window);

		glfwPollEvents();
	} // end while

	glfwDestroyWindow(window);

    glfwTerminate();
	exit(EXIT_SUCCESS);
} // end main
