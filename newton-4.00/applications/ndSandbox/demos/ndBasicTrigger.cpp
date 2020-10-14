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

#include "ndSandboxStdafx.h"
#include "ndSkyBox.h"
#include "ndTargaToOpenGl.h"
#include "ndDemoMesh.h"
#include "ndDemoCamera.h"
#include "ndPhysicsUtils.h"
#include "ndPhysicsWorld.h"
#include "ndDemoEntityManager.h"

class ndDemoTriggerVolume : public ndBodyTriggerVolume
{
	public:
	
	void OnTrigger(ndBodyKinematic* const body, dFloat32 timestep)
	{
		dAssert(0);
	}

	void OnTriggerEnter(ndBodyKinematic* const body, dFloat32 timestep)
	{
		dTrace(("enter trigger body: %d\n", body->GetId()));
	}

	void OnTriggerExit(ndBodyKinematic* const body, dFloat32 timestep)
	{
		dTrace(("exit trigger body: %d\n", body->GetId()));
	}

};

static void BuildFloor(ndDemoEntityManager* const scene)
{
	ndPhysicsWorld* const world = scene->GetWorld();

	ndShapeInstance box(new ndShapeBox(200.0f, 1.0f, 200.f));
	dMatrix uvMatrix(dGetIdentityMatrix());
	uvMatrix[0][0] *= 0.025f;
	uvMatrix[1][1] *= 0.025f;
	uvMatrix[2][2] *= 0.025f;
	ndDemoMesh* const geometry = new ndDemoMesh("box", scene->GetShaderCache(), &box, "marbleCheckBoard.tga", "marbleCheckBoard.tga", "marbleCheckBoard.tga", 1.0f, uvMatrix);
	
	dMatrix matrix(dGetIdentityMatrix());
	matrix.m_posit.m_y = -0.5f;
	ndDemoEntity* const entity = new ndDemoEntity(matrix, nullptr);
	entity->SetMesh(geometry, dGetIdentityMatrix());
	
	ndBodyDynamic* const body = new ndBodyDynamic();
	body->SetNotifyCallback(new ndDemoEntityNotify(scene, entity));
	body->SetMatrix(matrix);
	body->SetCollisionShape(box);
	
	world->AddBody(body);
	
	scene->AddEntity(entity);
	geometry->Release();
}

static void AddTrigger(ndDemoEntityManager* const scene)
{
	ndPhysicsWorld* const world = scene->GetWorld();

	ndShapeInstance box(new ndShapeBox(20.0f, 10.0f, 20.0f));
	dMatrix uvMatrix(dGetIdentityMatrix());
	uvMatrix[0][0] *= 1.0f / 20.0f;
	uvMatrix[1][1] *= 1.0f / 10.0f;
	uvMatrix[2][2] *= 1.0f / 20.0f;
	uvMatrix.m_posit = dVector(0.5f, 0.5f, 0.5f, 1.0f);
	ndDemoMesh* const geometry = new ndDemoMesh("trigger", scene->GetShaderCache(), &box, "metal_30.tga", "metal_30.tga", "logo_php.tga", 0.5f, uvMatrix);

	dVector floor(FindFloor(*world, dVector(0.0f, 100.0f, 0.0f, 0.0f), 200.0f));
	dMatrix matrix(dGetIdentityMatrix());
	matrix.m_posit = floor;
	matrix.m_posit.m_w = 1.0f;
	matrix.m_posit.m_y += 2.0f;

	ndDemoEntity* const entity = new ndDemoEntity(matrix, nullptr);
	entity->SetMesh(geometry, dGetIdentityMatrix());

	//ndBodyDynamic* const body = new ndBodyDynamic();
	ndBodyTriggerVolume* const body = new ndDemoTriggerVolume();
	body->SetNotifyCallback(new ndDemoEntityNotify(scene, entity));
	body->SetMatrix(matrix);
	body->SetCollisionShape(box);

	world->AddBody(body);

	scene->AddEntity(entity);
	geometry->Release();
}

