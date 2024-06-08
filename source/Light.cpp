#include "Light.h"
#include "error.h"


 std::string  Light::Ldirection = ".direction";
 std::string  Light::LSpotdirection = ".spotLightDirection";
 std::string  Light::Lposition = ".position";
 std::string  Light::Lambient = ".ambientIntensity";
 std::string  Light::Ldiffuse = ".intensity";
 std::string  Light::LCutoff = ".cutoff";
 std::string  Light::LInnerCutoff = ".innercutoff";
 std::string  Light::Lspecular = ".specularIntensity";
 std::string  Light::LisPoint = ".isPoint";
 std::string  Light::LisDirection = ".isDirection";
 std::string  Light::LisSpot = ".isSpot";
 std::string  Light::LColor = ".color";
 std::string  Light::Lkc = ".kc";
 std::string  Light::Lkl = ".kl";
 std::string  Light::Lkq = ".kq";
 std::string  Light::Lexponent = ".spotexponent";


Light::Light(glm::vec3 color, std::string name, float intensity, float ambiental, float specular) :
	transform(glm::mat4()),  intensity(intensity), ambient_intensity(ambiental), light_color(color), specular_intensity(specular)
{
	this->name = name;
}

void Light::set_position(glm::vec3 value)
{
	Position = value;
}

void Light::set_view_position(glm::vec3 value)
{
	Camera_Position = value;
}

void Light::set_direction(glm::vec3 value)
{
	direction = value;
}

void Light::set_Spotdirection(glm::vec3 value)
{
	spot = value;
}

/**
@brief Set the color of the light
@param color The color to set
*/
void Light::set_color(glm::vec3& color)
{
	light_color = color;
}

/**
@brief Set the ambient intensity
@param value The ambient intensity to set
*/
void Light::set_ambient_intensity(float value)
{
	ambient_intensity = value;
}

/**
@brief Set the intensity
@param The intensity to set
*/
void Light::set_intensity(float value)
{
	intensity = value;
}

/**
@brief Set the specular intensity
@param The specular intensity to set
*/
void Light::set_specular_intensity(float value)
{
	specular_intensity = value;
}

void Light::set_spotlight_cutoff(float value)
{
	cutoff = value;
}

void Light::set_spotlight_innercutoff(float value)
{
	innercutoff = value;
}

void Light::set_attenuation_constants(float exponent, float kc, float kl, float kq)
{
	spotExp = exponent;
	attenuation = glm::vec3(kc, kl, kq);
}

/**
@brief Gets the color of the light
@retun The light color
*/
glm::vec3& Light::get_color()
{
	return light_color;
}

/**
@brief Gets the light intensity
@return The light intensity
*/
float Light::get_intensity()
{
	return intensity;
}

/**
@brief Gets the ambient intensity
@return The ambient intensity
*/
float Light::get_ambient_intensity()
{
	return ambient_intensity;
}

/**
@brief Gets the specular intensity
@return The specular intensity
*/
float Light::get_specular_intensity()
{
	return specular_intensity;
}

/**
@brief Render. Update the lights values
@param camera The reference of the camera
*/

void Light::render(unsigned int shaderID)
{
	GL_ERROR_MACRO(glUseProgram(shaderID));

	int index = 0;

	if (!isSpot)
	{
		if (Directional)
		{
			GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Ldirection).c_str()));
			GL_ERROR_MACRO(glUniform3fv(index, 1, &direction[0]));

			GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + LisDirection).c_str()));
			GL_ERROR_MACRO(glUniform1f(index, 1.0f));
		}
		else
		{
			GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lposition).c_str()));
			GL_ERROR_MACRO(glUniform3fv(index, 1, &Position[0]));


			GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + LisPoint).c_str()));
			GL_ERROR_MACRO(glUniform1f(index, 1.0f));


			GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lkc).c_str()));
			GL_ERROR_MACRO(glUniform1f(index, attenuation.x));

			GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lkl).c_str()));
			GL_ERROR_MACRO(glUniform1f(index, attenuation.y));

			GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lkq).c_str()));
			GL_ERROR_MACRO(glUniform1f(index, attenuation.z));

		}
	}

	else
	{
		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lposition).c_str()));
		GL_ERROR_MACRO(glUniform3fv(index, 1, &Position[0]));


		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + LisPoint).c_str()));
		GL_ERROR_MACRO(glUniform1f(index, 1.0f));



		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + LSpotdirection).c_str()));
		GL_ERROR_MACRO(glUniform3fv(index, 1, &spot[0]));


		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + LisSpot).c_str()));
		GL_ERROR_MACRO(glUniform1f(index, 1.0f));

		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + LCutoff).c_str()));
		GL_ERROR_MACRO(glUniform1f(index, cutoff));	
		
		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + LInnerCutoff).c_str()));
		GL_ERROR_MACRO(glUniform1f(index, innercutoff));

		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lkc).c_str()));
		GL_ERROR_MACRO(glUniform1f(index, attenuation.x));

		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lkl).c_str()));
		GL_ERROR_MACRO(glUniform1f(index, attenuation.y));

		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lkq).c_str()));
		GL_ERROR_MACRO(glUniform1f(index, attenuation.z));

		GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lexponent).c_str()));
		GL_ERROR_MACRO(glUniform1f(index, spotExp));
	}
	

	GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + LColor).c_str() ));
	GL_ERROR_MACRO(glUniform3fv(index, 1, &light_color[0]));

	GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lambient).c_str() ));
	GL_ERROR_MACRO(glUniform1f(index, get_ambient_intensity()));

	GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Lspecular).c_str() ));
	GL_ERROR_MACRO(glUniform1f(index, get_specular_intensity()));

	GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, (name + Ldiffuse).c_str() ));
	GL_ERROR_MACRO(glUniform1f(index, get_intensity()));

	GL_ERROR_MACRO(index = glGetUniformLocation(shaderID, "viewPosition"));
	GL_ERROR_MACRO(glUniform3fv(index, 1, &Camera_Position[0]));
}
