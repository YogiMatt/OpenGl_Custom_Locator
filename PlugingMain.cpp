#pragma once

#pragma comment(lib, "Foundation.lib")
#pragma comment(lib, "OpenMaya.lib")
#pragma comment(lib, "OpenMayaUI.lib")
#pragma comment(lib, "opengl32.lib")
#include<maya/MDrawRegistry.h>
#include <maya/MFnPlugin.h>
#include "LocatorNodeVP12.h"

MStatus initializePlugin(MObject pluginObject) {
    MStatus status;
    MFnPlugin plugin(pluginObject, "Trevor van Hoof", "1.0", "Any", &status); CHECK_MSTATUS_AND_RETURN_IT(status);
    status = plugin.registerNode("LocatorNodeVP1", LocatorNodeVP12::typeId, &LocatorNodeVP12::creator, &LocatorNodeVP12::initialize, MPxNode::kLocatorNode, &LocatorNodeVP12::drawDbClassification); CHECK_MSTATUS_AND_RETURN_IT(status);
    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(LocatorNodeVP12::drawDbClassification, "LocatorNodeVP12", LocatorNodeVP12Override::creator); CHECK_MSTATUS_AND_RETURN_IT(status);

    return status;
}

MStatus uninitializePlugin(MObject pluginObject) {
    MStatus status;
    MFnPlugin plugin(pluginObject);
    status = plugin.deregisterNode(LocatorNodeVP12::typeId); CHECK_MSTATUS_AND_RETURN_IT(status);
    status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(LocatorNodeVP12::drawDbClassification, "LocatorNodeVP12"); CHECK_MSTATUS_AND_RETURN_IT(status);

    return status;
}