static void AddShape(ndDemoEntityManager* const scene, 
	ndDemoMesh* const sphereMesh, const ndShapeInstance& sphereShape,
	dFloat32 mass, const dVector& origin, const dFloat32 diameter, int count, dFloat32 xxxx)
{
	//dMatrix matrix(dGetIdentityMatrix());
	dMatrix matrix(dRollMatrix(90.0f * dDegreeToRad));
	//dMatrix matrix(dYawMatrix(90.0f * dDegreeToRad) * dPitchMatrix(-45.0f * dDegreeToRad));
	matrix.m_posit = origin;
	matrix.m_posit.m_w = 1.0f;

	ndPhysicsWorld* const world = scene->GetWorld();

	dVector floor(FindFloor(*world, matrix.m_posit + dVector(0.0f, 100.0f, 0.0f, 0.0f), 200.0f));
	matrix.m_posit.m_y = floor.m_y + diameter * 0.5f * 0.99f;

	matrix.m_posit.m_y += 10.0f;

	for (dInt32 i = 0; i < count; i++)
	{
		ndBodyDynamic* const body = new ndBodyDynamic();
		ndDemoEntity* const entity = new ndDemoEntity(matrix, nullptr);
		entity->SetMesh(sphereMesh, dGetIdentityMatrix());

		body->SetNotifyCallback(new ndDemoEntityNotify(scene, entity));
		body->SetMatrix(matrix);
		body->SetCollisionShape(sphereShape);
		body->SetMassMatrix(mass, sphereShape);
		body->SetGyroMode(true);

		world->AddBody(body);
		scene->AddEntity(entity);

		//matrix.m_posit.m_y += diameter * 0.99f;
		matrix.m_posit.m_y += diameter * 0.99f * 3.0f;
	}
}

static void AddShapes(ndDemoEntityManager* const scene, const dVector& origin)
{
	dFloat32 diameter = 1.0f;
	//ndShapeInstance shape(new ndShapeSphere(diameter * 0.5f));
	ndShapeInstance shape(new ndShapeCapsule(diameter * 0.5f, diameter * 0.5f, diameter * 1.0f));
	//ndShapeInstance shape(new ndShapeBox(diameter, diameter, diameter));
	//ndDemoMesh* const mesh = new ndDemoMesh("shape", scene->GetShaderCache(), &shape, "wood_0.tga", "wood_0.tga", "wood_0.tga");
	ndDemoMesh* const mesh = new ndDemoMesh("shape", scene->GetShaderCache(), &shape, "marble.tga", "marble.tga", "marble.tga");
	
	const int n = 1;
	const int stackHigh = 1;
	//const int n = 10;
	//const int stackHigh = 7;
	for (dInt32 i = 0; i < n; i++)
	{
		for (dInt32 j = 0; j < n; j++)
		{
			dVector location((j - n / 2) * 4.0f, 0.0f, (i - n / 2) * 4.0f, 0.0f);
			AddShape(scene, mesh, shape, 10.0f, location + origin, 1.0f, stackHigh, 2.0f);
		}
	}

	mesh->Release();
}


void ndBasicTrigger (ndDemoEntityManager* const scene)
{
	// load the sky box
	scene->CreateSkyBox();

	// sync just in case we are on a pending update
	scene->GetWorld()->Sync();

	// build a floor
	BuildFloor(scene);

	// build a floor
	AddTrigger(scene);

	dVector origin1(0.0f, 0.0f, 0.0f, 0.0f);
	AddShapes(scene, origin1);

	dQuaternion rot;
	//dVector origin(-80.0f, 5.0f, 0.0f, 0.0f);
	dVector origin(-40.0f, 5.0f, 0.0f, 0.0f);
	scene->SetCameraMatrix(rot, origin);
}