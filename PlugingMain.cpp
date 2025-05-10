

#pragma once

#pragma comment(lib, "Foundation.lib")
#pragma comment(lib, "OpenMaya.lib")
#pragma comment(lib, "OpenMayaUI.lib")
#pragma comment(lib, "OpenMayaRender.lib")
#pragma comment(lib, "opengl32.lib")



#include <maya/MFnPlugin.h>
#include<maya/MDrawRegistry.h>
#include "LocatorNodeVP12.h"


const unsigned int globalTypeIdStart = 0xFF;
unsigned int globalTypeId = globalTypeIdStart;


MStatus initializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj, "Matt Pierce", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.registerNode("LocatorNode", (MTypeId)globalTypeId++, &LocatorNode::creator, &LocatorNode::initialize, MPxNode::kLocatorNode);CHECK_MSTATUS_AND_RETURN_IT(status);

}

MStatus uninitializePlugin(MObject object)
{
	MStatus status;
	MFnPlugin plugin(pluginObject);CHECK_MSTATUS_AND_RETURN_IT(status);
	while (globalTypeId > globalTypeIdStart)
		status = plugin.deregisterNode((MTypeId)--globalTypeId); CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}
