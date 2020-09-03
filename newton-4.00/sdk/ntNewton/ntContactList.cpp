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

#include "ntNewtonStdafx.h"
#include "ntContact.h"
#include "ntContactList.h"
#include "ntBodyKinematic.h"

ntContact* ntContactList::CreateContact(ntBodyKinematic* const body0, ntBodyKinematic* const body1)
{
	ntContact temp(body0, body1);
	dScopeSpinLock lock(m_lock);
	dListNode* const node = Append(temp);
	ntContact* const contact = &node->GetInfo();
	contact->m_linkNode = node;
	return contact;
}

void ntContactList::DeleteContact(ntContact* const contact)
{
	contact->DetachFromBodies();
	Remove(contact->m_linkNode);
}

void ntContactList::DeleteAllContacts()
{
	while (GetFirst())
	{
		DeleteContact(&GetFirst()->GetInfo());
	}
	FlushFreeList();
}
