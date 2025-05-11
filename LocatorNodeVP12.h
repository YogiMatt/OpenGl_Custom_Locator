#pragma once


#include <maya/MPxLocatorNode.h>
#include<maya/MFnNumericAttribute.h>	
#include<maya/MPxDrawOverride.h>	
#include<maya/MPoint.h>
#include<maya/MUintArray.h>
#include <maya/MBoundingBox.h>
#include <maya/MFrameContext.h>
#include <maya/MDagPath.h>


class LocatorNodeVP12 : public MPxLocatorNode {
	static MObject drawIt;

public:
	static MTypeId typeId;
	static MString drawDbClassification;
	static void* creator();
	static MStatus initialize();

	bool isBounded() const override;
	MBoundingBox boundingBox() const override;
	void draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) override; 
};

class LocatorNodeVP12Override : public MHWRender::MPxDrawOverride {
public:
	static MHWRender::MPxDrawOverride* creator(const MObject& obj);
	MHWRender::DrawAPI supportedDrawAPIs() const override;
	MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* userData) override;
	bool hasUIDrawables() const override;
	void addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& farmeContext, const MUserData* userData) override;
	static void draw(const MHWRender::MDrawContext& context, const MUserData* userData);

private:
	LocatorNodeVP12Override(const MObject& obj);

	class UserData : public MUserData {
	public:
		UserData(): MUserData(false){ }
		MDagPath path;
	};

};