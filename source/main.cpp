#define GLEW_STATIC
#include "Light.h"
#include "events.h"
#include <iostream>
#include <string>
#include "Shader.h"
#include "Animation.h"
#include "freeLookCamera.h"
#include "objectDraw.h"
#include "Texture.h"
#include "torus.h"
#include "Sphere.h"
#include "skyBox.h"
#include "model.h"

glm::vec3 Light_2_position;

// main function
int main(int argc, char** argv)
{
	system("color 1");

	if (!glfwInit()) {
		std::cout << "initialization failed!" << std::endl;
		return 0;
	}
	unsigned int width = 1280, height = 720;

	GLFWwindow* m_window = glfwCreateWindow(width, height, "SOLAR SYSTEM PROJECT", nullptr, nullptr);

	int max, min, r;
	glfwGetVersion(&max, &min, &r);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, max);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);
	glfwMakeContextCurrent(m_window);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
		std::cout << "error" << std::endl;
	}

	glfwSetErrorCallback((GLFWerrorfun)[](int error_code, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
		});

	events dispachter = events(m_window);

	freeCamera scene_camera = freeCamera();
	scene_camera.Position = glm::f32vec3(-7800.0f, 700.0f, 27000.0f);

	glm::mat4 projection = glm::infinitePerspective(glm::radians(45.0f), 16.0f / 9.0f, 1.1f);
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 1.0f, 0.25f)), view = glm::mat4(1.0f);

	shaderCompilation Light_shader = shaderCompilation("shaders/vertex.glsl", "shaders/fragment.glsl", "Lighting Shader");
	Light_shader.Bind();

	Light PointLight = Light(glm::vec3(1.0f), "light[1]", 0.97f, 0.08f, 0.1f);
	PointLight.set_position(glm::vec3(0));
	PointLight.set_attenuation_constants(1.0f, 1.0f, 0.5f, 0.44f);

	Light_shader.Bind();
	Light_shader.setUniformMat4("projection", projection);

	float deltaTime, currentFrame, lastFrame = 0;

	GL_ERROR_MACRO(glEnable(GL_DEPTH_TEST));
	GL_ERROR_MACRO(glDepthFunc(GL_LEQUAL));

	Sphere sphere = Sphere(1.0f, 64, 64, true);
	Object sp = Object((float*)sphere.getVertices(), sphere.getVertexSize(), (float*)sphere.getNormals(), sphere.getNormalSize(),
		(float*)sphere.getTexCoords(), sphere.getTexCoordSize(), (unsigned int*)sphere.getIndices(), sphere.getIndexSize());

	Texture_2D sun_2D = Texture_2D("resources/textures/sun.jpg");
	Texture_2D Earth_2D = Texture_2D("resources/textures/earth.jpg");
	Texture_2D Mercury_2D = Texture_2D("resources/textures/mercury.jpg");
	Texture_2D Venus_2D = Texture_2D("resources/textures/venus.jpg");
	Texture_2D Mars_2D = Texture_2D("resources/textures/mars.jpg");
	Texture_2D Jupiter_2D = Texture_2D("resources/textures/jupiter.jpg");
	Texture_2D Saturn_2D = Texture_2D("resources/textures/saturn.jpg");
	Texture_2D Uranus_2D = Texture_2D("resources/textures/uranus.jpg");
	Texture_2D Neptune_2D = Texture_2D("resources/textures/neptune.jpg");


	glm::mat4 looke;
	genSkyBox(scene_camera);

	Timer timer = Timer();
	float Year = 8 * 60; // seconds
	float fraction;

	float camera_speed = 38000.0f;
	//float distance_offset = 4500.0f; // Reduced by factor of 10
	float distance_offset = 6963.400f;

	float radius = 0.0f;
	float circumference = 0.0f;
	float period = 0.0f;
	float planetary_speed = 0.0f;
	float angular_speed = 0.0f;

	float angle = 0;
	float VensAngle = 0;
	float earthAngle = 0;
	float marsAngle = 0;
	float jupiterAngle = 0;
	float saturnAngle = 0;
	float uranusAngle = 0;
	float neptneAngle = 0;
	float plutoAngle = 0;
	float S_angle = 0;
	fraction = glm::radians(360.0f) / Year;


	// Assuming you have a Torus class similar to Sphere
	Torus saturnRingTorus = Torus(0.1f, 0.9f, 64, 64);
	saturnRingTorus._createTorus();
	Object torusObject = Object(saturnRingTorus.getVertices().data(), saturnRingTorus.getVerticesSize(), saturnRingTorus.getNormals().data(), saturnRingTorus.getNormalsSize(),
		saturnRingTorus.getTexCoords().data(), saturnRingTorus.getTexSize());
	Texture_2D SaturnRing_2D = Texture_2D("resources/textures/saturn_ring.png");


	timer.reset();
	while (!glfwWindowShouldClose(m_window))
	{
		if (dispachter.checkKey(GLFW_KEY_ESCAPE)) break;

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (dispachter.checkKey(GLFW_KEY_A))
		{
			scene_camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime * camera_speed);
		}
		else if (dispachter.checkKey(GLFW_KEY_D))
		{
			scene_camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime * camera_speed);
		}
		else if (dispachter.checkKey(GLFW_KEY_W))
		{
			scene_camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime * camera_speed);
		}
		else if (dispachter.checkKey(GLFW_KEY_S))
		{
			scene_camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime * camera_speed);
		}
		else if (dispachter.checkKey(GLFW_KEY_DOWN))
		{
			Year -= (60 * 0.1f);
		}
		else if (dispachter.checkKey(GLFW_KEY_UP))
		{
			Year += (60 * 0.1f);
		}

		if (Year <= 0.0f)
		{
			Year = 0.1f * 60.0f;
		}

		if (dispachter.mouseMoved())
		{
			scene_camera.ProcessMouseMovement(dispachter.getCusorOffset()[0], dispachter.getCusorOffset()[1]);
		}
		view = scene_camera.GetViewMatrix();
		Light_shader.setUniformMat4("view", view);

		PointLight.set_view_position(scene_camera.Position);
		PointLight.set_light_type(false);
		PointLight.render(Light_shader.return_object());

		Light_shader.Bind();

		GL_ERROR_MACRO(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GL_ERROR_MACRO(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));

		// Sun
		{
			Light_shader.setUniformMat4("model", glm::translate(glm::mat4x4(1.0f), glm::vec3(0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(6963.400f))); // Scale down by 10

			Light_shader.setUniformFloat("Emit", 1.0f);
			Light_shader.setUniformFloat3("color", glm::vec3(0.0f) * glm::vec3(0.0f));

			sun_2D.bindTextureToSlot(2);
			Light_shader.setUniformInt("Texture", sun_2D.GetTextureSlot());

			sp.Draw_object(sphere.getIndexCount());
			Light_shader.setUniformFloat("Emit", 0.0f);
			Light_shader.setUniformFloat3("color", glm::vec3(0.5f) * glm::vec3(0.8f));

			Light_shader.setUniformFloat3("color", glm::vec3(0.0f));
		}

		// Mercury
		{
			radius = 579.1f + distance_offset;
			circumference = 2 * glm::pi<float>() * radius;
			period = 0.24f * Year;
			planetary_speed = circumference / period;
			angular_speed = planetary_speed / radius;

			angle += angular_speed;

			model = glm::mat4(1.0f);
			model = glm::rotate(glm::mat4(1.0f), glm::radians(7.0f), glm::vec3(1, 0., 0.0));
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(24.400f)); // Scale down by 10

			Light_shader.setUniformMat4("model", model);
			Mercury_2D.bindTextureToSlot(2);
			Light_shader.setUniformInt("Texture", Mercury_2D.GetTextureSlot());

			sp.Draw_object(sphere.getIndexCount());
		}

		// Venus
		{
			radius = 1082.0f + distance_offset;
			circumference = 2 * glm::pi<float>() * radius;
			period = 0.62f * Year;
			planetary_speed = circumference / period;
			angular_speed = planetary_speed / radius;

			VensAngle += angular_speed;

			model = glm::mat4(1.0f);
			model = glm::rotate(glm::mat4(1.0f), glm::radians(3.39f), glm::vec3(1, 0., 0.0));
			model = glm::rotate(model, glm::radians(VensAngle), glm::vec3(0, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(-0.9, 0., 0.017)) *
				glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 60), glm::vec3(0, 0., 1.0)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(60.52f)); // Scale down by 10

			Light_shader.setUniformMat4("model", model);
			Venus_2D.bindTextureToSlot(2);
			Light_shader.setUniformInt("Texture", Venus_2D.GetTextureSlot());

			sp.Draw_object(sphere.getIndexCount());
		}

		// Earth
		{
			radius = 1496.0f + distance_offset;
			circumference = 2 * glm::pi<float>() * radius;
			period = 1 * Year;
			planetary_speed = circumference / period;
			angular_speed = planetary_speed / radius;

			earthAngle += angular_speed;

			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(earthAngle), glm::vec3(0, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.9, 0., 0.317)) *
				glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 60), glm::vec3(0, 0.66, 0.4)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(63.71)); // Scale down by 10

			Light_shader.setUniformMat4("model", model);
			Earth_2D.bindTextureToSlot(2);
			Light_shader.setUniformInt("Texture", Earth_2D.GetTextureSlot());

			if (timer.elapsed() >= Year)
			{
				S_angle = 0.0f;
				timer.reset();
			}

			sp.Draw_object(sphere.getIndexCount());

			// Satellite orbiting Earth
			{
				//radius = 600.0f; // Scale down by 10
				//circumference = 2 * glm::pi<float>() * radius;
				//period = 1 * Year;
				//planetary_speed = circumference / period;
				//angular_speed = planetary_speed / radius;
				//
				//S_angle += (angular_speed) / radius;
				//
				//model = glm::mat4(1.0f);
				//model = glm::rotate(glm::mat4(1.0f), glm::radians(earthAngle), glm::vec3(0, 1.0f, 0.0f));
				//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1496.0f + distance_offset));
				//model = glm::rotate(model, S_angle, glm::vec3(0, 1, 0));
				//
				//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 600.0f))
				//	* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0))
				//	* glm::scale(glm::mat4(1.0f), glm::vec3(25.0f)); // Scale down by 10
				//
				////sp.Draw_object(sphere.getIndexCount());
			}
		}

		// Mars
		{
			radius = 2279.0f + distance_offset;
			circumference = 2 * glm::pi<float>() * radius;
			period = 1.88f * Year;
			planetary_speed = circumference / period;
			angular_speed = planetary_speed / radius;

			marsAngle += angular_speed;

			model = glm::mat4(1.0f);
			model = glm::rotate(glm::mat4(1.0f), glm::radians(1.85f), glm::vec3(1.f, 0., 0.0));
			model = glm::rotate(model, glm::radians(marsAngle), glm::vec3(0, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.9, 0., 0.4217)) *
				glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 60), glm::vec3(0, 0.66, 0.4)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(33.89f)); // Scale down by 10

			Light_shader.setUniformMat4("model", model);
			Mars_2D.bindTextureToSlot(2);
			Light_shader.setUniformInt("Texture", Mars_2D.GetTextureSlot());

			sp.Draw_object(sphere.getIndexCount());
		}

		// Jupiter
		{
			radius = 7783.0f + distance_offset;
			circumference = 2 * glm::pi<float>() * radius;
			period = 11.86f * Year;
			planetary_speed = circumference / period;
			angular_speed = planetary_speed / radius;

			jupiterAngle += angular_speed;

			model = glm::mat4(1.0f);
			model = glm::rotate(glm::mat4(1.0f), glm::radians(1.30f), glm::vec3(1.0f, 0, 0.0));
			model = glm::rotate(model, glm::radians(jupiterAngle), glm::vec3(0, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.9, 0, 0.05)) *
				glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 60), glm::vec3(0, 0, 1)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(850.0f)); // Scale down by 10

			Light_shader.setUniformMat4("model", model);
			Jupiter_2D.bindTextureToSlot(2);
			Light_shader.setUniformInt("Texture", Jupiter_2D.GetTextureSlot());

			sp.Draw_object(sphere.getIndexCount());
		}

		// Saturn
		{

			// Saturn
			{
				radius = 14427.0f + distance_offset;
				circumference = 2 * glm::pi<float>() * radius;
				period = 29.46f * Year;
				planetary_speed = circumference / period;
				angular_speed = planetary_speed / radius;

				saturnAngle += angular_speed;

				model = glm::mat4(1.0f);
				model = glm::rotate(glm::mat4(1.0f), glm::radians(2.49f), glm::vec3(1.0f, 0., 0.0));
				model = glm::rotate(glm::mat4(1.0f), glm::radians(saturnAngle), glm::vec3(0, 1.0f, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius)) *
					glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.894, 0., 0.45)) *
					glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 60), glm::vec3(0, 0, 1.0f)) *
					glm::scale(glm::mat4(1.0f), glm::vec3(582.0f)); // Scale down by 10

				Light_shader.setUniformMat4("model", model);
				Saturn_2D.bindTextureToSlot(2);
				Light_shader.setUniformInt("Texture", Saturn_2D.GetTextureSlot());

				sp.Draw_object(sphere.getIndexCount());

				// Render Saturn's ring (torus)
				{
					glm::mat4 ringModel = glm::mat4(1.0f);
					ringModel = glm::rotate(ringModel, glm::radians(saturnAngle), glm::vec3(0, 1.0f, 0.0f));
					ringModel = glm::translate(ringModel, glm::vec3(0.0f, 0.0f, radius)) *
						glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.894, 0., 0.45)) *
						glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 10.0f)); // Adjust the scale of the ring

					Light_shader.setUniformMat4("model", ringModel);
					SaturnRing_2D.bindTextureToSlot(3);
					Light_shader.setUniformInt("Texture", SaturnRing_2D.GetTextureSlot());

					torusObject.Draw_object(saturnRingTorus.getCount());
				}
			}

		}

		// Uranus
		{
			radius = 28710.0f + distance_offset;
			circumference = 2 * glm::pi<float>() * radius;
			period = 84.02f * Year;
			planetary_speed = circumference / period;
			angular_speed = planetary_speed / radius;

			uranusAngle += angular_speed;

			model = glm::mat4(1.0f);
			model = glm::rotate(glm::mat4(1.0f), glm::radians(0.77f), glm::vec3(1.0f, 0., 0.));
			model = glm::rotate(model, glm::radians(uranusAngle), glm::vec3(0, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.17, 0., 0.9817)) *
				glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 60), glm::vec3(0, 0.66, 0.4)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(253.0f)); // Scale down by 10

			Light_shader.setUniformMat4("model", model);
			Uranus_2D.bindTextureToSlot(2);
			Light_shader.setUniformInt("Texture", Uranus_2D.GetTextureSlot());

			sp.Draw_object(sphere.getIndexCount());
		}

		// Neptune
		{
			radius = 44950.0f + distance_offset;
			circumference = 2 * glm::pi<float>() * radius;
			period = 164.79f * Year;
			planetary_speed = circumference / period;
			angular_speed = planetary_speed / radius;

			neptneAngle += angular_speed;

			model = glm::mat4(1.0f);
			model = glm::rotate(glm::mat4(1.0f), glm::radians(1.77f), glm::vec3(1.0f, 0., 0.));
			model = glm::rotate(model, glm::radians(neptneAngle), glm::vec3(0, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.88, 0., 0.46)) *
				glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 60), glm::vec3(0, 0.0, 1.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(246.0f)); // Scale down by 10

			Light_shader.setUniformMat4("model", model);
			Neptune_2D.bindTextureToSlot(2);
			Light_shader.setUniformInt("Texture", Neptune_2D.GetTextureSlot());

			sp.Draw_object(sphere.getIndexCount());
		}

		renderSkyBox(scene_camera);

		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}
	glfwDestroyWindow(m_window);
	glfwTerminate();
	return 0;
}
