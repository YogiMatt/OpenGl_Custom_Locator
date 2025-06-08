
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>

#include "LocatorNodeVP12.h"

void* LocatorNodeVP12::creator() { return new LocatorNodeVP12; }

MStatus LocatorNodeVP12::initialize() {
    MStatus status;

    MFnNumericAttribute numericFn;
    drawIt = numericFn.create("drawIt", "drawIt", MFnNumericData::kBoolean, 1.0, &status); CHECK_MSTATUS_AND_RETURN_IT(status);
    status = addAttribute(drawIt); CHECK_MSTATUS_AND_RETURN_IT(status);

    return status;
}

bool LocatorNodeVP12::isBounded() const { return true; }

MBoundingBox LocatorNodeVP12::boundingBox() const {
    return MBoundingBox(
        MPoint(-2.0, -1.5, 0.0),
        MPoint(3.0, 1.5, 0.0)
    );
}

void LocatorNodeVP12::draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) {
    MPlug drawItP(thisMObject(), drawIt);
    bool drawItV;
    drawItP.getValue(drawItV);
    if (!drawItV) return;

    view.beginGL();

    glPushAttrib(GL_ALL_ATTRIB_BITS); // back up the current global settings

    glEnable(GL_BLEND); // allow transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // actually enable transparency

    if (status == M3dView::kLead) // change the color if this is the most recently selected object
        glColor4d(0.0, 1.0, 0.0, 0.3);
    else
        glColor4d(1.0, 1.0, 0.0, 0.3);

    glBegin(GL_TRIANGLES); // draw triangles
    glVertex3d(-2.0, 1.0, 0.0); // one triangle
    glVertex3d(-2.0, -1.0, 0.0);
    glVertex3d(2.0, 1.0, 0.0);

    glVertex3d(2.0, -1.0, 0.0); // two triangles
    glVertex3d(-2.0, -1.0, 0.0);
    glVertex3d(2.0, -1.0, 0.0);

    glVertex3d(2.0, -1.5, 0.0); // three triangles
    glVertex3d(2.0, 1.5, 0.0);
    glVertex3d(3.0, 1.0, 0.0);
    glEnd(); // stop drawing

    glPopAttrib(); // restore all the global settings to the last back up

    view.endGL();
}


MObject LocatorNodeVP12::drawIt;
MTypeId LocatorNodeVP12::typeId = 0x100;
MString LocatorNodeVP12::drawDbClassification = "drawdb/geometry/LocatorNodeVP12";

LocatorNodeVP12Override::LocatorNodeVP12Override(const MObject& obj) :
    MHWRender::MPxDrawOverride(obj, LocatorNodeVP12Override::draw) {}

void LocatorNodeVP12Override::draw(const MHWRender::MDrawContext& context, const MUserData* userData) {}

MHWRender::MPxDrawOverride* LocatorNodeVP12Override::creator(const MObject& obj) {
    return new LocatorNodeVP12Override(obj);
}

MHWRender::DrawAPI LocatorNodeVP12Override::supportedDrawAPIs() const {
    return MHWRender::kAllDevices;
}

bool LocatorNodeVP12Override::hasUIDrawables() const { return true; }

MUserData* LocatorNodeVP12Override::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath,
    const MHWRender::MFrameContext& frameContext, MUserData* userData)
{
    // Make sure we have an instance of our class
    UserData* castData = dynamic_cast<UserData*>(userData);
    if (castData == nullptr) {
        castData = new UserData;
    }

    // Fill out the data
    castData->path = objPath;

    return castData;
}

void LocatorNodeVP12Override::addUIDrawables(
    const MDagPath& objPath,
    MHWRender::MUIDrawManager& drawManager,
    const MHWRender::MFrameContext& frameContext,
    const MUserData* userData)
{
    const UserData* castData = dynamic_cast<const UserData*>(userData);
    if (castData == nullptr) {
        return;
    }

    MPointArray points;
    points.append(MPoint(-2.0, 1.0, 0.0));
    points.append(MPoint(-2.0, -1.0, 0.0));
    points.append(MPoint(2.0, 1.0, 0.0));
    points.append(MPoint(2.0, -1.0, 0.0));
    points.append(MPoint(2.0, -1.5, 0.0));
    points.append(MPoint(2.0, 1.5, 0.0));
    points.append(MPoint(3.0, 1.0, 0.0));

    MUintArray indices;
    indices.append(0); indices.append(1); indices.append(2);
    indices.append(1); indices.append(3); indices.append(2);
    indices.append(4); indices.append(5); indices.append(6);

    drawManager.beginDrawable();

    MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(objPath);

    MColor color;
    if ((frameContext.getDisplayStyle() &
        (MHWRender::MFrameContext::kFlatShaded |
            MHWRender::MFrameContext::kGouraudShaded |
            MHWRender::MFrameContext::kTextured)) != 0)
    {
        drawManager.setColor(color);
        drawManager.setDepthPriority(MHWRender::MRenderItem::sDormantFilledDepthPriority);
        drawManager.mesh(MUIDrawManager::kTriangles, points, nullptr, nullptr, &indices, nullptr);
    }

    drawManager.endDrawable();

}