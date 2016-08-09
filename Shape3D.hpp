/*
Shape3D.cpp

Represent the scale, translation, and rotation of a 3D shape.
If you overload the constructor you can create a shape with
arguments for scale, translation, and rotation.

Mike Barnes
8/24/2014
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Shape3D {
	private :
		int id, planet;
		glm::mat4 rotationMatrix;
		glm::mat4 scaleMatrix;
		glm::mat4 translationMatrix;
		glm::vec3 rotationAxis; 
		float radians; 
		bool orbital;

	public:
		Shape3D(int number) 
		{
			id = number;  // for debugging
			switch(id) 
			{
				case 0: // Ruber : red
					translationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
					rotationAxis = glm::vec3(0, 1, 0);
					radians = glm::radians(0.11f);
					orbital = false;
					planet = 0;
					printf("Created Ruber\n");
				break;

				case 1:  // Unum : blue
					translationMatrix = glm::translate(glm::mat4(), glm::vec3(4000, 0, 0));		
					rotationAxis = glm::vec3(0, 1, 0);
					radians = glm::radians(0.4f); //Rotate around Ruber
					orbital = true;
					planet = 0;
					printf("Created Unum\n");
				break;

				case 2:  // Duo : green
					translationMatrix = glm::translate(glm::mat4(), glm::vec3(-9000, 0, 0));		
					rotationAxis = glm::vec3(0, 1, 0);
					radians = glm::radians(0.2f); //Rotate around Ruber
					orbital = true;
					planet = 0;
					printf("Created Duo\n");
				break;

				case 3:  // Primus (pink)
					translationMatrix = glm::translate(glm::mat4(), glm::vec3(900, 0, 0));		
					rotationAxis = glm::vec3(0,  1, 0);
					radians = glm::radians(0.4f); //Rotate around Duo
					orbital = true;
					planet = 2;
					printf("Created Primus\n");
				break;

				case 4:  // Secundus (yellow)
					translationMatrix = glm::translate(glm::mat4(), glm::vec3(-7250, 0, 0));		
					rotationAxis = glm::vec3(0,  1, 0);
					radians = glm::radians(0.2f); //Rotate around Duo
					orbital = true;
					planet = 2;
					printf("Created Secundus\n");
				break;

				case 5: 
					translationMatrix = glm::translate(glm::mat4(), glm::vec3(5000, 1000, 5000));		
					rotationAxis = glm::vec3(0,  1, 0);
					radians = glm::radians(0.0f); //No Rotation
					orbital = false;
					printf("Created Warbird\n");
				break;

				case 6:
					translationMatrix = glm::translate(glm::mat4(), glm::vec3(4900, 1000, 4900));		
					rotationAxis = glm::vec3(0,  1, 0);
					radians = glm::radians(0.0f); //Rotate around Ruber
					orbital = false;
					printf("Created Missile\n");
				break;
				}
			rotationMatrix = glm::mat4();  // no initial orientation
		}

		glm::mat4 get_rotation()
		{
			return rotationMatrix;
		}

		glm::mat4 get_translation()
		{
			return translationMatrix;
		}

		void setScale (glm::vec3 matrix)
		{
			scaleMatrix = glm::scale(glm::mat4(), matrix);
		}

		glm::mat4 getPosition()
		{
			if (orbital)
			{
				if (orbital)
					return rotationMatrix * translationMatrix;
				else
					return translationMatrix * rotationMatrix;
			}
		}

		glm::mat4 getModelMatrix() 
		{
			if (orbital) // orbital rotation
				return(rotationMatrix * translationMatrix * scaleMatrix);
			else  // center rotation
				return(translationMatrix * rotationMatrix * scaleMatrix);
		}

		glm::mat4 getModelMatrix(glm::mat4 planet) 
		{
			if (orbital)
				if (orbital)
					return (planet * rotationMatrix * translationMatrix * scaleMatrix);
				else
					return (planet * translationMatrix * rotationMatrix * scaleMatrix);
		}

		void update() 
		{
			rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		}   
};
