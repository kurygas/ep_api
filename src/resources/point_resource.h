#pragma once

#include "resource.h"
#include "point.h"

class PointResource : public Resource<Point> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Ptr<Point>& point) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void postRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Point>& point) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Point>& point) const override;
};
