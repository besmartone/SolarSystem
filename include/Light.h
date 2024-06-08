#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"
#include <string>



class Light
{
	public:
		Light() {}
		~Light() {}

	Light(glm::vec3 color, std::string name = "light",  float intensity = 1.f, float ambiental = 0.25f, float specular = 0.5f);


	void set_position(glm::vec3 value);


	void set_view_position(glm::vec3 value);


	void set_direction(glm::vec3 value);


	void set_Spotdirection(glm::vec3 value);



	void set_color(glm::vec3& color);


	void set_ambient_intensity(float value);


	void set_intensity(float value);


	void set_specular_intensity(float value);


	void set_spotlight_cutoff(float value);



	void set_spotlight_innercutoff(float value);


	void set_attenuation_constants(float exponent, float kc, float kl, float kq);


	glm::vec3& get_color();

	float get_intensity();


	float get_ambient_intensity();

	float get_specular_intensity();



	void render(unsigned int shaderID);

	void set_light_type(bool spot)
	{
		Directional = false;
		isSpot = spot;
	}


	private:
		bool Directional;
		bool isSpot;

		float intensity;

		float ambient_intensity;

		float specular_intensity;

		glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 attenuation = glm::vec3(0.0f);

		glm::mat4 transform;

		std::string name;

		glm::vec3 spot = glm::vec3(0.0f, 0.0f, 1.0f);
		float cutoff = 1.0f;
		float innercutoff = 1.0f;
		float spotExp = 0.0f;

		union
		{
			glm::vec3 direction;
			glm::vec3 Position = glm::vec3(0.0f);
		};

		glm::vec3 Camera_Position;

		//float      Radius = 1.0f;
		//bool       Visible = true;

		static std::string Ldirection;
		static std::string LSpotdirection;
		static std::string Lposition;
		static std::string Lambient;
		static std::string Lspecular;
		static std::string Ldiffuse;
		static std::string LCutoff;
		static std::string LInnerCutoff;
		static std::string LisDirection;
		static std::string LisPoint;
		static std::string LisSpot;
		static std::string LColor;
		static std::string Lkc;
		static std::string Lkl;
		static std::string Lkq;
		static std::string Lexponent;
};


#endif
