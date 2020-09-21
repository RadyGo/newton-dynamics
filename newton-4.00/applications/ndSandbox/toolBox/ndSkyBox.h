/* Copyright (c) <2003-2019> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/


// RenderPrimitive.h: interface for the RenderPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SKY_BOX_H_
#define __SKY_BOX_H_

#include "ndSandboxStdafx.h"
#include "../ndDemoEntity.h"

class ndDemoEntityManager;

class ndSkyBox: public ndDemoEntity
{
	public:
	ndSkyBox(GLuint shader);
	~ndSkyBox();

	virtual void Render(dFloat timeStep, ndDemoEntityManager* const scene, const dMatrix& matrix) const;

	private:
	void DrawMesh () const;

	GLuint m_displayList;
	GLuint m_shader;
	GLuint m_textures[6];
};

#endif 

