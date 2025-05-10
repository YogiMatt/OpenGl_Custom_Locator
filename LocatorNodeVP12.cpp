#pragma once

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

void LocatorNodeVP12::draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) {
	MPlug drawItP(thisMObject(), drawIt);
	bool drawItV;
	drawItP.getValue(drawItV);
	if (!drawItV) return;

	view.beginGL();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

	if (status = M3dView::kLead)
		glColor4d(0.0, 1.0, 0.0, 0.3);
	else
		glColor4d(1.0, 1.0, 0.0, 0.3);

	glBegin(GL_TRIANGLES); //draw tris
	glVertex3d(-2.0, 1.0, 0.0); //tri one
	glVertex3d(-2.0, -1.0, 0.0);
	glVertex3d(2.0, 1.0, 0.0);

	glVertex3d(2.0, -1.0, 0.0); //tri two
	glVertex3d(-2.0, -1.0, 0.0);
	glVertex3d(2.0, -1.0, 0.0);

	glVertex3d(2.0, -1.5, 0.0); //tri three
	glVertex3d(2.0, 1.5, 0.0);
	glVertex3d(3.0, 1.0, 0.0);
	glEnd();
}


