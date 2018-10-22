#include "Kinematrix.h"
#include "GameAPI.h"
#include "GameTypes.h"
#include "InternalTasks.h"
#include "GameReferences.h"
#include "NiTypes.h"
#include "NiNodes.h"
#include "VRManager.h"


namespace Kinematrix
{
	static BSFixedString NPC_Right_Hand("NPC R Hand [RHnd]");
	static BSFixedString RoomNode("RoomNode");
	static BSFixedString NPC_LBody("CME LBody [LBody]");
	static NiAVObject* rootNode = nullptr;
	static NiNode* rootNodeTP = nullptr;
	static NiNode* rootNodeFP = nullptr;
	static NiNode* roomNode = nullptr;
	static NiNode* lowerBody = nullptr;
	static NiNode* rightHandNode = nullptr;

	void GameLoaded()
	{
		_MESSAGE("Getting root node");
		rootNode = (*g_thePlayer)->loadedState->node->m_parent;
		if (!rootNode)
			_MESSAGE("Error while getting root node");
		else
			_MESSAGE("Root node scale %f", rootNode->m_worldTransform.scale);

		rootNodeTP = (*g_thePlayer)->GetNiRootNode(0);
		if (!rootNodeTP)
			_MESSAGE("Error while getting rootNodeTP node");

		rootNodeFP = (*g_thePlayer)->GetNiRootNode(1);
		if (!rootNodeFP)
			_MESSAGE("Error while getting rootNodeFP node");

		NiNode* mostInterestingRoot = (rootNodeFP != nullptr) ? rootNodeFP : rootNodeTP;
		if (mostInterestingRoot == rootNodeFP)
			_MESSAGE("First person node existing....");
		else
			_MESSAGE("No first person node, falling back to third person....");

		_MESSAGE("Getting hand node");
		rightHandNode = ni_cast(mostInterestingRoot->GetObjectByName(&NPC_Right_Hand.data), NiNode);

		if (!rightHandNode)
			_MESSAGE("Error while getting hand node");

		_MESSAGE("Getting RoomNode node");
		roomNode = ni_cast(rootNode->GetObjectByName(&RoomNode.data), NiNode);

		if (!roomNode)
			_MESSAGE("Error while getting RoomNode node");
		else
			_MESSAGE("Room Node scale %f", roomNode->m_worldTransform.scale);

		lowerBody = ni_cast(mostInterestingRoot->GetObjectByName(&NPC_LBody.data), NiNode);
		if (!lowerBody)
			_MESSAGE("Error while getting NPC_LBody node");

		PapyrusVR::VRManager::GetInstance().RegisterVRUpdateListener(OnPoseUpdate);
	}

	void OnPoseUpdate(float deltaTime)
	{
		if (lowerBody != nullptr)
		{
			//Rotates hand
			NiMatrix33* rotation = new NiMatrix33();
			float angle = 10 * deltaTime;
			rotation->data[0][0] = 1;
			rotation->data[1][1] = cos(angle);
			rotation->data[1][2] = sin(angle);
			rotation->data[2][1] = -sin(angle);
			rotation->data[2][2] = cos(angle);

			//NiPoint3* translation = new NiPoint3(0, 0, 0);
			lowerBody->m_localTransform.rot = lowerBody->m_localTransform.rot * (*rotation);
			TaskInterface::UpdateWorldData(lowerBody);
			//(*g_thePlayer)->_QueueNiNodeUpdate_GetPtr()
		}
	}
}
