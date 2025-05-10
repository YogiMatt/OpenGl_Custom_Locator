#pragma once

#pragma comment(lib, "Foundation.lib")
#pragma comment(lib, "OpenMaya.lib")
#pragma comment(lib, "OpenMayaUI.lib")
#pragma comment(lib, "OpenMayaRender.lib")
#pragma comment(lib, "opengl32.lib")

#include <maya/MFnPlugin.h>
#include<maya/MDrawRegistry.h>

#include "LocatorNodeVP12.h"


//const unsigned int globalTypeIdStart = 0xFF;
//unsigned int globalTypeId = globalTypeIdStart;


MStatus initializePlugin(MObject pluginObject)
{
	MStatus status;

	MFnPlugin plugin(pluginObject, "Matt Pierce", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.registerNode("LocatorNodeVP12", LocatorNodeVP12::typeId, &LocatorNodeVP12::creator, &LocatorNodeVP12::initialize, MPxNode::kLocatorNode);CHECK_MSTATUS_AND_RETURN_IT(status);
	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(LocatorNodeVP12::drawDbClassification, "LocatorNodeVP12", LocatorNodeVP12Override::creator);CHECK_MSTATUS_AND_RETURN_IT(status);

}

MStatus uninitializePlugin(MObject pluginObject)
{
	MStatus status;
	MFnPlugin plugin(pluginObject);
	status = plugin.deregisterNode(LocatorNodeVP12::typeId); CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}
