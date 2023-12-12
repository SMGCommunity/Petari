#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Gravity/GravityInfo.h"
#include "Game/Gravity/PlanetGravity.h"

/// @brief Registers and holds individual PlanetGravity instances. Also provides the main function for calculating the
/// gravity vector at a specified position in a stage's space.
class PlanetGravityManager : public NameObj {
public:
	/// @brief Constructs a new PlanetGravityManager instance.
	/// @param pName The new name assigned to the object.
	PlanetGravityManager(const char *pName);

	virtual ~PlanetGravityManager() {}

	/// @brief Effectively does nothing.
	/// @param rIter Reference to the JMapInfoIter that supplies BCSV information.
	virtual void init(const JMapInfoIter &rIter);

	/// @brief Calculates the total gravity vector at the specified position in space. If the object that requests gravity
	/// matches the host of one or more gravity instances, these instances won't be taken into account when calculating
	/// the total gravity vector. Furthermore, gravity instances that do not affect the specified gravity contexts will be
	/// ignored as well.
	/// 
	/// @param pGravity The 3D vector to store the total gravity vector in.
	/// @param pInfo Holder to be filled with information about highest priority gravity affecting the position.
	/// @param rPosition The position in space to calculate the gravity at.
	/// @param gravityType The affected gravity context (normal, shadow, magnet, etc.)
	/// @param requester The pointer of the object requesting the gravity vector.
	bool calcTotalGravityVector(TVec3f *pGravity, GravityInfo *pInfo, const TVec3f &rPosition, u32 gravityType, u32 requester) const;

	/// @brief Registers a PlanetGravity instance to the array of gravities. The higher the gravity's priority is, the
	/// closer it will be placed towards the array's beginning.
	/// 
	/// @param pGravity The PlanetGravity instance to be registered.
	void registerGravity(PlanetGravity *pGravity);

private:
	/// @brief Sorts all PlanetGravity instances by their priority in reverse order. The higher the priority is, the lower
	/// it gets positioned in the array.
	void sortGravities();

	/// @brief Swaps the PlanetGravity instance at the specified index with the one instance right before it.
	/// @param i The index of the instance to be bubbled down.
	void bubbleDown(int i);

	/* 0x00C*/ PlanetGravity* mGravities[128]; //<Array of registered PlanetGravity instances
	/* 0x20C*/ s32 mNumGravities;              //<Number of registered PlanetGravity instances
};
