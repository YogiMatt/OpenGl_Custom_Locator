
#include <maya/MHWGeometry.h>
#include<maya/MHWGeometryUtilities.h>

#include"LocatorNodeVP12.h"

void* LocatorNodeVP12::creator() { return new LocatorNodeVP12; }

MStatus LocatorNodeVP12::initialize() {
	MStatus status;
	{
		MFnNumericAttribute numericFn;
		drawIt = numericFn.create("drawIt", "drawIt", MFnNumericData::kBoolean, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
		status = addAttribute(drawIt); CHECK_MSTATUS_AND_RETURN_IT(status);
	}
	return status;
}

bool LocatorNodeVP12::isBounded() const { return true; }

MBoundingBox LocatorNodeVP12::boundingBox()const{
	return MBoundingBox(
		MPoint(-2.0, -1.5, 0.0),
		MPoint(3.0, 1.5, 0.0));
}

void LocatorNodeVP12Override::addUIDrawables(
	const MDagPath& objPath,
	MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& frameContext,
	const MUserData* userData)
{
	const UserData* castData = dynamic_cast<const UserData*>(userData);
	if (!castData) {
		return;
	}

	drawManager.beginDrawable();

	// Set alpha-blended color based on display status
	MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(objPath);
	MColor color = (displayStatus == MHWRender::kLead)
		? MColor(0.0f, 1.0f, 0.0f, 0.3f)
		: MColor(1.0f, 1.0f, 0.0f, 0.3f);

	drawManager.setColor(color);
	drawManager.setDepthPriority(MHWRender::MRenderItem::sDormantFilledDepthPriority);

	MPointArray points;
	MUintArray indices;

	// Triangle 1
	points.append(MPoint(-2.0, 1.0, 0.0));
	points.append(MPoint(-2.0, -1.0, 0.0));
	points.append(MPoint(2.0, 1.0, 0.0));
	indices.append(0); indices.append(1); indices.append(2);

	// Triangle 2
	points.append(MPoint(2.0, -1.0, 0.0));
	indices.append(1); indices.append(3); indices.append(2);

	// Triangle 3
	points.append(MPoint(2.0, -1.5, 0.0));
	points.append(MPoint(2.0, 1.5, 0.0));
	points.append(MPoint(3.0, 1.0, 0.0));
	indices.append(4); indices.append(5); indices.append(6);

	drawManager.mesh(MUIDrawManager::kTriangles, points, nullptr, nullptr, &indices, nullptr);

	drawManager.endDrawable();
}


MObject LocatorNodeVP12::drawIt;
MTypeId LocatorNodeVP12::typeId = 0x100;
MString LocatorNodeVP12::drawDbClassification = "drawdb/geometry/LocatorNodeVP12";
MString LocatorNodeVP12::drawDbName = "LocatorNodeVP12";

LocatorNodeVP12Override::LocatorNodeVP12Override(const MObject& obj):
	MHWRender::MPxDrawOverride(obj, LocatorNodeVP12Override::draw){ }


void LocatorNodeVP12Override::draw(const MHWRender::MDrawContext& context, const MUserData* userData) { }

MHWRender::MPxDrawOverride* LocatorNodeVP12Override::creator(const MObject& obj) {
	return new LocatorNodeVP12Override(obj);

}

MHWRender::DrawAPI LocatorNodeVP12Override::supportedDrawAPIs() const {
	return MHWRender::kAllDevices;
}
bool LocatorNodeVP12Override::hasUIDrawables() const { return true; }

MUserData* LocatorNodeVP12Override::prepareForDraw(
	const MDagPath& objPath,
	const MDagPath& cameraPath,
	const MHWRender::MFrameContext& frameContext,
	MUserData* userData)
{
	MSharedPtr<MUserData> baseData(userData);
	UserData* castData = dynamic_cast<UserData*>(baseData.get());

	if (!castData) {
		castData = new UserData();
		baseData = MSharedPtr<MUserData>(castData);
	}

	castData->path = objPath;

	return baseData.get();
}
