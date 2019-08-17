namespace [base] {
	namespace [what] { // aka data type, renderer
		class [name] { // part of what
		struct [name] { // part of what
		enum [name]
		[return type] [name] ( [parameters] ) { [return or not] }
		 
		}
	}
}

// Future project layout, subject to change
namespace Raytracer { // this is in every file except main.cpp
	namespace Types { // this will be in files under DataTypes folder
		class PixelRGB {}; // raw pixel data that the internals works with and is what is used to render to the screen and rendered to file
	namespace RenderEngine { everything that will be under the RenderEngine folder
		class Screen  {}; // Handles writing to the screen, initializing of vulkan ect...
		class File {}; // Handles packing the file data getting it reading for writing
	}
	namespace Input {
		class Keyboard {};
		class Mouse {};
	}
	namespace System { // everything that is under System folder that deals with the low level stuff like console output and low level file interaction
		class Console {}; / handles wringing to the console, debug, error, warning, or plain messages
		class FileIO {}; // low level file io class
 } // 
}

class Object [renderable][nonrenderable]
class Sphere : public Object {}
class Scene {
	wrtie_to_file(std::string filename, std::vector<Pixel_t> data);
}
class Screen {}
class Renderer {
	trace(std::vector<Pixel_t>data, bool is_finale); // replace bool is_finale with render options???
	Scene scene;
	Scene screen;
}
			 namespace		namespace			class
??? so PlaceHolder::RenderEngine::Renderer renderer;
??? std::vector<Pixel_t> pixels;
??? renderer.trace(pixels, true);
??? renderer.scene.wrtie_to_file("scene.tga",pixels);
??? renderer.screen.draw( pixels);
[] refactor project layout

[complete]  Render to a file instead of stdout

[] Write a config class with json input/output in mind

[] Edit settings without having to recompile
[] Enable multithread rendering
[] Render to screen

Problem: changing the render size changes the camera's viewport ratio, which may not be the desired result.
