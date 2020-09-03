/* Copyright (c) <2003-2019> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __D_BODY_KINEMATIC_H__
#define __D_BODY_KINEMATIC_H__

#include "ntNewtonStdafx.h"
#include "ntBody.h"

D_MSV_NEWTON_ALIGN_32
class ntBodyKinematic: public ntBody
{
	class ntContactkey;
	class ntContactMap: public dTree<ntContact*, ntContactkey, dContainersFreeListAlloc<ntContact*>>
	{
		public:
		ntContactMap();
		ntContact* FindContact(const ntBody* const body0, const ntBody* const body1) const;
		void AttachContact(ntContact* const contact);
		void DetachContact(ntContact* const contact);

		mutable dSpinLock m_lock;
	};

	public:
	D_NEWTON_API ntBodyKinematic();
	D_NEWTON_API virtual ~ntBodyKinematic();

	dFloat32 GetInvMass() const;
	dVector GetMassMatrix() const;
	void SetMassMatrix(const dVector& massMatrix);

	D_NEWTON_API ntShapeInstance& GetCollisionShape();
	D_NEWTON_API const ntShapeInstance& GetCollisionShape() const;
	D_NEWTON_API void SetCollisionShape(const ntShapeInstance& shapeInstance);

	D_NEWTON_API virtual dFloat32 RayCast(ntRayCastNotify& callback, const dFastRayTest& ray, const dFloat32 maxT) const;

	void SetMassMatrix(dFloat32 mass, const ntShapeInstance& shapeInstance);
	void SetMassMatrix(dFloat32 Ixx, dFloat32 Iyy, dFloat32 Izz, dFloat32 mass);
	void GetMassMatrix(dFloat32& Ixx, dFloat32& Iyy, dFloat32& Izz, dFloat32& mass);

	D_NEWTON_API virtual void AttachContact(ntContact* const contact);
	D_NEWTON_API virtual void DetachContact(ntContact* const contact);
	D_NEWTON_API virtual ntContact* FindContact(const ntBody* const otherBody) const;
	D_NEWTON_API static void ReleaseMemory();

	virtual ntBodyKinematic* GetAsBodyKinematic() { return this; }

	private:
	void SetMassMatrix(dFloat32 mass, const dMatrix& inertia);

	protected:
	void UpdateCollisionMatrix();

	ntBroadPhaseBodyNode* GetBroadPhaseNode() const;
	void SetBroadPhaseNode(ntBroadPhaseBodyNode* const node);

	ntBroadPhaseAggregate* GetBroadPhaseAggregate() const;
	void SetBroadPhaseAggregate(ntBroadPhaseAggregate* const node);

	ntShapeInstance m_shapeInstance;
	dVector m_mass;
	dVector m_invMass;
	ntContactMap m_contactList;
	ntBroadPhaseBodyNode* m_broadPhaseNode;
	ntBroadPhaseAggregate* m_broadPhaseAggregateNode;

	friend class ntWorld;
	friend class ntBroadPhase;
	friend class ntBroadPhaseMixed;
	friend class ntBroadPhaseBodyNode;
} D_GCC_NEWTON_ALIGN_32;

inline dFloat32 ntBodyKinematic::GetInvMass() const
{
	return m_invMass.m_w;
}

inline dVector ntBodyKinematic::GetMassMatrix() const
{
	return m_mass;
}

inline void ntBodyKinematic::GetMassMatrix(dFloat32& Ixx, dFloat32& Iyy, dFloat32& Izz, dFloat32& mass)
{
	Ixx = m_mass.m_x;
	Iyy = m_mass.m_y;
	Izz = m_mass.m_z;
	mass = m_mass.m_w;
}

inline void ntBodyKinematic::SetMassMatrix(const dVector& massMatrix)
{
	dMatrix inertia(dGetZeroMatrix());
	inertia[0][0] = massMatrix.m_x;
	inertia[1][1] = massMatrix.m_y;
	inertia[2][2] = massMatrix.m_z;
	SetMassMatrix(massMatrix.m_w, inertia);
}

inline void ntBodyKinematic::SetMassMatrix(dFloat32 Ixx, dFloat32 Iyy, dFloat32 Izz, dFloat32 mass)
{
	SetMassMatrix(dVector(Ixx, Iyy, Izz, mass));
}

inline void ntBodyKinematic::SetMassMatrix(dFloat32 mass, const ntShapeInstance& shapeInstance)
{
	dMatrix inertia(shapeInstance.CalculateInertia());

	dVector origin(inertia.m_posit);
	for (dInt32 i = 0; i < 3; i++) {
		inertia[i] = inertia[i].Scale(mass);
		//inertia[i][i] = (inertia[i][i] + origin[i] * origin[i]) * mass;
		//for (dInt32 j = i + 1; j < 3; j ++) {
		//	dgFloat32 crossIJ = origin[i] * origin[j];
		//	inertia[i][j] = (inertia[i][j] + crossIJ) * mass;
		//	inertia[j][i] = (inertia[j][i] + crossIJ) * mass;
		//}
	}

	// although the engine fully supports asymmetric inertia, I will ignore cross inertia for now
	SetCentreOfMass(origin);
	SetMassMatrix(mass, inertia);
}


#endif 

